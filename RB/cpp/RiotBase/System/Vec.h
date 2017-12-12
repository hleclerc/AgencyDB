#pragma once

#include "TensorOrder.h"
#include "TypeConfig.h"
#include "EnableIf.h"
#include "Assert.h"
#include "Print.h"
#include "N.h"

#include <initializer_list>
#include <functional>
#include <stdlib.h>
#include <utility>
#include <new>


namespace RiotBase {
template<class T>
struct BinStream;


template<class T,int s=-1,int p=0>
class Vec;

struct Size {};
struct Rese {};

/*
  if static_size == 0
*/
template<class ItemType,int static_rese>
class Vec<ItemType,0,static_rese> {
public:
    typedef ItemType T;
    static const PT static_size = 0;
    static const PT static_prer = static_rese;

    Vec() {}

    template<class T0>
    Vec( const T0 & ) {}

    const T &operator[]( int i ) const { ASSERT_IF_DEBUG( 0 ); return *dummy(); }
    T &operator[]( int i ) { ASSERT_IF_DEBUG( 0 ); return *dummy(); }
    PT size() const { return 0; }

    void resize( int s ) const { ASSERT( s == 0, "..." ); }
    void remove( int i ) { ERROR( "..." ); }
    void remove_some( PT pos, PT len ) { ASSERT( len == 0, "..." );  }
    void insert_some( PT pos, PT len ) { ASSERT( len == 0, "..." );  }
    void remove_or_insert_some( PT pos, ST len ) { ASSERT( len == 0, "..." );  }

    Vec &operator<<( ItemType ) { ERROR( "forbidden" ); return *this; }

    T *dummy() const { return 0; }
};

/**
  @brief Simple vector, generic type, static or dynamic size, possible "prereservation", contiguous data

  Examples :
   - Vec<double> -> dynamic vector of doubles.
   - Vec<double,3> -> static vector of doubles, size 3.
   - Vec<double,0> -> static vector of doubles, size 0.
   - Vec<double,-1> -> dynamic vector. Size begins at 0.
   - Vec<double,-1,4> -> dynamic vector with reservation "on stack" of size 4 : if size remains <= 4, no malloc are necessary. Size begins at 0.

*/
template<class ItemType,int _static_size,int static_rese>
class Vec {
public:
    typedef ItemType T;
    static const PT static_size = _static_size;
    static const PT static_prer = static_rese;

    //#ifndef __CUDACC__
    Vec() {}
    //#endif

    template<class T0>
    void _init_using_1_arg( const T0 &v0, N<0> ) {
        for( PT i = 0; i < static_size; ++i )
            _data[ i ] = v0;
    }

    template<class T0>
    void _init_using_1_arg( const T0 &v0, N<1> ) {
        ASSERT_IF_DEBUG( v0.size() == static_size );
        for( PT i = 0; i < static_size; ++i )
            _data[ i ] = v0[ i ];
    }

    template<class T0>
    Vec( const T0 &v0 ) {
        _init_using_1_arg( v0, N<TensorOrder<T0>::res>() );
    }

    template<class T0,class T1>
    Vec( const T0 &v0, const T1 &v1 ) {
        ASSERT_IF_DEBUG( static_size == 2 );
        _data[ 0 ] = v0;
        _data[ 1 ] = v1;
    }

    template<class T0,class T1,class T2>
    Vec( const T0 &v0, const T1 &v1, const T2 &v2 ) {
        ASSERT_IF_DEBUG( static_size == 3 );
        _data[ 0 ] = v0;
        _data[ 1 ] = v1;
        _data[ 2 ] = v2;
    }

    template<class T0,class T1,class T2,class T3>
    Vec( const T0 &v0, const T1 &v1, const T2 &v2, const T3 &v3 ) {
        ASSERT_IF_DEBUG( static_size == 4 );
        _data[ 0 ] = v0;
        _data[ 1 ] = v1;
        _data[ 2 ] = v2;
        _data[ 3 ] = v3;
    }

    template<class T0,class T1,class T2,class T3,class T4>
    Vec( const T0 &v0, const T1 &v1, const T2 &v2, const T3 &v3, const T4 &v4 ) {
        ASSERT_IF_DEBUG( static_size == 5 );
        _data[ 0 ] = v0;
        _data[ 1 ] = v1;
        _data[ 2 ] = v2;
        _data[ 3 ] = v3;
        _data[ 4 ] = v4;
    }

    template<class T0,class T1,class T2,class T3,class T4,class T5>
    Vec( const T0 &v0, const T1 &v1, const T2 &v2, const T3 &v3, const T4 &v4, const T5 &v5 ) {
        ASSERT_IF_DEBUG( static_size == 6 );
        _data[ 0 ] = v0;
        _data[ 1 ] = v1;
        _data[ 2 ] = v2;
        _data[ 3 ] = v3;
        _data[ 4 ] = v4;
        _data[ 5 ] = v5;
    }

