#include <RiotBase/Tracker/Tracker.h>
#include <RiotBase/System/Check.h>
#include <RiotBase/System/Date.h>
#include <RiotBase/System/Log.h>
#include <RiotBase/Db/Db.h>

using namespace RiotBase;

PT my_fork( PT num ) {
    while ( --num )
        if ( fork() )
            return num;
    return num;
}

void test_sync() {
    spdlog::set_pattern("[%H:%M:%S:%e %t %l] %v");
    spdlog::set_level( spdlog::level::debug );
    PT num_inst = my_fork( 3 );

    // instanciate a tracker
    if ( num_inst == 0 ) {
        Tracker tracker( /*tracker id*/ 1, "test_password_db", "my_pass", /*db clean*/ true );
        tracker.bind( 8676, "mycert.pem" );
        tracker.run( 2.0 );
        return;
    }

    // databases
    // Date::sleep( 0.5 );
    db->connect( "root", "my_pass", "127.0.0.1", 8676 );

    //    String s = db->bind( "/test_" + to_String( num_inst ) );
    //    PRINT( db->bind( "/" ).operator Directory() );
    //    if ( num_inst == 1 )
    //        s += "AB";

    db->run( 2.0 );
}

int main() {
    test_sync();
}

//void test_with_client_server( const Vec<std::function<void()>> &client_fun ) {
//    spdlog::set_level( spdlog::level::debug );
//    spdlog::set_pattern("[%H:%M:%S:%e %t %l] %v");

//    Db::public_keys_by_usr_id[ KnownUsrId::root_usr ] = root_key_pub;

//    // IdGenerator
//    if ( fork() ) {
//        IdGenerator id_gen( 1 );
//        id_gen.endcode = 1;
//        id_gen.listen( "8880" );
//        id_gen.run();
//        return;
//    }

//    // instances connected to the listening one
//    for( PT i = 1; i < client_fun.size(); ++i ) {
//        if ( fork() ) {
//            db->connect( "sock_0" );
//            client_fun[ i ]();
//            return;
//        }
//    }

//    // first instance
//    if ( fork() ) {
//        db->listen( "sock_0" );
//        client_fun[ 0 ]();

//        // quit the id_server + the connection instances
//        qa_request( "localhost:8880", []( BBQ bq ) { bq.write_byte( 1 ); } );
//        for( RcPtr<DbSession> &s : db->sessions ) {
//            s->send_quit();
//            s->flush();
//        }
//        return;
//    }
//}

//void test_dist_exec() {
//    auto f0 = []() {
//        Number n_0 = db->bind( "/n_0" );
//        CHECK( n_0, 0 );
//        db->run();
//    };

//    auto f1 = []() {
//        Number n_1 = db->bind( "/n_1" );
//        CHECK( n_1, 0 );
//        db->run();
//    };

//    test_with_client_server( { f0, f1 } );
//}

//int main() {
//    test_dist_exec();
//}

//#include <RiotBase/Variable/Symbol/LazySourceKnownValue.h>
//#include <RiotBase/Variable/Symbol/LazySourceExtValue.h>
//#include <RiotBase/Variable/Symbol/Graphviz.h>
//#include <RiotBase/System/Check.h>
//#include <RiotBase/System/Log.h>
//#include <RiotBase/Db/Gev.h>
//#include <RiotBase/Db/Db.h>

//#include <boost/filesystem.hpp>
//using namespace RiotBase;

//void test_loc_bind() {
//    Db db( "root", priv_root_key, 100 );

//    // the same object
//    String s = db.bind( "/test" );
//    String t = db.bind( "/test" );
//    CHECK( s = "a"    , "a" );
//    CHECK( t          , "a" );
//    CHECK( db.bind( "/" ).operator Directory(), "{test:50@0}" );

//    // check category is correct
//    Number n = db.bind( "/test" );
//    CHECK( n          , "Error: Bad category" );

//    // rec directory creation
//    Number m = db.bind( "/foo/bar" );
//    Number l = db.bind( "/foo/bar" );
//    CHECK( m = 10, 10 );
//    CHECK( l     , 10 );
//    CHECK( db.bind( "/" ).operator Directory(), "{foo:51@0,test:50@0}" );
//}

//void test_lazy_ext() {
//    spdlog::set_level( spdlog::level::debug );

//    Db db_0( "root", priv_root_key, 100 );
//    Db db_1( "root", priv_root_key, 101 );
//    init_keys();

//    // connections
//    db_0.listen ( "sock_0" );
//    db_1.connect( "sock_0" );
//    db_sync();

//    // make an object in db_0
//    String s_0 = db_0.bind( "/test" ); // -> 50@100
//    s_0 = "AB";

//    // make a ref in db_1
//    DbItem *i_1 = new DbItem{ Directory::category, Inode{ 100, 50 }, &db_1 };
//    String  s_1 = LazySourceExtValue::mkl( i_1 );

//    db_sync();
//    CHECK( s_0, "AB" );
//    CHECK( s_1, "AB" );
//}

//void test_lazy_dirs() {
//    Db db_0( "root", priv_root_key, 100 );
//    Db db_1( "root", priv_root_key, 101 );
//    init_keys();

