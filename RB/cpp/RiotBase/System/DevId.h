#pragma once

#include "TypeConfig.h"
#include "GloToLoc.h"
#include "LocToGlo.h"
#include "Print.h"

namespace RiotBase {

///
struct DevId {
    using T = PI64;

    constexpr DevId( T val = 0 ) : val( val ) {}

    DevId        glo_dev           ( DevId cur_dev ) const { return val ? val : cur_dev.val; }
    DevId        loc_dev           ( DevId cur_dev ) const { return val != cur_dev.val ? val : 0; }
    void         write_to_stream   ( std::ostream &os ) const;

    template<class Bq>
    void         write_to  ( Bq &bq ) const { bq.write_le( val ); }

    template<class Bq>
    void         write_to  ( Bq &bq, DevId cur_dev ) const { bq.write_le( val ? val : cur_dev.val ); }

    template<class Bq>
    static DevId read_from ( Bq &bq ) { T res = bq.read_le(); return res; }

    template<class Bq>
    static DevId read_from ( Bq &bq, DevId cur_dev ) { T res = bq.read_le(); return res == cur_dev ? 0 : res; }

    bool         operator==( DevId dev ) const { return val == dev.val; }
    bool         operator!=( DevId dev ) const { return val != dev.val; }
    bool         operator< ( DevId dev ) const { return val <  dev.val; }
    operator     bool      () const { return val; }

    T            val;
};

void glo_to_loc( DevId &dev, DevId cur_dev, UsrId cur_usr );
void loc_to_glo( DevId &dev, DevId cur_dev, UsrId cur_usr );


/// predefined dev_ids
namespace KnownDevId {
    constexpr DevId dev_known_inodes { 1 }; ///< dev for predefined/reserved inodes
    constexpr DevId dev_perm_storage { 2 }; ///< dev for the permanent storage
    constexpr DevId first_free_dev   { 3 }; ///<
}

} // namespace RiotBase
