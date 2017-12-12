//#include "../../Variable/Core/OtMessage.h"
#include "../../Variable/Core/Methods.h"
#include "../../System/AutoPtr.h"
#include "DbCallback.h"
#include "DbCommands.h"
// #include "DbSession.h"
#include "DbItem.h"
#include "../Db.h"

namespace RiotBase {

DbCallback::DbCallback( DbItem *db_item ) : db_item( db_item ) {
}

void DbCallback::exec( Rp **rp ) {
    PRINT( "mod" );

//    Db *db = db_item->db;

//    // send changes to sessions
//    Vec<FqId> dev_ids; dev_ids.reserve( db_item->sessions.size() );
//    for( const RcPtr<DbSession> &s : db_item->sessions )
//        dev_ids << s->fqid();

//    if ( AutoPtr<OtMessage> msg = RO_CALL( get_changes_for, db_item->rp, dev_ids, db->fqid() ) ) {
//        for( OtMessage *p = msg.ptr(); p; p = p->prev.ptr() )
//            for( RcPtr<DbSession> &s : db_item->sessions )
//                if ( p->dst.contains( s->get_dev_id() ) )
//                    s->send_new_changes( db_item->inode, p->msg );
//    }

//    // send changes the permanent storage
//    if ( db->data_files.size() ) {
//        if ( AutoPtr<OtMessage> otm = RO_CALL( get_changes_for, db_item->rp, FqId{ db, KnownInodes::dev_perm_storage, {} }, db->fqid() ) ) {
//            // message
//            CbQueue cq; BinStream<CbQueue> bq( &cq );
//            bq << DbCommands::new_changes;
//            bq << db_item->inode.glo_dev( db->get_dev_id() );
//            bq << db_item->category->name();
//            bq << std::move( otm->msg );

//            // write
//            PT num_data_file = 0;
//            db->data_files[ num_data_file ]->sched_write( std::move( cq ) );
//        }
//    }
}

std::string DbCallback::name() const {
    return "CallbackDb";
}


} // namespace RiotBase
