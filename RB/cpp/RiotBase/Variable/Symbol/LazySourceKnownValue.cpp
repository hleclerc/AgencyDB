#include "LazySourceKnownValue.h"

namespace RiotBase {

LazySourceKnownValue::LazySourceKnownValue( Rp *rp ) : LazySource<LazySourceKnownValue>( RO_CALL( category, rp ) ), rp( rp ) {
}

LazySourceKnownValue::~LazySourceKnownValue() {
    if ( rp )
        RO_CALL( del, rp );
}

std::string LazySourceKnownValue::type_name() {
    return "LazySourceKnownValue";
}

void LazySourceKnownValue::get_lazy_source_info( LazySourceInfo &info, const LazySourceInfo **children_info ) const {
    RO_CALL( get_lazy_source_info, rp, info, children_info );
}

Rp *LazySourceKnownValue::exec_mp() {
    Rp *val = rp;
    rp = 0;
    return val;
}


} // namespace RiotBase
