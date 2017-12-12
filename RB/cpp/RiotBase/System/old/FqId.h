#pragma once

#include "UsrId.h"

namespace RiotBase {
class Db;

///
class FqId {
public:
    constexpr FqId( Db *db = 0, DevId dev = {}, UsrId usr = {} ) : db( db ), dev( dev ), usr( usr ) {}

    void        write_to_stream( std::ostream &os ) const { os << db << ":" << dev << "," << usr; }

    bool        operator==     ( const FqId &d ) const { return std::tie( db, dev, usr ) == std::tie( d.db, d.dev, d.usr ); }
    bool        operator!=     ( const FqId &d ) const { return std::tie( db, dev, usr ) != std::tie( d.db, d.dev, d.usr ); }
    bool        operator<      ( const FqId &d ) const { return std::tie( db, dev, usr ) <  std::tie( d.db, d.dev, d.usr ); }
    operator    bool           () const { return db; }

    Db   *db;  ///< which instance
    DevId dev; ///< which global dev, on db
    UsrId usr; ///< which user
};

} // namespace RiotBase
