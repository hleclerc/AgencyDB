#pragma once

#include "TcpRawConnection.h"

#include <openssl/ssl.h>
#include <string>

namespace RiotBase {

/**
*/
class TcpSslConnection : public TcpRawConnection {
public:
    TcpSslConnection( SSL_CTX *ssl_ctx, const InetAddress &addr ); ///< for client socket
    TcpSslConnection( SSL_CTX *ssl_ctx, int fd ); ///< for accepting (server) sockets
    TcpSslConnection( VtableOnly );
    virtual ~TcpSslConnection();

    virtual void flush                 (); ///< try to send the content of `cw`

protected:
    virtual void inp                   ();
    virtual void out                   ();
    virtual void ssl_error             ();
    virtual void frame_cw              ();
    virtual void skip_framed           ( PT len );

private:
    typedef enum {
        wants_inp,
        wants_out,
        ok
    } State;

    enum {
        EF_SSL_error = _last_Event_EF * 2
    };

    void         call_SSL_write        ();
    void         call_SSL_read         ();

    SSL         *ssl;
    State        state_SSL_read;  // states are meanly here for optimization (to avoid worthless system calls),
    State        state_SSL_write; // because openssl seems to be able to somewhat manage states by itself
    bool         server;
};

} // namespace RiotBase
