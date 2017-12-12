#pragma once

#include "RpTemplSym.h"

namespace RiotBase {

///
class LocExecGraphOutputLazyfier : public RpTemplSym<LocExecGraphOutputLazyfier,1> {
public:
    LocExecGraphOutputLazyfier( Rp *ch );

    static std::string type_name();
    void               del      ();
    Rp                *exec_mp  ();
};

} // namespace RiotBase
