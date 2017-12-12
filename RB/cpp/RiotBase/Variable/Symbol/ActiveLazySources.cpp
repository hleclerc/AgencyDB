#include "../../System/SplittedVec.h"
#include "../../System/MapCount.h"
#include "../../System/AutoPtr.h"
#include "../../System/Rcast.h"

#include "../../EvLoop/PostFunc.h"
#include "../../EvLoop/Gev.h"

#include "../../Db/Db.h"

#include "LocExecGraphOutputLazyfier.h"
#include "LocExecGraphOutputExt.h"
#include "ActiveLazySources.h"
#include "SymbolicVisitor.h"
#include "RecvFromExtExec.h"
#include "LazySourceInfo.h"
#include "SendToExtExec.h"
#include "ExtExecGraph.h"
#include "ExecList.h"
#include "Lazyfier.h"
#include "Graphviz.h"
#include "GetNout.h"

#include <fstream>
#include <metis.h>
#include <set>
#include <map>

#ifdef METIL_COMP_DIRECTIVE
#pragma lib_name metis
#endif // METIL_COMP_DIRECTIVE

namespace RiotBase {

namespace {

struct Graph {
    struct SubGraph;

    struct Node {
        Node( Rp *rp ) : rp( rp ), in_a_lazifier( false ), sub_graph( 0 ), op_id( 0 ) {
        }

        void write_to_stream( std::ostream &os ) const {
            RO_CALL( write_to_stream, rp, os );
        }

        bool all_ch_have_op_id( PI64 op_id ) const {
            for( Node *c : children )
                if ( c->op_id != op_id )
                    return false;
            return true;
        }

        // input
        Rp                     *rp;
        Vec<Node *>             links;
        Vec<Node *>             parents;
        Vec<Node *>             children;
        AutoPtr<LazySourceInfo> source_info;
        bool                    in_a_lazifier;
        LazySourceInfo          lazy_source_info;

        // output
        DevId                   loc;                  ///< location
        PT                      num;                  ///< node number (because we need indices)
        SubGraph               *sub_graph;
        double                  completion_date;
        PT                      num_subgraph_output;  ///< num output in subgraph (if there are external parents)
        PT                      num_node_in_subgraph;

        // graph
        mutable PI64            op_id;
    };

    struct SubGraph {
        SubGraph( DevId loc ) : loc( loc ) {}

        Rp                     *rp;
        Vec<Rp *>               outputs; ///< used by exec_list
        Vec<Node *>             nodes;
        DevId                   loc;
    };

    Graph() {
        cur_op_id = 0;
    }

    ~Graph() {
        for( Node     *n : nodes      ) delete n;
        for( SubGraph *g : sub_graphs ) delete g;
    }

    Node *add_src_rec( Rp *rp, bool comes_from_a_lazyfier = false ) {
        CommonSymbolicData *csd = RO_CALL( common_symbolic_data, rp );

        // there's already a node ?
        if ( csd and csd->op_id == CommonSymbolicData::cur_op_id )
            return rcast( csd->op_mp );

        // else, create a new one
        Node *n = new Node( rp );
        nodes << n;

        // register it
        if ( csd ) {
            csd->op_id = CommonSymbolicData::cur_op_id;
            csd->op_mp = n;

            if ( not csd->lazyfier_parents.empty() )
                n->in_a_lazifier = true;
        }

        // children
        for( int i = 0, l = RO_CALL( nb_symbolic_children, rp ); i < l; ++i ) {
            Node *c = add_src_rec( ccast( RO_CALL( symbolic_child, rp, i ) ) );
            c->parents  << n;
            n->children << c;
        }

        //
        if ( n->children.empty() )
            leaves << n;
        if ( comes_from_a_lazyfier )
            lazyfied << n;

        return n;
    }

