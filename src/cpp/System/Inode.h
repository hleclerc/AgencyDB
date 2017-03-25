#pragma once

#include <Hpipe/BinStream.h>
#include "DevId.h"

namespace AgencyDb {

/// object id
struct Inode {
    constexpr Inode( DevId dev = {}, PI64 num = 0 ) : num( num ), dev( dev ) {}

    template<class Cq>
    Inode( Hpipe::BinStream<Cq> &bq ) { if ( ( num = bq.read() ) ) dev = bq.read(); }

    template<class Cq>
    Inode( Hpipe::BinStream<Cq> &bq, DevId cur_dev ) : Inode( bq ) { if ( num and dev == cur_dev ) dev = 0; }

    bool          operator==     ( Inode b ) const { return num == b.num and ( num == 0 or  dev == b.dev ); }
    bool          operator!=     ( Inode b ) const { return num != b.num or  ( num != 0 and dev != b.dev ); }
    bool          operator<      ( Inode b ) const { return num != b.num ? num < b.num : ( num != 0 and dev < b.dev ); }
    explicit      operator bool  () const { return num; }
    void          write_to_stream( std::ostream &os ) const;

    template<class Bs>
    void          write_to      ( Bs &bq ) const { bq << num; if ( num ) bq << dev; }

    template<class Bs>
    void          write_to      ( Bs &bq, DevId cur_dev ) const { bq << num; if ( num ) bq << ( dev ? dev : cur_dev ); }

    template<class Bs>
    static Inode  read_from     ( Bs &bq ) { PI64 num = bq.read(); DevId dev; if ( num ) dev = bq.read(); return { dev, num }; }

    template<class Bs>
    static Inode  read_from     ( Bs &bq, DevId cur_dev ) { PI64 num = bq.read(); DevId dev; if ( num ) dev = DevId::read_from( bq, cur_dev ); return { dev, num }; }

    Inode         glo_dev       ( DevId cur_dev ) const { return { num ? dev.glo_dev( cur_dev ) : DevId{}, num }; }
    Inode         loc_dev       ( DevId cur_dev ) const { return { num ? dev.loc_dev( cur_dev ) : DevId{}, num }; }

    PI64          num; ///<
    DevId         dev; ///< session on which the object has been created
};

//void glo_to_loc( Inode &val, DevId cur_dev, UsrId cur_usr );
//void loc_to_glo( Inode &val, DevId cur_dev, UsrId cur_usr );


} // namespace AgencyDb
