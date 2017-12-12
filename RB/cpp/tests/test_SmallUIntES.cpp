#include <RiotBase/System/SmallUIntES.h>
#include <RiotBase/System/Check.h>
using namespace RiotBase;

int main() {
    SmallUIntES<3> b{ N<4>(), 2 };
    auto w = b.wst<4>();
    CHECK( w         , "2 2 2 2" );
    CHECK( w[ 1 ] = 7, "7" );
    CHECK( w         , "2 7 2 2" );
    CHECK( w[ 2 ] = 11, "3" );
    CHECK( w         , "2 7 3 2" );
}

