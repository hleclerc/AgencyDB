#pragma once

#include "Inode.h"

namespace RiotBase {

/// object id
struct UsrId : Inode {
    constexpr UsrId( DevId dev = 0, PI64 num = 0 ) : Inode( dev, num ) {}

    template<class Cq>
    UsrId( BinStream<Cq> &bq ) : Inode( bq ) {}

    template<class Cq>
    UsrId( BinStream<Cq> &bq, DevId cur_dev, UsrId cur_usr ) : Inode( bq, cur_dev ) { if ( *this == cur_usr ) num = 0; }

    bool         operator ==  ( UsrId b ) const { return num == b.num and ( num == 0 or  dev == b.dev ); }
    bool         operator !=  ( UsrId b ) const { return num != b.num or  ( num      and dev != b.dev ); }
    bool         operator <   ( UsrId b ) const { return num != b.num ? num < b.num : ( num != 0 and dev < b.dev ); }
    explicit     operator bool() const { return num; }

    template<class Cq>
    static UsrId read_from    ( BinStream<Cq> &bq ) { PI64 num = bq.read(); DevId dev; if ( num ) dev = bq.read(); return { dev, num }; }

    UsrId        _glo_dev     ( DevId cur_dev ) const { return { num ? dev.glo_dev( cur_dev ) : DevId{}, num }; }
    UsrId        _loc_dev     ( DevId cur_dev ) const { return { num ? dev.loc_dev( cur_dev ) : DevId{}, num }; }

    UsrId        glo_dev      ( DevId cur_dev, UsrId cur_usr ) const { return ( num ? *this : cur_usr )._glo_dev( cur_dev ); }
    UsrId        loc_dev      ( DevId cur_dev, UsrId cur_usr ) const { return *this != cur_usr ? _loc_dev( cur_dev ) : UsrId{}; }
};

void glo_to_loc( UsrId &usr, DevId cur_dev, UsrId cur_usr );
void loc_to_glo( UsrId &usr, DevId cur_dev, UsrId cur_usr );


/// predefined inodes (and data to construct new ones)
namespace KnownUsrId {
    constexpr UsrId root_usr                    { KnownDevId::dev_known_inodes, 4 }; ///< id of the root user
    constexpr UsrId anon_usr                    { KnownDevId::dev_known_inodes, 5 }; ///< id of the anonymous user
}

} // namespace RiotBase
