#pragma once

#include <Evel/SharedBufferParseAdapter.h>
#include <Evel/TlsConnection.h>
#include <functional>

namespace AgencyDb {
class Tracker;

/**
*/
class TlsConnectionTracker : public Evel::SharedBufferParseAdapter<Evel::TlsConnection> {
public:
    TlsConnectionTracker( Tracker *tracker, const Evel::InetAddress &addr, SSL_CTX *ssl_ctx, int accepting_fd );
    ~TlsConnectionTracker();

protected:
    //// nsmake alias TlsConnectionTrackerParser.h (#hpipe.cpp '--style' 'BUFFER_IN_CLASS' TlsConnectionTrackerParser.hpipe).h
    #define HPIPE_DECLARATIONS
    #include "TlsConnectionTrackerParser.h"
    #undef  HPIPE_DECLARATIONS

    HpipeData         hpipe_data;
    Tracker          *tracker;
    Evel::InetAddress addr;
};

} // namespace AgencyDb
