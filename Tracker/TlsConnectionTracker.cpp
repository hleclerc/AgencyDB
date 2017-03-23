#include "../src/cpp/System/Base64.h"
#include "../src/cpp/Log/Log.h"
#include "TlsConnectionTracker.h"
#include <Hpipe/TxtStream.h>
#include <Hpipe/Print.h>
#include <openssl/sha.h>

namespace AgencyDb {

namespace {

//// nsmake mission_key TlsConnectionTrackerIndexHtml html --no-dist --single-page ../html/index.jsx
//// nsmake alias TlsConnectionTrackerIndexHtml.h  (#txt_to_cpp.cpp (#add_http_header.js mission:TlsConnectionTrackerIndexHtml[0] 'text%2Fhtml'   ) 'index_html_str').h
//// nsmake alias TlsConnectionTrackerFaviconIco.h (#txt_to_cpp.cpp (#add_http_header.js ..%2Fhtml%2Ffavicon.ico                  'image%2Fx-icon') 'favicon_str'   ).h
#include "TlsConnectionTrackerIndexHtml.h"
#include "TlsConnectionTrackerFaviconIco.h"

}

TlsConnectionTracker::TlsConnectionTracker( Tracker *tracker, const Evel::InetAddress &addr, SSL_CTX *ssl_ctx, int accepting_fd ) : Evel::HpipeBufferParseAdapter<Evel::TlsConnection>( ssl_ctx, accepting_fd ), tracker( tracker ), addr( addr ) {
}

TlsConnectionTracker::~TlsConnectionTracker() {
}

void TlsConnectionTracker::index_html() {
    DISP_INFO( "sending index.html" );
    send( index_html_str, sizeof( index_html_str ) - 1 );
    close();
}

void TlsConnectionTracker::ws_handshake() {
    std::string key = hpipe_data.websocket_key;
    key += "258EAFA5-E914-47DA-95CA-C5AB0DC85B11";

    unsigned char hash[ SHA_DIGEST_LENGTH ];
    SHA1( (unsigned char *)key.data(), key.size(), hash );
    key = base64_encode( hash, SHA_DIGEST_LENGTH );

    // anwser
    Hpipe::CbQueue cw;
    Hpipe::TxtStream<Hpipe::CbQueue> os( &cw );
    os << "HTTP/1.1 101 Switching Protocols\r\n"; // WebSocket Protocol Handshake
    os << "Connection: Upgrade\r\n";
    os << "Sec-WebSocket-Accept: " << key << "\r\n";
    os << "Upgrade: WebSocket\r\n";
    os << "\r\n";

    send( std::move( cw ) );
}

void TlsConnectionTracker::ws_unmask() {
    int o = 0;
    const unsigned char *m = reinterpret_cast<unsigned char *>( &hpipe_data.websocket_mask );
    hpipe_data.websocket_content.data_visitor( [&]( const unsigned char *beg, const unsigned char *end ) {
        for( ; beg < end; ++beg )
            *const_cast<unsigned char *>( beg ) ^= m[ o++ % 4 ];
    } );
}

void TlsConnectionTracker::ws_content() {
    HpipeDataCb hpipe_data_cb;
    parse_cb( hpipe_data_cb, hpipe_data.websocket_content.get_buf(), hpipe_data.websocket_content.get_off(), hpipe_data.websocket_content.get_end() );
}

void TlsConnectionTracker::favicon() {
    DISP_INFO( "sending favicon.ico" );
    send( favicon_str, sizeof( favicon_str ) - 1 );
    close();
}

void TlsConnectionTracker::pwd_login( const Hpipe::CbStringPtr &login, const Hpipe::CbStringPtr &passwd ) {
    PRINT( "login", login, passwd );
}

#define HPIPE_DEFINITIONS
#define HPIPE_DEFINITION_PREFIX TlsConnectionTracker::

#undef HPIPE_PARSE_FUNC_NAME
#define HPIPE_PARSE_FUNC_NAME parse_cb
#undef HPIPE_DATA_STRUCT_NAME
#define HPIPE_DATA_STRUCT_NAME HpipeDataCb
#undef HPIPE_ADDITIONAL_ARGS
#define HPIPE_ADDITIONAL_ARGS HpipeDataCb &hpipe_data,
#include "TlsConnectionTrackerParserCb.h"
#undef  HPIPE_DEFINITIONS

#define HPIPE_DEFINITIONS
#undef HPIPE_PARSE_FUNC_NAME
#define HPIPE_PARSE_FUNC_NAME parse
#undef HPIPE_DATA_STRUCT_NAME
#define HPIPE_DATA_STRUCT_NAME HpipeData
#undef HPIPE_ADDITIONAL_ARGS
#define HPIPE_ADDITIONAL_ARGS
#include "TlsConnectionTrackerParser.h"

#undef  HPIPE_DEFINITIONS

} // namespace AgencyDb
