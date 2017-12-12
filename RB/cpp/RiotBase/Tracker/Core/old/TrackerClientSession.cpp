#include "../System/ValidityHook.h"
#include "../System/HashedCbQueue.h"
#include "../System/Log.h"

#include "../EvLoop/Util/Socket.h"
#include "../EvLoop/Util/Url.h"
#include "../EvLoop/Gev.h"

#include "Core/TrackerClientConnection.h"
#include "Core/TrackerCommands.h"
#include "TrackerClientSession.h"

#define BEG_CMD_RAII( BQ, CMD ) \
    HashedCbQueue<CbQueue,XxHash,true> BQ##_hash_buf( cb_queue() ); \
    BinStream<HashedCbQueue<CbQueue,XxHash,true>> BQ( &BQ##_hash_buf ); \
    DISP_DEBUG( "Sending {} to tracker", #CMD ); \
    BQ << PI8( TrackerCommands::CMD )

namespace RiotBase {

TrackerClientSession::TrackerClientSession( std::string addr, unsigned port ) : connection( 0 ), addr( addr ), port( port ) {
    in_a_connection_retry = false;
}

TrackerClientSession::~TrackerClientSession() {
    delete connection;
}

void TrackerClientSession::send_credentials( std::string user, std::string pass, std::function<void(DevId,UsrId)> &&f ) {
    BEG_CMD_RAII( bq, login ) << user << pass;
    on_identification( std::move( f ) );
}

void TrackerClientSession::send_get_dev_info( DevId glo_ext_dev, std::function<void(std::string, unsigned)> &&f ) {
    BEG_CMD_RAII( bq, get_dev_info ) << glo_ext_dev;
    _on_dev_info[ glo_ext_dev ] << std::move( f );
}

void TrackerClientSession::send_bind_obj( Inode inode, const VectorClock &known ) {
    on_identification( [ this, inode, known ]( DevId, UsrId ) {
        BEG_CMD_RAII( bq, bind_obj ) << inode.glo_dev( dev );
        known.write_to( bq, dev );
    } );
}

void TrackerClientSession::send_quit_cmd() {
    on_identification( [ this ]( DevId, UsrId ) {
        BEG_CMD_RAII( bq, quit );
    } );
}

void TrackerClientSession::on_connection_problem( std::function<void()> &&f ) {
    _on_connection_problem << std::move( f );
}

void TrackerClientSession::on_identification( std::function<void(DevId, UsrId)> &&f ) {
    if ( dev and usr )
        f( dev, usr );
    else
        _on_identification << std::move( f );
}

void TrackerClientSession::flush() {
    if ( connect_if_not_already_done() )
        connection->flush();
}


bool TrackerClientSession::connect_if_not_already_done( int num_retry ) {
    if ( connection )
        return true;
    if ( in_a_connection_retry and num_retry == nb_retry_max )
        return false;

    int fd = client_socket( addr.c_str(), to_string( port ).c_str(), true, false );
    if ( fd < 0 ) {
        if ( num_retry == 0 ) {
            DISP_ERROR( "Impossible to connect to tracker" );
            for( const std::function<void()> &f : _on_connection_problem ) f();
            _on_connection_problem.clear();
            return false;
        }
        // wait a bit and retry to connect
        in_a_connection_retry = true;
        gev->add_timer( [ this, num_retry, valid = valid_if_no_destruction_of( this ) ]( int ) {
            if ( valid )
                connect_if_not_already_done( num_retry - 1 );
            return false;
        }, 1.0 / num_retry );
        return false;
    }
    in_a_connection_retry = false;

    connection = new TrackerClientConnection( fd, this );
    *gev << connection;

    if ( not to_be_sent.empty() ) {
        connection->cb_queue()->write_some( std::move( to_be_sent ) );
        connection->flush();
    }

    if ( dev and usr and _on_identification.size() ) {
        for( const std::function<void(DevId,UsrId)> &f : _on_identification ) f( dev, usr );
        _on_identification.clear();
        connection->flush();
    }
    return true;
}


void TrackerClientSession::_send_inode_data( Inode inode, std::string addr, unsigned port, const VectorClock &known ) {
    PRINTL( "send", inode, addr, port, known );
}

void TrackerClientSession::recv_ans_login( DevId dev, UsrId usr, const CbString &addr, unsigned port ) {
    DISP_DEBUG( "recv_ans_login( {}, {}, {}, {} )", dev, usr, std::string( addr ), port );
    if ( not dev or not usr ) DISP_INFO( "Tracker says: bad credentials" );
    this->dev = dev;
    this->usr = usr;

    if ( _on_identification.size() ) {
        for( const std::function<void(DevId,UsrId)> &f : _on_identification ) f( dev, usr );
        _on_identification.clear();
        flush();
    }
}

void TrackerClientSession::recv_corrupted_data( PI64 code ) {
    DISP_INFO( "Recv corrupted data (code={}) from tracker", code );
}

void TrackerClientSession::recv_new_inode_connection( Inode inode, DevId dev, const VectorClock &known ) {
    inode = inode.loc_dev( this->dev );
    DISP_INFO( "recv_new_inode_connection( {}, {}, {} )", inode, dev, known );
}

void TrackerClientSession::recv_req_patch_data( Inode inode, DevId dev, const VectorClock &known ) {
    inode = inode.loc_dev( this->dev );
    DISP_INFO( "recv_req_patch_data( {}, {}, {} )", inode, dev, known );
}

void TrackerClientSession::recv_ans_dev_info( DevId dev, const CbString &addr, unsigned port ) {
    DISP_INFO( "recv_ans_dev_info( {}, {}, {} )", dev, addr, port );
    auto iter = _on_dev_info.find( dev );
    if ( iter != _on_dev_info.end() ) {
        for( const std::function<void( std::string addr, unsigned port )> &f : iter->second )
            f( addr, port );
        _on_dev_info.erase( iter );
    }
}

CbQueue *TrackerClientSession::cb_queue() {
    return connection ? connection->cb_queue() : &to_be_sent;
}

} // namespace RiotBase
