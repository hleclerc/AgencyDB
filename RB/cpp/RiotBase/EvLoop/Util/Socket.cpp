#include "../../System/BinStream.h"
#include "../../System/Assert.h"
#include "../../System/Print.h"
#include "FileDescriptor.h"
#include "Socket.h"

#include <netinet/tcp.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <sys/un.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <netdb.h>

#include <fcntl.h>

namespace RiotBase {

int SOCK_TYPE( SocketProtocol protocol ) {
    switch ( protocol ) {
    case SocketProtocol::tcp: return SOCK_STREAM;
    case SocketProtocol::udp: return SOCK_DGRAM;
    default: TODO; return 0;
    }
}

int listening_named_socket( const char *name, bool non_blocking ) {
    // socket
    int listen_fd = socket( AF_UNIX, non_blocking ? SOCK_STREAM | SOCK_NONBLOCK : SOCK_STREAM, 0 );
    if ( listen_fd < 0 )
        return -1;

    // bind
    struct sockaddr_un local;
    local.sun_family = AF_UNIX;
    if ( strlen( name ) >= sizeof( local.sun_path ) )
        ERROR( "socket name is too long (%i chars max)", sizeof( local.sun_path ) );
    strcpy( local.sun_path, name );
    unlink( local.sun_path );
    int len = strlen( local.sun_path ) + sizeof( local.sun_family );
    if ( bind( listen_fd, (struct sockaddr *)&local, len ) == -1 ) {
        close( listen_fd );
        return -1;
    }

    // listen
    if ( listen( listen_fd, SOMAXCONN ) < 0 ) {
        close( listen_fd );
        return -1;
    }

    return listen_fd;
}

int listening_socket( SocketProtocol protocol, unsigned port, bool non_blocking ) {
    TODO;

//    // stolen from https://banu.com/blog/2/how-to-use-epoll-a-complete-example-in-c/
//    addrinfo hints;
//    memset( &hints, 0, sizeof( addrinfo ) );
//    hints.ai_family   = AF_UNSPEC;   /* Return IPv4 and IPv6 choices */
//    hints.ai_socktype = SOCK_STREAM; /* We want a TCP socket */
//    hints.ai_flags    = AI_PASSIVE;  /* All interfaces */

//    addrinfo *result;
//    if ( int s = getaddrinfo( NULL, ref, &hints, &result ) ) {
//        fprintf( stderr, "getaddrinfo: %s\n", gai_strerror( s ) );
//        return -1;
//    }

//    for ( addrinfo *rp = result; rp; rp = rp->ai_next ) {
//        // socket ok ?
//        int listen_fd = socket( rp->ai_family, rp->ai_socktype, rp->ai_protocol );
//        if ( listen_fd < 0 )
//            continue;

//        // no "Address already in use" error message
//        int yes = 1;
//        if ( setsockopt( listen_fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof( yes ) ) < 0 )
//            perror( "setsockopt( SO_REUSEADDR ) failed" );
//        #ifdef SO_REUSEPORT
//        if ( setsockopt( listen_fd, SOL_SOCKET, SO_REUSEPORT, &yes, sizeof( yes ) ) < 0 )
//            perror( "setsockopt( SO_REUSEPORT ) failed" );
//        #endif

//        // // TCP_CORK
//        // if ( setsockopt( listen_fd, SOL_TCP, TCP_CORK, &yes, sizeof( yes ) ) == -1 )
//        //    perror( "setsockopt" );

//        // bind ok ?
//        if ( bind( listen_fd, rp->ai_addr, rp->ai_addrlen ) == 0 ) {
//            freeaddrinfo( result );

//            if ( non_blocking and set_non_blocking( listen_fd ) == -1 ) {
//                perror( "set_non_blocking" );
//                close( listen_fd );
//                return -1;
//            }

//            if ( listen( listen_fd, SOMAXCONN ) == -1 ) {
//                perror( "listen" );
//                close( listen_fd );
//                return -1;
//            }

//            return listen_fd;
//        }

//        close( listen_fd );
//    }

//    fprintf( stderr, "Could not bind\n" );
//    freeaddrinfo( result );
    return -1;
}


int accepting_socket( int listen_fd, bool non_blocking ) {
    TODO;
    return 0;
//    // accept
//    sockaddr_in local;
//    socklen_t addrlen = sizeof( sockaddr_in );
//    int conn_sock = accept( listen_fd, (struct sockaddr *)&local, &addrlen );
//    if ( conn_sock == -1 ) {
//        return -1;
//    }

//    // socket parameters
//    if ( non_blocking and set_non_blocking( conn_sock )) {
//        close( conn_sock );
//        return -1;
//    }

//    if ( addr or port ) {
//        struct sockaddr_storage ss;
//        socklen_t ss_len = sizeof ss;
//        if ( getpeername( conn_sock, (struct sockaddr*)&ss, &ss_len ) == 0 ) {
//            if ( ss.ss_family == AF_INET ) {
//                struct sockaddr_in *s = (struct sockaddr_in *)&ss;
//                *port = ntohs( s->sin_port );
//                inet_ntop( AF_INET, &s->sin_addr, addr, INET6_ADDRSTRLEN );
//            } else { // AF_INET6
//                struct sockaddr_in6 *s = (struct sockaddr_in6 *)&ss;
//                *port = ntohs( s->sin6_port );
//                inet_ntop( AF_INET6, &s->sin6_addr, addr, INET6_ADDRSTRLEN );
//            }
//        } else {
//            if ( addr ) *addr = 0;
//            if ( port ) *port = 0;
//        }
//    }

//    return conn_sock;
}

int client_named_socket( const char *name, const char **err_msg, bool non_blocking ) {
    // socket
    int fd = socket( AF_UNIX, non_blocking ? SOCK_NONBLOCK | SOCK_STREAM : SOCK_STREAM, 0 );
    if ( fd < 0 )
        return -1;

    // no "Address already in use" error message
    int yes = 1;
    if ( setsockopt( fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof( yes ) ) == -1 )
        perror( "setsockopt" );

    // bind
    struct sockaddr_un local;
    local.sun_family = AF_UNIX;
    if ( strlen( name ) + 1 >= sizeof local.sun_path )
        ERROR( "socket name is too long (%i chars max)", sizeof local.sun_path );
    strcpy( local.sun_path, name );
    if ( connect( fd, (struct sockaddr *)&local, sizeof( local.sun_family ) + strlen( name ) ) == -1 ) {
        close( fd );
        return -1;
    }

    return fd;
}

int client_socket( SocketProtocol protocol, const char *addr, unsigned port, const char **err_msg, bool non_blocking ) {
    // // noalready in use" error message
    // int yes = 1;
    // if ( setsockopt( listen_fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof( yes ) ) == -1 )
    //     perror( "setsockopt" );

    // // TCP_CORK
    // if ( setsockopt( listen_fd, SOL_TCP, TCP_CORK, &yes, sizeof( yes ) ) == -1 )
    //    perror( "setsockopt" );
    // mostly stolen from https://banu.com/blog/2/how-to-use-epoll-a-complete-example-in-c/
    addrinfo hints;
    memset( &hints, 0, sizeof( addrinfo ) );
    hints.ai_socktype = SOCK_STREAM; // We want a TCP socket
    hints.ai_family   = AF_UNSPEC;   // Return IPv4 and IPv6 choices
    hints.ai_flags    = AI_PASSIVE;  // All interfaces

    addrinfo *result;
    char c_port[ 11 ];
    snprintf( c_port, sizeof c_port, "%i", port );
    if ( int s = getaddrinfo( addr, c_port, &hints, &result ) ) {
        fprintf( stderr, "getaddrinfo: %s\n", gai_strerror( s ) );
        return -1;
    }

    for ( addrinfo *rp = result; rp; rp = rp->ai_next ) {
        // socket ok ?
        int fd = socket( rp->ai_family, non_blocking ? SOCK_NONBLOCK | rp->ai_socktype : rp->ai_socktype, rp->ai_protocol );
        if ( fd < 0 )
            continue;

        // // noalready in use" error message
        // int yes = 1;
        // if ( setsockopt( listen_fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof( yes ) ) == -1 )
        //     perror( "setsockopt" );

        // // TCP_CORK
        // if ( setsockopt( listen_fd, SOL_TCP, TCP_CORK, &yes, sizeof( yes ) ) == -1 )
        //    perror( "setsockopt" );


        // bind ok ?
        char ipstr[ INET6_ADDRSTRLEN ];
        if ( rp->ai_family == AF_INET ) { // IPv4
            inet_ntop( rp->ai_family, &((struct sockaddr_in *)rp->ai_addr)->sin_addr, ipstr, sizeof ipstr );
        } else { // IPv6
            inet_ntop( rp->ai_family, &((struct sockaddr_in6 *)rp->ai_addr)->sin6_addr, ipstr, sizeof ipstr );
            // struct sockaddr_in6 *ipv6 = (struct sockaddr_in6 *)p->ai_addr;
            //addr = &(ipv6->sin6_addr);
        }
        printf( "-> %s\n", ipstr );

        PRINT( rp->ai_family, rp->ai_socktype, rp->ai_protocol );
        CbQueue cq;
        cq.write_some( rp->ai_addr, rp->ai_addrlen );
        PRINT( cq );


        int res_connect = connect( fd, rp->ai_addr, rp->ai_addrlen );
        if ( res_connect != 0 and errno != EINPROGRESS ) {
            close( fd );
            continue;
        }

        // yes, we have a bound socket
        freeaddrinfo( result );
        return fd;
    }

    //if ( disp_error_messages )
    //    fprintf( stderr, "Could not connect\n" );
    freeaddrinfo( result );
    return -1;
}

} // namespace RiotBase
