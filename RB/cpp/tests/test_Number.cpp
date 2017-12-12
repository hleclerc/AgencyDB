#include <RiotBase/Variable/Symbol/LazySourceKnownValue.h>
#include <RiotBase/Variable/Symbol/LazySourceExtValue.h>
#include <RiotBase/Variable/Symbol/Graphviz.h>
#include <RiotBase/Variable/Number/Bn.h>
#include <RiotBase/Variable/Number.h>
#include <RiotBase/System/Check.h>
#include "OtMachine.h"
using namespace RiotBase;

void test_base_op() {
    Number a = 1;
    CHECK( a, 1 );
    CHECK( a + 4, 5 );
    CHECK( a + 4.5, 5.5 );
    CHECK( a += 4.5, 5.5 );
}

void test_raw_rw() {
    // write
    CbQueue cq;
    BBQ( &cq ) << Number( 10 );
    CHECK( cq, "00 0a" );

    CbString cr = cq;
    Number s = BinStream<CbString>( &cr ).read();
    CHECK( s, 11 );
}

//void test_lazyfier() {
//    Number s = Number::symbol( "s" ), b = s;
//    CHECK( b += 10, "add_s(s,10)"  );

//    //    Graphviz gr;
//    //    gr << s.rp;
//    //    gr << b.rp;
//    //    gr.display();

//    // set_lazy_val basic
//    // set_lazy_val( s.rp, Bn<SI32>::New( 12 ) );
//    CHECK( s, 12 );
//    CHECK( b, 22 );

//    // set_lazy_val with a value self_op'ed 2 times
//    Number t = Number::symbol( "t" );
//    Number c = t, d = t;
//    c += 10;
//    d += 20;

//    //    Graphviz gr;
//    //    gr << c.rp;
//    //    gr << d.rp;
//    //    gr << s.rp;
//    //    gr.display();

//    set_lazy_val( t.rp, Bn<SI32>::New( 12 ) );
//    CHECK( t, 12 );
//    CHECK( c, 22 );
//    CHECK( d, 32 );

//    // set_lazy_val with a op( term, sym )
//    Number u = Number::symbol( "u" );
//    Number e = 5 + u;

//    // operations
//    // set_lazy_val( u.rp, Bn<SI32>::New( 12 ) );
//    CHECK( u, 12 );
//    CHECK( e, 17 );
//}

//void test_lazysource() {
//    Number t = LazySourceKnownValue::mkl( Number::category, Bn<SI32>::New( 10 ) );
//    Number o = t + 5;

//    //    {
//    //    Graphviz gr;
//    //    gr << t.rp;
//    //    gr.display();
//    //    }

//    CHECK( t, "10" );
//    CHECK( o, "15" );

//    //    {
//    //    Graphviz gr;
//    //    gr << t.rp;
//    //    gr.display();
//    //    }
//}

void test_ot() {
    using T = Number;
    Vec<OtMachine<T>> machines( Size(), 3 );
    for( PI64 i = 0; i < 3; ++i )
        machines[ i ].dev = PI64( 100 + i );
    for( OtMachine<T> &m : machines )
        for( OtMachine<T> &n : machines )
            if ( &m != &n )
                m.links << &n;

    // init
    machines[ 0 ].data() = 10;
    sync( machines );
    CHECK( machines[ 0 ].data(), 10 );
    CHECK( machines[ 1 ].data(), 10 );
    CHECK( machines[ 2 ].data(), 10 );

    // modification
    machines[ 1 ].data() += 2;
    machines[ 2 ].data() += 3;
    sync( machines );
    CHECK( machines[ 0 ].data(), 15 );
    CHECK( machines[ 1 ].data(), 15 );
    CHECK( machines[ 2 ].data(), 15 );

}

int main() {
    // test_base_op();
    // test_raw_rw();
    // test_lazyfier();
    // test_lazysource();
    test_ot();
}


