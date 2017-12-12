#include "OtWrapperStringCursor.h"
#include "../../System/Scast.h"
#include "../Number/Bn.h"

namespace RiotBase {

OtWrapperStringCursor::OtWrapperStringCursor( OtWrapperString *pcs, OtWrapperString::Cursor *cur ) : pcs( pcs ), cur( cur ) {
}

OtWrapperStringCursor::~OtWrapperStringCursor() {
}

std::string OtWrapperStringCursor::type_name() {
    return "OtWrapperStringCursor";
}

void OtWrapperStringCursor::write_to_stream( std::ostream &os ) const {
    if ( cur ) os << cur->pos; else os << "deleted";
}

void OtWrapperStringCursor::write_structure( std::ostream &os ) const {
    RO_CALL( write_structure, pcs, os << "OtWrapperStringCursor(" );
    os << ')';
}

bool OtWrapperStringCursor::to_PT( PT *res, PT nb_items ) const {
    if ( not cur )
        return false;
    *res = cur->pos;
    return true;
}

bool OtWrapperStringCursor::to_Bool( bool *res, PT nb_items ) const {
    if ( not cur )
        return false;
    *res = cur->pos;
    return true;
}

Rp *OtWrapperStringCursor::copy() const {
    return Bn<PT>::New( cur ? cur->pos : 0 );
}

namespace {

RiotBase_DEF_FUNC_2( self_set_b, 1, tv0->isa<OtWrapperStringCursor>() and tv1->has_surdef_for( RF_to_PT() ) ) {
    OtWrapperStringCursor *p = scast( v0 );
    PT val = 0; RO_CALL( to_PT, v1, &val, 1 );
    p->pcs->_set_cursor( p->cur, val );
}

RiotBase_DEF_FUNC_2( self_add_b, 1, tv0->isa<OtWrapperStringCursor>() and tv1->has_surdef_for( RF_to_PT() ) ) {
    OtWrapperStringCursor *p = scast( v0 );
    SI32 val = 0; RO_CALL( to_SI32, v1, &val, 1 );
    if ( p->cur and val )
        p->pcs->_set_cursor( p->cur, p->cur->pos + val );
}

RiotBase_DEF_FUNC_2( self_sub_b, 1, tv0->isa<OtWrapperStringCursor>() and tv1->has_surdef_for( RF_to_PT() ) ) {
    OtWrapperStringCursor *p = scast( v0 );
    SI32 val = 0; RO_CALL( to_SI32, v1, &val, 1 );
    if ( p->cur and val )
        p->pcs->_set_cursor( p->cur, p->cur->pos - val );
}

} // namespace

} // namespace RiotBase
