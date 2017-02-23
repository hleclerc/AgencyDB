#include "TlsConnectionTracker.h"

#define HPIPE_METHOD_PREFIX TlsConnectionTracker::

namespace AgencyDb {

TlsConnectionTracker::TlsConnectionTracker( Tracker *tracker, const Evel::InetAddress &addr, SSL_CTX *ssl_ctx, int accepting_fd ) : Evel::SharedBufferParseAdapter<Evel::TlsConnection>( ssl_ctx, accepting_fd ), tracker( tracker ), addr( addr ) {
}

TlsConnectionTracker::~TlsConnectionTracker() {
}

#define HPIPE_DEFINITIONS
#include "TlsConnectionTrackerParser.h"
#undef  HPIPE_DEFINITIONS

} // namespace AgencyDb
