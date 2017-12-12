#include "../Symbol/LazySourceKnownValue.h"
#include "../Symbol/LazySourceInfo.h"
#include "../../System/BinStream.h"
#include "../../System/Scast.h"
#include "../Array/GenArray.h"
#include "../Core/Surdef.h"
#include "../Number/Bn.h"

#include "OtWrapperString.h"
#include "GenString.h"
#include "../String.h"

#include <string.h>

namespace RiotBase {

GenString::GenString( const char *str, PT size, PT rese ) : GenString( size, rese ) {
    memcpy( data(), str, size );
}

GenString::GenString( const char *str, PT size ) : GenString( str, size, size ) {
}

GenString::GenString( const char *str ) : GenString( str, strlen( str ) ) {
}

GenString::GenString( PT size, PT rese ) : size( size ), rese( rese ) {
}

std::string GenString::type_name() {
    return "GenString";
}

const Category *GenString::static_category() {
    return String::category;
}

const Category *GenString::category() const {
    return String::category;
}

void GenString::write_to_stream( std::ostream &os ) const {
    os.write( data(), size );
}

DaSi GenString::dasi() const {
    return { data(), size };
}

bool GenString::apply_DaSi( const std::function<void(DaSi)> &f ) const {
    f( { data(), size } );
    return true;
}

bool GenString::to_std_string( std::string *str, PT nb_items ) const {
    if ( nb_items != 1 )
        return false;
    *str = std::string( data(), data() + size );
    return true;
}

Rp *GenString::get_size() const {
    return Bn<PT>::New( size );
}

PT GenString::get_size_PT() const {
    return size;
}

Rp *GenString::copy() const {
    return GenString::New( data(), size, size );
}

Rp *GenString::make_ot_wrapper() {
    return OtWrapperString::New( this );
}

void GenString::serialize( BBQ bq, const PT *num_children ) const {
    bq << this->num_in_tuple_types() << size;
    bq.write_some( data(), size );
}

Rp *GenString::deserialize( BinStream<CbStringPtr> bq, Vec<RpAndUse> &children ) {
    PT size = bq.read_unsigned();
    GenString *res = GenString::New( size, size );
    bq.read_some( res->data(), size );
    return res;
}

void GenString::get_lazy_source_info( LazySourceInfo &info, const LazySourceInfo **children_info ) const {
    info.output_size = size;
}

void GenString::reserve( GenString *&a, PT new_size ) {
    if ( new_size > a->rese ) {
        GenString *r = GenString::New( a->data(), a->size, new_size );
        _del( a );
        a = r;
    }
}

void GenString::resize( GenString *&a, PT new_size ) {
    reserve( a, new_size );
    a->size = new_size;
}

void GenString::self_add( GenString *&a, const GenString *b ) {
    PT os = a->size;
    resize( a, os + b->size );
    memcpy( a->data() + os, b->data(), b->size );
}

void GenString::self_insert( GenString *&a, PT pos, DaSi b ) {
    PT os = a->size;
    if ( pos > os )
        return;
    resize( a, os + b.size );
    for( PT i = os; i-- > pos; )
        a->data()[ i + b.size ] = a->data()[ i ];
    memcpy( a->data() + pos, b.data, b.size );
}

void GenString::self_remove( GenString *&a, PT pos, PT len ) {
    char *d = a->data(); a->size -= len;
    for( PT i = pos; i < a->size; ++i )
        d[ i ] = d[ i + len ];
}

namespace {

RiotBase_DEF_FUNC_2( self_add_b, 1, tv0->isa<GenString>() and tv1->isa<GenString>() ) {
    GenString::self_add( reinterpret_cast<GenString *&>( v0 ), scast( v1 ) );
}

RiotBase_DEF_FUNC_2( add_bb, 1, tv0->isa<GenString>() and tv1->isa<GenString>() ) {
    const GenString *s0 = scast( v0 );
    const GenString *s1 = scast( v1 );
    GenString *res = GenString::New( s0->data(), s0->size, s0->size + s1->size );
    memcpy( res->data() + s0->size, s1->data(), s1->size );
    res->size = s0->size + s1->size;
    return res;
}

RiotBase_DEF_FUNC_3( self_insert_bb, 1, tv0->isa<GenString>() and tv1->has_surdef_for( RF_to_PT() ) and tv2->has_surdef_for( RF_dasi() ) ) {
    PT pos; if ( not RO_CALL( to_PT, v1, &pos, 1 ) ) return;
    DaSi s = RO_CALL( dasi, v2 );
    GenString::self_insert( reinterpret_cast<GenString *&>( v0 ), pos, s );
}

RiotBase_DEF_FUNC_3( self_remove_bb, 1, tv0->isa<GenString>() and tv1->has_surdef_for( RF_to_PT() ) and tv2->has_surdef_for( RF_to_PT() ) ) {
    PT pos; if ( not RO_CALL( to_PT, v1, &pos, 1 ) ) return;
    PT len; if ( not RO_CALL( to_PT, v2, &len, 1 ) ) return;
    GenString::self_remove( reinterpret_cast<GenString *&>( v0 ), pos, len );
}

RiotBase_DEF_FUNC_2( inf_immediate, 1, tv0->isa<GenString>() and tv1->isa<GenString>() ) {
    const GenString *s0 = scast( v0 ); DaSi d0 = s0->dasi();
    const GenString *s1 = scast( v1 ); DaSi d1 = s1->dasi();
    if ( d0.size != d1.size )
        return d0.size < d1.size;
    for( PT i = 0; i < d0.size; ++i )
        if ( d0.data[ i ] != d1.data[ i ] )
            return d0.data[ i ] < d1.data[ i ];
    return 0;
}

Rp *split( const Rp *v0, const Rp *v1 ) {
    GenArray *res = GenArray::New( Array<String>::category, String::category );
    RO_CALL( apply_DaSi, v0, [&]( DaSi src ) {
        RO_CALL( apply_DaSi, v1, [&]( DaSi spl ) {
            if ( src.size >= spl.size ) {
                for( PT i = 0, o = 0; ; ) {
                    if ( i > src.size - spl.size ) {
                        if ( o < i )
                            res->self_push_back_o( GenString::New( src.data + o, i - o ) );
                        break;
                    }
                    if ( bcmp( src.data + i, spl.data, spl.size ) == 0 ) {
                        if ( o < i )
                            res->self_push_back_o( GenString::New( src.data + o, i - o ) );
                        i += spl.size;
                        o = i;
                    } else
                        ++i;
                }
            }
        } );
    } );
    return res;
}

RiotBase_DEF_FUNC_2( split_ob, 0.5, tv0->has_surdef_for( RF_apply_DaSi() ) and tv0->has_surdef_for( RF_apply_DaSi() ) ) {
    // lazy evaluation ?
    //PT size = RO_CALL( get_size_PT, v0 );
    //if ( size > 2 ) // heuristic to be improved
    //    return RO_CALL( split_ob, LazySourceKnownValue::mkl( v0 ), v1 ); // TODO: remove copy using _ob if _split_ob
    AutoRpDel r0( v0 );
    return split( v0, v1 );
}

RiotBase_DEF_FUNC_2( split_bb, 0.5, tv0->has_surdef_for( RF_apply_DaSi() ) and tv0->has_surdef_for( RF_apply_DaSi() ) ) {
    // lazy evaluation ?
    //PT size = RO_CALL( get_size_PT, v0 );
    //if ( size > 2 ) // heuritic to be improved
    //    return RO_CALL( split_ob, LazySourceKnownValue::mkl( RO_CALL( copy, v0 ) ), v1 ); // TODO: remove copy using _ob if _split_ob
    return split( v0, v1 );
}

RiotBase_DEF_FUNC_1( new_cursor, 1, tv0->isa<GenString>(), void *int_id ) {
    v0 = RO_CALL( make_ot_wrapper, v0 );
    return RO_CALL( new_cursor, v0, int_id );
}

}

} // namespace RiotBase
