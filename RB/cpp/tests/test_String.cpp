#include <RiotBase/Variable/Symbol/ActiveLazySources.h>
#include <RiotBase/Variable/Symbol/Temporize.h>
#include <RiotBase/Variable/Symbol/Graphviz.h>
#include <RiotBase/Variable/Ot/OtMerger.h>
#include <RiotBase/Variable/String.h>
#include <RiotBase/System/Check.h>
#include <RiotBase/EvLoop/Gev.h>
#include "OtMachine.h"
#include <unistd.h>
#include <time.h>

using namespace RiotBase;

void test_base_op() {
    String a = "a";

    CHECK( a         , "a"    );
    CHECK( a += "bcd", "abcd" );

    CHECK( a.remove( 2, 1 ), "abd" );
    CHECK( a.remove( 2, 1 ), "ab"  );
}

void test_bin_stream() {
    String a = "a";

    CbQueue msg;
    a.write_to( &msg );
    CHECK( msg, "00 05 01 61" );

    CbStringPtr tmp = msg;
    String s = String::read_from( &tmp );
    CHECK( s, "a" );
}

void test_sym() {
    String a = "a", b = "b", s = String::symbol( "s" );
    CHECK( s, "s" );

    // on_change before Lazyfier
    int cpt = 0;
    a.on_change( [&]{ ++cpt; } );
    CHECK( a += s, "Add(sym(a),s)" );
    CHECK( a += s, "Add(Add(sym(a),s),s)" );
    Callback::exec_all();
    CHECK( cpt, 1 );

    // on_change after Lazyfier
    CHECK( b += s, "Add(sym(b),s)" );
    b.on_change( [&]{ ++cpt; } );
    CHECK( b += s, "Add(Add(sym(b),s),s)" );
    Callback::exec_all();
    CHECK( cpt, 2 );


    Graphviz gr;
    gr << a.rp;
    gr.display();
}


void test_ot_wrapper() {
    String a = "a";
    CbQueue changes;
    RO_CALL( pull_ot_changes, a.rp, &changes );
    CHECK( changes, "04 00 01 61" );

    std::ostringstream ot_data;
    RO_CALL( write_ot_data, a.rp, ot_data, changes );
    CHECK( ot_data.str(), "insert(0,61)" );

    CHECK( RO_CALL( get_usr_rights, a.rp, {} ), "add_rights rem_rights read insert append remove" );
    RO_CALL( rem_usr_right, a.rp, {}, "remove" );
    CHECK( RO_CALL( get_usr_rights, a.rp, {} ), "add_rights rem_rights read insert append" );

    AutoPtr<OtMerger> m = RO_CALL( new_ot_merger, a.rp );
    CbQueue updated_changes;
    CHECK( m->glo_to_loc( &updated_changes, changes, 100, KnownInodes::root_usr ), 1 );
    CHECK( updated_changes, "04 00 01 61" );

    CbQueue updated_new;
    m->new_patch( &updated_new, updated_changes, {}, 0, 0 );
    CHECK( updated_new, "04 00 01 61" );
    CHECK( a, "aa" );
}

