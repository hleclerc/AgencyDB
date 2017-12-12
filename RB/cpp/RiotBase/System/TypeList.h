#pragma once

#include "IsSame.h"
#include "N.h"

namespace RiotBase {

/**
 Enable to store types. Almost like a tuple, excepted that tuple store instances, whereas S stores nothing
*/
template<class... Args>
struct TypeList;

template<class Head,class... Tail>
struct TypeList<Head,Tail...> {
    using            T    = Head;
    static constexpr PT size = 1 + TypeList<Tail...>::size;

    template<class T>
    static constexpr PT index_of() { return IsSame<Head,T>::v ? 0 : 1 + TypeList<Tail...>::template index_of<T>(); }

    template<class Op>
    static void         apply   ( const Op &op ) { op( TypeList<Head>() ); TypeList<Tail...>::apply( op ); }

    template<class Op>
    static bool         apply   ( const Op &op, PT num ) { if ( num ) return TypeList<Tail...>::apply( op, num - 1 ); op( TypeList<Head>() ); return true; }

    /// will call op( mf( TypeList<T0> ), mf( TypeList<T1> ), ... )
    template<class MapFunc,class Op,class... Args>
    static void         map     ( const MapFunc &mf, const Op &op, Args&& ...args ) { TypeList<Tail...>::map( mf, op, std::forward<Args>( args )..., mf( TypeList<Head>{} ) ); }

    /// index of first item checking op. Else, return size
    template<class Op>
    static PT           find    ( const Op &op ) { return op( TypeList<Head>{} ) ? 0 : 1 + TypeList<Tail...>::find( op ); }
};

template<>
struct TypeList<> {
    static constexpr PT size = 0;

    template<class T>
    static constexpr PT index_of() { return 0; }

    template<class Op>
    static void         apply   ( const Op &op ) {}

    template<class Op>
    static bool         apply   ( const Op &op, PT num ) { return false; }

    template<class MapFunc,class Op,class... Args>
    static void         map     ( const MapFunc &mf, const Op &op, Args&& ...args ) { op( std::forward<Args>( args )... ); }

    template<class Op>
    static PT           find    ( const Op &op ) { return 0; }
};

} // namespace RiotBase
