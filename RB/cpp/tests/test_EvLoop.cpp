//// nsmake global inc_path ..

// TCP
#include <RiotBase/EvLoop/TcpRawConnection.h>
#include <RiotBase/EvLoop/TcpSslConnection.h>
#include <RiotBase/EvLoop/TcpListener.h>

// UDP
#include <RiotBase/EvLoop/UdpHashedCmdsConnection.h>
#include <RiotBase/EvLoop/UdpDtlsConnection.h>
#include <RiotBase/EvLoop/UdpRawConnection.h>
#include <RiotBase/EvLoop/UdpSocket.h>

// SSL
#include <RiotBase/EvLoop/Util/SslCtx.h>

// EVENTS
//#include <RiotBase/System/TxtStream.h>
//#include <RiotBase/EvLoop/Watcher.h>
//#include <RiotBase/EvLoop/Timer.h>
#include <RiotBase/EvLoop/Gev.h>

#include <RiotBase/System/Print.h>
#include <RiotBase/System/Check.h>
using namespace RiotBase;

//template<class TP>
//struct MyHttpRequest : TP {
//    template<class TI> MyHttpRequest( TI fd, SSL *ssl ) : TP( fd, ssl ) {}
//    template<class TI> MyHttpRequest( TI fd ) : TP( fd ) {}

//    virtual bool parse( Buffer *buffer ) {
//        std::cout.write( (const char *)buffer->data, buffer->used );
//        std::cout << std::endl;
//        TxtStream<CbQueue> tw( &this->cw );

//        if ( buffer->data[ 0 ] == 'G' ) {
//            CbString cq( IKnowWhatIDo(), buffer, 0, buffer->used );
//            if ( std::string( cq ).find( "GET /stop" ) == 0 ) {
//                tw << "HTTP/1.0 200 OK\nContent-Type: text/plain\n\nstop\n";

//                //                struct MyStop : public Timer {
//                //                    MyStop() : Timer( 20 ) {}
//                //                    virtual bool timeout( int ) {
//                //                        std::cout << "-> stop" << std::endl;
//                //                        this->ev_loop->stop();
//                //                        return false;
//                //                    }
//                //                };
//                //                std::cout << "beg timer stop" << std::endl;
//                //                *this->ev_loop << new MyStop;

//                this->ev_loop->stop();

//                return false;
//            }

//            tw << "HTTP/1.0 200 OK\nContent-Type: text/html\n\n"
//                  "<html>\n"
//                  " <head>\n"
//                  "  <title>test</title>\n"
//                  "  <meta http-equiv='content-type' content='text/html; charset=UTF-8'/>\n"
//                  "  <script type='text/javascript'>\n"
//                  "   upload_func = function( data ) {\n"
//                  "    var f = document.getElementById( 'yop' ).files[0];\n"
//                  "    var x = new XMLHttpRequest();\n"
//                  "    x.open( 'PUT', '/', true );\n"
//                  "    x.send( f );\n"
//                  "   }\n"
//                  "  </script>\n"
//                  " </head>\n"
//                  " <body>\n"
//                  "   <input type='file' name='yop' id='yop'/>\n"
//                  "   <input type='submit' value='Upload' onclick='upload_func()'/>\n"
//                  " </body>\n"
//                  "</html>\n";
//            return false;
//        }

//        if ( buffer->data[ 0 ] == 'P' ) {
//            tw << "HTTP/1.0 200 OK\n\n";
//            return false;
//        }

//        return false;
//    }
//};


//struct MySslTcpListener : TcpListener {
//    MySslTcpListener( const char *port, const char *perm ) : TcpListener( port ), ssl_ctx( perm, perm ), port( port ) {}
//    virtual bool connection( int fd ) { *ev_loop << new MyHttpRequest<TcpConnection_Ssl>( fd, MyHttpRequest<TcpConnection_Ssl>::new_ssl_connection( fd, ssl_ctx.ctx ) ); return true; }
//    virtual void rdy() { std::cout << "https ready on port " << port << std::endl; }
//    SslCtx ssl_ctx;
//    std::string port;
//};

//struct MyTcpListener : TcpListener {
//    MyTcpListener( const char *port ) : TcpListener( port ), port( port ) {}
//    virtual bool connection( int fd ) { *ev_loop << new MyHttpRequest<BufferedSocket>( fd ); return true; }
//    virtual void rdy() { std::cout << "http ready on port " << port << " (/stop to stop)"<< std::endl; }
//    std::string port;
//};

