#pragma once

#include <Evel/TlsConnection.h>
#include <functional>

namespace AgencyDb {
class Tracker;

/**
*/
class TlsConnectionTracker : public Evel::TlsConnection {
public:
    TlsConnectionTracker( Tracker *tracker, const Evel::InetAddress &addr, SSL_CTX *ssl_ctx, int accepting_fd );
    ~TlsConnectionTracker();

protected:
    //// nsmake alias TlsConnectionTrackerParserDecl.h (#hpipe.cpp '--only-struct' TlsConnectionTrackerParser.hpipe).h
    #include "TlsConnectionTrackerParserDecl.h"

    virtual void      parse( char **data, size_t size, size_t rese ) override;

    Tracker          *tracker;
    Evel::InetAddress addr;
};

} // namespace AgencyDb
