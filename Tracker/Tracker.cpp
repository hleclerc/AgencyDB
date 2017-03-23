#include "TlsConnectionTracker.h"
#include "../src/cpp/Log/Log.h"
#include <Evel/Listener.h>
#include <Evel/Gev.h>
#include "Tracker.h"

namespace AgencyDb {

class TlsListener : public Evel::Listener {
public:
    TlsListener( SSL_CTX *ssl_ctx, int port, Tracker *tracker ) : Evel::Listener( port ), tracker( tracker ), ssl_ctx( ssl_ctx ) {
        DISP_INFO( "Listening for TLS connection at port {}", port );
    }

    virtual void connection( int fd, const Evel::InetAddress &addr ) override {
        ev_loop->add_event_obj( new TlsConnectionTracker( tracker, addr, ssl_ctx, fd ) );
    }

    Tracker *tracker;
    SSL_CTX *ssl_ctx;
};

void Tracker::listen_tls( SSL_CTX *ssl_ctx, int port ) {
    Evel::Event *listener = new TlsListener( ssl_ctx, port, this );
    to_del.push_back( std::unique_ptr<Evel::Event>{ listener } );
    Evel::gev->add_event_obj( listener );
}

} // namespace AgencyDb