//struct MyWatcher : public Watcher {
//    virtual bool event( PI64 n ) {
//        std::cout << "MyWatcher -> " << n << std::endl;
//        if ( count-- )
//            sig();
//        return true;
//    }
//    int count = 5;
//};

//struct MyTimer : public Timer {
//    MyTimer( double delay ) : Timer( delay ) {
//        w = new MyWatcher;
//    }
//    virtual bool timeout( int nb_expirations ) {
//        std::cout << "timeout" << std::endl;
//        w->sig();
//        return false;
//    }
//    MyWatcher *w;
//};

//void test_watcher() {
//    EvLoop el;

//    // https
//    MySslTcpListener s( "8888", "mycert.pem" ); el << &s;
//    MyTcpListener l( "8887" ); el << &l;

//    MyTimer *t = new MyTimer( 0.5 );
//    el << t->w;
//    el << t;

//    el.run();
//}

void test_InetAddress() {
    CHECK( InetAddress( "127.0.0.2", 654 ), "::ffff:127.0.0.2:654" );
}

void test_TcpRawConnection() {
    static const char *txt = "12345";

    struct MyConnection : TcpRawConnection {
        using TcpRawConnection::TcpRawConnection;
        ~MyConnection() { ev_loop->stop(); }
        virtual bool parse() {
            CHECK( inp_buff->used == strlen( txt ) and strncmp( (const char *)inp_buff->data, txt, strlen( txt ) ) == 0, true );
            gev->stop();
            return true;
        }
    };

    struct MyListener : TcpListener {
        using TcpListener::TcpListener;
        virtual bool connection( int fd, const InetAddress &ia ) {
            *ev_loop << new MyConnection( fd );
            return true;
        }
    };

    // start a server (that will stop after the first message)
    *gev << new MyListener( 9876 );

    // client
    MyConnection *c = new MyConnection( { "127.0.0.1", 9876 } );
    c->cw.write_some( txt, strlen( txt ) ); // "GET / HTTP/1.1\n\n"
    c->flush();
    *gev << c;

    gev->add_timer( [](int) { CHECK( "Should not happen", "" ); gev->stop(); return false; }, 1.0 );
    gev->run();

}

void test_TcpSslConnection() {
    static const char *txt = "12345";

    struct MyConnection : TcpSslConnection {
        using TcpSslConnection::TcpSslConnection;
        virtual bool parse() {
            CHECK( inp_buff->used == strlen( txt ) and strncmp( (const char *)inp_buff->data, txt, strlen( txt ) ) == 0, true );
            gev->stop();
            return true;
        }
    };

    struct MyListener : TcpListener {
        MyListener( const char *perm, unsigned port ) : TcpListener( port ), ssl_ctx( SslCtx::Method::SSLv23, perm, perm ) {
        }
        virtual bool connection( int fd, const InetAddress &ia ) {
            *ev_loop << new MyConnection( ssl_ctx.ctx, fd );
            return true;
        }
        virtual void rdy() { DISP_INFO("RDY"); }
        SslCtx ssl_ctx;
    };

    // start a server (that will stop after the first message)
    *gev << new MyListener( "mycert.pem", 9876 );

    // client
    SslCtx ssl_ctx_client{ SslCtx::Method::SSLv23 };
    MyConnection *c = new MyConnection( ssl_ctx_client.ctx, { "127.0.0.1", 9876 } );
    c->cw.write_some( txt, strlen( txt ) );  // "GET / HTTP/1.1\n\n"
    c->flush();
    *gev << c;

    gev->add_timer( [](int) { CHECK( "Should not happen", "" ); gev->stop(); return false; }, 1.0 );
    gev->run();

}

//void test_UdpRawSocket() {
//    static const char *txt = "12345";

//    struct MyConnection : UdpRawConnection<MyConnection,500> {
//        void parse( char *data, PT size ) {
//            CHECK( size == strlen( txt ) and strncmp( data, txt, strlen( txt ) ) == 0, true );
//            udp_socket->ev_loop->stop();
//        }
//    };
//    struct MySocket : UdpSocket<MySocket,MyConnection> {
//    };

//    MySocket *sock_server = new MySocket;
//    sock_server->bind( 8746 );
//    *gev << sock_server;

//    MySocket *sock_client = new MySocket;
//    sock_client->connection( { "127.0.0.1", 8746 } )->write_some( txt, strlen( txt ) );
//    sock_client->connection( { "127.0.0.1", 8746 } )->flush();
//    *gev << sock_client;