    template<class T0,class T1,class T2,class T3,class T4,class T5,class T6>
    Vec( const T0 &v0, const T1 &v1, const T2 &v2, const T3 &v3, const T4 &v4, const T5 &v5 , const T6 &v6) {
        ASSERT_IF_DEBUG( static_size == 7 );
        _data[ 0 ] = v0;
        _data[ 1 ] = v1;
        _data[ 2 ] = v2;
        _data[ 3 ] = v3;
        _data[ 4 ] = v4;
        _data[ 5 ] = v5;
        _data[ 6 ] = v6;
    }

    template<class T0,class T1,class T2,class T3,class T4,class T5,class T6,class T7>
    Vec( const T0 &v0, const T1 &v1, const T2 &v2, const T3 &v3, const T4 &v4, const T5 &v5 , const T6 &v6, const T7 &v7) {
        ASSERT_IF_DEBUG( static_size == 8 );
        _data[ 0 ] = v0;
        _data[ 1 ] = v1;
        _data[ 2 ] = v2;
        _data[ 3 ] = v3;
        _data[ 4 ] = v4;
        _data[ 5 ] = v5;
        _data[ 6 ] = v6;
        _data[ 7 ] = v7;
    }

    template<class T0,class T1,class T2,class T3,class T4,class T5,class T6,class T7,class T8>
    Vec( const T0 &v0, const T1 &v1, const T2 &v2, const T3 &v3, const T4 &v4, const T5 &v5 , const T6 &v6, const T7 &v7, const T8 &v8) {
        ASSERT_IF_DEBUG( static_size == 9 );
        _data[ 0 ] = v0;
        _data[ 1 ] = v1;
        _data[ 2 ] = v2;
        _data[ 3 ] = v3;
        _data[ 4 ] = v4;
        _data[ 5 ] = v5;
        _data[ 6 ] = v6;
        _data[ 7 ] = v7;
        _data[ 8 ] = v8;
    }

    template<class T0,class T1,class T2,class T3,class T4,class T5,class T6,class T7,class T8,class T9>
    Vec( const T0 &v0, const T1 &v1, const T2 &v2, const T3 &v3, const T4 &v4, const T5 &v5 , const T6 &v6, const T7 &v7, const T8 &v8, const T9 &v9) {
        ASSERT_IF_DEBUG( static_size == 10 );
        _data[ 0 ] = v0;
        _data[ 1 ] = v1;
        _data[ 2 ] = v2;
        _data[ 3 ] = v3;
        _data[ 4 ] = v4;
        _data[ 5 ] = v5;
        _data[ 6 ] = v6;
        _data[ 7 ] = v7;
        _data[ 8 ] = v8;
        _data[ 9 ] = v9;
    }

    template<class T0,class T1,class T2,class T3,class T4,class T5,class T6,class T7,class T8,class T9,class T10>
    Vec( const T0 &v0, const T1 &v1, const T2 &v2, const T3 &v3, const T4 &v4, const T5 &v5 , const T6 &v6, const T7 &v7, const T8 &v8, const T9 &v9, const T10 &v10) {
        ASSERT_IF_DEBUG( static_size == 11 );
        _data[ 0 ] = v0;
        _data[ 1 ] = v1;
        _data[ 2 ] = v2;
        _data[ 3 ] = v3;
        _data[ 4 ] = v4;
        _data[ 5 ] = v5;
        _data[ 6 ] = v6;
        _data[ 7 ] = v7;
        _data[ 8 ] = v8;
        _data[ 9 ] = v9;
        _data[ 10 ] = v10;
    }

    Vec( Size, PT s ) {
        ASSERT_IF_DEBUG( s == static_size );
    }

    template<class T0>
    Vec( Size, PT s, const T0 &v0 ) {
        ASSERT_IF_DEBUG( s == static_size );
        for( PT i = 0; i < static_size; ++i )
            _data[ i ] = v0;
    }

    Vec( const Vec &v ) {
        ASSERT_IF_DEBUG( static_size == v.size() );
        for( PT i = 0; i < static_size; ++i )
            _data[ i ] = v[ i ];
    }

    void operator=( const Vec &v ) {
        ASSERT_IF_DEBUG( static_size == v.size() );
        for( PT i = 0; i < static_size; ++i )
            _data[ i ] = v[ i ];
    }

    template<class T0>
    void operator=( const T0 &v0 ) {
        _init_using_1_arg( v0, N<TensorOrder<T0>::res>() );
    }

    void set( T val ) {
        for( PT i = 0; i < static_size; ++i )
            _data[ i ] = val;
    }

    void operator+=( const Vec &v ) {
        for( int i = 0; i < static_size; ++i )
            _data[ i ] += v[ i ];
    }


    PT size() const {
        return static_size;
    }

    PT rese() const {
        return static_size;
    }

