#pragma once

#include "TypeConfig.h"

namespace RiotBase {

/**
  Streaming version of xxHash. Algorithm from https://github.com/Cyan4973/xxHash

  Presented as "Extremely fast non-cryptographic hash algorithm"

  A lot of stuff could be optimized in this implementation.

  The algorithm has been taken as is, without analysis... but it seems to be adequate according to the authors.
*/
class XxHash {
public:
    static constexpr PI32 LEN_TMP   =          8U;
    static constexpr PI32 PRIME32_1 = 2654435761U;
    static constexpr PI32 PRIME32_2 = 2246822519U;
    static constexpr PI32 PRIME32_3 = 3266489917U;
    static constexpr PI32 PRIME32_4 =  668265263U;
    static constexpr PI32 PRIME32_5 =  374761393U;

    XxHash() { reset(); }

    void reset() {
        off = 0;
        v[ 0 ] = PRIME32_1 + PRIME32_2;
        v[ 1 ] = PRIME32_2;
        v[ 2 ] = 0;
        v[ 3 ] = PRIME32_1;
    }

    static PI32 rotl32    ( PI32 val, PI32 rot ) { return ( val << rot ) | ( val >> ( 32 - rot ) ); }

    XxHash &operator<<( PI8 val ) { // could be optimized
        reinterpret_cast<PI8 *>( &tmp )[ ( off++ ) % 4 ] = val;
        if ( off % 4 == 0 ) {
            PI32 &n = *reinterpret_cast<PI32 *>( reinterpret_cast<PI8 *>( v ) + off % 16 );
            n = rotl32( n + tmp * PRIME32_2, 13 ) * PRIME32_1;
        }
        return *this;
    }

    void update( const void *data, PT size ) {
        for( PT i = 0; i < size; ++i)
            operator<<( reinterpret_cast<const PI8 *>( data )[ i ] );
    }

    PI32 digest() const {
        PI32 res = rotl32( v[ 0 ], 1 ) + rotl32( v[ 1 ], 7 ) + rotl32( v[ 2 ], 12 ) + rotl32( v[ 3 ], 18 );
        res ^= res >> 15;
        res *= PRIME32_2;
        res ^= res >> 13;
        res *= PRIME32_3;
        res ^= res >> 16;
        return res;
    }

protected:
    PI32        v[ 4 ];
    PI32        tmp;
    int         off;    ///< in bytes
};

} // namespace RiotBase
