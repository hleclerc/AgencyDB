#pragma once

#include "../../System/AutoPtr.h"
#include "../../System/CbQueue.h"
#include "../../System/DevId.h"
#include "../../System/Vec.h"

namespace RiotBase {

/**

*/
class OtMessage {
public:
    OtMessage( OtMessage *prev = 0 ) : prev( prev ) {}

    AutoPtr<OtMessage> prev;
    Vec<DevId>         dsts; ///< destinations (for a given db)
    CbQueue            msg;  ///< patch data
};

} // namespace RiotBase