    const T &operator[]( PT i ) const {
        ASSERT_IF_DEBUG( i >= 0 );
        ASSERT_IF_DEBUG( i < static_size );
        return _data[ i ];
    }

    T &operator[]( PT i ) {
        ASSERT_IF_DEBUG( i < static_size );
        return _data[ i ];
    }

    bool empty() const { return size() == 0; }

    Vec &operator<<( ItemType ) { ERROR( "forbidden" ); return *this; }

    void resize( PT new_size ) const { ASSERT_IF_DEBUG( new_size == static_size ); }
    void resize( PT new_size, const T &val ) const { ASSERT_IF_DEBUG( new_size == static_size ); }
    void remove( PT i ) { ERROR( "..." ); }
    void remove_unordered( PT i ) { ERROR( "..." ); }
    template<class U>
    void remove_first_unordered( const U &val ) { ERROR( "..." ); }
    void remove_some( PT pos, PT len ) { ASSERT( len == 0, "..." );  }
    void insert_some( PT pos, PT len ) { ASSERT( len == 0, "..." );  }
    void remove_or_insert_some( PT pos, ST len ) { ASSERT( len == 0, "..." );  }
    template<class TT>
    void remove_or_insert_some( PT pos, ST len, const TT & ) { ASSERT( len == 0, "..." );  }

    T *begin() { return _data; }
    T *end() { return _data + static_size; }
    const T *begin() const { return _data; }
    const T *end() const { return _data + static_size; }

    T *ptr() { return _data; }
    const T *ptr() const { return _data; }

    T &back() { return _data[ static_size - 1 ]; }
    const T &back() const { return _data[ static_size - 1 ]; }

    template<class T2>
    bool contains( const T2 &d ) const {
        for( PT i = 0; i < static_size; ++i )
            if ( _data[ i ] == d )
                return true;
        return false;
    }

    bool operator!=( const Vec &v ) const {
        for( PT i = 0; i < static_size; ++i )
            if ( _data[ i ] != v[ i ] )
                return true;
        return false;
    }

    bool operator<( const Vec &v ) const {
        for( PT i = 0; i < static_size; ++i )
            if ( _data[ i ] != v[ i ] )
                return _data[ i ] < v[ i ];
        return false;
    }

    bool operator==( const Vec &v ) const {
        for( PT i = 0; i < static_size; ++i )
            if ( _data[ i ] != v[ i ] )
                return false;
        return true;
    }

    bool operator==( const T &v ) const {
        for( PT i = 0; i < static_size; ++i )
            if ( _data[ i ] != v )
                return false;
        return true;
    }


    Vec operator+( const Vec &vec ) const {
        Vec res;
        for( PT i = 0; i < static_size; ++i )
            res[ i ] = operator[]( i ) + vec[ i ];
        return res;
    }

    void write_to_stream( std::ostream &os ) const {
        if ( size() ) {
            os << operator[]( 0 );
            for( PT i = 1; i < size(); ++i )
                os << " " << operator[]( i );
        }
    }
private:
    T _data[ static_size ];
};

// dynamic case
template<class T_,int static_prer_>
class Vec<T_,-1,static_prer_> {
public:
    typedef T_ T;
    static const ST static_size = -1;
    static const PT static_prer = static_prer_;

    Vec() : _size( 0 ), _rese( static_prer_ ), _data( (T_ *)_prer ) {}

    Vec( const T &v0 ) : _size( 1 ), _rese( 1 * sizeof( T ) ), _data( _alloc() ) {
        new( _data + 0 ) T_( v0 );
    }

    template<class T0>
    Vec( const std::initializer_list<T0> &lst ) : _size( lst.size() ), _rese( lst.size() * sizeof( T ) ), _data( _alloc() ) {
        ST cpt = 0;
        for( const T0 *i = lst.begin(); i != lst.end(); ++i, ++cpt )
            new( _data + cpt ) T_( *i );
    }

    template<class T0>
    Vec( const T0 &v0, typename EnableIf<TensorOrder<T0>::res==0,Nawak>::T = Nawak() ) : _size( 1 ), _rese( 1 * sizeof( T ) ), _data( _alloc() ) {
        new( _data + 0 ) T_( v0 );
    }

    template<class T0,class T1>
    Vec( const T0 &v0, const T1 &v1 ) : _size( 2 ), _rese( 2 * sizeof( T ) ), _data( _alloc() ) {
        new( _data + 0 ) T_( v0 );
        new( _data + 1 ) T_( v1 );
    }

    template<class T0,class T1,class T2>
    Vec( const T0 &v0, const T1 &v1, const T2 &v2 ) : _size( 3 ), _rese( 3 * sizeof( T ) ), _data( _alloc() ) {
        new( _data + 0 ) T_( v0 );
        new( _data + 1 ) T_( v1 );
        new( _data + 2 ) T_( v2 );
    }

