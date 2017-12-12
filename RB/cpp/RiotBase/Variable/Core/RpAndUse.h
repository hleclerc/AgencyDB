#pragma once

#include "../Core/Methods.h"

namespace RiotBase {
class Rp;

///
struct RpAndUse {
    RpAndUse( Rp *rp ) : rp( rp ), use( 0 ) {}
    // ~RpAndUse() { if ( rp and not use ) RO_CALL( del, rp ); }

    Rp *get_val() { return use++ ? RO_CALL( copy, rp ) : rp; }

    Rp *rp;
    PT  use;
};

}
