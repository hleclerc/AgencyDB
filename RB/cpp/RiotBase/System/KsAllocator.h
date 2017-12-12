#pragma once

#include "TypeConfig.h"
#include "N.h"

#ifdef DEBUG
#include <stdlib.h>
#endif

namespace RiotBase {
namespace KsAllocator {

/// used in SmallSizeAllocator
void *new_allocator_chunk( PT size );

///
template<PT size>
struct SmallSizeAllocator {
    // single thread
    static void *malloc() {
        #ifdef DEBUG
        return ::malloc( size );
        #else
        void *res = free_room;
        if ( not res )
            res = new_allocator_chunk( size );
        free_room = *reinterpret_cast<void **>( res );
        return res;
        #endif
    }
    static void free( const void *data ) {
        #ifdef DEBUG
        ::free( const_cast<void *>( data ) );
        #else
        *reinterpret_cast<void **>( const_cast<void *>( data ) ) = free_room;
        free_room = const_cast<void *>( data );
        #endif
    }

    // multithreaded version
    static void *malloc_mt() {
        #ifdef DEBUG
        return malloc();
        #else
        lock();
        void *res = malloc();
        unlock();
        return res;
        #endif
    }
    static void free_mt( const void *data ) {
        #ifdef DEBUG
        free( data );
        #else
        lock();
        *reinterpret_cast<void **>( const_cast<void *>( data ) ) = free_room;
        free_room = const_cast<void *>( data );
        unlock();
        #endif
    }

    static void lock();
    static void unlock();

