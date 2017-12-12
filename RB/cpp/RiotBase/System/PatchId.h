#pragma once

#include "../System/Print.h"
#include "Inode.h"

namespace RiotBase {

struct PatchId {
    PatchId( DevId dev = 0, PI64 num = 0 ) : num( num ), dev( dev ) { }

    template<class Cq>
    PatchId( BinStream<Cq> &bq ) { if ( ( num = bq.read() ) ) dev = bq.read(); }

    template<class Cq>
    PatchId( BinStream<Cq> &bq, DevId cur_dev ) : PatchId( bq ) { if ( num and dev == cur_dev ) dev = 0; }

    bool           operator==     ( PatchId p ) const { return num == p.num and ( num == 0 or  dev == p.dev ); }
    bool           operator!=     ( PatchId b ) const { return num != b.num or  ( num != 0 and dev != b.dev ); }
    bool           operator<      ( PatchId b ) const { return num != b.num ? num < b.num : ( num != 0 and dev < b.dev ); }
    operator       bool           () const { return num; }
    void           write_to_stream( std::ostream &os ) const;

    template<class Cq>
    BinStream<Cq> &write_to       ( BinStream<Cq> &bq ) const { bq << num; if ( num ) bq << dev; return bq; }

    template<class Cq>
    static PatchId read_from      ( BinStream<Cq> &bq ) { PI64 num = bq.read(); DevId dev; if ( num ) dev = bq.read(); return { dev, num }; }

    PatchId        glo_dev        ( DevId cur_dev ) const { return { num ? dev.glo_dev( cur_dev ) : DevId{}, num }; }
    PatchId        loc_dev        ( DevId cur_dev ) const { return { num ? dev.loc_dev( cur_dev ) : DevId{}, num }; }

    PI64           num; ///< number of the patch in the considered device
    DevId          dev; ///< device where the patch has been created
};

void glo_to_loc( PatchId &pid, DevId cur_dev, UsrId cur_usr );
void loc_to_glo( PatchId &pid, DevId cur_dev, UsrId cur_usr );

} // namespace RiotBase

