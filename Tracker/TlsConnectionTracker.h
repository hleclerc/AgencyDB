#pragma once

#include <Evel/HpipeBufferParseAdapter.h>
#include <Evel/TlsConnection.h>
#include <Hpipe/CbStringPtr.h>
#include <functional>

//// nsmake alias TrackerCmds.hpipe (..%2Fsrc%2Fgeneration%2Fmake_hpipe_from_cmds.ts TrackerCmdsDecl.cmds).hpipe
//// nsmake alias TlsConnectionTrackerParser.h (#hpipe.cpp '--style' 'HPIPE_BUFFER' TrackerCmds.hpipe TlsConnectionTrackerParser.hpipe).h
//// nsmake alias TlsConnectionTrackerParserCb.h (#hpipe.cpp '--style' 'HPIPE_CB_STRING_PTR' TrackerCmds.hpipe).h
#define HPIPE_PRELIMINARIES
#include "TlsConnectionTrackerParserCb.h"
#include "TlsConnectionTrackerParser.h"
#undef  HPIPE_PRELIMINARIES

namespace AgencyDb {
class Tracker;

/**
*/
class TlsConnectionTracker : public Evel::HpipeBufferParseAdapter<Evel::TlsConnection> {
public:
    TlsConnectionTracker( Tracker *tracker, const Evel::InetAddress &addr, SSL_CTX *ssl_ctx, int accepting_fd );
    ~TlsConnectionTracker();

protected:
    using PI32 = __uint32_t;

    void              index_html  ();
    void              ws_handshake();
    void              ws_unmask   ();
    void              ws_content  ();
    void              favicon     ();
    void              login       ();

    #include "(..%2Fsrc%2Fgeneration%2Fmake_cpp_decl_from_cmds.ts TrackerCmdsDecl.cmds).h"

    #define HPIPE_DECLARATIONS
    #undef HPIPE_PARSE_FUNC_NAME
    #define HPIPE_PARSE_FUNC_NAME parse_cb
    #undef HPIPE_DATA_STRUCT_NAME
    #define HPIPE_DATA_STRUCT_NAME HpipeDataCb
    #undef HPIPE_ADDITIONAL_ARGS
    #define HPIPE_ADDITIONAL_ARGS HpipeDataCb &hpipe_data,
    #include "TlsConnectionTrackerParserCb.h"
    #undef HPIPE_DECLARATIONS

    #define HPIPE_DECLARATIONS
    #undef HPIPE_PARSE_FUNC_NAME
    #define HPIPE_PARSE_FUNC_NAME parse
    #undef HPIPE_DATA_STRUCT_NAME
    #define HPIPE_DATA_STRUCT_NAME HpipeData
    #undef HPIPE_ADDITIONAL_ARGS
    #define HPIPE_ADDITIONAL_ARGS
    #include "TlsConnectionTrackerParser.h"
    #undef HPIPE_DECLARATIONS

    HpipeData         hpipe_data;
    Tracker          *tracker;
    Evel::InetAddress addr;
};

} // namespace AgencyDb
