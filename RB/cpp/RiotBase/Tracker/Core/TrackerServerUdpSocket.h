#pragma once

#include "TrackerServerUdpConnection.h"
#include "../../EvLoop/Util/SslCtx.h"
#include "../../EvLoop/UdpSocket.h"

namespace RiotBase {
class Tracker;

/**
  bind( ... )
*/
class TrackerServerUdpSocket : public UdpSocket<TrackerServerUdpSocket,TrackerServerUdpConnection> {
public:
    TrackerServerUdpSocket( Tracker *tracker, unsigned port, const char *cert_file, const char *key_file );
    ~TrackerServerUdpSocket();

    SslCtx   ctx_server;
    SslCtx   ctx_client;
    Tracker *tracker;
};

} // namespace RiotBase
