#include "LazySourceInfo.h"
#include "../Core/Surdef.h"

namespace RiotBase {

LazySourceInfo::LazySourceInfo() {
    output_size      = 0;
    time_to_complete = 0;
}

void LazySourceInfo::write_to_stream( std::ostream &os ) const {
    os << "pio=";
    for( const LocInfo &p : prob_is_on )
        os << p.loc << ":" << p.prob << " ";
}

//namespace {

//RiotBase_DEF_FUNC_1( get_lazy_source_info, 0, true, LazySourceInfo &info ) {
//    info.prob_is_on << LazySourceInfo::LocInfo{ {}, 1.0 };
//}

//} // namespace

} // namespace RiotBase