    template<class T0,class T1,class T2,class T3>
    Vec( const T0 &v0, const T1 &v1, const T2 &v2, const T3 &v3 ) : _size( 4 ), _rese( 4 * sizeof( T ) ), _data( _alloc() ) {
        new( _data + 0 ) T_( v0 );
        new( _data + 1 ) T_( v1 );
        new( _data + 2 ) T_( v2 );
        new( _data + 3 ) T_( v3 );
    }

    template<class T0,class T1,class T2,class T3,class T4>
    Vec( const T0 &v0, const T1 &v1, const T2 &v2, const T3 &v3, const T4 &v4 ) : _size( 5 ), _rese( 5 * sizeof( T ) ), _data( _alloc() ) {
        new( _data + 0 ) T_( v0 );
        new( _data + 1 ) T_( v1 );
        new( _data + 2 ) T_( v2 );
        new( _data + 3 ) T_( v3 );
        new( _data + 4 ) T_( v4 );
    }

    template<class T0,class T1,class T2,class T3,class T4, class T5>
    Vec( const T0 &v0, const T1 &v1, const T2 &v2, const T3 &v3, const T4 &v4, const T5 &v5 ) : _size( 6 ), _rese( 6 * sizeof( T ) ), _data( _alloc() ) {
        new( _data + 0 ) T_( v0 );
        new( _data + 1 ) T_( v1 );
        new( _data + 2 ) T_( v2 );
        new( _data + 3 ) T_( v3 );
        new( _data + 4 ) T_( v4 );
        new( _data + 5 ) T_( v5 );
    }

    template<class S>
    Vec( Size, S s ) : _size( s ), _rese( s * sizeof( T ) ), _data( _alloc() ) {
        for( S i = 0; i < s; ++i )
            new( _data  + i ) T_;
    }

    template<class S>
    Vec( Size, S s, T *ptr ) : _size( s ), _rese( s * sizeof( T ) ), _data( _alloc() ) {
        for( S i = 0; i < s; ++i )
            new( _data + i ) T_( ptr[ i ] );
    }

    template<class S,class T0>
    Vec( Size, S s, const T0 &v0 ) : _size( s ), _rese( s * sizeof( T ) ), _data( _alloc() ) {
        for( S i = 0; i < s; ++i )
            new( _data + i ) T_( v0 );
    }

    template<class S,class T0,class T1>
    Vec( Size, S s, const T0 &v0, const T1 &v1 ) : _size( s ), _rese( s * sizeof( T ) ), _data( _alloc() ) {
        for( S i=0; i < s; ++i )
            new( _data + i ) T_( v0, v1 );
    }

    /// init this with size = 0, but with reserved room in memory for s items.
    template<class S>
    Vec( Rese, S s ) : _size( 0 ), _rese( s * sizeof( T ) ), _data( _alloc() ) {
    }


    Vec( const Vec &v ) : _size( v.size() ), _rese( _size * sizeof( T ) ), _data( _alloc() ) {
        for( PT i = 0; i < _size; ++i )
            new( _data + i ) T_( v[ i ] );
    }

    Vec( Vec &&v ) : _size( v._size ), _rese( v._rese ), _data( v._data ) {
        if ( v._data == reinterpret_cast<T *>( v._prer ) ) {
            _data = reinterpret_cast<T *>( _prer );
            for( PT i = 0; i < _size; ++i )
                new( _data + i ) T_( std::move( v[ i ] ) );
            v.~Vec();
        }
        new ( &v ) Vec;
    }

    template<class T2,int s,int p>
    Vec( const Vec<T2,s,p> &v ) : _size( v.size() ), _rese( _size * sizeof( T ) ), _data( _alloc() ) {
        for( PT i = 0; i < _size; ++i )
            new( _data + i ) T_( v[ i ] );
    }

    Vec( T_ *beg, T_ *end ) : _size( end - beg ), _rese( _size * sizeof( T ) ), _data( _alloc() ) {
        for( PT i = 0; i < _size; ++i )
            new( _data + i ) T_( beg[ i ] );
    }

    void operator=( const Vec &v ) {
        reserve( v._size );
        for( PT i = _size; i > v._size; ) // destruction for i > v._size
            _data[ --i ].~T();
        for( PT i = 0; i < _size and i < v._size; ++i ) // operator= for i < both
            _data[ i ] = v._data[ i ];
        for( PT i = _size; i < v._size; ++i ) // placement new
            new( _data + i ) T_( v._data[ i ] );
        _size = v._size;
    }

    //    void operator=( Vec &&v ) {
    //        ~Vec();
    //        _size = v._size;
    //        _rese = v._rese;
    //        _data = v._data;
    //        if ( v._data == v._prer )
    //            for( ST i = 0; i < _size; ++i )
    //                new( _data + i ) T_( std::move( v[ i ] ) );
    //        else
    //            v._data = v._prer;
    //        v._size = 0;
    //    }

