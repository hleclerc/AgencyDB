#include "TlsConnectionTracker.h"
#include "../src/cpp/Log/Log.h"

namespace AgencyDb {

namespace {

//// nsmake mission_key TlsConnectionTrackerIndexHtml html ../html/index.jsx
//// nsmake alias TlsConnectionTrackerIndexHtml.h (#txt_to_cpp.cpp mission:TlsConnectionTrackerIndexHtml[0] 'index_html_str').h
#include "TlsConnectionTrackerIndexHtml.h"

}

TlsConnectionTracker::TlsConnectionTracker( Tracker *tracker, const Evel::InetAddress &addr, SSL_CTX *ssl_ctx, int accepting_fd ) : Evel::HpipeBufferParseAdapter<Evel::TlsConnection>( ssl_ctx, accepting_fd ), tracker( tracker ), addr( addr ) {
}

TlsConnectionTracker::~TlsConnectionTracker() {
}

void TlsConnectionTracker::index_html() {
    DISP_INFO( index_html_str );
    DISP_INFO( sizeof index_html_str );
}

void TlsConnectionTracker::websocket() {
    DISP_ERROR("TODO");
}

#define HPIPE_DEFINITION_PREFIX TlsConnectionTracker::
#define HPIPE_DEFINITIONS
#include "TlsConnectionTrackerParser.h"
#undef  HPIPE_DEFINITIONS

} // namespace AgencyDb
