#include "TlsConnectionTracker.h"

namespace AgencyDb {

TlsConnectionTracker::TlsConnectionTracker( Tracker *tracker, const Evel::InetAddress &addr, SSL_CTX *ssl_ctx, int accepting_fd ) : TlsConnection( ssl_ctx, accepting_fd ), tracker( tracker ), addr( addr ) {
}

TlsConnectionTracker::~TlsConnectionTracker() {
}

//// nsmake alias TlsConnectionTrackerParser.h (#hpipe.cpp '--class-name' 'TlsConnectionTracker' '--only-parse' TlsConnectionTrackerParser.hpipe).h
#include "TlsConnectionTrackerParser.h"

void TlsConnectionTracker::parse( char **data, size_t size, size_t rese ) {
    std::cout.write( *data, size );
    std::cout << std::endl;
}

} // namespace AgencyDb
