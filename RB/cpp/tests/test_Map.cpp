#include <RiotBase/Variable/Symbol/Graphviz.h>
#include <RiotBase/Variable/String.h>
#include <RiotBase/Variable/Map.h>
#include <RiotBase/System/Check.h>
#include "OtMachine.h"
using namespace RiotBase;

void test_basic() {
    Map<String,String> m;

    CHECK( m[ "a" ], "" );
    CHECK( m, "{a:}" );

    String v = m[ "a" ];
    CHECK( v = "A", "A" );
    CHECK( m, "{a:}" );

    Ref<String> r = m[ "a" ];
    CHECK( r = "A", "A" );
    CHECK( m, "{a:A}" );
}

void test_callbacks() {
    Map<String,String> m;
    int cpt = 0;

    m.on_change( [&]{ cpt += 10; } );
    CHECK( m[ "a" ] = "A", "A" );
    Callback::exec_all();
    CHECK( cpt, 10 );

    m[ "a" ].on_change( [&]{ cpt += 1; } );
    CHECK( m[ "a" ] += "B", "AB" );
    Callback::exec_all();
    CHECK( cpt, 21 );
}

//void test_sym() {
//    Map<String,String> m;
//    String s = String::symbol( "s" );
//    Ref<String> v = m[ s ];
//    CHECK( v, "select(add_key_s({},s),s)" );
//    v += "val";

//    //    Graphviz gr;
//    //    gr << m.rp;
//    //    gr << s.rp;
//    //    // gr << v.rp;
//    //    gr.display();

//    Rp *n; new ( &n ) String( "key" );
//    set_lazy_val( s.rp, n );
//    CHECK( m, "{key:val}" );
//    CHECK( s, "key" );
//}

//void test_sym_rec() {
//    Map<String,Map<String,String>> m;
//    String s = String::symbol( "s" );
//    //Ref<String> v = m[ s ][ "k1" ];
//    //CHECK( m, "add_key_extr[0](add_key[0](sym,s),k1,s)" );
//    //v += "v1";
//    m[ s ][ "k1" ] += "v1";

//    //    Graphviz gr;
//    //    gr << m.rp;
//    //    gr.display();

//    Rp *n; new ( &n ) String( "k0" );
//    set_lazy_val( s.rp, n );
//    CHECK( m, "{k0:{k1:v1}}" );
//}

void test_sym_map() {
    Map<String,String> m = Map<String,String>::symbol( "map" );
    //CHECK( m[ "a" ], "select(add_key_s(map,a),a)" );
    //CHECK( m, "add_key_s(map,a)" );
    // CHECK( m[ "a" ], "select(add_key_s(map,a),a)" );
    // m[ "a" ] += "b";
    m[ "a" ] = "b";

    // String c = v;
    Graphviz gr;
    gr << m.rp;
    // gr << c.rp;
    gr.display();

    //    Rp *n; new ( &n ) Map<String,String>;
    //    set_lazy_val( m.rp, n );
    //    CHECK( m, "{a:b}" );
}

//
template<class TM,class Op>
std::string cof( const Op &op, int nb_machines = 3 ) {
    Vec<OtMachine<TM>> machines( Size(), nb_machines );
    for( int i = 0; i < nb_machines; ++i )
        machines[ i ].dev = PI64( 100 + i );
    for( OtMachine<TM> &m : machines )
        for( OtMachine<TM> &n : machines )
            if ( &m != &n )
                m.links << &n;
    struct DataVec {
        TM &operator[]( PT i ) { return (*machines)[ i ].data(); }
        Vec<OtMachine<TM>> *machines;
    };

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

        //        //
        //        Vec<std::string> rights = machines[ i ].data.get_usr_rights( KnownInodes::anon_usr );
        //        if ( rights.size() )
        //            res[ i ] << " " << rights;

        // discrepencies
        if ( i == 0 )
            out << res[ i ].str();
        else if ( res[ i ].str() != res[ 0 ].str() )
            out << " ~[i=" << i << ":" << res[ i ].str() << "]";
    }

    return out.str();
}

#define OTCHECK( FUNC, RESU ) CHECK_TXT( #FUNC, cof<MSS>( []( auto &data, auto &m ) FUNC ), RESU )

void test_ot() {
    using MSS = Map<String,String>;

    // init
    OTCHECK( {
        data[ 0 ][ "a" ] = "A";
    }, "{a:A}" );

    // change val
    OTCHECK( {
        data[ 0 ][ "a" ] = "A";
        sync( m );
        data[ 1 ][ "a" ] += "B";
    }, "{a:AB}" );

    // change val in //
    OTCHECK( {
        data[ 0 ][ "a" ] = "ABC";
        sync( m );
        data[ 1 ][ "a" ].insert( 1, '1' );
        data[ 2 ][ "a" ].insert( 2, '2' );
    }, "{a:A1B2C}" );

    // rem item
    OTCHECK( {
        data[ 0 ][ "a" ] = "A";
        data[ 0 ][ "b" ] = "B";
        sync( m );
        data[ 0 ].del( "b" );
    }, "{a:A}" );
}

//void test_rights() {
//    Map<String,String> m;
//    m[ "a" ] = "A";
    
//    UsrId usr( 100, 1 );
//    m.add_usr_right( usr, "read" );
    
//    // test read
//    String a = "a";
//    String r = RO_CALL( mod_select, m.rp, a.rp, usr );
//    CHECK( r, "A" );
    
//    // test write
//    String b = "b";
//    String s = RO_CALL( mod_select, m.rp, b.rp, usr );
//    CHECK( s, "Error" );
//}

int main() {
    //    test_basic();
    //    test_callbacks();
    // test_sym();
    //    test_sym_rec();
    test_sym_map();
    // test_ot();
    //    test_rights();
}
