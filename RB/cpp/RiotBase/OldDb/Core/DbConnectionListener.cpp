#include "../../EvLoop/TcpRawConnection.h"
#include "../../EvLoop/TcpListener.h"
#include "../../EvLoop/EvLoop.h"
#include "DbTcpListener.h"
#include "DbConnection.h"
#include "DbSession.h"
#include "../Db.h"

namespace RiotBase {

/// TcpListener that will create TcpConnection_Ssls
//    struct TcpConnection_SslTcpListener : TcpListener {
//        TcpConnection_SslTcpListener( std::string port, SSL_CTX *ssl_ctx ) : TcpListener( port.c_str() ), ssl_ctx( ssl_ctx ), port( port ) { }
//        virtual bool connection( int fd ) { *ev_loop << new_TcpConnection_Ssl( fd, ssl_ctx, db ); return true; }
//        virtual void rdy() { std::cout << "Listening to port " << port << "(ex: https://localhost:" << port << "/ to start a session)" << std::endl; }
//        SSL_CTX    *ssl_ctx; ///<
//        std::string port;    ///< used only for the rdy() message
//        Db         *db;
//    };

//    /// TcpListener that will create unencrypted Connections
//    struct ConnectionTcpListener : TcpListener {
//        ConnectionTcpListener( std::string port ) : TcpListener( port.c_str() ), port( port ) { }
//        virtual bool connection( int fd ) { *ev_loop << new_Connection( fd, db ); return true; }
//        virtual void rdy() { std::cout << "Listening to port " << port << "(ex: http://localhost:" << port << "/ to start a session)" << std::endl; }
//        std::string port;    ///< used only for the rdy() message
//        Db         *db;
//    };

/// TcpListener that will create unencrypted Connections
struct DbTcpListener : TcpListener {
    DbTcpListener( Db *db, const std::string &ref, bool is_a_named_socket = false ) : TcpListener( ref.c_str(), is_a_named_socket ), ref( ref ), db( db ), is_a_named_socket( is_a_named_socket ) {
    }

    virtual bool connection( int fd ) {
        db->sessions << new DbSession( db, fd, true );
        return true;
    }

    virtual void rdy() {
        std::cout << "Listening to " << ( is_a_named_socket ? "named socket" : "port" ) << " '" << ref << "'" << std::endl;
    }

    std::string  ref; ///< used only for the rdy() message
    Db          *db;
    bool         is_a_named_socket;
};

//    // ssl connections
//    SslCtx ssl_ctx( certificate, certificate );
//    TcpConnection_SslTcpListener sc( ssl_port, ssl_ctx.ctx );
//    event_loop << &sc;

// unencrypted connections
TcpListener *new_DbTcpListener( Db *db, const std::string &ref, bool is_a_named_socket ) {
    return new DbTcpListener( db, ref, is_a_named_socket );
}


} // namespace RiotBase
