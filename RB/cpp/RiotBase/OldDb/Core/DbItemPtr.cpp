#include "../../System/Ccast.h"
#include "DbItemPtr.h"
#include "DbItem.h"

namespace RiotBase {

DbItemPtr::DbItemPtr( DbItem *db_item ) : db_item( db_item ) {
}

std::string DbItemPtr::type_name() {
    return "DbItem";
}

Rp **DbItemPtr::prox_ptr() {
    return &db_item->check_rp();
}

const Rp *DbItemPtr::prox_ref() const {
    return ccast( db_item )->check_rp();
}

void DbItemPtr::write_structure( std::ostream &os ) const {
    os << "DbItemPtr(";
    if ( db_item->rp )
        RO_CALL( write_structure, db_item->rp, os );
    os << ')';
}

void DbItemPtr::self_then( Callback *c ) const {
    RO_CALL( self_then, ccast( db_item )->check_rp(), c );
}

} // namespace RiotBase
