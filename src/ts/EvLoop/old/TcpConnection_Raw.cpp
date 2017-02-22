#include "TcpConnection_Raw.h"
#include "EvLoop.h"

#include "../System/Log.h"

#include <sys/socket.h>
#include <sys/ioctl.h>
#include <arpa/inet.h>
#include <stropts.h>

namespace RiotBase {

TcpConnection_Raw::TcpConnection_Raw( const std::string &addr, unsigned port ) : addr( addr, port ) {
    // base init
    inp_buff               = 0;
    ok_to_parse            = true;
    waiting_for_connection = false;

    // create an ipv6 socket
    fd = socket( AF_INET6, SOCK_NONBLOCK | SOCK_STREAM, 0 );
    if ( fd < 0 ) { DISP_ERROR( "socket {}", strerror( errno ) ); return; }

    // connect
    if ( connect( fd, (const struct sockaddr *)&this->addr.sa, sizeof this->addr.sa ) )
        on_connect_error( errno );
}

TcpConnection_Raw::TcpConnection_Raw( int fd ) : Event( fd ) {
    // base init
    inp_buff               = 0;
    ok_to_parse            = true;
    waiting_for_connection = false;
}

TcpConnection_Raw::TcpConnection_Raw( VtableOnly vo ) : Event( vo ) {
}

TcpConnection_Raw::~TcpConnection_Raw() {
    if ( inp_buff )
        dec_ref( inp_buff );
}

void TcpConnection_Raw::flush() {
    if ( waiting_for_connection )
        return;
    while ( not cw.empty() ) {
        PT nbuf = cw.nbuf();

        // messages from cw
        iovec iov[ nbuf ], *iov_ptr = iov;
        cw.data_visitor( [ &iov_ptr ]( const PI8 *beg, const PI8 *end ) {
            iov_ptr->iov_base = (void *)beg;
            iov_ptr->iov_len  = end - beg;
            ++iov_ptr;
        } );

        // sendmsg
        msghdr msg; bzero( &msg, sizeof msg );
        msg.msg_iovlen = nbuf;
        msg.msg_iov    = iov;

        ssize_t real = sendmsg( fd, &msg, MSG_NOSIGNAL );
        if ( real <= 0 ) { // error ?
            if ( real < 0 and ( errno == EAGAIN or errno == EWOULDBLOCK ) )
                return wait_for_more_out();
            return hup_error();
        }

        // what has been sent
        cw.skip_some( real );
    }
}

void TcpConnection_Raw::write_some( const void *data, PT size ) {
    // we store data in the buffer ?
    if ( size < 128 and cw.size() < 1024 )
        return cw.write_some( data, size );

    // else, send what we have
    while ( size and not cw.empty() ) {
        PT nbuf = cw.nbuf();

        // messages from cw
        iovec iov[ nbuf + 1 ], *iov_ptr = iov;
        cw.data_visitor( [ &iov_ptr ]( const PI8 *beg, const PI8 *end ) {
            iov_ptr->iov_base = (void *)beg;
            iov_ptr->iov_len  = end - beg;
            ++iov_ptr;
        } );

        // message from [data, size]
        iov_ptr->iov_base = (void *)data;
        iov_ptr->iov_len  = size;

        // sendmsg
        msghdr msg; bzero( &msg, sizeof msg );
        msg.msg_iovlen = nbuf + 1;
        msg.msg_iov    = iov;

        ssize_t real = sendmsg( fd, &msg, MSG_NOSIGNAL );
        if ( real <= 0 ) { // error ?
            if ( real < 0 and ( errno == EAGAIN or errno == EWOULDBLOCK ) ) {
                cw.write_some( data, size );
                return wait_for_more_out();
            }
            return hup_error();
        }

        // what has been sent
        cw.skip_some_sr( real );
        data = (const char *)data + real;
        size -= real;
    }
}

void TcpConnection_Raw::write_some( const CbStringPtr &s ) {
    s.data_visitor( [ this ]( const PI8 *beg, const PI8 *end ) {
        write_some( beg, end - beg );
    } );
}

template<class Cq>
void TcpConnection_Raw::write_some_owning_cq( Cq &&cq ) {
    // we store data in the buffer ?
    if ( cq.size() < 128 and cw.size() < 1024 )
        return cw.write_some( std::move( cq ) );

    // else, send what we have
    while ( not cw.empty() and not cq.empty() ) {
        PT nbuf = cw.nbuf() + cq.nbuf();

        // messages from cw and cq
        iovec iov[ nbuf ], *iov_ptr = iov;
        auto of = [ &iov_ptr ]( const PI8 *beg, const PI8 *end ) {
            iov_ptr->iov_base = (void *)beg;
            iov_ptr->iov_len  = end - beg;
            ++iov_ptr;
        };
        cw.data_visitor( of );
        cq.data_visitor( of );

        // sendmsg
        msghdr msg; bzero( &msg, sizeof msg );
        msg.msg_iovlen = nbuf;
        msg.msg_iov    = iov;

        ssize_t real = sendmsg( fd, &msg, MSG_NOSIGNAL );
        if ( real <= 0 ) { // error ?
            if ( real < 0 and ( errno == EAGAIN or errno == EWOULDBLOCK ) ) {
                cw.write_some( std::move( cq ) );
                return wait_for_more_out();
            }
            return hup_error();
        }

        // what has been sent
        cw.skip_some_sr( real );
        cq.skip_some_sr( real );
    }
}

void TcpConnection_Raw::write_some( CbString &&s ) {
    write_some_owning_cq( std::move( s ) );
}

void TcpConnection_Raw::write_some( CbQueue &&s ) {
    write_some_owning_cq( std::move( s ) );
}

void TcpConnection_Raw::on_connection_refused() {
    DISP_ERROR( "connection to {}:{} refused ", addr );
    reg_for_deletion();
}

void TcpConnection_Raw::on_connect_error( const char *msg ) {
    DISP_ERROR( "Error while attempting to tonnect to {}: {}", addr, msg );
    reg_for_deletion();
}

void TcpConnection_Raw::on_invalid_address() {
    DISP_ERROR( "invalid address {} (an ip address was expecpted, either ipv4 or ipv6)", addr );
    reg_for_deletion();
}

void TcpConnection_Raw::inp() {
    while ( ok_to_parse ) {
        // try to find the message length
        unsigned buff_size, max_buff_size = 1u << 20;
        if ( ioctl( fd, FIONREAD, &buff_size ) == -1 ) {
            DISP_INFO( "ioctl FIONREAD: {}", strerror( errno ) );
            buff_size = Buffer::default_size;
        } else if ( buff_size > max_buff_size )
            buff_size = max_buff_size;

        // if no buffer or not possible to reuse the old buffer, create a new one
        if ( not inp_buff ) {
            inp_buff = Buffer::New( buff_size ); ///< Rq: it leads to allocation
        } else if ( inp_buff->cpt_use > 0 and inp_buff->size >= buff_size ) {
            --inp_buff->cpt_use;
            inp_buff = Buffer::New( buff_size ); ///< Rq: it leads to allocation
        }

        // try to read some data
        inp_buff->used = recv( fd, inp_buff->data, inp_buff->size, MSG_DONTWAIT );
        if ( int( inp_buff->used ) <= 0 ) {
            if ( inp_buff->used and ( errno == EAGAIN or errno == EWOULDBLOCK ) ) {
                wait_for_more_inp();
                return flush();
            }
            return hup_error();
        }

        // parse
        __parse( inp_buff );
    }

    flush();
}


void TcpConnection_Raw::out() {
    if ( waiting_for_connection ) {
        waiting_for_connection = false;

        socklen_t len = sizeof errno;
        if ( getsockopt( fd, SOL_SOCKET, SO_ERROR, &errno, &len ) ) {
            DISP_ERROR( "getsockopt SO_ERROR: {}", strerror( errno ) );
            return;
        }
        _on_connect( errno );
    }
    flush();
}

void TcpConnection_Raw::_on_connect( int err ) {
    switch ( err ) {
    case            0: break;
    case  EINPROGRESS: waiting_for_connection = true; return;
    case ECONNREFUSED: return on_connection_refused();
    default:           return on_connect_error( strerror( errno ) );
    }
}

void TcpConnection_Raw::__parse( Buffer *buff ) {
    if (( ok_to_parse = parse( buff ) ))
        wait_for_more_inp();
}

} // namespace RiotBase
