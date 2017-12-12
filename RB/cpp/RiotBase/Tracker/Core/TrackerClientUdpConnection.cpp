//#include "../../System/VectorClock.h"
#include "TrackerClientUdpConnection.h"
#include "TrackerClientUdpSocket.h"
#include "../TrackerClient.h"

namespace RiotBase {

void TrackerClientUdpConnection::recv_error_msg( CmString msg ) {
    DISP_INFO( "Error message from connection {}: {}", inet_address, msg );
}

void TrackerClientUdpConnection::recv_corrupted_data() {
    DISP_INFO( "Connection {} says that we have sent corrupted data", inet_address );
}

void TrackerClientUdpConnection::recv_ans_login( DevId dev_id, UsrId usr_id ) {
    tracker_client()->set_dev_and_usr_id( dev_id, usr_id );
}

void TrackerClientUdpConnection::recv_bind_obj( Inode inode, DevId dev_id ) {
    tracker_client()->on_bind_obj( inode, dev_id );
}

void TrackerClientUdpConnection::recv_req_inode_data( Inode inode, DevId dev_id, const VectorClock &known ) {
    tracker_client()->on_req_inode_data( inode, dev_id, known );
}

void TrackerClientUdpConnection::parse_bs( BinStream<CmString> bs ) {
    while ( not bs.empty() ) {
        PI8 cmd = bs.read_byte();
        switch( cmd ) {
            #define DECL_CMD( CMD, NUM, ... ) case NUM: if ( not this->parse_cmd( bs, (&Unref<decltype(*this)>::T::recv_##CMD), #CMD ) ) return; break;
            #include "TrackerClientCommandsDecl.h"
            #undef DECL_CMD
        default:
            TODO;
        }
    }
}

TrackerClient *TrackerClientUdpConnection::tracker_client() {
    return static_cast<TrackerClientUdpSocket *>( udp_socket )->tracker_client;
}



//#include "../TrackerClientSession.h"
//#include "TrackerCommands.h"


//TrackerClientConnection::TrackerClientConnection( int fd, TrackerClientSession *session ) : BufferedSocket( fd ), pending_cb_queue( 0 ), inp_cont( 0 ), session( session ) {
//    new ( parsing_context ) ParsingContext;
//}

//TrackerClientConnection::~TrackerClientConnection() {
//    session->connection = 0;
//}

//bool TrackerClientConnection::parse( Buffer *buffer ) {
//    if ( not buffer->used )
//        return true;
//    unsigned char *data = buffer->data, *end = data + buffer->used;

//    // if we were interrupted reading a String
//    if ( pending_cb_queue ) {
//        pending_cb_queue->next = inc_ref( buffer );
//        pending_cb_queue = buffer;
//    }

//    // jump to previous "program counter" if previously stopped
//    if ( inp_cont )
//        goto *inp_cont;

//    //
//    while ( true ) {
//        switch ( *data ) {
//        // auth
//        case PI8( TrackerCommands::ans_login ): {
//            struct PC : ParsingContext { DevId dev; UsrId usr; CbString addr; PT port, shift; };
//            BEG_CMD_PC_READ( ans_login );
//            // we do not use READ_DEV_ID_HASHED or READ_USR_ID_HASHED because we want global dev_id and global usr_id
//            READ_UINT_HASHED( PCP->dev.val, PCP->shift );
//            READ_UINT_HASHED( PCP->usr.num, PCP->shift );
//            if ( PCP->usr.num ) {
//                READ_UINT_HASHED( PCP->usr.dev.val, PCP->shift );
//            }
//            READ_STRING_HASHED( PCP->addr, PCP->port, PCP->shift );
//            READ_UINT_HASHED( PCP->port, PCP->shift );
//            CHK_CMD_PC_READ;
//            session->recv_ans_login( PCP->dev, PCP->usr, PCP->addr, PCP->port );
//            END_CMD_PC_READ;
//            break;
//        }
//        case PI8( TrackerCommands::new_inode_connection ): {
//            struct PC : ParsingContext { Inode inode; VectorClock known; DevId dev; PT size, shift; };
//            BEG_CMD_PC_READ( new_inode_connection );
//            READ_INODE_HASHED( PCP->inode, PCP->shift );
//            READ_VectorClock_HASHED( PCP->known, PCP->dev, PCP->size, PCP->shift );
//            READ_DEV_ID_HASHED( PCP->dev, PCP->shift );
//            CHK_CMD_PC_READ;
//            session->recv_new_inode_connection( PCP->inode, PCP->dev, PCP->known );
//            END_CMD_PC_READ;
//            break;
//        }
//        case PI8( TrackerCommands::req_patch_data ): {
//            struct PC : ParsingContext { Inode inode; VectorClock known; DevId dev; PT size, shift; };
//            BEG_CMD_PC_READ( req_patch_data );
//            READ_INODE_HASHED( PCP->inode, PCP->shift );
//            READ_VectorClock_HASHED( PCP->known, PCP->dev, PCP->size, PCP->shift );
//            READ_DEV_ID_HASHED( PCP->dev, PCP->shift );
//            CHK_CMD_PC_READ;
//            session->recv_req_patch_data( PCP->inode, PCP->dev, PCP->known );
//            END_CMD_PC_READ;
//            break;
//        }
//        case PI8( TrackerCommands::ans_dev_info ): {
//            struct PC : ParsingContext { DevId dev; CbString addr; PT port, shift; };
//            BEG_CMD_PC_READ( ans_dev_info );
//            READ_DEV_ID_HASHED( PCP->dev, PCP->shift );
//            READ_STRING_HASHED( PCP->addr, PCP->port, PCP->shift );
//            READ_UINT_HASHED( PCP->port, PCP->shift );
//            CHK_CMD_PC_READ;
//            session->recv_ans_dev_info( PCP->dev, PCP->addr, PCP->port );
//            END_CMD_PC_READ;
//            break;
//        }
//        case PI8( TrackerCommands::corrupted_data ): {
//            struct PC : ParsingContext { PI64 code; PT shift; };
//            BEG_CMD_PC_READ( corrupted_data );
//            READ_UINT_HASHED( PCP->code, PCP->shift );
//            CHK_CMD_PC_READ;
//            session->recv_corrupted_data( PCP->code );
//            END_CMD_PC_READ;
//            break;
//        }
//        default:
//            DISP_ERROR( "tracker client says: corrupted data" );
//            return false;
//        }


//        READ_DATA;
//    }

//    return true;
//}

//DevId TrackerClientConnection::dev_id() {
//    return session->dev;
//}

//UsrId TrackerClientConnection::usr_id() {
//    return session->usr;
//}

} // namespace RiotBase
