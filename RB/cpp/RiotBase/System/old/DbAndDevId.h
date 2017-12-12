#pragma once

#include "DevId.h"

namespace RiotBase {
class Db;

///
struct DbAndDevId {
    constexpr DbAndDevId( Db *db = 0, DevId dev_id = {} ) : db( db ), dev_id( dev_id ) {}

    DbAndDevId glo_dev         ( DevId cur_dev ) const { return { db, dev_id.glo_dev( cur_dev ) }; }
    DbAndDevId loc_dev         ( DevId cur_dev ) const { return { db, dev_id.loc_dev( cur_dev ) }; }
    void        write_to_stream( std::ostream &os ) const { os << db << ":" << dev_id; }

    bool        operator==     ( DbAndDevId d ) const { return db == d.db and dev_id == d.dev_id; }
    bool        operator!=     ( DbAndDevId d ) const { return db != d.db or  dev_id != d.dev_id; }
    bool        operator<      ( DbAndDevId d ) const { return std::tie( db, dev_id ) < std::tie( d.db, d.dev_id ); }
    operator    bool           () const { return db; }

    Db   *db;
    DevId dev_id;
};

} // namespace RiotBase
