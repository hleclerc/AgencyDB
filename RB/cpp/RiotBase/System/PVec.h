#pragma once

#include "Vec.h"

namespace RiotBase {

/**
*/
template<class T>
class PVec {
public:
    PVec( const T *dat, PT len ) : dat( dat ), len( len ) {}
    PVec( const T &dat ) : dat( &dat ), len( 1 ) {}

    template<class U>
    PVec( const Vec<U> &dat ) : dat( dat.ptr() ), len( dat.size() ) {} ///< beware: assumes that dat will exist during the PVec life

    const T &operator[]( PT i ) const { ASSERT_IF_DEBUG( i < len ); return dat[ i ]; }
    PT       size      () const { return len; }

    const T *begin     () const { return dat; }
    const T *end       () const { return dat + len; }

private:
    const T *dat;
    PT       len;
};

}
