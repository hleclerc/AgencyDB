#include "UdpSocketAncestor.h"
#include "../System/Log.h"

// http://gafferongames.com/networking-for-game-programmers/sending-and-receiving-packets/ for portability

namespace RiotBase {

UdpSocketAncestor::UdpSocketAncestor() : Event( socket( AF_INET6, SOCK_NONBLOCK | SOCK_DGRAM, IPPROTO_UDP ) ) {
}

void UdpSocketAncestor::bind( unsigned port ) {
    // no "Address already in use" error message
    //    int yes = 1;
    //    if ( setsockopt( fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof( yes ) ) < 0 )
    //        DISP_ERROR( "setsockopt( SO_REUSEADDR ) failed: {}", strerror( errno ) );
    //    if ( setsockopt( fd, SOL_SOCKET, SO_REUSEPORT, &yes, sizeof( yes ) ) < 0 )
    //        DISP_ERROR( "setsockopt( SO_REUSEPORT ) failed: {}", strerror( errno ) );

    // bind
    InetAddress ia( "::", port );
    if ( ::bind( fd, (const struct sockaddr *)&ia.sa, sizeof ia.sa ) < 0 )
        on_bind_error( strerror( errno ) );
}

void UdpSocketAncestor::on_bind_error( const char *msg ) {
    DISP_ERROR( "Error while attempting to bind: {}", msg );
    reg_for_deletion();
}

} // namespace RiotBase
