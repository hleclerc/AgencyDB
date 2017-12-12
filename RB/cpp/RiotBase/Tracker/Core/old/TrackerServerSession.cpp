#include "TrackerServerSession.h"
#include "../Tracker.h"

//#include "../../System/VectorClock.h"
//#include "../../System/HashedCbQueue.h"
//#include "../../System/CbString.h"
//#include "../../System/Log.h"

//#include "TrackerConnection.h"
//#include "TrackerCommands.h"
//#include "TrackerSession.h"
//#include "TrackerErrors.h"

//#include <leveldb/db.h>
//#include <json/json.h>

//#define BEG_CMD_RAII( BQ, CMD )
//    HashedCbQueue<CbQueue,XxHash,true> BQ##_hash_buf( connections.front()->cb_queue() );
//    BinStream<HashedCbQueue<CbQueue,XxHash,true>> BQ( &BQ##_hash_buf );
//    DISP_DEBUG( "Sending {}", #CMD );
//    BQ << PI8( TrackerCommands::CMD )

namespace RiotBase {

TrackerServerSession::TrackerServerSession( Tracker *tracker, InetAddress inet_address, UdpSocket *udp_socket, SSL_CTX *ctx) : UdpDtlsConnection( inet_address, udp_socket, ctx, true ), tracker( tracker ) {
}

TrackerServerSession::~TrackerServerSession() {
    if ( tracker ) {
        if ( dev )
            tracker->devid_table.erase( dev );
    }
}

bool TrackerServerSession::parse( Buffer *buff ) {
    PRINT( buff->size );
    return true;
}

//void TrackerSession::add_connection( TrackerConnection *connection ) {
//    connections << connection;

//    for( const std::function<void()> &f : todo_when_connected )
//        f();
//    todo_when_connected.clear();
//}

//void TrackerSession::send_corrupted_data( PI64 code ) {
//    if ( connections.empty() ) {
//        todo_when_connected << [ code, this ]() { send_corrupted_data( code ); };
//        return;
//    }
//    BEG_CMD_RAII( bq, corrupted_data ) << code;
//}

//void TrackerSession::send_new_inode_connection( Inode inode, DevId dev, const VectorClock &known ) {
//    BEG_CMD_RAII( bq, new_inode_connection ) << inode;
//    known.write_to( bq, dev );
//    bq << dev;
//}

//void TrackerSession::send_req_patch_data( Inode inode, DevId dev, const VectorClock &known ) {
//    BEG_CMD_RAII( bq, req_patch_data ) << inode;
//    known.write_to( bq, dev );
//    bq << dev;
//}

//void TrackerSession::flush() {
//    for( TrackerConnection *c : connections )
//        c->flush();
//}

//void TrackerSession::recv_login( const CbString &user, const CbString &pass ) {
//    tracker->login( dev, usr, user, pass, this );
//    BEG_CMD_RAII( bq, ans_login ) << dev << usr << addr << port;
//}

//void TrackerSession::recv_bind_obj( Inode inode, const VectorClock &known ) {
//    if ( dev ) {
//        tracker->bind_inode_to_dev( inode, this, known );
//    } else {
//        send_error_msg( TrackerErrors::bind_obj_needs_an_auth, inode );
//    }
//}

//void TrackerSession::recv_get_dev_info( DevId dev ) {
//    BEG_CMD_RAII( bq, ans_dev_info ) << dev;
//    if ( const DevInfo *dev_info = tracker->dev_info( dev ) ) {
//        bq << dev_info->addr << dev_info->port;
//    } else {
//        bq << 0u << 0u;
//    }
//}

//void TrackerSession::recv_corrupted_data( PI64 code ) {
//    DISP_INFO( "Recv corrupted data (code={}) from tracker", code );
//}

//void TrackerSession::_send_error_msg( CbQueue &&data ) {
//    BEG_CMD_RAII( bq, error_msg ) << std::move( data );
//}

} // namespace RiotBase
