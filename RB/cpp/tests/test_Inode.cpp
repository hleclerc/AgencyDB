#include <RiotBase/Variable/ObjectId.h>
#include <RiotBase/System/Check.h>
#include "OtMachine.h"
using namespace RiotBase;

void test_base_op() {
    ObjectId i( Inode( 100, 1 ) );
    CHECK( i, "1@100" );

    Inode res;
    CHECK( i.to_Inode( res ), "1" );
    CHECK( res, "1@100" );

    i = Inode( 100, 2 );
    CHECK( i, "2@100" );
}

void test_ot() {
    using T = ObjectId;
    Vec<OtMachine<T>> machines( Size(), 3 );
    for( PI64 i = 0; i < machines.size(); ++i )
        machines[ i ].dev = PI64( 100 + i );
    for( OtMachine<T> &m : machines )
        for( OtMachine<T> &n : machines )
            if ( &m != &n )
                m.links << &n;

    // init
    machines[ 0 ].data() = Inode( 100, 1 );
    sync( machines );
    CHECK( machines[ 0 ].data(), "1@100" );
    CHECK( machines[ 1 ].data(), "1@100" );
    CHECK( machines[ 2 ].data(), "1@100" );

    // modification
    machines[ 1 ].data() = Inode( 101, 2 );
    sync( machines );
    CHECK( machines[ 0 ].data(), "2@101" );
    CHECK( machines[ 1 ].data(), "2@101" );
    CHECK( machines[ 2 ].data(), "2@101" );
}

int main() {
    test_base_op();
    test_ot();
}


