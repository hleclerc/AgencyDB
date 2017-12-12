//// nsmake global cpp_flag -g3
//// nsmake inc_path ..

#include <Evel/System/Print.h>
#include <Evel/Signal_WF.h>
#include <iostream>
#include <string>

#include "Tracker.h"

using namespace AgencyDb;

#define PREPARG_FILE <Tracker/args.h>
#include <PrepArg/usage.h>

int main( int argc, char **argv ) {
    // args
    #include <PrepArg/declarations.h>
    #include <PrepArg/parse.h>
    Tracker tracker;

    if ( ! connect )
        tracker.start_user();

    // https
    Evel::SslCtx ssl_ctx( Evel::SslCtx::Method::SSLv23, "test/cert.pem", "test/key.pem" );
    tracker.listen_tls( ssl_ctx.ctx, 7540 );

    // signals
    int sigs[] = { SIGINT, SIGTERM, -1 }; // , SIGQUIT, SIGKILL, SIGUSR1
    Evel::gev->add_event_obj( new Evel::Signal_WF( sigs, []( Evel::Signal_WF *s, int sig ){ Evel::gev->stop(); } ) );

    Evel::gev->run();
}
