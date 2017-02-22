#pragma once

#include "../System/InetAddress.h"
#include "TcpRawConnection.h"

namespace RiotBase {

/**
*/
class TcpConnection_Raw : public Event {
public:
    TcpConnection_Raw( const std::string &addr, unsigned port );
    TcpConnection_Raw( int fd = -1 ); ///< for already connected sockets
    TcpConnection_Raw( VtableOnly );
    virtual ~TcpConnection_Raw();

    virtual void     flush                 ();                                  ///< try to send the content (that may be in a buffer if connection was not available or if the data was too small)

    void             write_some            ( const void *data, PT size );       ///< append raw binary data
    void             write_some            ( const CbStringPtr &s );            ///< append raw binary data
    void             write_some            ( CbString &&s );                    ///< append raw binary data
    void             write_some            ( CbQueue &&s );                     ///< append raw binary data
    void             write_byte            ( PI8 val ) { cw.write_byte( val ); }
    void             write_byte_wo_beg_test( PI8 val ) { cw.write_byte_wo_beg_test( val ); }

protected:
    TcpConnection_Raw();

    virtual bool     parse                 ( Buffer *buff ) = 0;   ///< return false if we have enough data
    virtual void     on_connection_refused (); ///<
    virtual void     on_connect_error      ( const char *msg ); ///<
    virtual void     on_invalid_address    (); ///<

    virtual void     inp                   ();
    virtual void     out                   ();

    template<class Cq>
    void             write_some_owning_cq  ( Cq &&s );                    ///< append raw binary data

    void             _on_connect           ( int err );
    void             __parse               ( Buffer *buff );

    Buffer          *inp_buff = 0;                   ///< buffer for input data
    CbQueue          cw;                             ///< buffer for output data
    InetAddress      addr;
    bool             ok_to_parse = true;             ///< we need this because we may have inp() even if we don't say wait_for_input()
    bool             waiting_for_connection = false; ///<
};

} // namespace RiotBase
