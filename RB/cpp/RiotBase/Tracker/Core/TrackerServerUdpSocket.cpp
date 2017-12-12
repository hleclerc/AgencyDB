#include "TrackerServerUdpSocket.h"
#include "../Tracker.h"

namespace RiotBase {

TrackerServerUdpSocket::TrackerServerUdpSocket( Tracker *tracker, unsigned port, const char *cert_file, const char *key_file ) : ctx_server( SslCtx::Method::DTLSv1, cert_file, key_file ), ctx_client( SslCtx::Method::DTLSv1 ), tracker( tracker ) {
    bind( port );
}

TrackerServerUdpSocket::~TrackerServerUdpSocket() {
    if ( tracker )
        tracker->udp_sockets.remove_first_unordered( this );
}

} // namespace RiotBase
