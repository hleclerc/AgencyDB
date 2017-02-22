#pragma once

#include "../System/InetAddress.h"
#include "Event.h"

namespace RiotBase {

/**
  calls `connection` with an accepting socket fd each a new connection is required
 */
class TcpListener : public Event {
public:
    TcpListener( unsigned port ); ///<
    TcpListener( VtableOnly );

protected:
    virtual bool connection     ( int fd, const InetAddress &addr ) = 0; ///< called after an accept
    virtual void on_bind_error  ();             ///<
    virtual void on_listen_error();             ///<
    virtual void on_accept_error();

    virtual void inp            ();
};

} // namespace RiotBase
