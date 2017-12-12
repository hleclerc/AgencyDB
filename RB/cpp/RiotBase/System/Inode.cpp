#include "Inode.h"
#include "UsrId.h"

namespace RiotBase {

void Inode::write_to_stream( std::ostream &os ) const {
    os << num << "@" << ( num ? dev : DevId{} );
}

void glo_to_loc( Inode &val, DevId cur_dev, UsrId cur_usr ) {
    if ( val.num and val.dev == cur_dev ) val.dev.val = 0;
}

void loc_to_glo( Inode &val, DevId cur_dev, UsrId cur_usr ) {
    if ( val.num and val.dev.val == 0 ) val.dev = cur_dev;
}

}


