#include <RiotBase/Variable/Symbol/LazySourceKnownValueExt.h>
#include <RiotBase/Variable/String/GenString.h>
#include <RiotBase/Variable/Symbol/Graphviz.h>
#include <RiotBase/IdGenerator/IdGenerator.h>
#include <RiotBase/EvLoop/QaRequest.h>
#include <RiotBase/Variable/String.h>
#include <RiotBase/System/Check.h>
#include <RiotBase/System/Date.h>
#include <RiotBase/EvLoop/Gev.h>
#include <RiotBase/System/Log.h>
#include <RiotBase/Db/Db.h>
#include <unistd.h>
#include <fcntl.h>

#include "base_keys.h"

using namespace RiotBase;


void test_dist_exec() {
    spdlog::set_level( spdlog::level::debug );
    spdlog::set_pattern("[%H:%M:%S:%e %t %l] %v");

    Db::public_keys_by_usr_id[ KnownInodes::root_usr ] = root_key_pub;
    db->set_user_credentials( "root", root_key_prv );
    db->add_tracker( "localhost:8880" );

    if ( fork() ) {
        // Id server
        IdGenerator id_gen( 1 );
        id_gen.endcode = 1;
        id_gen.listen( "8880" );
        id_gen.run();
    } else if ( fork() ) {
        // db server
        db->listen( "sock_0" );
        gev->run();
    } else {
        // db instance
        DbSession *s = db->connect( "sock_0" );
        s->wait_for_dev_id();

        String res = String( LazySourceKnownValueExt::mkl( GenString::New( "a b c" ), s->dev_id() ) ).split( " " ).join( "+" );
        CHECK( res, "a+b+c" );

        // quit the servers
        qa_request( "localhost:8880", []( BBQ bq ) { bq.write_byte( 1 ); } );
        s->send_quit();
        s->flush();
    }
}

int main() {
    test_dist_exec();
}