//
template<class Op>
std::string cof( const Op &op, const char *init, int nb_machines = 3 ) {
    Vec<OtMachine<String>> machines( Size(), nb_machines );
    for( int i = 0; i < nb_machines; ++i )
        machines[ i ].dev = PI64( 100 + i );
    for( OtMachine<String> &m : machines )
        for( OtMachine<String> &n : machines )
            if ( &m != &n )
                m.links << &n;
    struct DataVec {
        String &operator[]( PT i ) { return (*machines)[ i ].data(); }
        Vec<OtMachine<String>> *machines;
    };

    // init
    machines[ 0 ].data() = init;
    sync( machines );

    // operations
    DataVec dv{ &machines };
    op( dv, machines );

    //    std::cout << " bef ->";
    //    for( OtMachine<String> &m : machines )
    //        m.patch_data.write_to_stream( std::cout << "\n  ", m.dev_id, m.usr_id );
    //    std::cout << std::endl;

    sync( machines );

    //    std::cout << " aft ->";
    //    for( OtMachine<String> &m : machines )
    //        m.patch_data.write_to_stream( std::cout << "\n  ", m.dev_id, m.usr_id );
    //    std::cout << std::endl;

    // outputs
    Vec<std::ostringstream> res( Size(), nb_machines );
    std::ostringstream out;
    for( int i = 0; i < nb_machines; ++i ) {
        // data
        res[ i ] << machines[ i ].data();

        // cursor(s)
        Vec<CursorData> cd;
        RO_CALL( get_cursors, machines[ i ].data().rp, cd );
        for( PT j = 0; j < cd.size(); ++j )
            res[ i ] << " C(" << reinterpret_cast<const Number &>( cd[ j ].val ) << ")";

        // rights
        Vec<std::string> rights = machines[ i ].data().get_usr_rights( KnownInodes::anon_usr );
        if ( rights.size() )
            res[ i ] << " " << rights;

        // discrepencies
        if ( i == 0 )
            out << res[ i ].str();
        else if ( res[ i ].str() != res[ 0 ].str() )
            out << " ~[i=" << i << ":" << res[ i ].str() << "]";
    }

    return out.str();
}

#define OTCHECK( FUNC, ORIG, RESU ) CHECK_TXT( #FUNC " with " #ORIG, cof( []( auto &data, auto &m ) FUNC, ORIG ), RESU )

