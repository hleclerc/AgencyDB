#include "GrpId.h"
#include "UsrId.h"

namespace RiotBase {

void glo_to_loc( GrpId &grp, DevId cur_dev, UsrId cur_usr ) {
    if ( grp.num and grp.dev == cur_dev ) grp.dev.val = 0;
}

void loc_to_glo( GrpId &grp, DevId cur_dev, UsrId cur_usr ) {
    if ( grp.num and grp.dev.val == 0 ) grp.dev = cur_dev;
}

} // namespace RiotBase
