#include "UsrId.h"

namespace RiotBase {

void glo_to_loc( DevId &dev, DevId cur_dev, UsrId cur_usr ) {
    if ( dev == cur_dev ) dev.val = 0;
}

void loc_to_glo( DevId &dev, DevId cur_dev, UsrId cur_usr ) {
    if ( dev.val == 0 ) dev = cur_dev;
}

void DevId::write_to_stream(std::ostream &os) const {
    os << val;
}

} // namespace RiotBase
