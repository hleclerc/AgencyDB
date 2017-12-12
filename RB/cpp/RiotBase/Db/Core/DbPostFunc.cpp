#include "DbTrackerClient.h"
#include "DbPostFunc.h"
#include "../Db.h"

namespace RiotBase {

DbPostFunc::DbPostFunc( Db *db ) : db( db ) {
}

bool DbPostFunc::exec() {
    bool ret = false;

    // data to be sent to the tracker
//    if ( not db->items_to_be_signaled_to_tracker.empty() ) {
//        for( DbItem *item : db->items_to_be_signaled_to_tracker ) {
//            item->patch_manager.freeze_last_mod();
//            db->tracker_client_session->send_bind_obj( item->inode, item->patch_manager.patch_ids );
//        }
//        db->items_to_be_signaled_to_tracker.clear();
//        db->tracker_client_session->flush();
//        ret = true;
//    }

    //        //
    //        ret |= Callback::exec_all();

    //        //
    //        if ( not db->changed_sessions.empty() ) {
    //            db->changed_sessions.clear( []( DbSession *s ) { s->flush(); } );
    //            ret = true;
    //        }

    return ret;
}


} // namespace RiotBase
