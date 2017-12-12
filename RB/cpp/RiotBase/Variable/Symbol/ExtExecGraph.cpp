#include "ExtExecGraph.h"
// #include "../../Db/Db.h"

namespace RiotBase {

Vec<std::pair<PI64,ExtExecGraph *>> ExtExecGraph::instances;

ExtExecGraph::ExtExecGraph( DevId loc ) : RpTemplSym<ExtExecGraph,-1>( (const Category *)nullptr, 0, 0 ), loc( loc ) {
    inst_num = new_inst_num();
    instances << std::make_pair( inst_num, this );
}

ExtExecGraph::~ExtExecGraph() {
    instances.remove_first_unordered( std::make_pair( inst_num, this ) );
}

std::string ExtExecGraph::type_name() {
    return "ExtExecGraph";
}

void ExtExecGraph::launch() {
    // loc.db->send_graph_cmd( loc.dev, inst_num, out_nums, graph );
    TODO;
}

PI64 ExtExecGraph::new_inst_num() {
    PI64 res = 0;
    for( std::pair<PI64,ExtExecGraph *> p : instances )
        res = std::max( res, p.first + 1 );
    return res;
}


} // namespace RiotBase
