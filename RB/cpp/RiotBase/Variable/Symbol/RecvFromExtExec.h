#pragma once

#include "../Core/RpAndUse.h"
#include "RpTemplSym.h"

namespace RiotBase {

///
class RecvFromExtExec : public RpTemplSym<RecvFromExtExec,0> {
public:
    RecvFromExtExec( DevId loc, PI64 num_item );
    ~RecvFromExtExec();

    static std::string     type_name  ();
    void                   serialize  ( BBQ bq, const PT *num_children ) const;
    static Rp             *deserialize( BinStream<CbStringPtr> bq, Vec<RpAndUse> &children );
    Rp                    *exec_mp    ();

    DevId                  loc;
    PI64                   num;
    Rp                    *rp;
};

} // namespace RiotBase
