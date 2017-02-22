#pragma once

#include <Evel/Stream/SharedBuffer.h>
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
    //// nsmake alias TlsConnectionTrackerParser.h (#hpipe.cpp TlsConnectionTrackerParser.hpipe).h
    #define HPIPE_BUFFER Evel::SharedBuffer
    #define HPIPE_NO_DEF
    #include "TlsConnectionTrackerParser.h"
    #undef  HPIPE_NO_DEF

    virtual void      parse      ( char **data, size_t size, size_t rese ) override;
    virtual size_t   offset_parse() const; ///< because we want to allocate SharedBuffers

    HpipeData         hpipe_data;
    Tracker          *tracker;
    Evel::InetAddress addr;
};

} // namespace AgencyDb
