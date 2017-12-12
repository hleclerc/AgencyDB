#include "../../EvLoop/EvLoop.h"
#include "../../System/Log.h"
#include "../Db.h"

#include "DbConnection.h"
#include "DbTcpListener.h"

namespace RiotBase {

DbTcpListener::DbTcpListener( Db *db, unsigned port) : TcpListener( to_string( port ).c_str() ), port( port ), db( db ) {
}

DbTcpListener::~DbTcpListener() {
    db->TcpListeners.remove_first_unordered( this );
}

bool DbTcpListener::connection( int fd ) {
    DbSession *s = new DbSession( db, true );
    db->sessions << s;

    DbConnection *connection = new DbConnection( fd, s );
    s->set_connection( connection );
    *ev_loop << connection;

    return true;
}

void DbTcpListener::rdy() {
    DISP_INFO( "Db listening to port '{}'", port );
}


} // namespace RiotBase