    void set_loc() {
        _set_size_estimation();

        // currently we use a greedy algorithm
        // DevFree enable to know if a device is free for computations and network transportation
        struct DevFree {
            struct Val {
                double cpu;
                double net;
            };

            Val &operator[]( DevId dev ) {
                std::map<DevId,Val>::iterator iter = data.find( dev );
                if ( iter == data.end() )
                    iter = data.insert( std::make_pair( dev, Val{ 0, 0 } ) ).first;
                return iter->second;
            }

            std::map<DevId,Val> data;
        };

        auto date_with_loc = [&]( DevFree &dev_free, DevId loc, Node *n ) {
            constexpr double bandwidth = 1e6; ///< byte/s

            // when we can start the computation
            double res = dev_free[ loc ].cpu;
            for( Node *c : n->children ) {
                double date_ava_c = c->completion_date;
                if ( loc != c->loc ) {
                    date_ava_c = std::max( date_ava_c, std::max( dev_free[ c->loc ].net, dev_free[ loc ].net ) ) + c->lazy_source_info.output_size * bandwidth;
                    dev_free[ c->loc ].net = date_ava_c;
                    dev_free[ loc ].net = date_ava_c;
                }
                res = std::max( res, date_ava_c );
            }

            //
            res += n->lazy_source_info.time_to_complete;
            dev_free[ loc ].cpu = res;
            return res;
        };

        DevFree dev_free;
        std::set<DevId> possible_devs;
        _ordered_node_visitor( [ & ]( Node *n ) { possible_devs.insert( n->loc ); } );

        _ordered_node_visitor( [ & ]( Node *n ) {
            if ( n->children.empty() ) {
                n->completion_date = date_with_loc( dev_free, n->loc, n );
                return;
            }

            // try all the possible devs
            n->completion_date = std::numeric_limits<double>::max();
            for( DevId tr_loc : possible_devs ) {
                DevFree tmp_dev_free = dev_free;
                double date = date_with_loc( tmp_dev_free, tr_loc, n );
                if ( n->completion_date > date ) {
                    n->completion_date = date;
                    n->loc = tr_loc;
                }
            }

            // use the best choice to update dev_free
            date_with_loc( dev_free, n->loc, n );
        } );

        // corrections for leaves
        for( Node *n : leaves ) {
            if ( n->parents.size() and not RO_CALL( common_symbolic_data, n->rp ) ) {
                MapCount<DevId> mc;
                for( Node *p : n->parents )
                    mc << p->loc;
                n->loc = mc.max_count();
            }
        }
    }

    void make_ext_graphs() {
        // make subgraphs (connected nodes with same loc)
        for( Node *n : nodes ) {
            if ( not n->sub_graph ) {
                SubGraph *s = new SubGraph( n->loc );
                n->sub_graph = s;
                sub_graphs << s;
                _propagate_sub_graph( n, s );
            }
        }


        // make ordered node lists in subgraphs
        // if nodes have lazyfier parents or parents in external subgraphs, add nodes for communication
        _ordered_node_visitor( [ & ]( Node *n ) {
            // basic sub_graph data
            CommonSymbolicData *csd = RO_CALL( common_symbolic_data, n->rp );
            SubGraph *s = n->sub_graph;
            s->nodes << n;

            // replace n->rp by RecvFromExtExec in p->rp
            GetLink get_link( this, n );
            for( Node *p : n->parents ) {
                if ( p->sub_graph != n->sub_graph ) {
                    for( PT i = 0; i < p->children.size(); ++i ) {
                        if ( p->children[ i ] == n ) {
                            Node *r = get_link( p->sub_graph );
                            RO_CALL( set_child, p->rp, i, r->rp, false ); // TODO: remove parents
                            n->parents.remove_first_unordered( p );
                            p->children[ i ] = r;
                            r->parents << p;
                        }
                    }
                }
            }

            // if there is a lazyfier output (in the main thread)
            if ( csd and csd->lazyfier_parents.size() and n->rp->type_num != StaticStore<TypeTempl<LocExecGraphOutputLazyfier>>::data.type_num ) {
                if ( n->loc ) { // if data comes from another inst
                    Node *o = get_link();
                    for( Lazyfier *l : csd->lazyfier_parents ) {
                        if ( CommonSymbolicData *o_csd = RO_CALL( common_symbolic_data, o->rp ) )
                            o_csd->lazyfier_parents << l;
                        l->cur = o->rp;
                    }

                    for( PT i = 0; i < csd->lazyfier_parents.size() - 1; ++i ) // -1 because one is in the sender
                        RO_CALL( del, n->rp );
                    csd->lazyfier_parents.clear();
                } else { // if data is local (comes from another thread)
                    // interpose a LocExecGraphOutputLazyfier (to have a thread proof link)
                    LocExecGraphOutputLazyfier *o = LocExecGraphOutputLazyfier::New( n->rp );
                    Node *no = new Node( o );

                    no->sub_graph = n->sub_graph;
                    s->nodes << no;
                    no->children << n;
                    n->parents << no;
                    no->loc = n->loc;
                    nodes << no;

                    for( Lazyfier *l : csd->lazyfier_parents ) {
                        o->csd.lazyfier_parents << l;
                        l->cur = RO_CALL( copy, o );
                    }
                    s->outputs << o;

                    for( PT i = 0; i < csd->lazyfier_parents.size() - 1; ++i ) // -1 because one is in the sender
                        RO_CALL( del, n->rp );
                    csd->lazyfier_parents.clear();
                }
            }
        } );
    }

