#include "../../System/SplittedVec.h"
#include "../../System/AutoPtr.h"
#include "../../System/Rcast.h"
#include "ActiveLazySources.h"
#include "SymbolicVisitor.h"
#include "LazySourceInfo.h"
#include "ExtExecGraph.h"
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
    struct Node {
        Node( const Rp *rp, bool has_lazifier_parent, bool done ) : rp( rp ), has_lazifier_parent( has_lazifier_parent ), done( done ) {
            subgraph = 0;
            op_id    = 0;
        }

        void write_to_stream( std::ostream &os ) const {
            RO_CALL( write_to_stream, rp, os );
        }

        const Rp               *rp;
        PT                      num;                  ///< node number (because we use pointer for children and parents which do no give indices)
        DevId                   loc;
        Vec<Node *>             parents;
        Vec<Node *>             children;

        ExtExecGraph           *subgraph;
        PT                      num_node_in_subgraph; ///< where the node is serialized in the subgraph

        AutoPtr<LazySourceInfo> source_info;
        PT                      output_size;
        PI64                    op_id;
        bool                    has_lazifier_parent;
        bool                    done;
    };

    Graph() {
        cur_op_id = 0;
    }

    ~Graph() {
        for( Node *n : nodes )
            delete n;
    }

    Node *add_src_rec( const Rp *rp, bool comes_from_a_lazyfier = false ) {
        CommonSymbolicData *csd = RO_CALL( common_symbolic_data, rp );

        // there's already a node ?
        if ( csd and csd->op_id == CommonSymbolicData::cur_op_id )
            return rcast( csd->op_mp );

        // else, create a new one
        Node *n = new Node( rp, comes_from_a_lazyfier, not csd );
        nodes << n;

        // register it
        if ( csd ) {
            csd->op_id = CommonSymbolicData::cur_op_id;
            csd->op_mp = n;
        }

        // children
        for( int i = 0, l = RO_CALL( nb_symbolic_children, rp ); i < l; ++i ) {
            Node *c = add_src_rec( RO_CALL( symbolic_child, rp, i ) );
            c->parents  << n;
            n->children << c;
        }

        //
        if ( n->children.empty() ) {
            RO_CALL( get_lazy_source_info, n->rp, *( n->source_info = new LazySourceInfo ) );
            leaves << n;
        }
        if ( comes_from_a_lazyfier )
            lazyfied << n;

        return n;
    }

    void set_leaves_locs() {
        for( Node *n : leaves ) {
            if ( not n->source_info->prob_is_on.empty() ) {
                // use the best probability
                PT best = 0;
                for( PT i = 1; i < n->source_info->prob_is_on.size(); ++i )
                    if ( n->source_info->prob_is_on[ best ].prob < n->source_info->prob_is_on[ i ].prob )
                        best = i;
                n->loc = n->source_info->prob_is_on[ best ].loc;
            }
        }
    }

    void set_output_size() {
        for( Node *n : nodes )
            n->output_size = 1;
    }

    void metis_graph_partitionning() {
        set_output_size();

        // preparation
        Vec<DevId> devs;
        for( Node *n : leaves )
            devs.push_back_unique( n->loc );

        // METIS path
        if ( devs.size() > 1 ) {
            for( PT i = 0; i < nodes.size(); ++i )
                nodes[ i ]->num = i;

            // basic METIS values
            idx_t       nvtxs  = nodes.size() + devs.size(); // number of vertices in the graph
            idx_t       ncon   = 1;                          // The number of balancing constraints (see vwgt)
            idx_t       nparts = devs.size();                // number of parts

            // edges
            Vec<idx_t>  xadj;
            Vec<idx_t>  adjncy;
            Vec<idx_t>  adjwgt; // weight of the edges
            for( Node *o : nodes ) {
                xadj << (idx_t)adjncy.size();

                // children and parents
                for( Node *n : o->parents ) {
                    adjncy << idx_t( n->num );
                    adjwgt << idx_t( 100 * o->output_size );
                }
                for( Node *n : o->children ) {
                    adjncy << idx_t( n->num );
                    adjwgt << idx_t( 100 * n->output_size );
                }

                // if it's a leaf or has_lazifier_parent, connect it to the corresponding dev node
                idx_t weight_dev = 0;

                if ( o->children.empty() )
                    weight_dev += 100 * o->output_size;
                if ( o->has_lazifier_parent )
                    weight_dev += 100 * o->output_size;

                if ( weight_dev ) {
                    adjncy << idx_t( nodes.size() + devs.first_index_equal_to( o->loc ) );
                    adjwgt << weight_dev;
                }

                // sort
                std::sort( adjncy.ptr() + xadj.back(), adjncy.end() );
            }
            for( DevId d : devs ) {
                xadj << (idx_t)adjncy.size();

                // connect all the leaves
                for( Node *n : leaves ) {
                    if ( n->loc == d ) {
                        adjncy << idx_t( n->num );
                        adjwgt << idx_t( 100 * n->output_size );
                    }
                }
                for( Node *n : lazyfied ) {
                    if ( n->loc == d and not n->children.empty() ) {
                        adjncy << idx_t( n->num );
                        adjwgt << idx_t( 100 * n->output_size );
                    }
                }

            }
            xadj << idx_t( adjncy.size() );

            idx_t       objval;
            Vec<idx_t>  part( Size(), nvtxs );
            Vec<idx_t>  vwgt( Size(), nvtxs, 1 );
            idx_t       options[ METIS_NOPTIONS ];
            METIS_SetDefaultOptions( options );
            options[ METIS_OPTION_NUMBERING ] = 0;

            int res = METIS_PartGraphRecursive( &nvtxs, &ncon, xadj.ptr(), adjncy.ptr(), vwgt.ptr(), 0, adjwgt.ptr(), &nparts, 0, 0, options, &objval, part.ptr() );
            if ( res != METIS_OK )
                PRINT( "Metis pb: ", res );

            // assuming that the linking nodes have the right loc
            PT md = 0;
            for( PT i = nodes.size(); i < part.size(); ++i )
                md = std::max( md, PT( part[ i ] ) );

            Vec<DevId> dev_sp( Size(), md + 1 );
            for( PT i = 0; i < devs.size(); ++i )
                dev_sp[ part[ nodes.size() + i ] ] = devs[ i ];

            // DevId assignation
            for( PT i = 0; i < nodes.size(); ++i )
                nodes[ i ]->loc = dev_sp[ part[ i ] ];
        } else if ( devs.size() ) {
            // DevId assignation
            for( PT i = 0; i < nodes.size(); ++i )
                nodes[ i ]->loc = devs[ 0 ];
        }
    }

    void repl_ext_subgraphs() { // output = new_leaves (leaves after repl_ext_subgraphs proc)
        for( Node *n : leaves ) {
            if ( n->loc and not n->subgraph ) {
                ExtExecGraph *eg = ExtExecGraph::New( n->loc ); // ...

                Node *en = new Node( eg, n->has_lazifier_parent, n->done );
                en->loc = n->loc;
                nodes << en;

                new_leaves << en;

                ++cur_op_id;
                PT nb_nodes_in_subgraph = 0;
                _propagate_subgraph( n, en, eg, nb_nodes_in_subgraph );
            } else
                new_leaves << n;
        }
    }

    PT _propagate_subgraph( Node *n, Node *en, ExtExecGraph *eg, PT &nb_nodes_in_subgraph ) { // return num node in eg->graph, or PT_WRONG
        if ( n->op_id == cur_op_id )
            return n->num_node_in_subgraph;
        n->op_id = cur_op_id;
        n->subgraph = eg;

        // data from children of n
        Vec<PT> ch_nums;
        for( Node *c : n->children ) {
            if ( c->loc != en->loc and not c->done )
                return PT_WRONG;
            PT rc = _propagate_subgraph( c, en, eg, nb_nodes_in_subgraph );
            if ( rc == PT_WRONG )
                return rc;
            ch_nums << rc;
        }

        // data from n
        n->num_node_in_subgraph = nb_nodes_in_subgraph++;
        TODO;
        // RO_CALL( loc_to_glo, ccast( n->rp ), eg->loc ); // we know where graph is sent -> use global ids
        RO_CALL( graph_serialize, n->rp, &eg->graph, ch_nums.ptr() );

        //
        GetNout *_pg = 0;
        Node *_pn = 0;
        auto _upd_gn = [&]() {
            _pg = GetNout::New( RO_CALL( category, n->rp ), eg, n->num_node_in_subgraph );
            eg->out_nums << n->num_node_in_subgraph;

            _pn = new Node( _pg, false, false );
            _pn->children << en;
            en->parents << _pn;
            nodes << _pn;
        };
        auto pg = [&]() {
            if ( not _pg )
                _upd_gn();
            return _pg;
        };
        auto pn = [&]() {
            if ( not _pn )
                _upd_gn();
            return _pn;
        };

        // propagate (look for parents with same dev_id and with children with same dev_id)
        for( Node *p : n->parents ) {
            if ( p->loc != en->loc and not p->done ) {
                // remove p from parents of n
                n->parents.remove_first_unordered( p );
                // substitution
                for( PT i = 0; i < p->children.size(); ++i) {
                    if ( p->children[ i ] == n ) {
                        p->children[ i ] = pn();
                        pn()->parents << p;
                        RO_CALL( set_child, ccast( p->rp ), i, pg() );
                        break;
                    }
                }
            } else
                _propagate_subgraph( p, en, eg, nb_nodes_in_subgraph );
        }

        //
        if ( CommonSymbolicData *csd = RO_CALL( common_symbolic_data, n->rp ) ) {
            for( Lazyfier *l : csd->lazyfier_parents ) {
                pn()->has_lazifier_parent = true;
                to_del << l->cur;
                l->cur = pg();
            }
        }

        return n->num_node_in_subgraph;
    }

    int display_graphviz( const std::string &base_filename = ".res" ) {
        std::string dot_file = base_filename + ".dot";
        std::ofstream fout( dot_file.c_str() );
        write_graphviz( fout );
        fout.flush();

        return system(( "dot -Tpdf " + base_filename + ".dot > " + base_filename + ".pdf && okular " + base_filename + ".pdf 2> /dev/null" ).c_str());
    }

    void write_graphviz( std::ostream &os ) {
        os << "digraph my_digraph {\n";
        for( Node *n : nodes ) {
            os << "  node_" << n << " [label=\"";
            if ( RO_CALL( nb_symbolic_children, n->rp ) )
                os << Type::get_type_of( n->rp )->name();
            else
                RO_CALL( write_to_stream, n->rp, os );
            os << "(";
            if ( n->done )
                os << "x";
            else
                os << n->loc;
            os << ")\"];\n";
            for( Node *c : n->children )
                os << "  node_" << n << " -> node_" << c << ";\n";
        }
        os << "}\n";
    }

    static constexpr PT PT_WRONG = std::numeric_limits<PT>::max();
    PI64                cur_op_id;

    Vec<Node *>         nodes;
    Vec<Node *>         leaves;
    Vec<Node *>         lazyfied;
    Vec<AutoRpDel>      to_del;

    Vec<Node *>         new_leaves;
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

}
ActiveLazysources active_lazy_sources;

void ActiveLazysources::operator<<( Rp *rp ) {
    std::unique_lock<std::mutex> lock( mutex );
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
    g.set_leaves_locs();

    // assign dev_id to the other nodes
    g.metis_graph_partitionning();

    // graph splitting -> replace parts to be executed elsewhere to local nodes
    g.repl_ext_subgraphs();

    //    g.display_graphviz();
    //    Graphviz gr;
    //    for( Lazyfier *l : lazyfiers )
    //        gr << l->cur;
    //    gr.display();

    // launch (exec of local, send the command if not)
    // would be great to have strategies to decide when to exec parents, when to wait for the children
    for( Graph::Node *l : g.new_leaves )
        if ( not l->done )
            RO_CALL( launch, l->rp );
    sources.clear();

    return true;
}


} // namespace RiotBase
