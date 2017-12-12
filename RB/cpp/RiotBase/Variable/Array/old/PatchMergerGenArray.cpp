#include "PatchMergerGenArray.h"
#include "../Core/Category.h"
#include "GenArray.h"

namespace RiotBase {

PatchMergerGenArray::PatchMergerGenArray( GenArray *m ) : m( m ) {
}

bool PatchMergerGenArray::is_valid( const CbString &msg ) const {
    return true;
}

void PatchMergerGenArray::set_creator( UsrId usr_id ) {
    TODO;
}

void PatchMergerGenArray::new_patch( CbString &data, const FqId &id, UsrId as_usr, PT nb_rem_unk, PT nb_rem_std ) {
    TODO;
}

void PatchMergerGenArray::unk_patch( CbString &data, const FqId &id, UsrId as_usr, PT nb_rem_new, PT nb_rem_std ) {
    TODO;
}

void PatchMergerGenArray::std_patch( CbString &data, const FqId &id, UsrId as_usr, PT nb_rem_unk, PT nb_rem_new ) {
    TODO;
}

void PatchMergerGenArray::undo( const CbString &data, const FqId &id ) {
    TODO;
}

void PatchMergerGenArray::finalize( const FqId &id ) {
}

void PatchMergerGenArray::transmission( BinStream<CbQueue> bq, const CbString &patch_data, const FqId &id, UsrId as_usr ) {
    TODO;
}


} // namespace RiotBase