    void launch() {
        for( SubGraph *s : sub_graphs ) {
            if ( s->loc ) {
                PT cpt = 0;
                CbQueue cq;
                for( Node *n : s->nodes ) {
                    n->num_node_in_subgraph = cpt++;
                    Vec<PT> num_nodes_ch;
                    for( Node *c : n->children )
                        num_nodes_ch << c->num_node_in_subgraph;
                    RO_CALL( serialize, n->rp, &cq, num_nodes_ch.ptr() );
                }
                // the graph is serialized -> we can delete it
                for( Rp *o : s->outputs )
                    RO_CALL( del, o );
                db->send_graph_cmd( s->loc, cq );
            } else { // -> local stuff
                *exec_list << s->outputs;
            }
        }
    }


    int display_graphviz( std::function<void(std::ostream &os,Node *n)> f, const std::string &base_filename = ".res" ) {
        std::string dot_file = base_filename + ".dot";
        std::ofstream fout( dot_file.c_str() );
        write_graphviz( f, fout );
        fout.flush();

        return system(( "dot -Tpdf " + base_filename + ".dot > " + base_filename + ".pdf ; okular " + base_filename + ".pdf 2> /dev/null" ).c_str());
    }

    void write_graphviz( std::function<void(std::ostream &os,Node *n)> f, std::ostream &os ) {
        os << "digraph my_digraph {\n";
        for( Node *n : nodes ) {
            os << "  node_" << n << " [label=\"";
            if ( RO_CALL( nb_symbolic_children, n->rp ) )
                os << Type::get_type_of( n->rp )->name();
            else
                RO_CALL( write_to_stream, n->rp, os );
            os << "(";
            f( os, n );
            os << ")\"];\n";
            for( Node *c : n->children )
                os << "  node_" << n << " -> node_" << c << ";\n";
            for( Node *p : n->parents )
                os << "  node_" << p << " -> node_" << n << " [color=red];\n";
            for( Node *p : n->links )
                os << "  node_" << p << " -> node_" << n << " [color=red,style=dashed];\n";

            if ( CommonSymbolicData *csd = RO_CALL( common_symbolic_data, n->rp ) ) {
                for( Lazyfier *l : csd->lazyfier_parents ) {
                    os << "  node_" << l << " [label=LZ,shape=box];\n";
                    os << "  node_" << l << " -> node_" << n << " [color=grey];\n";
                }
            }
        }
        os << "}\n";
    }

    Vec<SubGraph *> sub_graphs;

protected:
    // helper, to make sender and receivers for a given node
    struct GetLink {
        struct Link {
            Node *receiver;
            Node *sender;
        };

        GetLink( Graph *g, Node *n ) : g( g ), n( n ) {
        }

        // give link nodes for a given ext subgraph. Return the receiver
        Node *operator()( SubGraph *ps = 0 ) {
            if ( not ps ) {
                ps = new SubGraph( {} );
                g->sub_graphs << ps;
            }
            std::map<SubGraph *,Link>::iterator iter = links.find( ps );

            if ( iter == links.end() ) {
                // get a reference id for communication
                static PI64 cur_num = 0; PI64 num = cur_num++;

                // sender
                Node *sender = new Node( SendToExtExec::New( n->rp, ps->loc.glo_dev( db->get_dev_id() ), num ) );
                sender->loc           =  n->loc;
                sender->sub_graph     =  n->sub_graph;
                g->nodes              << sender;

                sender->children      << n;
                n->parents            << sender;

                n->sub_graph->outputs << sender->rp;

                // receiver
                Node *receiver = new Node( RecvFromExtExec::New( n->loc.glo_dev( db->get_dev_id() ), num ) );
                receiver->loc         =  ps->loc;
                receiver->sub_graph   =  ps;
                g->nodes              << receiver;

                sender->links         << receiver;

                iter = links.insert( std::make_pair( ps, Link{ receiver, sender } ) ).first;
            } else
                ++static_cast<RecvFromExtExec *>( iter->second.receiver->rp )->cpt_use;

            return iter->second.receiver;
        }

