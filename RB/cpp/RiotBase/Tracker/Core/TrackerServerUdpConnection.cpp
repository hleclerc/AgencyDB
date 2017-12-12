/// This file is generated (by bin/Method_template.py). Do not modify it directly.
#include "TrackerServerUdpConnection.h"
#include "TrackerServerUdpSocket.h"
#include "TrackerServerSession.h"
#include "../Tracker.h"

namespace RiotBase {

TrackerServerUdpConnection::~TrackerServerUdpConnection() {
    session->udp_connections.remove_first_unordered( this );
}

void TrackerServerUdpConnection::init( TrackerServerUdpSocket *us, bool server ) {
    UdpHashedCmdsConnection<UdpDtlsConnection<TrackerServerUdpConnection,512>>::init( us, server );
    session = new TrackerServerSession( us->tracker );
    session->add_udp_connection( this );
}

void TrackerServerUdpConnection::send_error_msg( const CbString &msg ) {
    DISP_DEBUG( "send error_msg to {}", session->dev );
    bs_out_auto_close() << 0u << msg;
}
void TrackerServerUdpConnection::send_corrupted_data(  ) {
    DISP_DEBUG( "send corrupted_data to {}", session->dev );
    bs_out_auto_close() << 1u;
}
void TrackerServerUdpConnection::send_ans_login( DevId dev, UsrId usr, const CbString &token ) {
    DISP_DEBUG( "send ans_login to {}", session->dev );
    bs_out_auto_close() << 3u << dev << usr << token;
}
void TrackerServerUdpConnection::send_ans_logout(  ) {
    DISP_DEBUG( "send ans_logout to {}", session->dev );
    bs_out_auto_close() << 4u;
}
void TrackerServerUdpConnection::send_bind_obj( Inode id, DevId dev ) {
    DISP_DEBUG( "send bind_obj to {}", session->dev );
    bs_out_auto_close() << 5u << id << dev;
}
void TrackerServerUdpConnection::send_req_inode_data( Inode id, DevId dev ) {
    DISP_DEBUG( "send req_inode_data to {}", session->dev );
    bs_out_auto_close() << 6u << id << dev;
}
void TrackerServerUdpConnection::send_req_inode_updates( Inode id, DevId dev, const VectorClock &known ) {
    DISP_DEBUG( "send req_inode_updates to {}", session->dev );
    bs_out_auto_close() << 7u << id << dev << known;
}
void TrackerServerUdpConnection::send_req_and_bind_inode_data( Inode id, DevId dev ) {
    DISP_DEBUG( "send req_and_bind_inode_data to {}", session->dev );
    bs_out_auto_close() << 8u << id << dev;
}
void TrackerServerUdpConnection::send_webrtc_offer( DevId dev, const CbString &sdp ) {
    DISP_DEBUG( "send webrtc_offer to {}", session->dev );
    bs_out_auto_close() << 9u << dev << sdp;
}
void TrackerServerUdpConnection::send_webrtc_answer( DevId dev, const CbString &sdp ) {
    DISP_DEBUG( "send webrtc_answer to {}", session->dev );
    bs_out_auto_close() << 10u << dev << sdp;
}
void TrackerServerUdpConnection::send_ice_candidate( DevId dev, PI8 initiator, const CbString &sdp ) {
    DISP_DEBUG( "send ice_candidate to {}", session->dev );
    bs_out_auto_close() << 11u << dev << initiator << sdp;
}
void TrackerServerUdpConnection::send_ans_create_user( const CbString &res ) {
    DISP_DEBUG( "send ans_create_user to {}", session->dev );
    bs_out_auto_close() << 12u << res;
}

void TrackerServerUdpConnection::recv_error_msg( CmString msg ) {
    DISP_INFO( "Error message from connection {}: {}", inet_address, msg );
}

void TrackerServerUdpConnection::recv_corrupted_data() {
    DISP_INFO( "Connection {} says that we have sent corrupted data", inet_address );
}

void TrackerServerUdpConnection::recv_login( CmString user, CmString pass ) {
    tracker()->login( session, user, pass );
    send_ans_login( dev_id, usr_id, session->token );
}

void TrackerServerUdpConnection::recv_bind_obj( Inode obj, const VectorClock &known ) {
    // tracker()->bind_obj( session, obj, known );
    TODO;
}

void TrackerServerUdpConnection::recv_get_dev_info( DevId dev ) {
    TODO;
}

void TrackerServerUdpConnection::recv_ice_candidate( PI8 initiator, CmString data ) {
    TODO;
}

void TrackerServerUdpConnection::recv_webrtc_offer( DevId destination, CmString data ) {
    TODO;
}

void TrackerServerUdpConnection::recv_webrtc_answer( DevId destination, CmString data ) {
    TODO;
}

void TrackerServerUdpConnection::parse_bs( BinStream<CmString> bc ) {
    session->activate();

    while ( not bc.empty() ) {
        // CmString ws_ptr = *bs.buf;
        // BinStream<CmString> bs( &ws_ptr );
        // PI8 cmd = bs.read_byte();
        // switch( cmd ) {
        //     % for cmd in tracker_server_cmds:
        //     case 12: { // 
        //         DISP_DEBUG( "recv ans_create_user from {}", session->dev );
        //         % for arg in cmd.args:
        //         CbString res = bs.read();
        //         % endfor
        //         if ( not this->check_checksum( websocket_data, websocket_data.size() - ws_ptr.size(), "ans_create_user" ) )
        //             return false;
        //         recv_ans_create_user( std::move( res ) );
        //         break;
        //     }
        //     % endfor
        //     default:
        //         TODO;
        // }
        TODO;
    }

    // while ( not bs.empty() ) {
    //     PI8 cmd = bs.read_byte();
    //     switch( cmd ) {
    //         #define DECL_CMD( CMD, NUM, ... ) case NUM: if ( not this->parse_cmd( bs, (&Unref<decltype(*this)>::T::recv_##CMD), #CMD ) ) return; break;
    //         #include "TrackerServerCommandsDecl.h"
    //         #undef DECL_CMD
    //     default:
    //         TODO;
    //     }
    // }
}

Tracker *TrackerServerUdpConnection::tracker() {
    return static_cast<TrackerServerUdpSocket *>( udp_socket )->tracker;
}


} // namespace RiotBase
