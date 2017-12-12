#include "UsrId.h"

namespace RiotBase {

void glo_to_loc( UsrId &usr, DevId cur_dev, UsrId cur_usr ) {
    if ( usr.num and usr.dev == cur_dev )
        usr.dev.val = 0;
    if ( usr == cur_usr )
        usr.num = 0;
}

void loc_to_glo( UsrId &usr, DevId cur_dev, UsrId cur_usr ) {
    if ( usr.num and usr.dev.val == 0 )
        usr.dev = cur_dev;
    if ( usr.num == 0 )
        usr = cur_usr;
}

} // namespace RiotBase
