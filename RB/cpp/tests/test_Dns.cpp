//// nsmake global inc_path ..

#include "../RiotBase/System/Check.h"
#include "../RiotBase/EvLoop/Gev.h"
#include "../RiotBase/EvLoop/Dns.h"

using namespace RiotBase;

void test_send() {
    Dns dns( gev.ptr() );
    dns.query( "www.google.com"   , []( const Vec<Dns::Ipv4> &i4, const Vec<Dns::Ipv6> &i6 ) { CHECK( i4.size() + i6.size() != 0, true ); }, []( int code ) { CHECK( code, -1 ); } );
    dns.query( "www.google.com"   , []( const Vec<Dns::Ipv4> &i4, const Vec<Dns::Ipv6> &i6 ) { CHECK( i4.size() + i6.size() != 0, true ); }, []( int code ) { CHECK( code, -1 ); } );
    dns.query( "wxww.gsoqogle.com", []( const Vec<Dns::Ipv4> &i4, const Vec<Dns::Ipv6> &i6 ) { CHECK( i4.size() + i6.size(), 0 );         }, []( int code ) { CHECK( code, -1 ); } );
    gev->run( 1 );

    dns.query( "www.google.com"   , []( const Vec<Dns::Ipv4> &i4, const Vec<Dns::Ipv6> &i6 ) { CHECK( i4.size() + i6.size() != 0, true ); }, []( int code ) { CHECK( code, -1 ); } );
    gev->run( 1 );
}

int main() {
    test_send();
}
