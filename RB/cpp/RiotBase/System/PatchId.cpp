#include "PatchId.h"
#include "UsrId.h"

namespace RiotBase {

void PatchId::write_to_stream( std::ostream &os ) const {
    os << num << "@" << ( num ? dev : DevId{} );
}

void glo_to_loc( PatchId &pid, DevId cur_dev, UsrId cur_usr) {
    if ( pid.num and pid.dev == cur_dev ) pid.dev.val = 0;
}

void loc_to_glo( PatchId &pid, DevId cur_dev, UsrId cur_usr ) {
    if ( pid.num and pid.dev.val == 0 ) pid.dev = cur_dev;
}

} // namespace RiotBase


