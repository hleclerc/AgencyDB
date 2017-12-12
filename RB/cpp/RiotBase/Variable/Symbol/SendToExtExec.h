#pragma once

#include "../Core/RpAndUse.h"
#include "RpTemplSym.h"

namespace RiotBase {

///
class SendToExtExec : public RpTemplSym<SendToExtExec,1> {
public:
    SendToExtExec( Rp *ch, DevId loc, PI64 num );

    static std::string     type_name  ();
    void                   serialize  ( BBQ bq, const PT *num_children ) const;
    static Rp             *deserialize( BinStream<CbStringPtr> bq, Vec<RpAndUse> &children );
    Rp                    *exec_mp    ();

    DevId                  loc;
    PI64                   num;
};

} // namespace RiotBase