        std::map<SubGraph *,Link> links;
        Graph                    *g;
        Node                     *n;
    };

    template<class Op>
    void _ordered_node_visitor( const Op &op ) {
        ++cur_op_id;
        Vec<Node *> front = leaves;
        while ( front.size() ) {
            Node *n = front.back();
            front.pop_back();

            if ( n->op_id == cur_op_id )
                continue;
            n->op_id = cur_op_id;

            op( n );

            for( Node *p : n->parents )
                if ( p->all_ch_have_op_id( cur_op_id ) )
                    front << p;
            for( Node *p : n->links )
                if ( p->all_ch_have_op_id( cur_op_id ) )
                    front << p;
        }
    }

    void _set_size_estimation() {
        Vec<LazySourceInfo *> children_info;
        _ordered_node_visitor( [ & ]( Node *n ) {
            children_info.clear();
            for( Node *c : n->children ) children_info << &c->lazy_source_info;
            RO_CALL( get_lazy_source_info, n->rp, n->lazy_source_info, (const LazySourceInfo **)children_info.ptr() );

            // force loc
            if ( not n->lazy_source_info.prob_is_on.empty() )
                n->loc = val_max( n->lazy_source_info.prob_is_on, []( const LazySourceInfo::LocInfo &l ) { return l.prob; } )->loc;
        } );
    }

    void _propagate_sub_graph( Node *n, SubGraph *s ) {
        n->sub_graph = s;
        for( Node *c : n->children ) if ( c->loc == s->loc and not c->sub_graph ) _propagate_sub_graph( c, s );
        for( Node *p : n->parents  ) if ( p->loc == s->loc and not p->sub_graph ) _propagate_sub_graph( p, s );
    }

    PI64                cur_op_id;

    Vec<Node *>         nodes;
    Vec<Node *>         leaves;
    Vec<Node *>         lazyfied;
    Vec<AutoRpDel>      to_del;
};

const Rp *get_leaf_symbol( const Rp *rp ) {
    if ( RO_CALL( is_a_s_op, rp ) and RO_CALL( nb_symbolic_children, rp ) )
        return get_leaf_symbol( RO_CALL( symbolic_child, rp, 0 ) );
    return rp;
}

void get_lazyfiers( std::set<Lazyfier *> &lazyfiers, const Rp *rp ) {
    if ( rp->type_num == StaticStore<TypeTempl<Lazyfier>>::data.type_num )
        return get_lazyfiers( lazyfiers, get_leaf_symbol( static_cast<const Lazyfier *>( rp )->cur ) );

    if ( CommonSymbolicData *csd = RO_CALL( common_symbolic_data, rp ) ) {
        if ( csd->op_id == CommonSymbolicData::cur_op_id )
            return;
        csd->op_id = CommonSymbolicData::cur_op_id;

        for( Lazyfier *p : csd->lazyfier_parents )
            lazyfiers.insert( p );
        for( const Rp *p : csd->parents )
            get_lazyfiers( lazyfiers, p );
    }
}

class PostFuncActiveLazySources : public PostFunc {
public:
    virtual bool exec() {
        return active_lazy_sources->exec();
    }
};
PostFuncActiveLazySources post_func_active_lazy_sources;

}

LazyNew<ActiveLazysources> active_lazy_sources;

ActiveLazysources::ActiveLazysources() {
    *gev << &post_func_active_lazy_sources;
}

void ActiveLazysources::operator<<( Rp *rp ) {
    sources << rp;
}

bool ActiveLazysources::exec() {
    // get all the lazyfiers
    std::set<Lazyfier *> lazyfiers;
    ++CommonSymbolicData::cur_op_id;
    for( Rp *s : sources )
        get_lazyfiers( lazyfiers, s );
    if ( lazyfiers.empty() )
        return false;

    // make an alternative representation of the graph
    Graph g;
    ++CommonSymbolicData::cur_op_id;
    for( Lazyfier *l : lazyfiers )
        g.add_src_rec( l->cur, true );

    // assign dev_id to leaf nodes
    g.set_loc();


    // make ext graphs if necessary (graphs that will be sent to ext db instances)
    g.make_ext_graphs();

    // g.display_graphviz( []( std::ostream &os, Graph::Node *n ) { os << ( n->loc.val != 0 ); } );

    g.launch();

    //    Graphviz gr;
    //    for( Lazyfier *l : lazyfiers )
    //        gr << l->cur;
    //    gr.display();

    return true;
}


} // namespace RiotBase
