#include "Methods.h"

namespace RiotBase {

CursorData::CursorData( Rp *val, DevId dev_id, PI64 loc_id, UsrId usr_id, void *int_id ) : val( val ), id{ dev_id, loc_id, usr_id, int_id } {
}

CursorData::CursorData( CursorData &&c ) {
    val = c.val;
    id  = c.id;

    c.val = 0;
}

CursorData::~CursorData() {
    if ( val )
        RO_CALL( del, val );
}



} // namespace RiotBase
