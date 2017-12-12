#include <RiotBase/HttpTestServer/HttpTestServer.h>
#include <RiotBase/System/KnownInodes.h>
#include <RiotBase/Tracker/Tracker.h>
#include <RiotBase/System/Check.h>
#include <RiotBase/System/Log.h>
#include <RiotBase/EvLoop/Gev.h>
#include <RiotBase/EvLoop/Dns.h>

#include <RiotBase/EvLoop/UdpRawConnection.h>
#include <RiotBase/EvLoop/UdpSocket.h>

using namespace RiotBase;

void test_ret() {
    spdlog::set_pattern("[%H:%M:%S:%e %t %l] %v");
    spdlog::set_level( spdlog::level::debug );


    //    struct MyConnection : UdpRawConnection<MyConnection,500> {
    //        void parse( char *data, PT size ) {
    //            PRINT( size );
    //        }
    //    };
    //    struct MySocket : UdpSocket<MySocket,MyConnection> {
    //    };

    //    MySocket *sock_server = new MySocket;
    //    sock_server->bind( 8676 );
    //    *gev << sock_server;
    Dns dns( gev.ptr() );

    Tracker tracker( gev.ptr(), &dns, 1, "test_password_db", "my_pass", true );
    tracker.tcp_bind( 8676, "localhost.cert", "localhost.key" );
    tracker.udp_bind( 8676, "localhost.cert", "localhost.key" );

    HttpTestServer ht;
    // ht.launch_browser( "https://localhost:9876/tests/test_WebRTC.html" );
    ht.bind( gev.ptr(), 8001, "localhost.cert", "localhost.key" );
    ht.bind( gev.ptr(), 8000 );

    gev->run();
}

int main() {
    test_ret();
}
