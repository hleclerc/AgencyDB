#pragma once

#include "../../EvLoop/Util/SslCtx.h"
#include "../../EvLoop/TcpListener.h"

namespace RiotBase {
class Tracker;

/**
*/
class TrackerServerTcpListener : public TcpListener {
public:
    TrackerServerTcpListener( Tracker *tracker, unsigned port, const char *cert_file, const char *key_file );
    ~TrackerServerTcpListener();

protected:
    friend class Tracker;

    virtual bool connection ( int fd, const InetAddress &addr );

    SslCtx       ssl_ctx;
    Tracker     *tracker;
};

} // namespace RiotBase
