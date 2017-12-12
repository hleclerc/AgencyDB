#include "KsAllocator.h"
#include <pthread.h>

#ifdef WIN32
#include <windows.h>
#else
#include <sys/mman.h>
#endif

//void *malloc( size_t s ) {
//    return new char[ s ];
//}
//void free( void *ptr ) {
//    delete [] (char *)ptr;
//}

namespace RiotBase {
namespace KsAllocator {

static const PT size_mmap = 8192;

void *new_allocator_chunk( PT size_in_mem ) {
    #ifdef WIN32
    void *res = std::malloc( size_mmap );
    #else
    void *res = mmap( 0, size_mmap, PROT_EXEC | PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0 ); //
    #endif

    // chained list
    char *beg = reinterpret_cast<char *>( res );
    char *end = beg + size_mmap - 2 * size_in_mem + 1;
    for( ; beg < end ; beg += size_in_mem )
        *reinterpret_cast<void **>( beg ) = reinterpret_cast<void *>( beg + size_in_mem );
    *reinterpret_cast<void **>( beg ) = 0;

    return res;
}

void *new_room_large_size( PT &size_in_mem ) {
    size_in_mem = ( size_in_mem + size_mmap - 1 ) & ~( size_mmap - 1 );
    #ifdef WIN32
      return std::malloc( size_in_mem );
    #else
      #ifdef DEBUG
        return ::malloc( size_in_mem );
      #else
        return mmap( 0, size_in_mem, PROT_EXEC | PROT_READ | PROT_WRITE, MAP_PRIVATE, -1, 0 ); ///< MAP_ANONYMOUS |
      #endif
    #endif
}

void del_room_large_size( const void *data, ST size_in_mem ) {
    #ifdef WIN32
      std::free( const_cast<void *>( data ) );
    #else
      #ifdef DEBUG
        ::free( const_cast<void *>( data ) );
      #else
        size_in_mem = ( size_in_mem + size_mmap - 1 ) & ~( size_mmap - 1 );
        munmap( const_cast<void *>( data ), size_in_mem );
      #endif
    #endif
}

void *malloc( PT &size ) {
    if ( size <=    8 ) { size =    8; return SmallSizeAllocator<   8>::malloc(); }
    if ( size <=   16 ) { size =   16; return SmallSizeAllocator<  16>::malloc(); }
    if ( size <=   20 ) { size =   20; return SmallSizeAllocator<  20>::malloc(); }
    if ( size <=   24 ) { size =   24; return SmallSizeAllocator<  24>::malloc(); }
    if ( size <=   28 ) { size =   28; return SmallSizeAllocator<  28>::malloc(); }
    if ( size <=   32 ) { size =   32; return SmallSizeAllocator<  32>::malloc(); }
    if ( size <=   48 ) { size =   48; return SmallSizeAllocator<  48>::malloc(); }
    if ( size <=   64 ) { size =   64; return SmallSizeAllocator<  64>::malloc(); }
    if ( size <=   80 ) { size =   80; return SmallSizeAllocator<  80>::malloc(); }
    if ( size <=   96 ) { size =   96; return SmallSizeAllocator<  96>::malloc(); }
    if ( size <=  112 ) { size =  112; return SmallSizeAllocator< 112>::malloc(); }
    if ( size <=  128 ) { size =  128; return SmallSizeAllocator< 128>::malloc(); }
    if ( size <=  160 ) { size =  160; return SmallSizeAllocator< 160>::malloc(); }
    if ( size <=  192 ) { size =  192; return SmallSizeAllocator< 192>::malloc(); }
    if ( size <=  224 ) { size =  224; return SmallSizeAllocator< 224>::malloc(); }
    if ( size <=  256 ) { size =  256; return SmallSizeAllocator< 256>::malloc(); }
    if ( size <=  512 ) { size =  512; return SmallSizeAllocator< 512>::malloc(); }
    if ( size <= 1024 ) { size = 1024; return SmallSizeAllocator<1024>::malloc(); }
    if ( size <= 2048 ) { size = 2048; return SmallSizeAllocator<2048>::malloc(); }
    if ( size <= 4096 ) { size = 4096; return SmallSizeAllocator<4096>::malloc(); }
    if ( size <= 8192 ) { size = 8192; return SmallSizeAllocator<8192>::malloc(); }
    return new_room_large_size( size );
}

void free( void *address, PT size ) {
    if ( size <=    8 ) return SmallSizeAllocator<   8>::free( address );
    if ( size <=   16 ) return SmallSizeAllocator<  16>::free( address );
    if ( size <=   20 ) return SmallSizeAllocator<  20>::free( address );
    if ( size <=   24 ) return SmallSizeAllocator<  24>::free( address );
    if ( size <=   28 ) return SmallSizeAllocator<  28>::free( address );
    if ( size <=   32 ) return SmallSizeAllocator<  32>::free( address );
    if ( size <=   48 ) return SmallSizeAllocator<  48>::free( address );
    if ( size <=   64 ) return SmallSizeAllocator<  64>::free( address );
    if ( size <=   80 ) return SmallSizeAllocator<  80>::free( address );
    if ( size <=   96 ) return SmallSizeAllocator<  96>::free( address );
    if ( size <=  112 ) return SmallSizeAllocator< 112>::free( address );
    if ( size <=  128 ) return SmallSizeAllocator< 128>::free( address );
    if ( size <=  160 ) return SmallSizeAllocator< 160>::free( address );
    if ( size <=  192 ) return SmallSizeAllocator< 192>::free( address );
    if ( size <=  224 ) return SmallSizeAllocator< 224>::free( address );
    if ( size <=  256 ) return SmallSizeAllocator< 256>::free( address );
    if ( size <=  512 ) return SmallSizeAllocator< 512>::free( address );
    if ( size <= 1024 ) return SmallSizeAllocator<1024>::free( address );
    if ( size <= 2048 ) return SmallSizeAllocator<2048>::free( address );
    if ( size <= 4096 ) return SmallSizeAllocator<4096>::free( address );
    if ( size <= 8192 ) return SmallSizeAllocator<8192>::free( address );
    del_room_large_size( address, size );
}


template<> void *SmallSizeAllocator<   8>::free_room = 0;
template<> void *SmallSizeAllocator<  16>::free_room = 0;
template<> void *SmallSizeAllocator<  20>::free_room = 0;
template<> void *SmallSizeAllocator<  24>::free_room = 0;
template<> void *SmallSizeAllocator<  28>::free_room = 0;
template<> void *SmallSizeAllocator<  32>::free_room = 0;
template<> void *SmallSizeAllocator<  48>::free_room = 0;
template<> void *SmallSizeAllocator<  64>::free_room = 0;
template<> void *SmallSizeAllocator<  80>::free_room = 0;
template<> void *SmallSizeAllocator<  96>::free_room = 0;
template<> void *SmallSizeAllocator< 112>::free_room = 0;
template<> void *SmallSizeAllocator< 128>::free_room = 0;
template<> void *SmallSizeAllocator< 160>::free_room = 0;
template<> void *SmallSizeAllocator< 192>::free_room = 0;
template<> void *SmallSizeAllocator< 224>::free_room = 0;
template<> void *SmallSizeAllocator< 256>::free_room = 0;
template<> void *SmallSizeAllocator< 512>::free_room = 0;
template<> void *SmallSizeAllocator<1024>::free_room = 0;
template<> void *SmallSizeAllocator<2048>::free_room = 0;
template<> void *SmallSizeAllocator<4096>::free_room = 0;
template<> void *SmallSizeAllocator<8192>::free_room = 0;

template<PT size>
void SmallSizeAllocator<size>::lock() {}

template<PT size>
void SmallSizeAllocator<size>::unlock() {}

template<PT n> struct SmallSizeAllocatorMutex { static pthread_mutex_t mutex; };

template<> pthread_mutex_t SmallSizeAllocatorMutex<   8>::mutex = PTHREAD_MUTEX_INITIALIZER;
template<> pthread_mutex_t SmallSizeAllocatorMutex<  16>::mutex = PTHREAD_MUTEX_INITIALIZER;
template<> pthread_mutex_t SmallSizeAllocatorMutex<  20>::mutex = PTHREAD_MUTEX_INITIALIZER;
template<> pthread_mutex_t SmallSizeAllocatorMutex<  24>::mutex = PTHREAD_MUTEX_INITIALIZER;
template<> pthread_mutex_t SmallSizeAllocatorMutex<  28>::mutex = PTHREAD_MUTEX_INITIALIZER;
template<> pthread_mutex_t SmallSizeAllocatorMutex<  32>::mutex = PTHREAD_MUTEX_INITIALIZER;
template<> pthread_mutex_t SmallSizeAllocatorMutex<  48>::mutex = PTHREAD_MUTEX_INITIALIZER;
template<> pthread_mutex_t SmallSizeAllocatorMutex<  64>::mutex = PTHREAD_MUTEX_INITIALIZER;
template<> pthread_mutex_t SmallSizeAllocatorMutex<  80>::mutex = PTHREAD_MUTEX_INITIALIZER;
template<> pthread_mutex_t SmallSizeAllocatorMutex<  96>::mutex = PTHREAD_MUTEX_INITIALIZER;
template<> pthread_mutex_t SmallSizeAllocatorMutex< 112>::mutex = PTHREAD_MUTEX_INITIALIZER;
template<> pthread_mutex_t SmallSizeAllocatorMutex< 128>::mutex = PTHREAD_MUTEX_INITIALIZER;
template<> pthread_mutex_t SmallSizeAllocatorMutex< 160>::mutex = PTHREAD_MUTEX_INITIALIZER;
template<> pthread_mutex_t SmallSizeAllocatorMutex< 192>::mutex = PTHREAD_MUTEX_INITIALIZER;
template<> pthread_mutex_t SmallSizeAllocatorMutex< 224>::mutex = PTHREAD_MUTEX_INITIALIZER;
template<> pthread_mutex_t SmallSizeAllocatorMutex< 256>::mutex = PTHREAD_MUTEX_INITIALIZER;
template<> pthread_mutex_t SmallSizeAllocatorMutex< 512>::mutex = PTHREAD_MUTEX_INITIALIZER;
template<> pthread_mutex_t SmallSizeAllocatorMutex<1024>::mutex = PTHREAD_MUTEX_INITIALIZER;
template<> pthread_mutex_t SmallSizeAllocatorMutex<2048>::mutex = PTHREAD_MUTEX_INITIALIZER;
template<> pthread_mutex_t SmallSizeAllocatorMutex<4096>::mutex = PTHREAD_MUTEX_INITIALIZER;
template<> pthread_mutex_t SmallSizeAllocatorMutex<8192>::mutex = PTHREAD_MUTEX_INITIALIZER;

template<> void SmallSizeAllocator<   8>::lock() { pthread_mutex_lock( &SmallSizeAllocatorMutex<   8>::mutex ); }
template<> void SmallSizeAllocator<  16>::lock() { pthread_mutex_lock( &SmallSizeAllocatorMutex<  16>::mutex ); }
template<> void SmallSizeAllocator<  20>::lock() { pthread_mutex_lock( &SmallSizeAllocatorMutex<  20>::mutex ); }
template<> void SmallSizeAllocator<  24>::lock() { pthread_mutex_lock( &SmallSizeAllocatorMutex<  24>::mutex ); }
template<> void SmallSizeAllocator<  28>::lock() { pthread_mutex_lock( &SmallSizeAllocatorMutex<  28>::mutex ); }
template<> void SmallSizeAllocator<  32>::lock() { pthread_mutex_lock( &SmallSizeAllocatorMutex<  32>::mutex ); }
template<> void SmallSizeAllocator<  48>::lock() { pthread_mutex_lock( &SmallSizeAllocatorMutex<  48>::mutex ); }
template<> void SmallSizeAllocator<  64>::lock() { pthread_mutex_lock( &SmallSizeAllocatorMutex<  64>::mutex ); }
template<> void SmallSizeAllocator<  80>::lock() { pthread_mutex_lock( &SmallSizeAllocatorMutex<  80>::mutex ); }
template<> void SmallSizeAllocator<  96>::lock() { pthread_mutex_lock( &SmallSizeAllocatorMutex<  96>::mutex ); }
template<> void SmallSizeAllocator< 112>::lock() { pthread_mutex_lock( &SmallSizeAllocatorMutex< 112>::mutex ); }
template<> void SmallSizeAllocator< 128>::lock() { pthread_mutex_lock( &SmallSizeAllocatorMutex< 128>::mutex ); }
template<> void SmallSizeAllocator< 160>::lock() { pthread_mutex_lock( &SmallSizeAllocatorMutex< 160>::mutex ); }
template<> void SmallSizeAllocator< 192>::lock() { pthread_mutex_lock( &SmallSizeAllocatorMutex< 192>::mutex ); }
template<> void SmallSizeAllocator< 224>::lock() { pthread_mutex_lock( &SmallSizeAllocatorMutex< 224>::mutex ); }
template<> void SmallSizeAllocator< 256>::lock() { pthread_mutex_lock( &SmallSizeAllocatorMutex< 256>::mutex ); }
template<> void SmallSizeAllocator< 512>::lock() { pthread_mutex_lock( &SmallSizeAllocatorMutex< 512>::mutex ); }
template<> void SmallSizeAllocator<1024>::lock() { pthread_mutex_lock( &SmallSizeAllocatorMutex<1024>::mutex ); }
template<> void SmallSizeAllocator<2048>::lock() { pthread_mutex_lock( &SmallSizeAllocatorMutex<2048>::mutex ); }
template<> void SmallSizeAllocator<4096>::lock() { pthread_mutex_lock( &SmallSizeAllocatorMutex<4096>::mutex ); }
template<> void SmallSizeAllocator<8192>::lock() { pthread_mutex_lock( &SmallSizeAllocatorMutex<8192>::mutex ); }

template<> void SmallSizeAllocator<   8>::unlock() { pthread_mutex_unlock( &SmallSizeAllocatorMutex<   8>::mutex ); }
template<> void SmallSizeAllocator<  16>::unlock() { pthread_mutex_unlock( &SmallSizeAllocatorMutex<  16>::mutex ); }
template<> void SmallSizeAllocator<  20>::unlock() { pthread_mutex_unlock( &SmallSizeAllocatorMutex<  20>::mutex ); }
template<> void SmallSizeAllocator<  24>::unlock() { pthread_mutex_unlock( &SmallSizeAllocatorMutex<  24>::mutex ); }
template<> void SmallSizeAllocator<  28>::unlock() { pthread_mutex_unlock( &SmallSizeAllocatorMutex<  28>::mutex ); }
template<> void SmallSizeAllocator<  32>::unlock() { pthread_mutex_unlock( &SmallSizeAllocatorMutex<  32>::mutex ); }
template<> void SmallSizeAllocator<  48>::unlock() { pthread_mutex_unlock( &SmallSizeAllocatorMutex<  48>::mutex ); }
template<> void SmallSizeAllocator<  64>::unlock() { pthread_mutex_unlock( &SmallSizeAllocatorMutex<  64>::mutex ); }
template<> void SmallSizeAllocator<  80>::unlock() { pthread_mutex_unlock( &SmallSizeAllocatorMutex<  80>::mutex ); }
template<> void SmallSizeAllocator<  96>::unlock() { pthread_mutex_unlock( &SmallSizeAllocatorMutex<  96>::mutex ); }
template<> void SmallSizeAllocator< 112>::unlock() { pthread_mutex_unlock( &SmallSizeAllocatorMutex< 112>::mutex ); }
template<> void SmallSizeAllocator< 128>::unlock() { pthread_mutex_unlock( &SmallSizeAllocatorMutex< 128>::mutex ); }
template<> void SmallSizeAllocator< 160>::unlock() { pthread_mutex_unlock( &SmallSizeAllocatorMutex< 160>::mutex ); }
template<> void SmallSizeAllocator< 192>::unlock() { pthread_mutex_unlock( &SmallSizeAllocatorMutex< 192>::mutex ); }
template<> void SmallSizeAllocator< 224>::unlock() { pthread_mutex_unlock( &SmallSizeAllocatorMutex< 224>::mutex ); }
template<> void SmallSizeAllocator< 256>::unlock() { pthread_mutex_unlock( &SmallSizeAllocatorMutex< 256>::mutex ); }
template<> void SmallSizeAllocator< 512>::unlock() { pthread_mutex_unlock( &SmallSizeAllocatorMutex< 512>::mutex ); }
template<> void SmallSizeAllocator<1024>::unlock() { pthread_mutex_unlock( &SmallSizeAllocatorMutex<1024>::mutex ); }
template<> void SmallSizeAllocator<2048>::unlock() { pthread_mutex_unlock( &SmallSizeAllocatorMutex<2048>::mutex ); }
template<> void SmallSizeAllocator<4096>::unlock() { pthread_mutex_unlock( &SmallSizeAllocatorMutex<4096>::mutex ); }
template<> void SmallSizeAllocator<8192>::unlock() { pthread_mutex_unlock( &SmallSizeAllocatorMutex<8192>::mutex ); }

} // namespace KsAllocator
} // namespace RiotBase
