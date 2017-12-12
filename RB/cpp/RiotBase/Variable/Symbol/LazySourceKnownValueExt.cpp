#include "LazySourceKnownValueExt.h"
#include "../Core/Deserialize.h"
#include "LazySourceInfo.h"

namespace RiotBase {

LazySourceKnownValueExt::LazySourceKnownValueExt( Rp *rp, DevId loc ) : LazySource<LazySourceKnownValueExt>( RO_CALL( category, rp ) ), rp( rp ), loc( loc ) {
}

LazySourceKnownValueExt::~LazySourceKnownValueExt() {
    if ( rp )
        RO_CALL( del, rp );
}

std::string LazySourceKnownValueExt::type_name() {
    return "LazySourceKnownValueExt";
}

void LazySourceKnownValueExt::get_lazy_source_info( LazySourceInfo &info, const LazySourceInfo **children_info ) const {
    RO_CALL( get_lazy_source_info, rp, info, children_info );
    info.prob_is_on.clear();
    info.prob_is_on << LazySourceInfo::LocInfo{ loc, 1.0 };
}

void LazySourceKnownValueExt::serialize( BBQ bq, const PT *num_children ) const {
    RO_CALL( serialize, rp, bq << num_in_tuple_types() << loc, nullptr );
}

Rp *LazySourceKnownValueExt::deserialize( BinStream<CbStringPtr> bq, Vec<RpAndUse> &children ) {
    DevId loc = bq.read();
    Vec<RpAndUse> loc_children;
    return LazySourceKnownValueExt::New( RiotBase::deserialize( bq, loc_children ), loc );
}

Rp *LazySourceKnownValueExt::exec_mp() {
    Rp *val = rp;
    rp = 0;
    return val;
}


} // namespace RiotBase
