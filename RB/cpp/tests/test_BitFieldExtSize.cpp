//// nsmake global inc_path ..

#include <RiotBase/System/BitFieldExtSize.h>
#include <RiotBase/System/Check.h>
using namespace RiotBase;

int main() {
    BitFieldExtSize b{ N<15>(), 0 };
    auto w = b.wst<15>();
    CHECK( w         , "000000000000000" );
    CHECK( w[ 1 ] = 1, "1" );
    CHECK( w         , "010000000000000" );
}