//    db_0.listen ( "sock_0" ); // '/' directory will be created locally
//    db_1.connect( "sock_0" ); // '/' directory will be defined on the other side

//    // symbolic value for s_1 (because '/' is not known yet)
//    String s_0 = db_0.bind( "/test" );
//    String s_1 = db_1.bind( "/test" );
//    CHECK( s_0, "" );
//    CHECK( s_1, "bind_by_kn_object_id(select(make_inode_if_z_extr_s(add_key_s(on_change_s(Sym<ExtS,2@1>),test),test),test))" );

//    //    Graphviz gr;
//    //    gr << n.rp;
//    //    gr.display();

//    db_sync();
//    CHECK( s_0, "" );
//    CHECK( s_1, "" );
//    s_1 += "ABC";

//    db_sync();
//    CHECK( s_0, "ABC" );
//    CHECK( s_1, "ABC" );

//    CHECK( db_0.bind( "/" ).operator Directory(), "test:50@0" );
//    CHECK( db_1.bind( "/" ).operator Directory(), "test:50@100" );
//}

////void test_update() {
////    Db db_0( 100, KnownInodes::root_usr );
////    Db db_1( 101, KnownInodes::root_usr );

////    Number n_0 = db_0.bind( "/test" );
////    Number n_1 = db_1.bind( "/test" );

////    n_0 = 5;
////    n_1 = 6;

////    // dbs are not synchronized yet
////    CHECK( db_0.bind( "/" ).operator Directory(), "test:50@100" );
////    CHECK( db_1.bind( "/" ).operator Directory(), "test:50@101" );
////    CHECK( n_0, "5" );
////    CHECK( n_1, "6" );

////    //
////    db_0.listen ( "sock_0" );
////    db_1.connect( "sock_0" );
////    db_sync();
////    CHECK( db_0.bind( "/" ).operator Directory(), "test:50@100" );
////    CHECK( db_1.bind( "/" ).operator Directory(), "test:50@100" );
////    CHECK( n_0, "5" );
////    CHECK( n_1, "5" );

////    n_1 = 10;
////    db_sync();
////    CHECK( n_0, "10" );
////    CHECK( n_1, "10" );
////}



////void test_save() {
////    // clear
////    std::string dir;
////    while ( true ) {
////        dir = "test_" + to_string( rand() );
////        if ( not boost::filesystem::exists( dir ) )
////            break;
////    }
////    boost::filesystem::create_directory( dir );
////    PRINT( dir );

////    // create some data in a Db that will be destroyed
////    {
////        Db db( 100 );
////        CHECK( int( db.set_main_dir ( dir             ) ), int( Db::SetMainDirStatusCode ::ok ) );
////        CHECK( int( db.add_data_file( dir + "/data_0" ) ), int( Db::AddDataFileStatusCode::ok ) );

////        Number n = db.bind( "/num" ); n = 10;
////        String s = db.bind( "/str" ); s += "ABC";
////        db.flush();
////    }
////    system( ( "ls -l " + dir ).c_str() );
////    // system( ( "cat " + dir + "/main_0" ).c_str() );
////    // system( ( "hexdump " + dir + "/data_0" ).c_str() );

////    // read it
////    Db db( 100 );
////    int nb_checks = 0;
////    {
////        CHECK( int( db.set_main_dir( dir ) ), int( Db::SetMainDirStatusCode ::ok ) );

////        Number n = db.bind( "/num" );
////        String s = db.bind( "/str" );

////        n.then( [ &nb_checks ]( Number &n ){ CHECK( n, 10    ); nb_checks += 10; } );
////        s.then( [ &nb_checks ]( String &s ){ CHECK( s, "ABC" ); nb_checks +=  1; } );
        
////    }
////    for( PT i = 0; i < 50 and nb_checks != 11; ++i) // we need a loop because valgrind may need more time
////        db_sync();
////    CHECK( nb_checks, 11 );

////    // wait
////    {
////        Db db( 100 );
////        CHECK( int( db.set_main_dir( dir ) ), int( Db::SetMainDirStatusCode ::ok ) );

////        Number n = db.bind( "/num" );
////        n.wait();
////        String s = db.bind( "/str" );
////        s.wait();
////        CHECK( n, 10    );
////        CHECK( s, "ABC" );
////    }

////    boost::filesystem::remove_all( dir );
////}

////void test_merge() {
////    Db db_1( 100, KnownInodes::root_usr, priv_root_key ); db_1.listen ( "my_socket" );
////    Db db_2( 101, KnownInodes::root_usr, priv_root_key ); db_2.connect( "my_socket" );

////    Number n_1 = db_1.bind( "/test" );
////    Number n_2 = db_2.bind( "/test" );
////    n_1 = 10;

////    db_sync();
////    CHECK( db_2.bind( "/" ).operator Directory(), "test:50@101" );
////    CHECK( n_1, 10 );
////    CHECK( n_2, 10 );
////}

//int main() {
//    // test_auth();
//    // test_loc_bind();
//    test_lazy_ext();
//    // test_lazy_dirs();
//    // test_update();
//    // test_save();
//    //test_merge();
//}
