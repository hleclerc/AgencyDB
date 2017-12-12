#include <RiotBase/Variable/Image.h>
#include <RiotBase/System/Check.h>
#include "OtMachine.h"
using namespace RiotBase;

void test_base_op() {
    //    Image i( 2, 2, 255, 255, 255, 0 );
    //    i.apply_brush( 0.5, 0.5, Image::make_pixel( 0, 0, 0 ) );

    Image i( 1, 1, 255, 255, 255, 0 );
    i.apply_brush( 0.5, 0.5, Image::make_pixel( 0, 0, 0, 255 ) );
    PRINT( i );
    // RO_CALL( write_patch_data, i.rp, std::cout, 4, nullptr ); std::cout << std::endl;

    //    DevId dst = 128;
    //    AutoPtr<OtMessage> ot_msg = i.get_changes_for( &dst, 1, 100, KnownInodes::anon_usr );
    //    PRINT( ot_msg->msg );

    // i.apply_brush( 1, 1, Image::make_pixel( 0, 0, 0 ) );
    // PRINT( i );
}

//void test_get_changes() {
//    ObjectId i( Inode( 100, 1 ) );
//    UsrId usr;
//    CbQueue cq;
//    i.get_changes( cq, 100, usr );
//    CHECK( cq, "01 64 " );

//    cq.clear();
//    i.get_changes( cq, 100, usr );
//    CHECK( cq, "" );

//}

void test_ot() {
    using T = Image;
    Vec<OtMachine<T>> machines( Rese(), 3 );
    for( PI64 i = 0; i < 3; ++i )
        machines << 100 + i;
    for( OtMachine<T> &m : machines )
        for( OtMachine<T> &n : machines )
            if ( &m != &n )
                m.links << &n;

    // init
    for( PI64 i = 0; i < machines.size(); ++i )
        machines[ i ].data = Image( 1, 1, 255, 255, 255, 0 );

    machines[ 0 ].data.apply_brush( 0.5, 0.5, Image::make_pixel( 0, 0, 0 ) );
    sync( machines );
    CHECK( machines[ 0 ].data, "0 0 0 255\n" );
    CHECK( machines[ 1 ].data, "0 0 0 255\n" );
    CHECK( machines[ 2 ].data, "0 0 0 255\n" );
}

int main() {
    // test_base_op();
    //    test_get_changes();
    test_ot();
}


