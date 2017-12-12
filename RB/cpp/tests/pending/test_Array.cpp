//#include <RiotBase/Variable/Symbol/SetLazyVal.h>
//#include <RiotBase/Variable/String/GenString.h>
//#include <RiotBase/Variable/Symbol/Graphviz.h>
//#include <RiotBase/Variable/String.h>
//#include <RiotBase/Variable/Array.h>
#include <RiotBase/Variable/Core/AutoRpDel.h>
#include <RiotBase/Variable/Array/Tuple.h>
#include <RiotBase/System/Check.h>
#include "OtMachine.h"
using namespace RiotBase;

//void test_base_ops() {
//    Array<String> m;
//    CHECK( m << "a" << "b" << "c", "a b c" );
//    CHECK( m.size()        , 3 );
//    CHECK( m.back()        , "c" );
//    CHECK( m.head()        , "a b" );
//    CHECK( m.slice( 1, 10 ), "b c" );

//    CHECK( String( " a  b c " ).split( " " ), "a b c" );

//    CHECK( m.reduction( []( const Number &a, const String &b ){ return a + b.size(); }, Number( 0 ) ), 3 );

//    Number n = 10;
//    CHECK( *m.reduction( []( Number *a, const String &b ){ return a; }, &n ), 10 );

//    CHECK( m[ 0 ]      , "a" );
//    CHECK( m[ 0 ] = "A", "A" );
//    CHECK( m           , "A b c" );
//}

//void test_changes() {
//    //    String v = m[ "a" ];
//    //    CHECK( v = "A", "A" );
//    //    CHECK( m, "a:A" );

//    //    CbQueue cq;
//    //    m.get_changes( cq, 100 );
//    //    CHECK( cq, "00 01 64 04 00 00 01 61 04 00 00 01 41 " );

//    //    // -> changer un item (.val par exemple)
//    //    CHECK( m[ "a" ] += "B", "AB" );

//    //    cq.clear();
//    //    m.get_changes( cq, 100 );
//    //    CHECK( cq, "01 01 64 04 00 01 01 42 " );
//}

//void test_sym_repl_item() {
//    Array<String> m;
//    String s = String::symbol( "s" );
//    CHECK( m << "a" << s << "c", "push_back_s(push_back_s(sym,s),c)" );

//    //    Graphviz gr;
//    //    gr << m.rp;
//    //    gr.display();

//    set_lazy_val( s.rp, GenString::New( "b", 1 ) );
//    CHECK( m, "a b c" );
//    CHECK( s, "b" );
//}

//void test_sym_repl_array() {
//    Array<String> l = Array<String>::symbol( "lst" );
//    CHECK( l << "b", "push_back_s(lst,b)" );

//    //    Graphviz gr;
//    //    gr << l.rp;
//    //    gr.display();

//    set_lazy_val( l.rp, new_GenArray( Array<String>::category, String::category ) );
//    CHECK( l, "b" );
//}

//void test_callbacks() {
//    //    Map<String,String> m;
//    //    int cpt = 0;

//    //    m.on_change( [&]{ cpt += 10; } );
//    //    CHECK( m[ "a" ] = "A", "A" );
//    //    Callback::exec_all();
//    //    CHECK( cpt, 10 );

//    //    m[ "a" ].on_change( [&]{ cpt += 1; } );
//    //    CHECK( m[ "a" ] += "B", "AB" );
//    //    Callback::exec_all();
//    //    CHECK( cpt, 21 );
//}

//void test_ot() {
//    //    using T = Map<String,String>;
//    //    Vec<OtMachine<T>> machines( Rese(), 4 );
//    //    for( PI64 i = 0; i < 4; ++i )
//    //        machines << 100 + i;
//    //    for( OtMachine<T> &m : machines )
//    //        for( OtMachine<T> &n : machines )
//    //            if ( &m != &n )
//    //                m.links << &n;

//    //    // init
//    //    machines[ 0 ].data[ "a" ] = "A";
//    //    sync( machines );
//    //    CHECK( machines[ 0 ].data, "a:A" );
//    //    CHECK( machines[ 1 ].data, "a:A" );
//    //    CHECK( machines[ 2 ].data, "a:A" );
//    //    CHECK( machines[ 3 ].data, "a:A" );

//    //    // insert
//    //    machines[ 1 ].data[ "b" ] = "B";
//    //    sync( machines );
//    //    CHECK( machines[ 0 ].data, "a:A b:B" );
//    //    CHECK( machines[ 1 ].data, "a:A b:B" );
//    //    CHECK( machines[ 2 ].data, "a:A b:B" );
//    //    CHECK( machines[ 3 ].data, "a:A b:B" );
//}

void test_tuple() {
    Tuple<int,int> *t = Tuple<int,int>::New( 5, 10 );
    RO_CALL( write_to_stream, t, std::cout ); std::cout << std::endl;
    std::cout << t->type_name() << std::endl;

    Number z( 0 ), o( 1 );
    AutoRpDel v0 = RO_CALL( select_bb, t, z.rp );
    AutoRpDel v1 = RO_CALL( select_bb, t, o.rp );
    RO_CALL( write_to_stream, v0.rp, std::cout ); std::cout << std::endl;
    RO_CALL( write_to_stream, v1.rp, std::cout ); std::cout << std::endl;
}

int main() {
    // test_base_ops();
    // test_changes();
    // test_sym_repl_item();
    // test_sym_repl_array();
    // test_callbacks();
    // test_ot();
    test_tuple();
}

