#pragma once

#include <tuple>
#include "N.h"

namespace RiotBase {

///
template<class Op>
void apply_va( const Op &op ) {
}
template<class Op,class Head,class ...Tail>
void apply_va( const Op &op, Head &&head, Tail&& ...tail ) {
    op( std::forward<Head>( head ) );
    apply_va( op, std::forward<Tail>( tail )... );
}

// helper
template<class Op,class Tuple,PT n>
void __apply_va_tuple( const Op &op, Tuple &data, N<n> ) {
    __apply_va_tuple( op, data, N<n-1>{} );
    op( std::get<n-1>( data ) );
}
template<class Op,class Tuple>
void __apply_va_tuple( const Op &op, Tuple &data, N<0> ) {
}

///
template<class Op,class Tuple>
void apply_va_tuple( const Op &op, Tuple &data ) {
    __apply_va_tuple( op, data, N<std::tuple_size<Tuple>::value>{} );
}



///
template<class Op,class ...Args>
auto apply_van( const Op &op, Args&& ...args ) {
    return _apply_van( op, N<0>(), std::forward<Args>( args )... );
}

template<class Op,PT n,class Head,class ...Tail>
auto _apply_van( const Op &op, N<n> vn, Head &&head, Tail&& ...tail ) {
    op( std::forward<Head>( head ), vn );
    return _apply_van( op, N<n+1>(), std::forward<Tail>( tail )... );
}

template<class Op,PT n>
auto _apply_van( const Op &op, N<n> res ) {
    return res;
}

}