//    gev->add_timer( [](int) { CHECK( "Should not happen", "" ); gev->stop(); return false; }, 1.0 );
//    gev->run();
//}

//void test_DtlsSocket() {
//    static const char *txt = "12345678 12345678 12345678 12345678 12345678 12345678 12345678 12345678 12345678 12345678 12345678 12345678 12345678 123456";

//    struct MyConnection : UdpDtlsConnection<MyConnection,500> {
//        void parse( char *data, PT size ) {
//            CHECK( size == strlen( txt ) and strncmp( data, txt, strlen( txt ) ) == 0, true );
//            udp_socket->ev_loop->stop();
//        }
//    };
//    struct MySocket : UdpSocket<MySocket,MyConnection> {
//        MySocket( SSL_CTX *ctx_server, SSL_CTX *ctx_client ) : ctx_server( ctx_server ), ctx_client( ctx_client ) { }
//        SSL_CTX *ctx_server, *ctx_client;
//    };

//    // SSL methods and certificates
//    SslCtx ssl_ctx_server( SslCtx::Method::DTLSv1, "mycert.pem", "mycert.pem" );
//    SslCtx ssl_ctx_client( SslCtx::Method::DTLSv1 );

//    // server
//    MySocket *sock_server = new MySocket( ssl_ctx_server.ctx, ssl_ctx_client.ctx );
//    sock_server->bind( 8764 );
//    *gev << sock_server;

//    // client
//    MySocket *sock_client = new MySocket( ssl_ctx_server.ctx, ssl_ctx_client.ctx );
//    sock_client->connection( { "127.0.0.1", 8764 } )->write_some( txt, strlen( txt ) );
//    sock_client->connection( { "127.0.0.1", 8764 } )->flush();
//    *gev << sock_client;

//    gev->add_timer( [](int) { CHECK( "Should not happen", "" ); gev->stop(); return false; }, 1.0 );
//    gev->run();
//}


//// Prop: quand on a fini une commande, ok_to_flush
//template<template<class,PT> class TConn>
//void test_DtlsCmdSocket() {
//    static PT res_v1 = 0, res_v2 = 0;

//    struct MyConnection : UdpHashedCmdsConnection<TConn<MyConnection,500>> {
//        void recv_cmd1( PT v1, PT v2 ) {
//            res_v1 = v1;
//            res_v2 = v2;
//        }
//        void parse_bs( BinStream<CmString> bs ) {
//            PI32 cmd = bs.read_byte();
//            CHECK( cmd, 0u );
//            CHECK( PARSE_CMD( cmd1 ), true );
//            this->udp_socket->ev_loop->stop();
//        }
//    };
//    struct MySocket : UdpSocket<MySocket,MyConnection> {
//        MySocket( SSL_CTX *ctx_server, SSL_CTX *ctx_client ) : ctx_server( ctx_server ), ctx_client( ctx_client ) { }
//        SSL_CTX *ctx_server, *ctx_client;
//    };

//    // SSL methods and certificates
//    SslCtx ssl_ctx_server( SslCtx::Method::DTLSv1, "mycert.pem", "mycert.pem" );
//    SslCtx ssl_ctx_client( SslCtx::Method::DTLSv1 );

//    // server
//    MySocket *sock_server = new MySocket( ssl_ctx_server.ctx, ssl_ctx_client.ctx );
//    sock_server->bind( 8765 );
//    *gev << sock_server;

//    // flush sert à dire que la commande est terminées

//    // client
//    MySocket *sock_client = new MySocket( ssl_ctx_server.ctx, ssl_ctx_client.ctx );
//    BinStream<MyConnection> bq( sock_client->connection( { "127.0.0.1", 8765 } ) );
//    bq << 0u << 1u << 2u << flush;
//    *gev << sock_client;

//    gev->add_timer( [](int) { CHECK( "Should not happen", "" ); gev->stop(); return false; }, 1.0 );
//    gev->run();

//    CHECK( res_v1, 1u );
//    CHECK( res_v2, 2u );
//}

int main() {
    // test_InetAddress();
    // test_TcpRawConnection();
    test_TcpSslConnection();
    // test_UdpRawSocket();
    // test_DtlsSocket();
    // test_DtlsCmdSocket<UdpRawConnection >();
    // test_DtlsCmdSocket<UdpDtlsConnection>();
 }


