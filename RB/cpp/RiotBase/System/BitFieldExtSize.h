#pragma once

#include "Assert.h"
#include "Print.h"
#include "N.h"

namespace RiotBase {

/// bit field, with size stored elsewhere
class BitFieldExtSize {
public:
    template<PT size>
    struct SelectT {
        SelectT &operator=( bool val ) { bfes->set( N<size>(), n, val ); return *this; }
        operator bool() const { return ((const BitFieldExtSize*)bfes)->select( N<size>(), n ); }

        BitFieldExtSize *bfes;
        PT               n;
    };

    struct Select {
        Select &operator=( bool val ) { bfes->set( size, n, val ); return *this; }
        operator bool() const { return ((const BitFieldExtSize*)bfes)->select( size, n ); }

        BitFieldExtSize *bfes;
        PT               size;
        PT               n;
    };

    template<PT size>
    struct WST {
        bool          operator[]     ( PT n ) const { return bfes->select( N<size>(), n ); }
        SelectT<size> operator[]     ( PT n ) { return { bfes, n }; }
        void          write_to_stream( std::ostream &os ) const { bfes->write_to_stream( N<size>(), os ); }

        BitFieldExtSize *bfes;
    };

    struct WS {
        bool   operator[]     ( PT n ) const { return bfes->select( size, n ); }
        Select operator[]     ( PT n ) { return { bfes, size, n }; }
        void   write_to_stream( std::ostream &os ) const { bfes->write_to_stream( size, os ); }

        BitFieldExtSize *bfes;
        PT               size;
    };

    // compile time size versions
    template<PT size> BitFieldExtSize              ( N<size> n, bool val ) { _init( N<( size > 8 * sizeof( PT ) )>(), size, val ); }
    template<PT size> BitFieldExtSize              ( N<size> n ) { _init( N<( size > 8 * sizeof( PT ) )>(), size ); }
    template<PT size> bool          select         ( N<size>, PT n ) const { return _select( N<( size > 8 * sizeof( PT ) )>(), n ); }
    template<PT size> SelectT<size> select         ( N<size>, PT n ) { return { this, n }; }
    template<PT size> void          set            ( N<size>, PT n, bool val ) { _set( N<( size > 8 * sizeof( PT ) )>(), n, val ); }
    template<PT size> void          write_to_stream( N<size>, std::ostream &os ) const { return _write_to_stream( os, N<( size > 8 * sizeof( PT ) )>(), size ); }
    template<PT size> WST<size>     wst            ( N<size> ) { return { this }; }
    template<PT size> WST<size>     wst            () { return { this }; }

    // run time size versions
    BitFieldExtSize        ( PT size, bool val ) { if ( size > 8 * sizeof( PT ) ) _init( N<1>(), size ); else _init( N<0>(), size, val ); }
    BitFieldExtSize        ( PT size ) { if ( size > 8 * sizeof( PT ) ) _init( N<1>(), size ); else _init( N<0>(), size ); }
    bool    select         ( PT size, PT n ) const { return size > 8 * sizeof( PT ) ? _select( N<1>(), n ) : _select( N<0>(), n ); }
    Select  select         ( PT size, PT n ) { return { this, size, n }; }
    void    set            ( PT size, PT n, bool val ) { size > 8 * sizeof( PT ) ? _set( N<1>(), n, val ) : _set( N<0>(), n, val ); }
    void    write_to_stream( PT size, std::ostream &os ) const { return size > 8 * sizeof( PT ) ? _write_to_stream( os, N<1>(), size ) : _write_to_stream( os, N<0>(), size ); }
    WS      wst            ( PT size ) { return { this, size }; }

private:
    void _init           ( N<0>, PT size, bool val ) { data = val ? ~PT( 0 ) : PT( 0 ); }
    void _init           ( N<0>, PT size ) {}
    bool _select         ( N<0>, PT n ) const { return data & ( 1 << n ); }
    void _set            ( N<0>, PT n, bool val ) { if ( val ) data |= 1 << n; else data &= ~( 1 << n ); }
    void _write_to_stream( std::ostream &os, N<0>, PT size ) const { for( PT i = 0; i < size; ++i ) os << _select( N<0>(), i ); }

    void _init           ( N<1>, PT size, bool val ) { TODO; }
    void _init           ( N<1>, PT size ) { TODO; }
    bool _select         ( N<1>, PT n ) const { TODO; return 0; }
    void _set            ( N<1>, PT n, bool val ) { TODO; }
    void _write_to_stream( std::ostream &os, N<1>, PT size ) const { TODO; }

    PT data; ///< may be a pointer if size > 8 * sizeof( PT )
};

} // namespace RiotBase
