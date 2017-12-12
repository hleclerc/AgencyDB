#include "RecvFromExtExec.h"

namespace RiotBase {

RecvFromExtExec::RecvFromExtExec( DevId loc, PI64 num ) : RpTemplSym<RecvFromExtExec,0>( nullptr ), loc( loc ), num( num ), rp( 0 ) {
}

RecvFromExtExec::~RecvFromExtExec() {
    if ( rp )
        RO_CALL( del, rp );
}

std::string RecvFromExtExec::type_name() {
    return "RecvFromExtExec";
}

void RecvFromExtExec::serialize( BBQ bq, const PT *num_children ) const {
    bq << num_in_tuple_types() << loc << num;
}

Rp *RecvFromExtExec::deserialize( BinStream<CbStringPtr> bq, Vec<RpAndUse> &children ) {
    DevId loc = bq.read();
    PI64  num = bq.read();
    return RecvFromExtExec::New( loc, num );
}

Rp *RecvFromExtExec::exec_mp() {
    ASSERT_IF_DEBUG( rp );
    Rp *val = rp;
    rp = 0;
    return val;
}

} // namespace RiotBase
