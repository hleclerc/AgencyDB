#pragma once

#include "RpTemplSym.h"

namespace RiotBase {

///
class ExtExecGraph : public RpTemplSym<ExtExecGraph,-1> {
public:
    using VecInst = Vec<std::pair<PI64,ExtExecGraph *>>;

    ExtExecGraph( DevId loc );
    ~ExtExecGraph();

    static std::string type_name   ();
    void               launch      ();

    static PI64        new_inst_num();

    Vec<PT>            out_nums; ///< num of output nodes in `graph`
    PI64               inst_num; ///< num (.first) in instances
    CbQueue            graph;    ///< serialized graph data
    DevId              loc;      ///< where to send the graph

    static VecInst     instances;
};

} // namespace RiotBase
