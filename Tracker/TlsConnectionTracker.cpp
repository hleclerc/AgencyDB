#include "TlsConnectionTracker.h"

namespace AgencyDb {

TlsConnectionTracker::TlsConnectionTracker( Tracker *tracker, const Evel::InetAddress &addr, SSL_CTX *ssl_ctx, int accepting_fd ) : TlsConnection( ssl_ctx, accepting_fd ), tracker( tracker ), addr( addr ) {
}

TlsConnectionTracker::~TlsConnectionTracker() {
}

#define HPIPE_NO_DECL
#define HPIPE_METHOD_PREFIX TlsConnectionTracker::
#include "TlsConnectionTrackerParser.h"

void TlsConnectionTracker::parse( char **data, size_t size, size_t rese ) {
    Evel::SharedBuffer *buf = new ( *data ) Evel::SharedBuffer( size, rese );
    parse( &hpipe_data, buf, false );
    if ( buf->cpt_use )
        *data = 0;
}

size_t TlsConnectionTracker::offset_parse() const {
    return sizeof( Evel::SharedBuffer ) - Evel::SharedBuffer::nb;
}

} // namespace AgencyDb
