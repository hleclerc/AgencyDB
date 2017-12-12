#include "TcpRawConnection.h"
#include "../System/Log.h"
#include <sys/socket.h>
#include <sys/ioctl.h>

namespace RiotBase {

TcpRawConnection::TcpRawConnection( const InetAddress &addr ) : waiting_for_connection( true ) {
    // create an ipv6 socket
    fd = socket( AF_INET6, SOCK_NONBLOCK | SOCK_STREAM, 0 );
    if ( fd < 0 ) { DISP_ERROR( "socket {}", strerror( errno ) ); return; }

    // connect
    if ( connect( fd, (const struct sockaddr *)&addr.sa, sizeof addr.sa ) < 0 and errno != EINPROGRESS )
        on_connect_error( errno );
}

TcpRawConnection::TcpRawConnection( int accepting_fd ) : waiting_for_connection( true ) {
    fd = accepting_fd;
}

TcpRawConnection::TcpRawConnection( VtableOnly vo ) : TcpConnection( vo ) {
}

void TcpRawConnection::flush() {
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
            #warning EINTR ?
            if ( real < 0 and ( errno == EAGAIN or errno == EWOULDBLOCK ) )
                return wait_for_more_out();
            return hup_error();
        }

        // what has been sent
        cw.skip_some( real );
    }
}

void TcpRawConnection::on_connect_error( int error ) {
    DISP_ERROR( "connection refused: {}", strerror( error ) );
    reg_for_deletion();
}

void TcpRawConnection::on_connect() {
}

void TcpRawConnection::inp() {
    while ( true ) {
        // try to find the message length
        unsigned buff_size, max_buff_size = 1u << 20;
        if ( ioctl( fd, FIONREAD, &buff_size ) == -1 ) {
            DISP_INFO( "ioctl FIONREAD: {}", strerror( errno ) );
            buff_size = Buffer::default_size;
        } else if ( buff_size > max_buff_size )
            buff_size = max_buff_size;

        // if no buffer or not possible to reuse the old buffer, create a new one
        if ( not inp_buff ) {
            inp_buff = Buffer::New( buff_size );
        } else if ( inp_buff->cpt_use > 0 ) {
            --inp_buff->cpt_use;
            inp_buff = Buffer::New( buff_size );
        } else if ( inp_buff->size < buff_size ) {
            delete inp_buff;
            inp_buff = Buffer::New( buff_size );
        }

        // try to read some data
        inp_buff->used = recv( fd, inp_buff->data, inp_buff->size, MSG_DONTWAIT );
        if ( int( inp_buff->used ) <= 0 ) {
            #warning EINTR ?
            if ( inp_buff->used and ( errno == EAGAIN or errno == EWOULDBLOCK ) )
                return wait_for_more_inp();
            return hup_error();
        }

        // parse
        bool cont = parse();
        flush();
        if ( not cont )
            return;
    }
}

void TcpRawConnection::out() {
    if ( waiting_for_connection ) {
        waiting_for_connection = false;

        int error;
        socklen_t len = sizeof error;
        if ( getsockopt( fd, SOL_SOCKET, SO_ERROR, &error, &len ) ) {
            DISP_ERROR( "getsockopt SO_ERROR: {}", strerror( errno ) );
            return;
        }
        if ( error )
            on_connect_error( error );
        else
            on_connect();
    }

    flush();
}

} // namespace RiotBase