    template<class T2,int s,int p>
    void operator=( const Vec<T2,s,p> &v ) {
        reserve( v.size() );
        for( PT i = _size; i > v.size(); ) // destruction for i >= v._size
            _data[ --i ].~T();
        for( PT i = 0; i < ( _size < v.size() ? _size : v.size() ); ++i ) // operator= for i < both
            _data[ i ] = v[ i ];
        for( PT i = _size; i < v.size(); ++i ) // placement new
            new( _data + i ) T_( v[ i ] );
        _size = v.size();
    }

    ~Vec() {
        for( PT i = _size; i; )
            _data[ --i ].~T_();
        __free( _data, _rese );
    }

    void clear() {
        for( PT i = _size; i; )
            _data[ --i ].~T_();
        __free( _data, _rese );

        _size = 0;
        _rese = static_prer_;
        _data = (T_ *)_prer;
    }

    void set( T val ) {
        for( PT i = 0; i < _size; ++i )
            _data[ i ] = val;
    }

    const T &operator[]( PT i ) const {
        ASSERT_IF_DEBUG( i < _size );
        return _data[ i ];
    }

    T &operator[]( PT i ) {
        ASSERT_IF_DEBUG( i < _size );
        return _data[ i ];
    }

    Vec slice( PT beg, PT end ) {
        Vec res( Size(), end - beg );
        for( PT i = beg, o = 0; i < end; ++i, ++o )
            res[ o ] = operator[]( i );
        return res;
    }

    PT size() const { return _size; }

    bool empty() const { return size() == 0; }

    template<class T2>
    Vec &operator<<( T2 &&val ) {
        push_back( std::forward<T2>( val ) );
        return *this;
    }

    T *push_back() {
        reserve_wm( _size + 1 );
        new( _data + _size ) T_;
        return _data + _size++;
    }

    template<class T0>
    T *push_back( T0 &&v0 ) {
        reserve_wm( _size + 1 );
        new( _data + _size ) T_{ std::forward<T0>( v0 ) };
        return _data + _size++;
    }

    template<class T0,class T1>
    T *push_back( const T0 &v0, const T1 &v1 ) {
        reserve_wm( _size + 1 );
        new( _data + _size ) T_{ v0, v1 };
        return _data + _size++;
    }

    template<class T0,class T1,class T2>
    T *push_back( const T0 &v0, const T1 &v1, const T2 &v2 ) {
        reserve_wm( _size + 1 );
        new( _data + _size ) T_{ v0, v1, v2 };
        return _data + _size++;
    }

    template<class T0,class T1,class T2,class T3>
    T *push_back( const T0 &v0, const T1 &v1, const T2 &v2, const T3 &v3 ) {
        reserve_wm( _size + 1 );
        new( _data + _size ) T_{ v0, v1, v2, v3 };
        return _data + _size++;
    }

    template<class T0,class T1,class T2,class T3,class T4>
    T *push_back( const T0 &v0, const T1 &v1, const T2 &v2, const T3 &v3, const T4 &v4 ) {
        reserve_wm( _size + 1 );
        new( _data + _size ) T_{ v0, v1, v2, v3, v4 };
        return _data + _size++;
    }

    void reserve_wm( PT wanted ) { // if not enough, reserve more than wanted...
        wanted *= sizeof( T_ );
        if ( wanted <= _rese )
            return;
        PT n = 2 * ( _rese + 1 );
        _mv_data_for_size( wanted > n ? wanted : n );
    }

    void reserve( PT wanted ) {
        wanted *= sizeof( T_ );
        if ( wanted <= _rese )
            return;
        _mv_data_for_size( wanted );
    }

    PT rese() const { return _rese / sizeof( T_ ); }

    void resize( PT new_size ) {
        for( PT i = _size; i > new_size; )
            _data[ --i ].~T();
        reserve( new_size );
        for( PT i = _size; i < new_size; ++i )
            new( _data + i ) T;
        _size = new_size;
    }

    template<class T0>
    void resize( PT new_size, const T0 &v0 ) {
        for( PT i = _size; i > new_size; )
            _data[ --i ].~T();
        reserve( new_size );
        for( PT i = _size; i < new_size; ++i )
            new( _data + i ) T( v0 );
        _size = new_size;
    }

    template<class T0,class T1>
    void resize( PT new_size, const T0 &v0, const T1 &v1 ) {
        for( PT i = _size; i > new_size; )
            _data[ --i ].~T();
        reserve( new_size );
        for( ST i = _size; i < new_size; ++i )
            new( _data + i ) T( v0, v1 );
        _size = new_size;
    }

    T &pop_back() {
        _data[ --_size ].~T();
        return _data[ _size ];
    }

    T pop_back_val() {
        T res = back();
        pop_back();
        return res;
    }

