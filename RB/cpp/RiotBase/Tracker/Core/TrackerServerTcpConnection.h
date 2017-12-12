#pragma once

#include "../../EvLoop/TcpSslConnection.h"
#include "../../System/BinStream.h"
#include "../../System/Crc32c.h"
#include "../../System/Inode.h"

namespace RiotBase {
class TrackerServerSession;
class VectorClock;
class Tracker;

/**
*/
class TrackerServerTcpConnection : public TcpSslConnection {
public:
    TrackerServerTcpConnection( SSL_CTX *ssl_ctx, int fd, Tracker *tracker, const InetAddress &addr );
    ~TrackerServerTcpConnection();

    // server -> client
    #include "TrackerServerTcpConnectionSendFuncs.h"

protected:
    enum class WebsocketStatusCode {
        normal                                                             = 1000,
        going_away                                                         = 1001,
        protocol_error                                                     = 1002,
        unsupported_data                                                   = 1003,
        reserved                                                           = 1004,
        reserved_for_extensions_0                                          = 1005,
        reserved_for_extensions_1                                          = 1006,
        inconsistent_or_invalid_data                                       = 1007,
        policy_violation                                                   = 1008,
        message_too_big                                                    = 1009,
        extension_handshake_missing                                        = 1010,
        an_unexpected_condition_prevented_the_request_from_being_fulfilled = 1011
    };
    struct BsAc {
        BsAc( CbQueue *cw ) : bs( cw ), old_size( cw->size() ) {}
        ~BsAc() { PI32 hash = Crc32c()( CbString( *bs.buf, old_size, bs.buf->size() - old_size ) ); bs.buf->write_some( &hash, sizeof hash ); }
        template<class T> BsAc &operator<<( const T &val ) { bs << val; return *this; }
        BinStream<CbQueue> bs;
        PT old_size;
    };

    virtual bool          parse                      ();
    virtual void          frame_cw                   ();
    virtual void          skip_framed                ( PT len );
    bool                  parse_websocket_msg        ();
    bool                  method_not_allowed         ();
    bool                  websocket_handshake        ();
    bool                  http_get                   ();
    bool                  websocket_connection_close ( WebsocketStatusCode why  );
    bool                  websocket_connection_closed();
    void                  websocket_ping             ();
    void                  websocket_pong             ();

    // client -> server
    void                  recv_error_msg             ( CbString msg );
    void                  recv_corrupted_data        ();
    void                  recv_login                 ( CbString user, CbString pass );
    void                  recv_logout                ();
    void                  recv_bind_obj              ( Inode obj, const VectorClock &known );
    void                  recv_load_and_bind_obj     ( Inode obj );
    void                  recv_load_obj              ( Inode obj );
    void                  recv_get_dev_info          ( DevId dev );
    void                  recv_ice_candidate         ( DevId destination, PI8 initiator, const CbString &data );
    void                  recv_webrtc_offer          ( DevId destination, const CbString &data );
    void                  recv_webrtc_answer         ( DevId destination, const CbString &data );
    void                  recv_create_user           ( const CbString &user, const CbString &pass, const CbString &email );

    BsAc                  bs_out_auto_close          () { return { &cw }; }

    //template<class TF>
    //bool                  parse_cmd                  ( CbStringPtr &ws_ptr, CbString &ws_cmd, const TF &f, const char *cmd );
    bool                  check_checksum             ( CbString &ws_cmd, PT delta, const char *cmd );

    void                  recv_auth                  ( CbString user, CbString pass );

    InetAddress           inet_address;
    TrackerServerSession *session;
    Tracker              *tracker;
    PT                    framed; ///< how much data has been framed to be sent in a websocket

    // parsing websocket data
    CbString              url;
    CbString              websocket_key;
    int                   websocket_version;
    int                   websocket_byte_0;
    unsigned              websocket_payload_length;
    char                  websocket_masking_key[ 4 ];
    CbString              websocket_data;
    bool                  websocket_mode;

    // helpers for continuation
    void                 *inp_cont;
    Buffer               *pending_cb_queue;    ///< if we need to add the current buffer to a previous one
    unsigned              tmp_shift_read;      ///< used by READ_UINT, READ_INT, ...

    Crc32c                crc_func;
};

} // namespace RiotBase
