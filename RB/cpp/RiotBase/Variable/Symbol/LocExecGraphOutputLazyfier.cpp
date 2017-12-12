#include "LocExecGraphOutputLazyfier.h"
#include "ExecList.h"

namespace RiotBase {

//namespace {

//void cancel_execution_rec( Rp *rp ) {
//    if ( CommonSymbolicData *csd = RO_CALL( common_symbolic_data, rp ) ) {
//        PT &status = reinterpret_cast<PT &>( csd->op_mp );
//        if ( status == ExecList::OBJ_STATE_EXECUTING ) {
//            status = ExecList::OBJ_STATE_CANCELED;
//        } else {
//            for( int i = 0, n = RO_CALL( nb_symbolic_children, rp ); i < n; ++i )
//                cancel_execution_rec( RO_CALL( symbolic_child, rp, i ) );
//        }
//    }
//}

//}

LocExecGraphOutputLazyfier::LocExecGraphOutputLazyfier( Rp *ch ) : RpTemplSym<LocExecGraphOutputLazyfier,1>( RO_CALL( category, ch ), ch ) {
}

std::string LocExecGraphOutputLazyfier::type_name() {
    return "LocExecGraphOutputLazyfier";
}

void LocExecGraphOutputLazyfier::del() {
    // del ne peut venir du main thread
    //  - ou bien depuis le result_watcher (-> children[ 0 ]==0), depuis le main_thread
    //  - ou bien de façon standard depuis le main_thread aussi
    if ( cpt_use )
        --cpt_use;
    else {
        if ( Rp *ch = children[ 0 ] ) {
            exec_list->mutex.lock();
            RO_CALL( del_or_cancel_exec, ch );
            exec_list->mutex.unlock();
            children[ 0 ] = 0;
        }
        RpTempl<LocExecGraphOutputLazyfier>::_del( this );
    }
}

Rp *LocExecGraphOutputLazyfier::exec_mp() {
    // add this to completed stuff in exec_list
    exec_list->results_mutex.lock();
    exec_list->legoll << this;
    exec_list->results_mutex.unlock();

    // wake up the main thread to use this completed stuff
    exec_list->result_watcher.sig();
    return nullptr;
}


} // namespace RiotBase
