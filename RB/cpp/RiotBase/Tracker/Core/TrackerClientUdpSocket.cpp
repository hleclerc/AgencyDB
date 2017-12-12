#include "TrackerClientUdpSocket.h"

namespace RiotBase {

TrackerClientUdpSocket::TrackerClientUdpSocket( TrackerClient *tracker_client ) : tracker_client( tracker_client ), ctx_client( SslCtx::Method::DTLSv1 ) {
}


} // namespace RiotBase
