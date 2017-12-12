#pragma once

#include "RpTemplSym.h"

namespace RiotBase {

///
class GetNout : public RpTemplSym<GetNout,1> {
public:
    GetNout( const Category *category, Rp *ch, int nout ) : RpTemplSym( category, ch ), nout( nout ) {}

    static std::string     type_name      ();

    int                    nout;
};

} // namespace RiotBase