    // append val if not present and return pointer to new elem. Else, return pointer to elem which is equal to val
    template<class T2>
    T *push_back_unique( const T2 &val ) {
        for( PT i = 0; i < _size; ++i )
            if ( _data[ i ] == val )
                return _data + i;
            return push_back( val );
    }

    // assumes that room for val that will be constructed is already reserved
    template<class T0>
    T *push_back_in_reserved( const T0 &v0 ) {
        T *res = _data + _size++;
        new( res ) T( v0 );
        return res;
    }

    template<class V>
    void append( const V &v ) {
        for( const T &i : v )
            push_back( i );
    }

    void append( Vec &&v ) {
        reserve( size() + v.size() );
        for( T &i : v )
            push_back( std::move( i ) );
        //        v.__free( v._data, v._rese );

        //        v._size = 0;
        //        v._rese = static_prer_;
        //        v._data = (T_ *)_prer;
    }

    template<class V>
    bool subset_of( const V &v ) const {
        for( const T &i : *this )
            if ( not v.contains( i ) )
                return false;
        return true;
    }

    Vec without_vals_of( const Vec &v ) const {
        Vec res;
        for( const T &val : *this )
            if ( not v.contains( val ) )
                res << val;
        return res;
    }

    // erase first element equal to val
    template<class U>
    void remove_first_unordered( const U &val ) {
        for( PT i = 0; i < _size; ++i ) {
            if ( _data[ i ] == val ) {
                remove_unordered( i );
                return;
            }
        }
    }

    // erase first element equal to val
    template<class U>
    void remove_first_unordered_that_checks( const U &func ) {
        for( PT i = 0; i < _size; ++i ) {
            if ( func( _data[ i ] ) ) {
                remove_unordered( i );
                return;
            }
        }
    }

    // erase first element equal to val
    template<class U>
    void remove_first( const U &val ) {
        for( PT i = 0; i < _size; ++i ) {
            if ( _data[ i ] == val ) {
                remove( i );
                return;
            }
        }
    }

    // erase element number i (order in list is not preserved)
    void remove_unordered( PT pos ) {
        if ( pos + 1 != _size )
            _data[ pos ] = std::move( back() );
        pop_back();
    }

    // erase element at position `pos`
    void remove( PT pos ) {
        for( PT j = pos; j < _size - 1; ++j )
            operator[]( j ) = std::move( operator[]( j + 1 ) );
        if ( pos < _size )
            pop_back();
    }

    //
    void remove_some( PT pos, PT len ) {
        for( PT j = pos; j < _size - len; ++j )
            operator[]( j ) = std::move( operator[]( j + len ) );
        _size -= len;
    }

    //
    template<class TT>
    void insert( PT pos, const TT &val ) {
        insert_some( pos, 1, val );
    }

    //
    void insert_some( PT pos, PT len ) {
        if ( len ) {
            reserve_wm( _size + len ); _size += len;
            for( PT j = _size - 1; j >= pos + len; --j )
                operator[]( j ) = std::move( operator[]( j - len ) );
            for( PT j = pos; j < pos + len; ++j )
                new ( &operator[]( j ) ) T;
        }
    }

    //
    template<class TT>
    void insert_some( PT pos, PT len, const TT &val ) {
        if ( len ) {
            reserve_wm( _size + len ); _size += len;
            for( PT j = _size - 1; j >= pos + len; --j )
                operator[]( j ) = std::move( operator[]( j - len ) );
            for( PT j = pos; j < pos + len; ++j )
                new ( &operator[]( j ) ) T( val );
        }
    }

    //
    template<class TT>
    void insert_some( PT pos, const TT *beg, const TT *end ) {
        if ( PT len = end - beg ) {
            reserve_wm( _size + len ); _size += len;
            for( PT j = _size - 1; j >= pos + len; --j )
                operator[]( j ) = std::move( operator[]( j - len ) );
            for( ; beg < end; ++beg, ++pos )
                new ( &operator[]( pos ) ) T( *beg );
        }
    }

    //
    template<class TT>
    void append( const TT *beg, const TT *end ) {
        insert_some( 0, beg, end );
    }

    //
    void remove_or_insert_some( PT pos, ST len ) {
        len > 0 ? remove_some( pos, len ) : insert_some( pos, -len );
    }

    //
    template<class TT>
    void remove_or_insert_some( PT pos, ST len, const TT &val ) {
        len > 0 ? remove_some( pos, len ) : insert_some( pos, -len, val );
    }

    void remove_doubloons_unordered() {
        for( PT i = 0; i < size(); ++i )
            for( PT j = i + 1; j < size(); ++j )
                if ( operator[]( i ) == operator[]( j ) )
                    remove_unordered( j-- );
    }

    T *begin() { return _data;              }
    T *end  () { return _data + _size;      }
    T *ptr  () { return _data;              }
    T &back () { return _data[ _size - 1 ]; }
    T &front() { return _data[ 0 ]; }

