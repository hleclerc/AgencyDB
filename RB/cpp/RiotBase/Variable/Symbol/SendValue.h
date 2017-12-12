#pragma once

#include "RpTemplSym.h"

namespace RiotBase {
class DbSession;

///
class SendValue : public RpTemplSym<SendValue,1> {
public:
    SendValue( DbSession *session, Rp *ch );

    static std::string     type_name         ();

    DbSession *session;
};

} // namespace RiotBase
