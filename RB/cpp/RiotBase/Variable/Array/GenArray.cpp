#include "../../System/Ccast.h"
#include "../../System/Scast.h"
#include "../Core/Category.h"
#include "../Core/Surdef.h"
#include "../Core/Error.h"
#include "../Number/Bn.h"
// #include "PatchMergerGenArray.h"
#include "GenArraySelect.h"
#include "GenArray.h"

namespace RiotBase {

Rp *new_GenArray( const Category *category_array, const Category *category_val, UsrId usr_id ) {
    return GenArray::New( category_array, category_val, usr_id );
}

GenArray::GenArray( const Category *category_array, const Category *category_val, UsrId usr_id ) : OtWrapper<GenArray>( usr_id ), category_array( category_array ), category_val( category_val ) {
}

std::string GenArray::type_name() {
    return "GenArray";
}

void GenArray::write_to_stream( std::ostream &os ) const {
    os << '[';
    int cpt = 0;
    for( const AutoRpDel &item : items )
        RO_CALL( write_to_stream, item.rp, os << ( cpt++ ? "," : "" ) );
    os << ']';
}

Rp *GenArray::select( const Rp *key, bool mod_right ) const {
    PT index;
    if ( not RO_CALL( to_PT, key, &index, 1 ) )
        return Error::New( "Bad index (cannot be converted to PT)" );
    if ( index >= items.size() )
        return Error::New( "Bad index (>= size)" );
    return GenArraySelect::New( ccast( this ), index );
}

void GenArray::self_push_back_b( Rp *val ) {
    self_push_back_o( RO_CALL( copy, val ) );
}

void GenArray::self_push_back_o( Rp *val ) {
    items << val;
}

void GenArray::_reserve( PT size ) {
    items.reserve( size );
}

const Category *GenArray::category() const {
    return category_array;
}

Rp *GenArray::copy() const {
    GenArray *res = GenArray::New( category_array, category_val, UsrId{} );
    res->_reserve( items.size() );
    for( const AutoRpDel &item : items )
        res->self_push_back_b( item.rp );
    return res;
}

Rp *GenArray::get_size() const {
    return Bn<PT>::New( items.size() );
}

Rp *GenArray::front_b() const {
    if ( not items.size() )
        return Error::New( "back on a 0 sized array" );
    return RO_CALL( copy, items.front().rp );
}

Rp *GenArray::front_list_b() const {
    GenArray *res = GenArray::New( category_array, category_val );
    if ( items.size() )
        res->self_push_back_b( items.front().rp );
    return res;
}

Rp *GenArray::back_b() const {
    if ( not items.size() )
        return Error::New( "back on a 0 sized array" );
    return RO_CALL( copy, items.back().rp );
}

Rp *GenArray::back_list_b() const {
    GenArray *res = GenArray::New( category_array, category_val );
    if ( items.size() )
        res->self_push_back_b( items.back().rp );
    return res;
}

Rp *GenArray::head_b() const {
    GenArray *res = GenArray::New( category_array, category_val );
    if ( items.size() ) {
        res->items.reserve( items.size() - 1 );
        for( PT i = 0; i < items.size() - 1; ++i )
            res->self_push_back_b( items[ i ].rp );
    }
    return res;
}

Rp *GenArray::tail_b() const {
    GenArray *res = GenArray::New( category_array, category_val );
    if ( items.size() ) {
        res->items.reserve( items.size() - 1 );
        for( PT i = 1; i < items.size(); ++i )
            res->self_push_back_b( items[ i ].rp );
    }
    return res;
}

void GenArray::apply( std::function<void (const Rp *)> f ) const {
    for( const AutoRpDel &item : items )
        f( item.rp );
}

// apply_op
void GenArray::apply_op( op_insert, Aod &aod, PT pos, const Variable &var ) {
    if ( pos <= items.size() ) {
        items.insert( pos, RO_CALL( copy, var.rp ) );
        for( GenArraySelect *s : set_array_selects )
            if ( s->pos >= pos )
                ++s->pos;
    }
    sig_change();
}

void GenArray::apply_op( op_remove, Aod &aod, PT pos, PT len ) {
    PT beg = std::min( pos, items.size() );
    PT end = std::min( pos + len, items.size() );
    items.remove_some( beg, end - beg );

    for( GenArraySelect *s : set_array_selects ) {
        if ( s->pos >= beg ) {
            if ( s->pos >= end )
                s->pos -= len;
            else
                s->pos = items.size();
        }
    }

    sig_change();
}

void GenArray::apply_op( op_remove_und, Aod &aod, PT pos, const Variable &var ) {
    apply_op( op_remove(), aod, pos, 1 );
}

// transmission
void GenArray::transmission( op_remove_und, BBQ br, PT pos, Variable &old ) {
    transmission( op_remove{}, br, pos, 1 );
}

// register (from a transmission)
void GenArray::register_patch_op( op_remove, BBQ bq, PT pos, PT len ) {
    for( PT i = std::min( pos, items.size() ); i < std::min( pos + len, items.size() ); ++i )
        register_patch_op( op_remove_und(), bq, i, reinterpret_cast<Variable &>( items[ i ] ) );
}

// undo
void GenArray::undo_patch( op_insert, Aod &aod, PT pos, const Variable &var ) {
    apply_op( op_remove{}, aod, pos, 1 );
}

void GenArray::undo_patch( op_remove, Aod &aod, PT pos, PT len ) {
    WARNING( "Undo of a basic remove should not happen" );
}

void GenArray::undo_patch( op_remove_und, Aod &aod, PT pos, const Variable &var ) {
    apply_op( op_insert{}, aod, pos, var );
}

namespace {

RiotBase_DEF_FUNC_2( select_bb, 1, tv0->isa<GenArray>() and tv1->known_term_value() ) {
    return static_cast<const GenArray *>( v0 )->select( v1, false );
}

RiotBase_DEF_FUNC_2( mod_select, 1, tv0->isa<GenArray>() and tv1->known_term_value() ) {
    return static_cast<GenArray *>( v0 )->select( v1, true ); // both select_bb and mod_select will actually return a GenArraySelect (because we don't want to copy the stuff)
}

RiotBase_DEF_FUNC_2( self_push_back_b, 1, tv0->isa<GenArray>() and tv1->known_term_value() ) {
    static_cast<GenArray *>( v0 )->self_push_back_b( RO_CALL( copy, v1 ) );
}

RiotBase_DEF_FUNC_2( join_bb, 1, tv0->isa<GenArray>() and tv1->known_term_value() ) {
    const GenArray *a = scast( v0 );
    if ( a->items.size() == 0 )
        return a->category_val->create();
    if ( a->items.size() == 1 )
        return RO_CALL( copy, a->items[ 0 ].rp );
    Rp *res = RO_CALL( add_bb, a->items[ 0 ].rp, v1 );
    for( PT i = 1; i < a->items.size() - 1; ++i )
        res = RO_CALL( add_ob, RO_CALL( add_ob, res, a->items[ i ].rp ), v1 );
    return RO_CALL( add_ob, res, a->items.back().rp );
}

RiotBase_DEF_FUNC_3( slice_bbb, 1, tv0->isa<GenArray>() and tv1->known_term_value() and tv2->known_term_value() ) {
    PT beg, end;
    if ( not RO_CALL( to_PT, v1, &beg, 1 ) or not RO_CALL( to_PT, v2, &end, 1 ) )
        return Error::New( "Bad index (not convertible)" );
    const GenArray *a = static_cast<const GenArray *>( v0 );
    end = std::min( end, a->items.size() );
    beg = std::min( beg, end );
    GenArray *res = GenArray::New( a->category_array, a->category_val );
    res->items.reserve( end - beg );
    for( ; beg < end; ++beg )
        res->self_push_back_b( a->items[ beg ].rp );
    return res;
}

}

} // namespace RiotBase
