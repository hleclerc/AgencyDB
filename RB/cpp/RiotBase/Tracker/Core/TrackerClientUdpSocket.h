#pragma once

#include "TrackerClientUdpConnection.h"
#include "../../EvLoop/Util/SslCtx.h"
#include "../../EvLoop/UdpSocket.h"

namespace RiotBase {

/**
*/
class TrackerClientUdpSocket : public UdpSocket<TrackerClientUdpSocket,TrackerClientUdpConnection> {
public:
    TrackerClientUdpSocket( TrackerClient *tracker_client );

    TrackerClient *tracker_client;
    SslCtx         ctx_server;
    SslCtx         ctx_client;
};

} // namespace RiotBase