    const T *begin() const { return _data;              }
    const T *end  () const { return _data + _size;      }
    const T *ptr  () const { return _data;              }
    const T &back () const { return _data[ _size - 1 ]; }
    const T &front() const { return _data[ 0 ]; }

    template<class T2>
    bool contains( const T2 &d ) const {
        for( PT i = 0; i < _size; ++i )
            if ( _data[ i ] == d )
                return true;
        return false;
    }

    /// true if this contains all items of vec
    template<int a,int b>
    bool contains( const Vec<T,a,b> &vec ) const {
        for( PT i = 0; i < vec.size(); ++i )
            if ( not contains( vec[ i ] ) )
                return false;
        return true;
    }

    template<class T2>
    int first_index_equal_to( const T2 &d ) const { /// returns -1 if not found
        for( PT i = 0; i < _size; ++i )
            if ( _data[ i ] == d )
                return i;
        return -1;
    }

    template<class T2>
    int first_item_equal_to( const T2 &d ) const { /// returns -1 if not found
        for( PT i = 0; i < _size; ++i )
            if ( _data[ i ] == d )
                return _data[ i ];
        return -1;
    }

    template<class TA,class TB>
    void replace( const TA &a, const TB &b ) {
        for( PT i = 0; i < _size; ++i )
            if ( _data[ i ] == a )
                _data[ i ] = b;
    }

    bool operator==( const Vec &v ) const {
        if ( size() != v.size() )
            return false;
        for( int i = 0; i < _size; ++i )
            if ( _data[ i ] != v[ i ] )
                return false;
        return true;
    }

    bool operator!=( const Vec &v ) const {
        if ( size() != v.size() )
            return true;
        for( PT i = 0; i < _size; ++i )
            if ( _data[ i ] != v[ i ] )
                return true;
        return false;
    }

    bool operator==( const T &v ) const {
        if ( size() != v.size() )
            return false;
        for( int i = 0; i < _size; ++i )
            if ( _data[ i ] != v )
                return false;
        return true;
    }

    bool operator!=( const T &v ) const {
        for(int i = 0; i < _size; ++i )
            if ( _data[ i ] != v )
                return true;
            return false;
    }

    bool operator<( const Vec &y ) const {
        if ( size() != y.size() )
            return size() < y.size();
        for( int i = 0; i < size(); ++i )
            if ( (*this)[ i ] != y[ i ] )
                return (*this)[ i ] < y[ i ];
        return false;
    }

    Vec operator+( const Vec &vec ) const {
        Vec res( Size(), ( size() < vec.size() ? size() : vec.size() ) );
        for( int i = 0; i < res.size(); ++i )
            res[ i ] = operator[]( i ) + vec[ i ];
        return res;
    }

    Vec left_to( ST end ) const {
        Vec res( Rese(), end );
        for( int i = 0; i < end; ++i )
            res << operator[]( i );
        return res;
    }

    template<class T2>
    void replace( const T2 &a, const T2 &b ) { ///< replace a by b
        for( PT i = 0; i < size(); ++i )
            if ( operator[]( i ) == a )
                operator[]( i ) = b;
    }

    void write_to_stream( std::ostream &os ) const {
        if ( size() ) {
            os << operator[]( 0 );
            for( PT i = 1; i < size(); ++i ) {
                os << " " << operator[]( i );
            }
        }
    }


    template<class Cq>
    BinStream<Cq> &write_to( BinStream<Cq> &bq ) const {
        bq << size();
        for( PT i = 0; i < size(); ++i )
            bq << operator[]( i );
        return bq;
    }

    template<class Cq>
    static Vec read_from( BinStream<Cq> &bq ) {
        Vec res;
        PT size = bq.read(); res.reserve( size );
        while( size-- )
            res.push_back( bq.read() );
        return res;
    }

private:

    void __free( T *ptr, PT rese ) {
        if ( ptr != reinterpret_cast<T *>( _prer ) )
            free( ptr ); // , rese
    }

    void _mv_data_for_size( PT wanted_in_bytes ) { // no check if reservation if necessary or not (size < old_size, ...)
        // -> TODO use of realloc
        T_ *old_data = _data;
        PT old_rese = _rese;
        _rese = wanted_in_bytes;
        _data = _alloc();
        for( PT i = 0; i < _size; ++i )
            new( _data + i ) T_( std::move( old_data[ i ] ) );
        __free( old_data, old_rese );
    }

    T *_alloc() { // use and update _rese
        if ( _rese <= PT( static_prer_ * sizeof( T ) ) ) {
            _rese = PT( static_prer_ * sizeof( T ) );
            return reinterpret_cast<T *>( _prer );
        }
        return reinterpret_cast<T *>( malloc( _rese ) );
    }

