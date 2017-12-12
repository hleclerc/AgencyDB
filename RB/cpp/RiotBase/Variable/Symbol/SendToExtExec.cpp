#include "SendToExtExec.h"

namespace RiotBase {

SendToExtExec::SendToExtExec( Rp *ch, DevId loc, PI64 num ) : RpTemplSym<SendToExtExec,1>( RO_CALL( category, ch ), ch ), loc( loc ), num( num ) {
}

std::string SendToExtExec::type_name() {
    return "SendToExtExec";
}

void SendToExtExec::serialize( BBQ bq, const PT *num_children ) const {
    if ( not num_children ) TODO;
    bq << num_in_tuple_types() << num_children[ 0 ] << loc << num;
}

Rp *SendToExtExec::deserialize( BinStream<CbStringPtr> bq, Vec<RpAndUse> &children ) {
    PT    nch = bq.read();
    DevId loc = bq.read();
    PI64  num = bq.read();
    return SendToExtExec::New( children[ nch ].get_val(), loc, num );
}

Rp *SendToExtExec::exec_mp() {
    CbQueue dat;
    RO_CALL( serialize, children[ 0 ], &dat, nullptr );

    exec_list->results_mutex.lock();
    exec_list->senders << ExecList::DataToSend{ loc, num, std::move( dat ) };
    exec_list->results_mutex.unlock();

    // wake up the main thread to use this completed stuff
    exec_list->result_watcher.sig();
    return nullptr;
}


} // namespace RiotBase
