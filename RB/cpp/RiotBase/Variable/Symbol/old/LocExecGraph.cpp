#include "LocExecGraph.h"
#include "ExecList.h"

namespace RiotBase {

LocExecGraph::LocExecGraph() : RpTemplSym<LocExecGraph,-1>( nullptr, 0, nullptr ) {
}

std::string LocExecGraph::type_name() {
    return "LocExecGraph";
}

void LocExecGraph::launch() {
    *exec_list << std::move( ccast( outputs ) );
}


} // namespace RiotBase
