#include "TlsConnectionTracker.h"
#include "../src/cpp/Log/Log.h"
#include <Evel/Listener.h>
#include <rocksdb/db.h>
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

Tracker::Tracker() {
    db = 0;
}

void Tracker::open_local_db( const std::string &filename ) {
    if ( db ) {
        DISP_ERROR( "Db is already opened" );
        return;
    }
    rocksdb::Options options;
    options.create_if_missing = true;
    rocksdb::Status status = rocksdb::DB::Open( options, filename, &db );
    if ( ! status.ok() ) {
        DISP_ERROR( "Failed to open db {}", filename );
        abort();
    }
}

void Tracker::start_user() {
    std::string login, passwd;
    std::cout << "Login: ";
    std::cin >> login;
    std::cout << "Password: ";
    std::cin >> passwd;
}

void Tracker::listen_tls( SSL_CTX *ssl_ctx, int port ) {
    Evel::Event *listener = new TlsListener( ssl_ctx, port, this );
    to_del.push_back( std::unique_ptr<Evel::Event>{ listener } );
    Evel::gev->add_event_obj( listener );
}

} // namespace AgencyDb
