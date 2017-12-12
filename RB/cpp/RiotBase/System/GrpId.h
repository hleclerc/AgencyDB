#pragma once

#include "Inode.h"

namespace RiotBase {

/// object id
struct GrpId : Inode {
    constexpr GrpId( DevId dev = 0, PI64 num = 0 ) : Inode( dev, num ) {}

    template<class Cq>
    GrpId( BinStream<Cq> &bq ) : Inode( bq ) {}

    template<class Cq>
    GrpId( BinStream<Cq> &bq, DevId cur_dev ) : Inode( bq, cur_dev ) {}

    bool         operator ==  ( GrpId b ) const { return num == b.num and ( num == 0 or  dev == b.dev ); }
    bool         operator !=  ( GrpId b ) const { return num != b.num or  ( num      and dev != b.dev ); }
    bool         operator <   ( GrpId b ) const { return num != b.num ? num < b.num : ( num != 0 and dev < b.dev ); }
    explicit     operator bool() const { return num; }
#include "Inode.h"


    template<class Cq>
    static GrpId read_from    ( BinStream<Cq> &bq ) { PI64 num = bq.read(); DevId dev; if ( num ) dev = bq.read(); return { dev, num }; }

    GrpId        glo_dev      ( DevId cur_dev ) const { return { num ? dev.glo_dev( cur_dev ) : DevId{}, num }; }
    GrpId        loc_dev      ( DevId cur_dev ) const { return { num ? dev.loc_dev( cur_dev ) : DevId{}, num }; }
};

void glo_to_loc( GrpId &grp, DevId cur_dev, UsrId cur_usr );
void loc_to_glo( GrpId &grp, DevId cur_dev, UsrId cur_usr );

} // namespace RiotBase
