#pragma once

#include <boost/intrusive/avl_set.hpp>
#include "../System/InetAddress.h"
#include "../System/Vec.h"
#include "../System/Log.h"

#include "UdpSocketAncestor.h"


namespace RiotBase {

/**
  TODO
  - if we try to send something in a full buffer,
*/
template<class Impl,class Conn>
class UdpSocket : public UdpSocketAncestor {
public:
    ~UdpSocket();

    Conn          *connection    ( const InetAddress &addr, bool server_if_creation = false ); ///< get connection for a given address

protected:
    virtual void   inp           ();
    virtual void   out           ();
    virtual Conn  *new_connection( const InetAddress &addr, bool server );
};

template<class Impl,class Conn>
UdpSocket<Impl,Conn>::~UdpSocket() {
    // we make a Vec of connections to delete them. Is there a clever solution to avoid this ?
    Vec<Conn *> v( Rese{}, connection_map.size() );
    for( UdpConnection &n : connection_map )
        v << static_cast<Conn *>( &n );
    connection_map.clear();
    for( Conn *n : v )
        delete n;
}


template<class Impl,class Conn>
Conn *UdpSocket<Impl,Conn>::connection( const InetAddress &addr, bool server_if_creation ) {
    auto iter = connection_map.find( addr, Cmp{} );
    if ( iter == connection_map.end() )
        iter = connection_map.insert( *new_connection( addr, server_if_creation ) );
    return static_cast<Conn *>( &*iter );
}

template<class Impl,class Conn>
void UdpSocket<Impl,Conn>::inp() {
    char inp_buf[ Conn::inp_buf_size ];
    while ( true ) {
        // try to read some data
        InetAddress src;
        socklen_t src_sa_len = sizeof src.sa;
        ssize_t inp_len = recvfrom( fd, inp_buf, Conn::inp_buf_size, MSG_WAITALL, (struct sockaddr *)&src.sa, &src_sa_len );
        if ( inp_len <= 0 ) {
            // DISP_ERROR( "inp_len={} from {}, errno={}", inp_len, PT( this ), errno );
            if ( errno == EAGAIN or errno == EWOULDBLOCK )
                return wait_for_more_inp();
            DISP_ERROR( "recvfrom {} (errno={},inp_len={})", strerror( errno ), errno, inp_len );
            return hup_error();
        }

        // parse
        Conn *c = connection( src, true );
        c->parse_anc( inp_buf, inp_len );
    }
}


template<class Impl,class Conn>
void UdpSocket<Impl,Conn>::out() {
    connections_with_pending_output.clear( []( UdpConnection *c ) {
        static_cast<Conn *>( c )->out();
    } );
}

template<class Impl,class Conn>
Conn *UdpSocket<Impl,Conn>::new_connection( const InetAddress &inet_address, bool server ) {
    Conn *c = new Conn;
    c->inet_address = inet_address;
    c->udp_socket = this; // static_cast<Impl *>( this );
    c->init( static_cast<Impl *>( this ), server );
    return c;
}

} // namespace RiotBase
