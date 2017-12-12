#include "SendValue.h"

namespace RiotBase {

SendValue::SendValue( DbSession *session, Rp *ch ) : RpTemplSym<SendValue,1>( nullptr, ch ), session( session ) {
}

std::string SendValue::type_name() {
    return "SendValue";
}


} // namespace RiotBase
