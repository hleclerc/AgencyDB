#pragma once

#include "../System/InetAddress.h"
#include "TcpConnection.h"

namespace RiotBase {

/**
    BinStream<TcpConnection> compatibility
*/
class TcpRawConnection : public TcpConnection {
public:
    TcpRawConnection( const InetAddress &addr ); ///< client mode
    TcpRawConnection( int accepting_fd );        ///< accepting (server) mode
    TcpRawConnection( VtableOnly );              ///< a constructor that does not assign any attribute (else than the vtable). Permits to do a new( ptr ) T to change _only_ the vtable (underlying type)

    virtual void flush           ();

protected:
    virtual void on_connect_error( int error );
    virtual void on_connect      ();
    virtual void inp             ();
    virtual void out             ();

    bool         waiting_for_connection;
};

} // namespace RiotBase
