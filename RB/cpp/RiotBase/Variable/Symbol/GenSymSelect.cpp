#include "../../System/Scast.h"
#include "CommonSymbolicData.h"
#include "../Core/AutoRpDel.h"
#include "GenSymSelect.h"
#include "SymOpExtr.h"
#include "Lazyfier.h"
#include "Symbol.h"
#include "SymOp.h"
#include "Op.h"

namespace RiotBase {

GenSymSelect::GenSymSelect( Lazyfier *container, Rp *key ) : GenSymSelect( container ) {
    keys << key;
}

GenSymSelect::GenSymSelect( Lazyfier *container ) : container( container ) {
    container->sym_selects << this;
}

GenSymSelect::~GenSymSelect() {
    for( Rp *key : keys )
        RO_CALL( del, key );
    if ( container )
        container->sym_selects.remove_first_unordered( this );
}

std::string GenSymSelect::type_name() {
    return "GenSymSelect";
}

Rp *GenSymSelect::prox_val() const {
    Rp *val = RO_CALL( select_bb, container, keys[ 0 ] );
    for( PT i = 1; i < keys.size(); ++i ) {
        AutoRpDel old( val );
        val = RO_CALL( select_bb, val, keys[ i ] );
    }
    return val;
}

namespace {

// generic definition of mod_select
RiotBase_DEF_FUNC_2( mod_select, 2, tv0->isa<GenSymSelect>() ) {
    RO_CALL( self_add_key_b, v0, v1 ); // mandatory because if we don't do nothing with the value, we still have to insert the key

    GenSymSelect *ori = scast( v0 ), *res = GenSymSelect::New( ori->container );
    for( Rp *key : ori->keys )
        res->keys << RO_CALL( copy, key );
    res->keys << RO_CALL( copy, v1 );
    return res;
}
RiotBase_DEF_FUNC_2( mod_select, 2, tv0->isa<Lazyfier>() ) {
    RO_CALL( self_add_key_b, v0, v1 ); // mandatory because if we don't do nothing with the value, we still have to insert the key
    return GenSymSelect::New( scast( v0 ), RO_CALL( copy, v1 ) );
}
RiotBase_DEF_FUNC_2( mod_select, 1.5, tv0->unknown_value() or tv1->unknown_value() ) {
    Lazyfier *l = Lazyfier::New( RO_CALL( add_key_sb, v0, v1 ) );
    v0 = l;
    return GenSymSelect::New( l, RO_CALL( copy, v1 ) );
}
//RiotBase_DEF_FUNC_2( mod_select, 1.25, tv0->known_value() and tv1->unknown_value() ) {
//    Lazyfier *l = Lazyfier::New( RO_CALL( add_key_sb, v0, v1 ) );
//    v0 = l;
//    return GenSymSelect::New( l, RO_CALL( copy, v1 ) );
//}

//
#include "GenSymSelect_sb_extr_defs.h"

} // namespace

} // namespace RiotBase
