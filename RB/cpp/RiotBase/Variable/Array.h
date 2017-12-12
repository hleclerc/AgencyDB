#pragma once

#include "Number.h"

namespace RiotBase {

Rp *new_GenArray( const Category *category_array, const Category *category_val, UsrId usr_id );

///
template<class V>
class Array : public VariableTempl<Array<V>> {
public:
    using TP = VariableTempl<Array<V>>;

    Array() : Array( Variable::CreatorId{}, UsrId{} ) {}
    Array( Variable::CreatorId, UsrId usr_id ) : TP( new_GenArray( TP::category, V::category, usr_id ) ) {}
    Array( const Array &array ) : TP( RO_CALL( copy, array.rp ) ) {}
    Array( Array &&array ) : TP( array.rp ) { array.rp = new_GenArray( TP::category, V::category, {} ); }
    Array( Rp *rp ) : TP( rp ) {}

    const Ref<V>           operator[]( const Number &index ) const { return RO_CALL( select_bb, this->rp, index.rp ); }
    Ref<V>                 operator[]( const Number &index ) { return RO_CALL( mod_select, this->rp, index.rp ); }
                          
    Array                 &operator= ( const Array &array ) { RO_CALL( self_set_b, this->rp, array.rp ); return *this; }
    Array                 &operator<<( const V &val ) { RO_CALL( self_push_back_b, this->rp, val.rp ); return *this; }
                          
    Number                 size      () const { return RO_CALL( get_size, this->rp ); }
                          
    V                      front     () const { return RO_CALL( front_b     , this->rp ); } ///< first item (return an error is size == 0)
    V                      back      () const { return RO_CALL( back_b      , this->rp ); } ///< last item (return an error is size == 0)
    Array                  front_list() const { return RO_CALL( front_list_b, this->rp ); } ///< [ first item ] or [] if size == 0
    Array                  back_list () const { return RO_CALL( back_list_b , this->rp ); } ///< [ last item ] or [] if size == 0
                          
    Array                  head      () const { return RO_CALL( head_b      , this->rp ); } ///< list without the last element
    Array                  tail      () const { return RO_CALL( tail_b      , this->rp ); } ///< list without the first element
    Array                  slice     ( Number beg, Number end ) const { return RO_CALL( slice_bbb, this->rp, beg.rp, end.rp ); }
                          
    void                   apply     ( std::function<void(const Rp *)> f ) const { return RO_CALL( apply, this->rp, f ); }

    template<class T> V    join      ( T &&sep )      && { return RF_join::call( std::move( *this ), as_a_Variable( std::forward<T>( sep ) ) ); }
    template<class T> V    join      ( T &&sep ) const & { return RF_join::call(            *this  , as_a_Variable( std::forward<T>( sep ) ) ); }

    template<class TF,class TV>
    TV                     reduction ( const TF &f, TV init ) const;

    static std::string     category_name   () { return "Array<" + V::category_name() + ">"; }
    static const Category *get_category_key() { return Number::category; }
    static const Category *get_category_val() { return V::category; }
};

template<class V>
template<class TF,class TV>
TV Array<V>::reduction( const TF &f, TV init ) const {
    TV res = init;
    apply( [ &res, f ]( const Rp *val ) {
        res = f( res, reinterpret_cast<const V &>( val ) );
    } );
    return res;
}

} // namespace RiotBase
