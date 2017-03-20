#pragma once

#include <Evel/HpipeBufferParseAdapter.h>
#include <Evel/TlsConnection.h>
#include <functional>

namespace AgencyDb {
class Tracker;

/**
*/
class TlsConnectionTracker : public Evel::HpipeBufferParseAdapter<Evel::TlsConnection> {
public:
    TlsConnectionTracker( Tracker *tracker, const Evel::InetAddress &addr, SSL_CTX *ssl_ctx, int accepting_fd );
    ~TlsConnectionTracker();

protected:
    void              index_html();
    void              websocket ();

    //// nsmake alias TlsConnectionTrackerParser.h (#hpipe.cpp '--style' 'HPIPE_BUFFER' TlsConnectionTrackerParser.hpipe).h
    #define HPIPE_DECLARATIONS
    #include "TlsConnectionTrackerParser.h"
    #undef  HPIPE_DECLARATIONS

    HpipeData         hpipe_data;
    Tracker          *tracker;
    Evel::InetAddress addr;
};

} // namespace AgencyDb
