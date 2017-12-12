#pragma once

#include "CbString.h"
#include "CbQueue.h"

#include <arpa/inet.h>

namespace RiotBase {

/**
  Enable send/recv in network byte order
*/
template<class TB>
struct NetStream {
    NetStream( TB *buf ) : buf( buf ) {}

    operator bool() const { return not buf->error(); }
    bool error() const { return buf->error(); }
    bool empty() const { return buf->empty(); }
    PT size() const { return buf->size(); }

    // generic writers
    NetStream &write_byte( PI8 val ) { buf->write_byte( val ); return *this; }

    // integer writers
    NetStream &write_integer( PI8  val ) { buf->write_byte( val ); return *this; }
    NetStream &write_integer( PI16 val ) { PI16 v = htons( val ); buf->write_some( &v, sizeof v ); return *this; }
    NetStream &write_integer( PI32 val ) { PI16 v = htonl( val ); buf->write_some( &v, sizeof v ); return *this; }

    // base writers
    NetStream &operator<<( Bool val ) { return write_integer( val ); }

    NetStream &operator<<( PI8  val ) { return write_integer( val ); }
    NetStream &operator<<( PI16 val ) { return write_integer( val ); }
    NetStream &operator<<( PI32 val ) { return write_integer( val ); }
    NetStream &operator<<( PI64 val ) { return write_integer( val ); }

    NetStream &operator<<( SI8  val ) { return write_integer( val ); }
    NetStream &operator<<( SI16 val ) { return write_integer( val ); }
    NetStream &operator<<( SI32 val ) { return write_integer( val ); }
    NetStream &operator<<( SI64 val ) { return write_integer( val ); }

    NetStream &operator<<( char val ) { return write_byte( val ); }

    NetStream &operator<<( NetStream &&str ) { buf->write_some( std::move( *str.buf ) ); return *this; }

    // readers
    void read_integer( PI8  &val ) { val = buf->read_byte(); }
    void read_integer( PI16 &val ) { buf->read_some( val, sizeof val ); val = ntohs( val ); }
    void read_integer( PI32 &val ) { buf->read_some( val, sizeof val ); val = ntohl( val ); }

    // read helpers
    void read( PI8  &val ) { read_integer( val ); }
    void read( PI16 &val ) { read_integer( val ); }
    void read( PI32 &val ) { read_integer( val ); }
    void read( PI64 &val ) { read_integer( val ); }

    void read( SI8  &val ) { read_integer( val ); }
    void read( SI16 &val ) { read_integer( val ); }
    void read( SI32 &val ) { read_integer( val ); }
    void read( SI64 &val ) { read_integer( val ); }

    // std notations
    template<class T>
    NetStream &operator>>( T &val ) {
        read( val );
        return *this;
    }

    // readers like T res = b.read() (or b.read_integer(), ...)
    struct UnsReader { template<class T> operator T() { T res; b->read_integer( res ); return res; } NetStream *b; };
    struct GenReader { template<class T> operator T() { T res; b->read( res ); return res; } NetStream *b; };

    UnsReader read_integer() { return { this }; }
    GenReader read() { return { this }; }

    TB *buf;
};

} // namespace RiotBase