void test_ot() {
    OTCHECK( {}, "0123", "0123" );

    // simple inserts and removals
    OTCHECK( {
        data[ 1 ].insert( 2, "ab" );
    }, "0123", "01ab23" );

    OTCHECK( {
        data[ 1 ].remove( 1, 2 );
    }, "0123", "03" );

    // parallel insert/... operations
    OTCHECK( {
        data[ 1 ].insert( 1, "ab" );
        data[ 2 ].insert( 3, "cd" );
    }, "0123", "0ab12cd3" );

    OTCHECK( {
        data[ 2 ].insert( 3, "cd" );
        data[ 1 ].insert( 1, "ab" );
    }, "0123", "0ab12cd3" );

    OTCHECK( {
        data[ 0 ].insert( 3, "ab" );
        data[ 1 ].remove( 1, 1 );
    }, "012345", "02ab345" );

    OTCHECK( {
        data[ 0 ].insert( 3, "ab" );
        data[ 1 ].remove( 1, 4 );
    }, "012345", "0ab5" );

    OTCHECK( {
        data[ 0 ].insert( 3, "ab" );
        data[ 1 ].remove( 4, 1 );
    }, "012345", "012ab35" );

    // parallel remove/... operations
    OTCHECK( {
        data[ 1 ].remove( 1, 1 );
        data[ 0 ].insert( 3, "ab" );
    }, "012345", "02ab345" );

    OTCHECK( {
        data[ 1 ].remove( 1, 4 );
        data[ 0 ].insert( 3, "ab" );
    }, "012345", "0ab5" );

    OTCHECK( {
        data[ 1 ].remove( 4, 1 );
        data[ 0 ].insert( 3, "ab" );
    }, "012345", "012ab35" );


    OTCHECK( {
        data[ 1 ].remove( 1, 2 );
        data[ 0 ].remove( 7, 2 );
    }, "0123456789", "034569" );

    OTCHECK( {
        data[ 0 ].remove( 1, 4 );
        data[ 1 ].remove( 3, 4 );
    }, "0123456789", "0789" );

    OTCHECK( {
        data[ 0 ].remove( 1, 8 );
        data[ 1 ].remove( 3, 4 );
    }, "0123456789", "09" );

    OTCHECK( {
        data[ 0 ].remove( 7, 2 );
        data[ 1 ].remove( 1, 2 );
    }, "0123456789", "034569" );

    OTCHECK( {
        data[ 1 ].remove( 3, 4 );
        data[ 0 ].remove( 1, 4 );
    }, "0123456789", "0789" );

    OTCHECK( {
        data[ 1 ].remove( 3, 4 );
        data[ 0 ].remove( 1, 8 );
    }, "0123456789", "09" );

    // cursor
    OTCHECK( {
        data[ 0 ].insert( 1, "a" );
        Number cursor = RO_CALL( new_cursor, data[ 1 ].rp, (void *)0 );
        cursor = 2;
    }, "0123", "0a123 C(3)" );

    OTCHECK( {
        Number cursor = RO_CALL( new_cursor, data[ 0 ].rp, (void *)0 );
        cursor = 2;
        data[ 1 ].insert( 1, "a" );
    }, "0123", "0a123 C(3)" );

    OTCHECK( {
        data[ 0 ].remove( 1, 2 );
        Number cursor = RO_CALL( new_cursor, data[ 1 ].rp, (void *)0 );
        cursor = 2;
    }, "0123", "03 C(1)" );

    OTCHECK( {
        Number cursor = RO_CALL( new_cursor, data[ 0 ].rp, (void *)0 );
        cursor = 2;
        data[ 1 ].remove( 1, 2 );
    }, "0123", "03 C(1)" );

    // rights
    OTCHECK( { // transmission
        data[ 0 ].add_usr_right( "read", KnownInodes::anon_usr );
    }, "0", "0 read" );

    OTCHECK( { // transmission with rem
        data[ 0 ].add_usr_right( "read"  , KnownInodes::anon_usr );
        data[ 0 ].add_usr_right( "insert", KnownInodes::anon_usr );
        sync( m );
        data[ 1 ].rem_usr_right( "insert", KnownInodes::anon_usr );
    }, "0", "0 read" );

    OTCHECK( { // right to do things
        data[ 0 ].rem_usr_right( "insert", {} );
        sync( m );
        data[ 1 ].insert( 1, "a" );
    }, "01", "01" );

    // with std patches
    OTCHECK( {
        data[ 0 ].insert( 1, "c" );  // [c] [] []
        sync( m, { &m[ 1 ], &m[ 2 ] } ); // we want date of "c" < date of "b"

        data[ 1 ].insert( 2, "b" );  // [c] [b] []
        sync( m, { &m[ 0 ] } ); // [c] [b] [b]
        sync( m, { &m[ 2 ] } ); // [cb] [cb] [b]
        // -> [cb] [cb] [cb]
    }, "012", "0c1b2" );

    OTCHECK( {
        data[ 0 ].remove( 2, 2 );   // 01234 -> 014
        sync( m, { &m[ 1 ], &m[ 2 ] } );
        data[ 1 ].insert( 1, "b" ); // 0b1234 -> 0b1234
        sync( m, { &m[ 0 ] } );
        sync( m, { &m[ 2 ] } );
    }, "01234", "0b14" );

    OTCHECK( {
        data[ 0 ].remove( 1, 2 );   // 01234 -> 034
        sync( m, { &m[ 1 ], &m[ 2 ] } );
        data[ 1 ].insert( 4, "b" ); // 01234 -> 0123b4
        sync( m, { &m[ 0 ] } );
        sync( m, { &m[ 2 ] } );
    }, "01234", "03b4" );

    OTCHECK( {
        data[ 0 ].remove( 1, 4 );   // 012345 -> 05
        sync( m, { &m[ 1 ], &m[ 2 ] } );
        data[ 1 ].insert( 3, "new" ); // 012345 -> 012new345
        sync( m, { &m[ 0 ] } );
        sync( m, { &m[ 2 ] } );
    }, "012345", "0new5" );

    OTCHECK( {
        data[ 0 ].insert( 1, "b" ); // 01234 -> 0b1234
        sync( m, { &m[ 1 ], &m[ 2 ] } );
        data[ 1 ].remove( 2, 2 );   // 01234 -> 0b14
        sync( m, { &m[ 0 ] } );
        sync( m, { &m[ 2 ] } );
    }, "01234", "0b14" );

    OTCHECK( {
        data[ 0 ].insert( 4, "b" ); // 01234 -> 0123b4
        sync( m, { &m[ 1 ], &m[ 2 ] } );
        data[ 1 ].remove( 1, 2 );   // 01234 -> 034
        sync( m, { &m[ 0 ] } );
        sync( m, { &m[ 2 ] } );
    }, "01234", "03b4" );

    OTCHECK( {
        data[ 0 ].insert( 3, "b" ); // 01234 -> 012b34
        sync( m, { &m[ 1 ], &m[ 2 ] } );
        data[ 1 ].remove( 1, 3 );   // 01234 -> 04
        sync( m, { &m[ 0 ] } );
        sync( m, { &m[ 2 ] } );
    }, "01234", "0b4" );

    OTCHECK( {
        data[ 0 ].remove( 1, 2 ); // 01234567 -> 034567
        sync( m, { &m[ 1 ], &m[ 2 ] } );
        data[ 1 ].remove( 5, 2 ); // 01234567 -> 012347
        sync( m, { &m[ 0 ] } );
        sync( m, { &m[ 2 ] } );
    }, "01234567", "0347" );

    OTCHECK( {
        data[ 0 ].remove( 5, 2 ); // 01234567 -> 034567
        sync( m, { &m[ 1 ], &m[ 2 ] } );
        data[ 1 ].remove( 1, 2 ); // 01234567 -> 012347
        sync( m, { &m[ 0 ] } );
        sync( m, { &m[ 2 ] } );
    }, "01234567", "0347" );

    OTCHECK( {
        data[ 0 ].remove( 1, 5 ); // 01234567 -> 067
        sync( m, { &m[ 1 ], &m[ 2 ] } );
        data[ 1 ].remove( 2, 5 ); // 01234567 -> 017
        sync( m, { &m[ 0 ] } );
        sync( m, { &m[ 2 ] } );
    }, "01234567", "07" );

    OTCHECK( {
        data[ 0 ].remove( 2, 5 ); // 01234567 -> 067
        sync( m, { &m[ 1 ], &m[ 2 ] } );
        data[ 1 ].remove( 1, 5 ); // 01234567 -> 017
        sync( m, { &m[ 0 ] } );
        sync( m, { &m[ 2 ] } );
    }, "01234567", "07" );
}

