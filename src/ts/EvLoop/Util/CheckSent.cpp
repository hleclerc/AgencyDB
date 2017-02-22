#include "../EvLoop.h"
#include "CheckSent.h"

#include <linux/sockios.h>
#include <sys/ioctl.h>

namespace RiotBase {

bool CheckSent::timeout( int ) {
    if ( not ev_loop->last_ev_to_del )
        ev_loop->timer = 0;
    return ev_loop->last_ev_to_del;
}

} // namespace RiotBase
