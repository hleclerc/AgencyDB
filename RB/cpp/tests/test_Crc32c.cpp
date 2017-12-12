//// nsmake global inc_path ..
#include <RiotBase/System/CbStringPtr.h>
#include <RiotBase/System/CbQueue.h>
#include <RiotBase/System/Crc32c.h>
#include <RiotBase/System/Check.h>
#include <string.h>
using namespace RiotBase;

int main() {
    Crc32c c;

    char txt[] = "my test";
    unsigned expected = 492567828;
    CHECK( c( txt, sizeof txt ), expected );

    CbQueue cq_0;
    cq_0.write_some( txt, sizeof txt );
    CHECK( c( cq_0               ), expected );
    CHECK( c( cq_0.splitted( 2 ) ), expected );
    CHECK( c( cq_0.splitted( 1 ) ), expected );

    // CHECK( c( cq_1 ), 2440484327 );
}
