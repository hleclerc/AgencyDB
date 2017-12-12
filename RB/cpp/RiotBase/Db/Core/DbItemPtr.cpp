#include "../../System/Ccast.h"
#include "DbItemPtr.h"
#include "DbItem.h"

namespace RiotBase {

DbItemPtr::DbItemPtr( RcPtr<DbItem> db_item ) : db_item( db_item ) {
}

std::string DbItemPtr::type_name() {
    return "DbItem";
}

Rp **DbItemPtr::prox_ptr() {
    return &db_item->patch_manager.rp;
}

const Rp *DbItemPtr::prox_ref() const {
    return ccast( db_item.ptr() )->patch_manager.rp;
}

void DbItemPtr::write_structure( std::ostream &os ) const {
    os << "DbItemPtr(";
    if ( db_item->patch_manager.rp )
        RO_CALL( write_structure, db_item->patch_manager.rp, os );
    os << ')';
}

void DbItemPtr::self_then( Callback *c ) const {
    RO_CALL( self_then, ccast( db_item.ptr() )->patch_manager.rp, c );
}

} // namespace RiotBase
