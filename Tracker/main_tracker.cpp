#include "Tracker.h"
#include <iostream>
#include <string>
using namespace AgencyDb;

int main() {
    Tracker tracker;

    Evel::SslCtx ssl_ctx( Evel::SslCtx::Method::SSLv23, "test/cert.pem", "test/key.pem" );
    tracker.listen_tls( ssl_ctx.ctx, 7540 );

    Evel::gev->run();
}