    static void *free_room;
};

template<PT n>
struct GetN {
    enum { v =
       n <=    8 ?    8 : (
       n <=   16 ?   16 : (
       n <=   20 ?   20 : (
       n <=   24 ?   24 : (
       n <=   28 ?   28 : (
       n <=   32 ?   32 : (
       n <=   48 ?   48 : (
       n <=   64 ?   64 : (
       n <=   80 ?   80 : (
       n <=   96 ?   96 : (
       n <=  112 ?  112 : (
       n <=  128 ?  128 : (
       n <=  160 ?  160 : (
       n <=  192 ?  192 : (
       n <=  224 ?  224 : (
       n <=  256 ?  256 : (
       n <=  512 ?  512 : (
       n <= 1024 ? 1024 : (
       n <= 2048 ? 2048 : (
       n <= 4096 ? 4096 : (
       8192 ))))))))))))))))))))
    };
};

void *malloc( PT &size ); ///< generic malloc (size is updated to reflect the actual reserved room)

inline void *malloc( N<   8> ) { return SmallSizeAllocator<   8>::malloc(); }
inline void *malloc( N<  16> ) { return SmallSizeAllocator<  16>::malloc(); }
inline void *malloc( N<  20> ) { return SmallSizeAllocator<  20>::malloc(); }
inline void *malloc( N<  24> ) { return SmallSizeAllocator<  24>::malloc(); }
inline void *malloc( N<  28> ) { return SmallSizeAllocator<  28>::malloc(); }
inline void *malloc( N<  32> ) { return SmallSizeAllocator<  32>::malloc(); }
inline void *malloc( N<  48> ) { return SmallSizeAllocator<  48>::malloc(); }
inline void *malloc( N<  64> ) { return SmallSizeAllocator<  64>::malloc(); }
inline void *malloc( N<  80> ) { return SmallSizeAllocator<  80>::malloc(); }
inline void *malloc( N<  96> ) { return SmallSizeAllocator<  96>::malloc(); }
inline void *malloc( N< 112> ) { return SmallSizeAllocator< 112>::malloc(); }
inline void *malloc( N< 128> ) { return SmallSizeAllocator< 128>::malloc(); }
inline void *malloc( N< 160> ) { return SmallSizeAllocator< 160>::malloc(); }
inline void *malloc( N< 192> ) { return SmallSizeAllocator< 192>::malloc(); }
inline void *malloc( N< 224> ) { return SmallSizeAllocator< 224>::malloc(); }
inline void *malloc( N< 256> ) { return SmallSizeAllocator< 256>::malloc(); }
inline void *malloc( N< 512> ) { return SmallSizeAllocator< 512>::malloc(); }
inline void *malloc( N<1024> ) { return SmallSizeAllocator<1024>::malloc(); }
inline void *malloc( N<2048> ) { return SmallSizeAllocator<2048>::malloc(); }
inline void *malloc( N<4096> ) { return SmallSizeAllocator<4096>::malloc(); }
inline void *malloc( N<8192> ) { return SmallSizeAllocator<8192>::malloc(); }

template<PT n>
inline void *malloc( N<n> ) {
    if ( n > 8192 ) {
        PT s = n;
        return malloc( s );
    }
    return malloc( N<GetN<n>::v>() );
}

template<PT n>
inline void *malloc( PT &r, N<n> ) {
    if ( n > 8192 ) {
        r = n;
        return malloc( r );
    }
    r = GetN<n>::v;
    return malloc( N<GetN<n>::v>() );
}

inline void *malloc( PT &r, PT n ) {
    r = n;
    return malloc( r );
}

/// for instance: MyClass *a = KsAllocator::New();
struct New {
    template<class T>
    operator T *() const {
        return new ( malloc( N<sizeof(T)>() ) ) T;
    }
};

void free( void *data, PT size ); ///< generic free

inline void free( void *data, N<   8> ) { SmallSizeAllocator<   8>::free( data ); }
inline void free( void *data, N<  16> ) { SmallSizeAllocator<  16>::free( data ); }
inline void free( void *data, N<  20> ) { SmallSizeAllocator<  20>::free( data ); }
inline void free( void *data, N<  24> ) { SmallSizeAllocator<  24>::free( data ); }
inline void free( void *data, N<  28> ) { SmallSizeAllocator<  28>::free( data ); }
inline void free( void *data, N<  32> ) { SmallSizeAllocator<  32>::free( data ); }
inline void free( void *data, N<  48> ) { SmallSizeAllocator<  48>::free( data ); }
inline void free( void *data, N<  64> ) { SmallSizeAllocator<  64>::free( data ); }
inline void free( void *data, N<  80> ) { SmallSizeAllocator<  80>::free( data ); }
inline void free( void *data, N<  96> ) { SmallSizeAllocator<  96>::free( data ); }
inline void free( void *data, N< 112> ) { SmallSizeAllocator< 112>::free( data ); }
inline void free( void *data, N< 128> ) { SmallSizeAllocator< 128>::free( data ); }
inline void free( void *data, N< 160> ) { SmallSizeAllocator< 160>::free( data ); }
inline void free( void *data, N< 192> ) { SmallSizeAllocator< 192>::free( data ); }
inline void free( void *data, N< 224> ) { SmallSizeAllocator< 224>::free( data ); }
inline void free( void *data, N< 256> ) { SmallSizeAllocator< 256>::free( data ); }
inline void free( void *data, N< 512> ) { SmallSizeAllocator< 512>::free( data ); }
inline void free( void *data, N<1024> ) { SmallSizeAllocator<1024>::free( data ); }
inline void free( void *data, N<2048> ) { SmallSizeAllocator<2048>::free( data ); }
inline void free( void *data, N<4096> ) { SmallSizeAllocator<4096>::free( data ); }
inline void free( void *data, N<8192> ) { SmallSizeAllocator<8192>::free( data ); }

template<PT n>
void free( void *data, N<n> ) {
    if ( n > 8192 ) { return free( data, n ); }
    return free( data, N<GetN<n>::v>() );
}

template<class T>
void free( T *data ) {
    free( data, N<sizeof(T)>() );
}

template<class T>
void del( T *data ) {
    data->~T();
    free( data );
}

struct Delete {
    template<class T>
    void operator()( T *data ) { if ( data ) del( data ); }
};

} // namespace KsAllocator

} // namespace RiotBase
