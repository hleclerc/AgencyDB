#pragma once

#include "../../System/AutoPtr.h"

namespace RiotBase {

///
class RetExec {
public:
    RetExec( Rp *rp = 0, bool del = true, RetExec *next = 0 ) : rp( rp ), next( next ), del( del ) {}

    Rp              *rp;
    AutoPtr<RetExec> next;
    bool             del;
};

} // namespace RiotBase
