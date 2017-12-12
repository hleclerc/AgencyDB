#pragma once

#include "../../System/UsrId.h"

namespace RiotBase {
class Rp;

///
struct CursorData {
    struct Id {
        bool  operator<( const Id &c ) const { return dev_id == c.dev_id ? loc_id < c.loc_id : dev_id < c.dev_id; }

        DevId dev_id;
        PI64  loc_id;
        UsrId usr_id;
        void *int_id;
    };

    CursorData( Rp *val, DevId dev_id, PI64 loc_id, UsrId usr_id, void *int_id = 0 );
    CursorData( CursorData &&c );
    ~CursorData();

    Rp *val;
    Id  id;
};

} // namespace RiotBase
