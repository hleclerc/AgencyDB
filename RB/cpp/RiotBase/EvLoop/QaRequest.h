#pragma once

#include "TcpRawConnection.h"
#include "Util/Socket.h"
#include "Util/Url.h"
#include "Gev.h"

namespace RiotBase {

/** input -> const std::function<void(BBQ)>
    output = fixed size answers ( 2 * 64 bits )
*/
template<class Op>
struct Qa_Connection : TcpRawConnection {
    Qa_Connection( int fd, const Op &output ) : TcpRawConnection( fd ), output( output ) {
    }

    virtual bool parse( Buffer *buff ) {
        return output( &cw, buff );
    }

    Op output; // e.g. a std::function<bool(BBQ, const Buffer *)>
};

/// version that does not wait for any answer
template<class Ip>
void qa_request( const char *url, const Ip &input ) {
    // set up a new connection, send the data, and delete
    Url a( url );
    auto op = []( BBQ, Buffer * ) { return false; };
    Qa_Connection<decltype( op )> c{ client_socket( a.host(), a.port() ), op };
    input( c.cb_queue() );
    c.flush();
}

template<class Ip,class Op>
void qa_request( const char *url, const Ip &input, const Op &output, double timeout = 10.0 ) {
    // set up a new connection
    Url a( url );

    Qa_Connection<Op> *c = new Qa_Connection<Op>{ client_socket( a.host(), a.port() ), output };
    *gev << c;

    // send the data
    input( c->cb_queue() );
    c->flush();
}

template<class Ip,class Op>
void qa_sized_request( const char *url, const Ip &input, const Op &output, PT initial_size, double timeout = 10.0 ) {
    struct TmpOp {
        TmpOp( const Op &output, PT initial_size ) : output( output ), size( initial_size ) {
        }

        bool operator()( BBQ bq, const Buffer *buff ) const {
            data.append( buff->data, buff->data + buff->used );
            while ( size and data.size() >= size ) {
                PT new_size = output( bq, reinterpret_cast<char *>( data.ptr() ) );
                data.remove_some( 0, size );
                size = new_size;
            }
            return size;
        }

        Op               output;
        mutable PT       size;
        mutable Vec<PI8> data;
    };

    qa_request( url, input, TmpOp{ output, initial_size }, timeout );
}

} // namespace RiotBase

