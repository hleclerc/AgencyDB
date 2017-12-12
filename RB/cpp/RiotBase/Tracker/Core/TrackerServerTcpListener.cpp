#include "TrackerServerTcpConnection.h"
#include "TrackerServerTcpListener.h"
#include "../../EvLoop/EvLoop.h"
#include "../Tracker.h"

namespace RiotBase {

TrackerServerTcpListener::TrackerServerTcpListener( Tracker *tracker, unsigned port, const char *cert_file, const char *key_file ) : TcpListener( port ), ssl_ctx( SslCtx::Method::SSLv23, cert_file, key_file ), tracker( tracker ) {
}

TrackerServerTcpListener::~TrackerServerTcpListener() {
    if ( tracker )
        tracker->tcp_listeners.remove_first_unordered( this );
}

bool TrackerServerTcpListener::connection( int fd, const InetAddress &addr ) {
    *ev_loop << new TrackerServerTcpConnection( ssl_ctx, fd, tracker, addr );
    return true;
}


} // namespace RiotBase
