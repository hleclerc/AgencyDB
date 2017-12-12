#pragma once

#include "TypeConfig.h"
#include "RcPtr.h"

#include <stdlib.h>

namespace RiotBase {

/**
*/
class Buffer {
public:
    enum { default_size = 2048 - 3 * sizeof( unsigned ) - sizeof( Buffer * ) };

    static Buffer *New( unsigned size = default_size, Buffer *prev = 0 ) {
        Buffer *res = (Buffer *)malloc( sizeof( Buffer ) + size - 4 );
        if ( prev ) prev->next = res;
        res->cpt_use = 0;
        res->used    = 0;
        res->next    = 0;
        res->size    = size;
        return res;
    }

    static void operator delete( void *ptr ) {
        free( ptr );
    }

    unsigned room() const {
        return size - used;
    }

    PT cum_size() const {
       PT res = 0;
       for( const Buffer *b = this; b; b = b->next )
           res += b->used;
       return res;
    }

    // attributes
    mutable int cpt_use;   ///< destroyed if < 0
    unsigned    used;      ///< nb items stored in data
    Buffer     *next;      ///<
    unsigned    size;      ///< real size of data[]
    PI8         data[ 4 ]; ///<
};

}