    char _prer[ static_prer_ * sizeof( T ) ];
    PT   _size; // number of item (type T)
    PT   _rese; // number of bytes
    T   *_data;
};


template<class T> Vec<T,1> vec( const T &a ) { return Vec<T,1>( a ); }
template<class T> Vec<T,2> vec( const T &a, const T &b ) { return Vec<T,2>( a, b ); }
template<class T> Vec<T,3> vec( const T &a, const T &b, const T &c ) { return Vec<T,3>( a, b, c ); }
template<class T> Vec<T,4> vec( const T &a, const T &b, const T &c, const T &d ) { return Vec<T,4>( a, b, c, d ); }

template<class T> Vec<T> dvec( const T &a ) { return Vec<T>( a ); }
template<class T> Vec<T> dvec( const T &a, const T &b ) { return Vec<T>( a, b ); }
template<class T> Vec<T> dvec( const T &a, const T &b, const T &c ) { return Vec<T>( a, b, c ); }
template<class T> Vec<T> dvec( const T &a, const T &b, const T &c, const T &d ) { return Vec<T>( a, b, c, d ); }

template<class TV>
bool all( const TV &vec ) {
    for( PT i = 0; i < vec.size(); ++i )
        if ( not vec[ i ] )
            return false;
    return true;
}

template<class TV>
bool any( const TV &vec ) {
    for( PT i = 0; i < vec.size(); ++i )
        if ( vec[ i ] )
            return true;
    return false;
}

template<class T_,int s_,int p_,class T2>
Vec<bool,s_,p_> operator>=( const Vec<T_,s_,p_> &a, const T2 &b ) {
    Vec<bool,s_,p_> res( Size(), a.size() );
    for( PT i = 0; i < a.size(); ++i )
        res[ i ] = a[ i ] >= b;
    return res;
}

template<class T_,int s_,int p_,class T2>
Vec<bool,s_,p_> operator<( const Vec<T_,s_,p_> &a, const T2 &b ) {
    Vec<bool,s_,p_> res( Size(), a.size() );
    for( PT i = 0; i < a.size(); ++i )
        res[ i ] = a[ i ] < b;
    return res;
}

template<class T_,int s_,int p_,class T2,int s2,int p2>
Vec<bool,s_,p_> operator<( const Vec<T_,s_,p_> &a, const Vec<T2,s2,p2> &b ) {
    Vec<bool,s_,p_> res( Size(), a.size() );
    for( PT i = 0; i < a.size(); ++i )
        res[ i ] = a[ i ] < b[ i ];
    return res;
}

template<class T_,int s_,int p_,class T2,int s2,int p2>
Vec<bool,s_,p_> operator>=( const Vec<T_,s_,p_> &a, const Vec<T2,s2,p2> &b ) {
    Vec<bool,s_,p_> res( Size(), a.size() );
    for( PT i = 0; i < a.size(); ++i )
        res[ i ] = a[ i ] >= b[ i ];
    return res;
}

//template<class T_,int s_,int p_,class T2,int s2,int p2>
//Vec<bool,s_,p_> operator!=( const Vec<T_,s_,p_> &a, const Vec<T2,s2,p2> &b ) {
//    Vec<bool,s_,p_> res( Size(), a.size() );
//    for(ST i = 0; i < a.size(); ++i )
//        res[ i ] = a[ i ] != b[ i ];
//    return res;
//}

//template<class T_,int s_,int p_,class T2,int s2,int p2>
//Vec<bool,s_,p_> operator==( const Vec<T_,s_,p_> &a, const Vec<T2,s2,p2> &b ) {
//    Vec<bool,s_,p_> res( Size(), a.size() );
//    for(ST i = 0; i < a.size(); ++i )
//        res[ i ] = a[ i ] == b[ i ];
//    return res;
//}

template<class T,int s,int p>
struct TensorOrder<Vec<T,s,p> > {
    static const int res = 1;
};

/** can be used for vec comparisons (for map, set, ...) */
struct LessVec {
    template<class T0,int s0,int p0,class T1,int s1,int p1>
    bool operator()( const Vec<T0,s0,p0> &x, const Vec<T1,s1,p1> &y ) const {
        if ( x.size() != y.size() )
            return x.size() < y.size();
        for( PT i = 0; i < x.size(); ++i )
            if ( x[ i ] != y[ i ] )
                return x[ i ] < y[ i ];
        return false;
    }
};

/** item that maximize fun */
template<class T,int s,int p,class Op>
const T *val_max( const Vec<T,s,p> &vec, const Op &fun ) {
    if ( vec.empty() )
        return 0;
    using R = decltype( fun( vec[ 0 ] ) );
    R max = fun( vec[ 0 ] );
    const T *res = &vec[ 0 ];
    for( PT i = 1; i < vec.size(); ++i ) {
        R val = fun( vec[ i ] );
        if ( max < val ) {
            max = val;
            res = &vec[ i ];
        }
    }
    return res;
}


}
