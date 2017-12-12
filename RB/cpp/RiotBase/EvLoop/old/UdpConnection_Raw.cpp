//#include "UdpConnection_Raw.h"
//#include "UdpSocket.h"

//namespace RiotBase {

//UdpConnection_Raw::UdpConnection_Raw( InetAddress inet_address, UdpSocket *udp_socket ) : UdpConnection( inet_address, udp_socket ) {
//}

//void UdpConnection_Raw::flush() {
//    while ( not cw.empty() ) {
//        PT nbuf = cw.nbuf();

//        // messages from cw
//        iovec iov[ nbuf ], *iov_ptr = iov;
//        cw.data_visitor( [ &iov_ptr ]( const PI8 *beg, const PI8 *end ) {
//            iov_ptr->iov_base = (void *)beg;
//            iov_ptr->iov_len  = end - beg;
//            ++iov_ptr;
//        } );

//        // sendmsg
//        msghdr msg; bzero( &msg, sizeof msg );
//        msg.msg_name    = &inet_address.sa;
//        msg.msg_namelen = sizeof inet_address.sa;
//        msg.msg_iovlen  = nbuf;
//        msg.msg_iov     = iov;

//        ssize_t real = sendmsg( udp_socket->fd, &msg, MSG_NOSIGNAL );
//        if ( real <= 0 ) { // error ?
//            if ( real < 0 and ( errno == EAGAIN or errno == EWOULDBLOCK ) )
//                return udp_socket->wait_for_more_out();
//            return udp_socket->hup_error();
//        }

//        // what has been sent
//        cw.skip_some( real );
//    }
//}

//void UdpConnection_Raw::write_some( const void *data, PT size ) {
//    // better to temporarily store data in the cw buffer ?
//    if ( size < 128 and cw.size() < 1024 )
//        return cw.write_some( data, size );

//    // else, send what we have
//    while ( not cw.empty() ) {
//        PT nbuf = cw.nbuf();

//        // messages from cw
//        iovec iov[ nbuf + 1 ], *iov_ptr = iov;
//        cw.data_visitor( [ &iov_ptr ]( const PI8 *beg, const PI8 *end ) {
//            iov_ptr->iov_base = (void *)beg;
//            iov_ptr->iov_len  = end - beg;
//            ++iov_ptr;
//        } );

//        // message from [data, size]
//        iov_ptr->iov_base = (void *)data;
//        iov_ptr->iov_len  = size;

//        // sendmsg
//        msghdr msg; bzero( &msg, sizeof msg );
//        msg.msg_name    = &inet_address.sa;
//        msg.msg_namelen = sizeof inet_address.sa;
//        msg.msg_iovlen  = nbuf + 1;
//        msg.msg_iov     = iov;

//        ssize_t real = sendmsg( udp_socket->fd, &msg, MSG_NOSIGNAL );
//        if ( real <= 0 ) { // error ?
//            if ( real < 0 and ( errno == EAGAIN or errno == EWOULDBLOCK ) ) {
//                cw.write_some( data, size );
//                return udp_socket->wait_for_more_out();
//            }
//            return udp_socket->hup_error();
//        }

//        // what has been sent
//        cw.skip_some_sr( real );
//        data = (const char *)data + real;
//        size -= real;
//    }
//    while ( size ) {
//        ssize_t real = sendto( udp_socket->fd, data, size, MSG_NOSIGNAL, (const sockaddr *)&inet_address.sa, sizeof inet_address.sa );
//        if ( real <= 0 ) { // error ?
//            if ( real < 0 and ( errno == EAGAIN or errno == EWOULDBLOCK ) ) {
//                cw.write_some( data, size );
//                return udp_socket->wait_for_more_out();
//            }
//            return udp_socket->hup_error();
//        }

//        // what has been sent
//        data = (const char *)data + real;
//        size -= real;
//    }
//}

//void UdpConnection_Raw::write_some( const CbStringPtr &s ) {
//    s.data_visitor( [ this ]( const PI8 *beg, const PI8 *end ) {
//        write_some( beg, end - beg );
//    } );
//}

//template<class Cq>
//void UdpConnection_Raw::write_some_owning_cq( Cq &&cq ) {
//    // we store data in the buffer ?
//    if ( cq.size() < 128 and cw.size() < 1024 )
//        return cw.write_some( std::move( cq ) );

//    // else, send what we have
//    while ( not cw.empty() and not cq.empty() ) {
//        PT nbuf = cw.nbuf() + cq.nbuf();

//        // messages from cw and cq
//        iovec iov[ nbuf ], *iov_ptr = iov;
//        auto of = [ &iov_ptr ]( const PI8 *beg, const PI8 *end ) {
//            iov_ptr->iov_base = (void *)beg;
//            iov_ptr->iov_len  = end - beg;
//            ++iov_ptr;
//        };
//        cw.data_visitor( of );
//        cq.data_visitor( of );

//        // sendmsg
//        msghdr msg; bzero( &msg, sizeof msg );
//        msg.msg_name    = &inet_address.sa;
//        msg.msg_namelen = sizeof inet_address.sa;
//        msg.msg_iovlen  = nbuf;
//        msg.msg_iov     = iov;

//        ssize_t real = sendmsg( udp_socket->fd, &msg, MSG_NOSIGNAL );
//        if ( real <= 0 ) { // error ?
//            if ( real < 0 and ( errno == EAGAIN or errno == EWOULDBLOCK ) ) {
//                cw.write_some( std::move( cq ) );
//                return udp_socket->wait_for_more_out();
//            }
//            return udp_socket->hup_error();
//        }

//        // what has been sent
//        cw.skip_some_sr( real );
//        cq.skip_some_sr( real );
//    }
//}

//void UdpConnection_Raw::write_some( CbString &&s ) {
//    write_some_owning_cq( std::move( s ) );
//}

//void UdpConnection_Raw::write_some( CbQueue &&s ) {
//    write_some_owning_cq( std::move( s ) );
//}

//bool UdpConnection_Raw::parse_anc( Buffer *buff ) {
//    return parse( buff );
//}


//} // namespace RiotBase
