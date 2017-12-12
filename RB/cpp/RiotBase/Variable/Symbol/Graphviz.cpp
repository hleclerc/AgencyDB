#include "CommonSymbolicData.h"
#include "Lazyfier.h"
#include "Graphviz.h"
#include <sstream>
#include <fstream>
#include <map>
#include <set>

namespace RiotBase {

namespace {

// return the choosen label
static const Rp *write_rec( std::ostream &os, const Rp *obj, std::map<const Rp *,const Rp *> &visited ) {
    if ( visited.count( obj ) )
        return visited[ obj ];

    // node label (and extra data)
    RO_CALL( write_graphviz, obj, os << "    " );
    visited[ obj ] = obj;
    os << "\n";

    //    os << "    node_" << obj << " [label=\"";
    //    for( PT i = 0; i < l.size() and i < 20; ++i ) {
    //        switch ( l[ i ] ) {
    //        case '\n': os << "\\n"; break;
    //        case '\r': os << "\\r"; break;
    //        case '\t': os << "\\t"; break;
    //        default:   os << l[ i ];
    //        }
    //    }
    //    os << "\"];\n";

    // symbolic children + recursion
    for( int i = 0; i < RO_CALL( nb_symbolic_children, obj ); ++i ) {
        const Rp *ch = write_rec( os, RO_CALL( symbolic_child, obj, i ), visited );
        os << "    node_" << obj << " -> node_" << ch << ";\n";
    }

    return obj;
}

// return the choosen label
static void write_par( std::ostream &os, const Rp *obj, std::map<const Rp *,const Rp *> &visited, std::set<const Rp *> &visited_par ) {
    if ( visited_par.count( obj ) )
        return;
    visited_par.insert( obj );

    // recursion
    const Rp *adr = visited[ obj ];
    for( int i = 0; i < RO_CALL( nb_symbolic_children, adr ); ++i )
        write_par( os, RO_CALL( symbolic_child, adr, i ), visited, visited_par );

    // parents
    if ( CommonSymbolicData *csd = RO_CALL( common_symbolic_data, adr ) ) {
        for( const Rp *par : csd->parents )
            if ( visited.count( par ) )
                os << "    node_" << visited[ par ] << " -> node_" << adr << " [color=red];\n";

        for( Lazyfier *par : csd->lazyfier_parents ) {
            if ( visited.count( (const Rp *)par ) ) {
                os << "    node_" << visited[ par ] << " -> node_" << adr << " [color=green];\n";
                //if ( par->sym and visited.count( (const Rp *)par->sym ) )
                //    os << "    node_" << visited[ par ] << " -> node_" << visited[ (const Rp *)par->sym ] << " [color=green style=dotted];\n";
            }
        }
    }
}

}

Graphviz &Graphviz::operator<<( const Rp *v ) {
    lst_var << v;
    return *this;
}

int Graphviz::display( const std::string &base_filename ) {
    std::string dot_file = base_filename + ".dot";
    std::ofstream fout( dot_file.c_str() );
    write( fout );
    fout.flush();

    return system(( "dot -Tpdf " + base_filename + ".dot > " + base_filename + ".pdf ; okular " + base_filename + ".pdf 2> /dev/null" ).c_str());
}

void Graphviz::write( std::ostream &os ) {
    os << "digraph my_digraph {\n";

    //    for( const Vp *vp : vp_list ) {
    //        vp->get_rp_wb( [&]( const Rp *rp ) {
    //            os << "    resu_" << vp << " [label=\"" << vp->get_name() << "\" style=filled fillcolor=yellow];\n";
    //            os << "    resu_" << vp << " -> node_" << disp_ptr( rp ) << ";\n";
    //            write_rec( os, rp, visited );
    //        } );
    //    }

    std::map<const Rp *,const Rp *> visited;
    for( const Rp *var : lst_var )
        write_rec( os, var, visited );

    std::set<const Rp *> visited_par;
    for( const Rp *var : lst_var )
        write_par( os, var, visited, visited_par );

    os << "}\n";
}



} // namespace RiotBase
