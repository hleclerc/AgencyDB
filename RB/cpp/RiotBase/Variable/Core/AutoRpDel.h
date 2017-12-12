#pragma once

#include "Methods.h"

namespace RiotBase {

///
class AutoRpDel {
public:
    AutoRpDel( Rp *rp = 0 ) : rp( rp ) {}
    AutoRpDel( AutoRpDel &&a ) : rp( a.rp ) { a.rp = 0; }
    ~AutoRpDel() { if ( rp ) RO_CALL( del, rp ); }

    void clear    () { if ( rp ) { RO_CALL( del, rp ); rp = 0; } }
    void operator=( AutoRpDel &&a ) { if ( rp ) RO_CALL( del, rp ); rp = a.rp; a.rp = 0; }

    Rp *rp;
};


} // namespace RiotBase
