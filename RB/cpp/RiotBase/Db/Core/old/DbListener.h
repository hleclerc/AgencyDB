#pragma once

#include "../../EvLoop/TcpListener.h"
#include <string>

namespace RiotBase {
class Db;

///
class DbTcpListener : public TcpListener {
public:
    DbTcpListener( Db *db, unsigned port );
    ~DbTcpListener();

    virtual bool connection( int fd );

    virtual void rdy();

    unsigned     port; ///< used only for the rdy() message
    Db          *db;
};

} // namespace RiotBase
