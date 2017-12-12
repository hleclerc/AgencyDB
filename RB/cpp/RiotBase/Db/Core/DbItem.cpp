//#include "ObjInPeer.h"
//#include "ObjInFile.h"
//#include "DataFile.h"

#include "DbItem.h"
#include "../Db.h"

namespace RiotBase {

///
class DbCallback : public Callback {
public:
    DbCallback( DbItem *db_item ) : db_item( db_item ) { }
    virtual void        exec( Rp **rp = 0 ) { db->items_with_updates_to_be_sent << db_item; }
        virtual std::string name() const { return "CallbackDb"; }
    DbItem             *db_item;
};

DbItem::DbItem( Db *db, Inode inode, Rp *rp ) : inode( inode ), patch_manager( rp ) {
    RO_CALL( self_on_change, patch_manager.rp, new DbCallback( this ) );
    prev_in_db_cycled_list = nullptr;
    cpt_use                = 0;

    //    prev_with_dbuff  = 0;
    //    datafile_buffer  = 0;
    //    datafile         = 0;

    //
    db->items_to_be_signaled_to_tracker << this;
}

DbItem::~DbItem() {
    //    if ( datafile )
    //        datafile->to_read.remove_first_unordered( this );
}

//bool DbItem::exec_all() {
//    bool res = not list_with_dbuff.empty();

//    // new data from files
//    for( DbItem *item : list_with_dbuff ) {
//        // prepare the data
//        CbString cs( IKnowWhatIDo(), item->datafile_buffer, 0, item->datafile_buffer->used );
//        dec_ref( item->datafile_buffer );
//        item->datafile_buffer = 0;

//        // make the new item
//        Rp *rp = item->rp;
//        item->rp = 0; // item->category->create();

//        CbQueue ans;
//        IMPORTANT_TODO;
//        // RO_CALL( app_changes, item->rp, &ans, cs, item->db->fqid(), FqId{ item->db, KnownInodes::dev_perm_storage, {} }, item->db->fqid() );

//        // add the callback
//        RO_CALL( self_on_change, item->rp, &item->callback );

//        // lazy reg
//        std::swap( item->rp, rp );
//        set_lazy_val( item->rp, rp );
//    }
//    list_with_dbuff.clear();

//    return res;
//}

} // namespace RiotBase
