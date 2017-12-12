//// nsmake global inc_path ..

#include <RiotBase/System/BinStreamWithOffsets.h>
#include <RiotBase/System/CbStringPtr.h>
#include <RiotBase/System/TxtStream.h>
#include <RiotBase/System/Check.h>

using namespace RiotBase;

void test_bin_stream_cb() {
    PI32 uvals[] = { 1,  10, 100,  1000 };
    SI32 svals[] = { 1, -10, 100, -1000, - 1024 * 1024, -5 };

    // bin writer
    CbQueue os;
    BinStream<CbQueue> bw( &os );
    for( PI32 u : uvals )
        bw.write_unsigned( u );
    for( SI32 s : svals )
        bw.write_signed( s );

    // copy
    CbString cs( os );
    BinStream<CbString> bs( &cs );

    // bin reader
    auto f = [ uvals, svals ]( auto bw ) {
        for( PI32 u : uvals ) {
            int r = bw.read_unsigned();
            CHECK( bw.empty(), 0 );
            CHECK( bw.error(), 0 );
            CHECK( r, u );
        }
        for( SI32 s : svals ) {
            int r = bw.read_signed();
            CHECK( bw.empty(), s == - 5 );
            CHECK( bw.error(), 0 );
            CHECK( r, s );
        }
        (int)bw.read_signed();
        CHECK( bw.error(), true );
        CHECK( bw.empty(), true );
        CHECK( bw.size(), 0 );
    };

    f( bw );
    f( bs );
}

void test_txt_stream_cb() {
    // txt writer
    CbQueue os;
    TxtStream<CbQueue> bw( &os );

    bw << 127;
    CHECK( std::string( os ), "127" );
    CHECK( int( bw.read_unsigned() ), 127 );
}


void test_bin_stream_cm() {
    PI32 uvals[] = { 1,  10, 100,  1000 };
    SI32 svals[] = { 1, -10, 100, -1000, - 1024 * 1024, -5 };
    PI8  rdata[ 28 ];

    // bin writer
    CmQueue os( rdata );
    BinStream<CmQueue> bw( &os );
    for( PI32 u : uvals )
        bw.write_unsigned( u );
    for( SI32 s : svals )
        bw.write_signed( s );

    // bin reader
    for( PI32 u : uvals ) {
        int r = bw.read_unsigned();
        CHECK( bw.empty(), 0 );
        CHECK( bw.error(), 0 );
        CHECK( r, u );
    }
    for( SI32 s : svals ) {
        int r = bw.read_signed();
        CHECK( bw.empty(), s == - 5 );
        CHECK( bw.error(), 0 );
        CHECK( r, s );
    }
    (int)bw.read_signed();
    CHECK( bw.error(), true );
    CHECK( bw.empty(), true );
    CHECK( bw.size(), 0 );
}

void test_compressed_of() {
    // remove chuncks
    CbQueue cb;
    for( int i = 0; i < 16; ++i )
        cb.write_byte( i );
    CHECK( cb, "00 01 02 03 04 05 06 07 08 09 0a 0b 0c 0d 0e 0f" );

    cb.remove_chunks( Vec<PT>({2,10}), Vec<PT>({2,2}) );
    CHECK( cb, "00 01 04 05 06 07 08 09 0c 0d 0e 0f" );

    //
    CbQueue cn; BinStreamWithOffsets<CbQueue> bn( &cn );
    bn.write_byte( 0x40 );
    bn.beg_mark();
      bn.write_byte( 0x41 );
      bn.beg_mark();
        bn.write_byte( 0x42 );
      bn.end_mark();
      bn.write_byte( 0x43 );
    bn.end_mark();
    bn.write_byte( 0x44 );

    bn.crunch();
    CHECK( cn, "40 04 41 01 42 43 44" );
}

void test_utf8() {
    const char *data = "€123456";
    CbQueue c;
    c.write_some( data, strlen( data ) );
    CbString s = c;
    Vec<unsigned> r;
    CHECK( s.find_utf8( [ &r ]( unsigned v ) { r << v; return false; } ), false );
    CHECK( r, "14844588 49 50 51 52 53 54" );
}

void test_CbStringPtr() {
    CbString cs;
    {
        CbQueue cq; BinStream<CbQueue> bq( &cq );
        bq << "123456";
        //PRINT( cq );

        cs = cq;
        //PRINT( cs );
    }

    CbStringPtr cp( cs );
    //PRINT( cp );
    BinStream<CbStringPtr> bp( &cp );

    CbString s = bp.read();
    CHECK( std::string( s ), "123456" );
}

void test_write_some() {
    CbQueue cq_1;

    {
        BBQ bq_1( &cq_1 );
        bq_1 << 1u;

        CbQueue cq_0;
        BBQ( &cq_0 ) << 3u << 4;

        bq_1 << 2u;
        bq_1.write_some( std::move( cq_0 ) ); //
    }
    CHECK( cq_1, "01 02 03 04" );
}

void test_read_line() {
    CbQueue cq; cq.write_some( "ab\ncd\n\nef" );
    CbString cs = std::move( cq );
    Vec<std::string> expected( "ab", "cd", "ef" );
    while ( CbString line = cs.read_line() ) {
        CHECK( std::string( line ), expected[ 0 ] );
        expected.remove( 0 );
    }
    CHECK( expected.size(), 0 );
    CHECK( cq.size(), 0 );
}

void test_starts_with() {
    CbQueue cq; cq.write_some( "abcd" );
    CbString cs = std::move( cq );

    CHECK( cs.starts_with( "ab" ), true );
    CHECK( cs.starts_with( "abcd" ), true );
    CHECK( cs.starts_with( "abcdef" ), false );

    CHECK( cs.starts_with( "bc" ), false );
}

void test_PT_rw() {
    CbQueue cq;
    BinStream<CbQueue> bq( &cq );
    for( PT var = 0; var < 258; ++var )
        bq << var;
    Vec<PT> res, exp;
    for( PT var = 0; var < 258; ++var ) {
        res << bq.read();
        exp << var;
    }
    CHECK( res, exp );
}

void test_ST_rw() {
    CbQueue cq;
    BinStream<CbQueue> bq( &cq );
    for( ST var = -256; var < 258; ++var )
        bq << var;
    Vec<ST> res, exp;
    for( ST var = -256; var < 258; ++var ) {
        res << bq.read();
        exp << var;
    }
    CHECK( res, exp );
}

int main() {
    //    test_bin_stream_cb();
    //    test_txt_stream_cb();
    //    test_bin_stream_cm();
    //    test_compressed_of();
    //    test_utf8();
    //    test_CbStringPtr();
    //    test_write_some();
    // test_read_line();
    // test_starts_with();
    test_PT_rw();
    test_ST_rw();
}
