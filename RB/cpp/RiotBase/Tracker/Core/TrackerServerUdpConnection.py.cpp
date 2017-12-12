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

% for cmd in tracker_client_cmds:
void TrackerServerUdpConnection::send_${ cmd.name }( ${ string.join( [ cmd.cpp_arg( arg[ 0 ] ) + arg[ 1 ] for arg in cmd.args ], ', ' ) } ) {
    DISP_DEBUG( "send ${ cmd.name } to {}", session->dev );
    bs_out_auto_close() << ${ cmd.num }u\
    % for arg in cmd.args:
 << ${ arg[ 1 ] }\
    % endfor
;
}
% endfor

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
        //     case ${ cmd.num }: { // ${ cmd.comment }
        //         DISP_DEBUG( "recv ${ cmd.name } from {}", session->dev );
        //         % for arg in cmd.args:
        //         ${ cmd.cpp_equ( arg[ 0 ] ) } ${ arg[ 1 ] } = bs.read();
        //         % endfor
        //         if ( not this->check_checksum( websocket_data, websocket_data.size() - ws_ptr.size(), "${ cmd.name }" ) )
        //             return false;
        //         recv_${ cmd.name }( ${ string.join( [ "std::move( " + arg[ 1 ] + " )" for arg in cmd.args ], ', ' ) } );
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
