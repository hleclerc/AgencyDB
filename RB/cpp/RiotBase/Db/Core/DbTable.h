#pragma once

#include "DbItem.h"

namespace RiotBase {

///
class DbTable {
public:
    struct Cmp {
        bool operator()( const DbItem &a, const DbItem &b ) const { return a.inode < b.inode; }
        bool operator()( const DbItem &a, Inode         b ) const { return a.inode < b      ; }
        bool operator()( Inode         a, const DbItem &b ) const { return a       < b.inode; }
    };
    using Map = boost::intrusive::avl_multiset<DbItem,
        boost::intrusive::compare<Cmp>,
        boost::intrusive::member_hook<
            DbItem,
            boost::intrusive::avl_set_member_hook<>,
            &DbItem::table_hook
        >
    >;

    ~DbTable();

    DbItem                *find  ( Inode inode );
    DbItem                *insert( DbItem *item );
    void                   clear ();

    Map                    data;
};

} // namespace RiotBase
