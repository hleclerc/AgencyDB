#include "../../Variable/Core/Methods.h"
#include "DbTable.h"

namespace RiotBase {

DbTable::~DbTable() {
    clear();
}

DbItem *DbTable::find( Inode inode ) {
    Map::iterator iter = data.find( inode, Cmp() );
    return iter != data.end() ? &*iter : 0;
}

DbItem *DbTable::insert( DbItem *item ) {
    data.insert( *item );
    return item;
}

void DbTable::clear() {
    Vec<DbItem *> items;
    for( DbItem &item : data )
        items << &item;
    data.clear();
    for( DbItem *item : items )
        dec_ref( item );

}

} // namespace RiotBase
