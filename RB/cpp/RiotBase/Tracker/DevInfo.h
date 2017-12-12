#pragma once

#include "../EvLoop/Util/InetAddress.h"

namespace RiotBase {
class TrackerServerSession;

/**
*/
class DevInfo {
public:
    DevInfo();

    TrackerServerSession *session; ///< may be null
    InetAddress     addr;
};

} // namespace RiotBase
