#include "Util/FileDescriptor.h"
#include "TcpListener.h"
#include "EvLoop.h"

#include "../System/Assert.h"
#include "../System/Log.h"

namespace RiotBase {

TcpListener::TcpListener( unsigned port ) {
    fd = socket( AF_INET6, SOCK_NONBLOCK | SOCK_STREAM, 0 );
    if ( fd < 0 ) { DISP_ERROR( "socket {}", strerror( errno ) ); return; }

    // no "Address already in use" error message
    int yes = 1;
    if ( setsockopt( fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof( yes ) ) < 0 )
        perror( "setsockopt( SO_REUSEADDR ) failed" );
    #ifdef SO_REUSEPORT
    if ( setsockopt( fd, SOL_SOCKET, SO_REUSEPORT, &yes, sizeof( yes ) ) < 0 )
        perror( "setsockopt( SO_REUSEPORT ) failed" );
    #endif

    // bind
    InetAddress ia( "::", port );
    if ( bind( fd, (const struct sockaddr *)&ia.sa, sizeof ia.sa ) ) {
        on_bind_error();
        return;
    }

    if ( listen( fd, SOMAXCONN ) == -1 ) {
        on_listen_error();
        return;
    }
}

//TcpListener::TcpListener( EvLoop *ev_loop, const char *name, bool non_blocking ) : Event( ev_loop ) {
//    // socket
//    fd = socket( AF_UNIX, non_blocking ? SOCK_STREAM | SOCK_NONBLOCK : SOCK_STREAM, 0 );
//    if ( fd < 0 ) { DISP_ERROR( "socket {}", strerror( errno ) ); return; }

////    // bind
////    struct sockaddr_un local;
////    local.sun_family = AF_UNIX;
////    if ( strlen( name ) >= sizeof( local.sun_path ) )
////        ERROR( "socket name is too long (%i chars max)", sizeof( local.sun_path ) );
////    strcpy( local.sun_path, name );
////    unlink( local.sun_path );
////    int len = strlen( local.sun_path ) + sizeof( local.sun_family );
////    if ( bind( listen_fd, (struct sockaddr *)&local, len ) == -1 ) {
////        close( listen_fd );
////        return -1;
////    }

////    // listen
////    if ( listen( listen_fd, SOMAXCONN ) < 0 ) {
////        close( listen_fd );
////        return -1;
////    }
//    TODO;

//    ev_loop->add( this );
//}

TcpListener::TcpListener( VtableOnly vo ) : Event( vo ) {
}

void TcpListener::inp() {
    while ( true ) {
        // accept
        InetAddress ia;
        socklen_t len_ia_sa = sizeof ia.sa;
        int conn_sock = accept( fd, (struct sockaddr *)&ia.sa, &len_ia_sa );
        if ( conn_sock < 0 ) {
            #warning EINTR ?
            if ( errno == EWOULDBLOCK or errno == EAGAIN )
                return wait_for_more_inp();
            on_accept_error();
        }

        // non_blocking
        set_non_blocking( conn_sock );

        // call connection
        if ( not connection( conn_sock, ia ) )
            return;
    }
}

void TcpListener::on_bind_error() {
    DISP_ERROR( "Bind error", strerror( errno ) );
    reg_for_deletion();
}

void TcpListener::on_listen_error() {
    DISP_ERROR( "Listen error", strerror( errno ) );
    reg_for_deletion();
}

void TcpListener::on_accept_error() {
    DISP_ERROR( "Accept error", strerror( errno ) );
}

} // namespace RiotBase
