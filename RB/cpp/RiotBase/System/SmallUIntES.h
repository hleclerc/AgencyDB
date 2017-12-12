#pragma once

#include "Assert.h"
#include "Print.h"
#include "N.h"

namespace RiotBase {

/// to store collections of small unsigned integers, with size known from an external source (compile time or run time)
/// Typicall application: set of bits, sets of 2 bits integers...
template<int uint_size_in_bits>
class SmallUIntES {
public:
    template<PT size>
    struct SelectT {
        SelectT &operator=( unsigned val ) { bfes->set( N<size>(), n, val ); return *this; }
        operator unsigned () const { return ((const SmallUIntES*)bfes)->select( N<size>(), n ); }

        SmallUIntES *bfes;
        PT           n;
    };

    struct Select {
        Select  &operator=( unsigned val ) { bfes->set( size, n, val ); return *this; }
        operator unsigned () const { return ((const SmallUIntES*)bfes)->select( size, n ); }

        SmallUIntES *bfes;
        PT           size;
        PT           n;
    };

    template<PT size>
    struct WST {
        unsigned      operator[]     ( PT n ) const { return bfes->select( N<size>(), n ); }
        SelectT<size> operator[]     ( PT n ) { return { bfes, n }; }
        void          write_to_stream( std::ostream &os ) const { bfes->write_to_stream( N<size>(), os ); }

        SmallUIntES  *bfes;
    };

    struct WS {
        unsigned     operator[]     ( PT n ) const { return bfes->select( size, n ); }
        Select       operator[]     ( PT n ) { return { bfes, size, n }; }
        void         write_to_stream( std::ostream &os ) const { bfes->write_to_stream( size, os ); }

        SmallUIntES *bfes;
        PT           size;
    };

    // compile time size versions
    template<PT size> SmallUIntES                  ( N<size> n, unsigned val ) { _init( N<( uint_size_in_bits * uint_size_in_bits * size > 8 * sizeof( PT ) )>(), size, val ); }
    template<PT size> SmallUIntES                  ( N<size> n ) { _init( N<( uint_size_in_bits * size > 8 * sizeof( PT ) )>(), size ); }
    template<PT size> unsigned      select         ( N<size>, PT n ) const { return _select( N<( uint_size_in_bits * size > 8 * sizeof( PT ) )>(), n ); }
    template<PT size> SelectT<size> select         ( N<size>, PT n ) { return { this, n }; }
    template<PT size> void          set            ( N<size>, PT n, unsigned val ) { _set( N<( uint_size_in_bits * size > 8 * sizeof( PT ) )>(), n, val ); }
    template<PT size> void          write_to_stream( N<size>, std::ostream &os ) const { return _write_to_stream( os, N<( uint_size_in_bits * size > 8 * sizeof( PT ) )>(), size ); }
    template<PT size> WST<size>     wst            ( N<size> ) { return { this }; }
    template<PT size> WST<size>     wst            () { return { this }; }

    // run time size versions
    SmallUIntES              ( PT size, unsigned val ) { if ( uint_size_in_bits * size > 8 * sizeof( PT ) ) _init( N<1>(), size ); else _init( N<0>(), size, val ); }
    SmallUIntES              ( PT size ) { if ( uint_size_in_bits * size > 8 * sizeof( PT ) ) _init( N<1>(), size ); else _init( N<0>(), size ); }
    unsigned select          ( PT size, PT n ) const { return uint_size_in_bits * size > 8 * sizeof( PT ) ? _select( N<1>(), n ) : _select( N<0>(), n ); }
    Select   select          ( PT size, PT n ) { return { this, size, n }; }
    void     set             ( PT size, PT n, unsigned val ) { uint_size_in_bits * size > 8 * sizeof( PT ) ? _set( N<1>(), n, val ) : _set( N<0>(), n, val ); }
    void     write_to_stream ( PT size, std::ostream &os ) const { return uint_size_in_bits * size > 8 * sizeof( PT ) ? _write_to_stream( os, N<1>(), size ) : _write_to_stream( os, N<0>(), size ); }
    WS       wst             ( PT size ) { return { this, size }; }

private:
    void     _init           ( N<0>, PT size, unsigned val ) { for( PT i = 0; i < size; ++i ) _set( N<0>(), i, val ); }
    void     _init           ( N<0>, PT size ) {}
    unsigned _select         ( N<0>, PT n ) const { return ( data >> uint_size_in_bits * n ) & ( ( 1 << uint_size_in_bits ) - 1 ); }
    void     _set            ( N<0>, PT n, unsigned val ) { PT cl = ( ( 1 << uint_size_in_bits ) - 1 ) << uint_size_in_bits * n; data &= ~cl; data |= ( val << uint_size_in_bits * n ) & cl; }
    void     _write_to_stream( std::ostream &os, N<0>, PT size ) const { for( PT i = 0; i < size; ++i ) os << ( i ? " " : "" ) << _select( N<0>(), i ); }

    void     _init           ( N<1>, PT size, unsigned val ) { TODO; }
    void     _init           ( N<1>, PT size ) { TODO; }
    unsigned _select         ( N<1>, PT n ) const { TODO; return 0; }
    void     _set            ( N<1>, PT n, unsigned val ) { TODO; }
    void     _write_to_stream( std::ostream &os, N<1>, PT size ) const { TODO; }

    PT       data; ///< may be a pointer if uint_size_in_bits * size > 8 * sizeof( PT )
};

} // namespace RiotBase
