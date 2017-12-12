#include "../../Variable/Symbol/SetLazyVal.h"
#include "../../Variable/Core/Methods.h"
#include "ObjInPeer.h"
#include "ObjInFile.h"
#include "DataFile.h"
#include "DbItem.h"
#include "../Db.h"

namespace RiotBase {

CycleEndedList<DbItem,DbItem::Pwdb> DbItem::list_with_dbuff;
std::mutex DbItem::mutex_lwdb_list;

DbItem::DbItem( const Category *category, Inode inode, Db *db, Rp *rp ) : rp( rp ), callback( this ), category( category ), inode( inode ), db( db ) {
    callback.cpt_use += 2; // we don't want callback to be deleted since it is stored locally
    prev_with_dbuff  = 0;
    datafile_buffer  = 0;
    datafile         = 0;
    cpt_use          = 0;

    if ( this->rp )
        RO_CALL( self_on_change, this->rp, &callback );
}

DbItem::~DbItem() {
    if ( rp )
        RO_CALL( del, rp );
    if ( datafile )
        datafile->to_read.remove_first_unordered( this );
}

Rp *&DbItem::check_rp() {
    if ( not rp ) {
        auto purely_a_client = [ this ](){
            for( const RcPtr<DbSession> &s : sessions )
                if ( s->is_a_server_session() )
                    return false;
            return bool( sessions.size() );
        };

        if ( datafile ) {
            rp = ObjInFile::mkl( category );
            //datafile->sched_read( this );
            //rp = new_Symbol( "Sym<File," + to_string( inode ) + ">", category );
        } else if ( purely_a_client() ) {
            TODO;
            //sessions[ 0 ]->send_req_content( inode, category );
            //rp = new_Symbol( "Sym<ExtS," + to_string( inode ) + ">", category );
        } else {
            rp = category->create();
        }
        RO_CALL( self_on_change, rp, &callback );
    }
    return rp;
}

bool DbItem::exec_all() {
    bool res = not list_with_dbuff.empty();

    // new data from files
    for( DbItem *item : list_with_dbuff ) {
        // prepare the data
        CbString cs( IKnowWhatIDo(), item->datafile_buffer, 0, item->datafile_buffer->used );
        dec_ref( item->datafile_buffer );
        item->datafile_buffer = 0;

        // make the new item
        Rp *rp = item->rp;
        item->rp = 0; // item->category->create();

        CbQueue ans;
        IMPORTANT_TODO;
        // RO_CALL( app_changes, item->rp, &ans, cs, item->db->fqid(), FqId{ item->db, KnownInodes::dev_perm_storage, {} }, item->db->fqid() );

        // add the callback
        RO_CALL( self_on_change, item->rp, &item->callback );

        // lazy reg
        std::swap( item->rp, rp );
        set_lazy_val( item->rp, rp );
    }
    list_with_dbuff.clear();

    return res;
}

} // namespace RiotBase
