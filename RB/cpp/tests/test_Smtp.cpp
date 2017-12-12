#include "../RiotBase/System/Check.h"
#include "../RiotBase/EvLoop/Smtp.h"
#include "../RiotBase/EvLoop/Gev.h"

using namespace RiotBase;


void test_send() {
    send_mail( "hugo.leclerc@socadb.com", "s", "pouet", []( SmtpStatus s ) { PRINT( int( s ) ); } );
    gev->run( 5 );
}

int main() {
    test_send();
}
