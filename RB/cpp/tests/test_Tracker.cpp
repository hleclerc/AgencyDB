#include <RiotBase/Tracker/TrackerClient.h>
#include <RiotBase/Tracker/Tracker.h>
#include <RiotBase/System/AutoPtr.h>
#include <RiotBase/System/Check.h>
#include <RiotBase/System/Log.h>
#include <RiotBase/EvLoop/Gev.h>
#include <unistd.h>

#include <RiotBase/System/InetAddress.h>
using namespace RiotBase;

void test_basic_auth() {
    struct MyTrackerClient : TrackerClient {
        using TrackerClient::TrackerClient;
        virtual void on_ans_credentials( DevId dev_id, UsrId usd_id ) { gev->stop(); }
    };

    spdlog::set_pattern("[%H:%M:%S:%e %t %l] %v");
    // spdlog::set_level( spdlog::level::debug );

    // start a server
    Tracker tracker( 1, "test_password_db", "my_pass", true );
    tracker.bind( 8676, "mycert.pem" );

    // client: make connection, send credentials and stop
    MyTrackerClient tc( "127.0.0.1", 8676 );
    tc.send_credentials( "root", "my_pass" );
    tc.flush();

    gev->run( 1.0 );
    CHECK( tc.dev_id().val != 0, 1 );
    CHECK( tc.usr_id(), KnownUsrId::root_usr );
}


void test_obj_list() {
    struct MyTrackerClient : TrackerClient {
        using TrackerClient::TrackerClient;
        virtual void on_bind_obj      ( Inode inode, DevId dev_id ) { CHECK( inode, Inode( 100, 10 ) ); bound << dev_id; if ( ++*n_bounds == 6 ) gev->stop(); }
        virtual void on_req_inode_data( Inode inode, DevId dev_id, const VectorClock &known ) { CHECK( inode, Inode( 100, 10 ) ); bound << dev_id; if ( ++*n_bounds == 6 ) gev->stop(); koths << known; }
        Vec<VectorClock> koths;
        Vec<DevId> bound;
        int *n_bounds;
    };

    spdlog::set_pattern("[%H:%M:%S:%e %t %l] %v");
    // spdlog::set_level( spdlog::level::debug );

    // Tracker instance
    Tracker tracker( 1, "test_password_db", "my_pass", true );
    tracker.bind( 8677, "mycert.pem" );

    //
    VectorClock known; known += PatchId{ 100, 5 };
    Vec<AutoPtr<MyTrackerClient>> clients;
    int n_bounds = 0;
    for( PT i = 0; i < 3; ++i ) {
        MyTrackerClient *tc = new MyTrackerClient( "127.0.0.1", 8677 );
        tc->send_credentials( "root", "my_pass" );
        tc->send_bind_obj( Inode{ 100, 10 }, known, {} );
        tc->n_bounds = &n_bounds;
        tc->flush();
        clients << tc;
    }

    gev->run( 2.0 );

    CHECK( clients[ 0 ]->bound, Vec<DevId>( clients[ 1 ]->dev_id(), clients[ 2 ]->dev_id() ) );
    CHECK( clients[ 1 ]->bound, Vec<DevId>( clients[ 0 ]->dev_id(), clients[ 2 ]->dev_id() ) );
    CHECK( clients[ 2 ]->bound, Vec<DevId>( clients[ 0 ]->dev_id(), clients[ 1 ]->dev_id() ) );

    CHECK( clients[ 0 ]->koths, "5@100 5@100" );
    CHECK( clients[ 1 ]->koths, "" );
    CHECK( clients[ 2 ]->koths, "" );
}

int main() {
    test_basic_auth();
    test_obj_list();
}
