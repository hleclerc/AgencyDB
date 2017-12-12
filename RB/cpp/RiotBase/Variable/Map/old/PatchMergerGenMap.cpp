#include "PatchMergerGenMap.h"
#include "../Core/Category.h"
#include "GenMap.h"

namespace RiotBase {

PatchMergerGenMap::PatchMergerGenMap( GenMap *m ) : m( m ) {
}

bool PatchMergerGenMap::is_valid( const CbString &msg ) const {
    return true;
}

void PatchMergerGenMap::set_creator( UsrId usr_id ) {
    TODO;
}

void PatchMergerGenMap::new_patch( CbString &cb, const FqId &id, UsrId as_usr, PT nb_rem_unk, PT nb_rem_std ) {
    TODO;
    // BinStream<CbString> data( &cp );

    //    while ( true ) {
    //        PI8 code = data.read_byte();
    //        if ( data.error() )
    //            break;

    //        switch ( code ) {
    //        case GenMap::NEW_NODE: {
    //            Inode id; id.read_from( data, cur_dev );
    //            CbString key_dat = data.read();
    //            CbString val_dat = data.read();
    //            if ( data.error() )
    //                return;

    //            Rp *key = m->category_key->create( key_dat, db );
    //            Rp *val = m->category_val->create( val_dat, db );
    //            new_items << Item{ id, key, val };
    //            break;
    //          }
    //        case GenMap::MOD_KEY:
    //            TODO;
    //            break;
    //        case GenMap::MOD_VAL:
    //            TODO;
    //            break;
    //        default:
    //            return; // weird
    //        }
    //    }
}

void PatchMergerGenMap::unk_patch( CbString &cb, const FqId &id, UsrId as_usr, PT nb_rem_new, PT nb_rem_std ) {
    // CbString cp = cb; BinStream<CbString> data( &cp );
}

void PatchMergerGenMap::std_patch( CbString &cb, const FqId &id, UsrId as_usr, PT nb_rem_unk, PT nb_rem_new ) {
    // CbString cp = cb; BinStream<CbString> data( &cp );
}

void PatchMergerGenMap::undo( const CbString &data, const FqId &id ) {
    TODO;
}

void PatchMergerGenMap::finalize( const FqId &id ) {
    for( Item &item : new_items )
        m->add_item( item.id, item.key, item.val );
}

void PatchMergerGenMap::transmission( BinStream<CbQueue> bq, const CbString &patch_data, const FqId &id, UsrId as_usr ) {
    TODO;
}


} // namespace RiotBase
