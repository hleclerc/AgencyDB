#pragma once

#include "../Core/AutoRpDel.h"
#include "RpTemplSym.h"

namespace RiotBase {
class LocExecGraphInput;

///
class LocExecGraph : public RpTemplSym<LocExecGraph,-1> {
public:
    LocExecGraph();

    static std::string       type_name   ();
    void                     launch      ();

    Vec<LocExecGraphInput *> inputs;
    Vec<AutoRpDel>           outputs;
};

} // namespace RiotBase