void test_cursor() {
    String str( "abcdefgh" );
    Number cur = RO_CALL( new_cursor, str.rp, 0 );
    CHECK( cur                 , 0 );
    CHECK( PT( cur )           , 0 );
    CHECK( cur  = 5            , 5 );
    CHECK( cur -= 2            , 3 );
    CHECK( str.insert( 1, "A" ), "aAbcdefgh" );
    CHECK( cur                 , 4 );
    CHECK( str.insert( 5, "B" ), "aAbcdBefgh" );
    CHECK( cur                 , 4 );
}

void test_rights() {
    Vec<OtMachine<String>> machines( Size(), 2 );
    for( PI64 i = 0; i < 2; ++i )
        machines[ i ].dev = 100 + i;
    for( OtMachine<String> &m : machines )
        for( OtMachine<String> &n : machines )
            if ( &m != &n )
                m.links << &n;

    // operations
    String &s = machines[ 0 ].data();
    s = "AB";
    s.rem_usr_right( "insert" );
    s.add_usr_right( "append" );

    CHECK( s.get_usr_rights(), "read remove append" );
    s += "CD";
    CHECK( s, "ABCD" );
    s.insert( 1, "aa" );
    CHECK( s, "ABCD" );

    // transfer
    sync( machines );
    CHECK( machines[ 1 ].data().get_usr_rights(), "read remove append" );
}


void test_big_split() {
    // direct evaluation
    String txt = "a bc d";
    CHECK( txt.size(), 6 );
    CHECK( txt.split( " " ), "[a,bc,d]" );

    // use of thread(s)
    String alt = lazyfy( txt ).split( " " ).join( "+" );
    CHECK( active_lazy_sources->sources.size(), 1 );
    CHECK( alt, "a+bc+d" );
}

void test_cancel_execution() {
    {
        String alt = temporize( lazyfy( String( "a bc d" ) ).split( " " ), 2.0 ).join( "+" );
        active_lazy_sources->exec();
        sleep( 1 );
        // alt won't be finished (blocked here in Temporized) but will be deleted
    }
    // end the current computations
    exec_list.clear();

    // -> mainly a test for valgrind
}

int main() {
    // test_base_op();
    // test_bin_stream();
    // test_sym();
    // test_ot_wrapper();
    // test_ot();
    // test_cursor();
    // test_rights();
    // TODO: tester forward
    test_big_split();
    // test_cancel_execution();
}

