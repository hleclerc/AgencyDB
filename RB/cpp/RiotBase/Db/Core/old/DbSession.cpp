#include "../../Variable/Core/Deserialize.h"
#include "../../Variable/Symbol/ExecList.h"

#include "../../System/HashedCbQueue.h"
#include "../../System/Date.h"
#include "../../System/Log.h"
#include "../../System/Evp.h"

#include "../../EvLoop/Util/Socket.h"
#include "../../EvLoop/Gev.h"

#include "DbConnection.h"
#include "DbCommands.h"

#include "../Db.h"

#include <openssl/rand.h>

namespace RiotBase {

#define BEG_CMD_RAII( BQ, CMD ) \
    HashedCbQueue<CbQueue,XxHash,true> BQ##_hash_buf( cb_queue() ); \
    BinStream<HashedCbQueue<CbQueue,XxHash,true>> BQ( &BQ##_hash_buf ); \
    DISP_DEBUG( "Sending {}", #CMD ); \
    BQ << PI8( DbCommands::CMD )

//#define END_CMD( BQ )
//    hash_digest = BQ##_hash_buf.hash.digest();
//    BQ##_hash_buf.buf->write_some( &hash_digest, sizeof( hash_digest ) );
//    flush()

DbSession::DbSession( Db *db, std::string addr, unsigned port, int fd ) : db( db ), cpt_use( 0 ), server_mode( false ), connection( nullptr ), addr( addr ), port( port ) {
    connection = new DbConnection( fd, this );
}

DbSession::DbSession( Db *db, int fd ) : db( db ), cpt_use( 0 ), server_mode( true ), port( 0 ) {
    connection = new DbConnection( fd, this );
}

DbSession::~DbSession() {
    delete connection;
}


void DbSession::send_req_identity() {
    TODO;
    //    if ( not connection )
    //        return when_connection_up( [ this ](){ send_req_identity(); } );

    //    struct MyGenerator {
    //        MyGenerator() { if ( RAND_load_file( "/dev/urandom", 128 ) != 128 ) ERROR( "Pb seeding..." ); }
    //        void get( unsigned char *buf, int len ) { if ( not RAND_bytes( buf, len ) ) ERROR( "Pb rand..." ); }
    //    };

    //    // make a random string that will be encrypted for db->usr and it's parents (because of a important probability that we have one of these in the other side)
    //    // i.e. we're offering the other side the possibility to make a quick answer if cur_usr or a parent
    //    static MyGenerator mg;
    //    ran.resize( 32 );
    //    mg.get( (unsigned char *)&ran[ 0 ], ran.size() );

    //    // request for information from the distant instance
    //    BEG_CMD_RAII( bq, req_identity );
    //    TODO;
    //    //bq.write_unsigned( Db::public_keys_by_usr_id.size() ); // -> encrypted version of the random string using the known public keys (TODO: use a selection)
    //    //for( std::pair<const UsrId &,const RsaKey &> p : Db::public_keys_by_usr_id )
    //    //    bq << p.first << p.second.encrypt( ran );
}

void DbSession::send_ans_identity( Vec<std::pair<UsrId, CbString> > &keys ) {
    //    // anonymous
    //    if ( db->usr_id == KnownInodes::anon_usr ) {
    //        BEG_CMD_RAII( bq, ans_identity ) << 0u;
    //        return;
    //    }

    //    // encoded by a public key which private counterpart is known ?
    //    if ( db->private_key ) {
    //        for( const std::pair<UsrId,CbString> &p : keys ) {
    //            if ( p.first.glo_dev( db->dev_id, db->usr_id ) == db->usr_id ) { //
    //                // content that will be encrypted
    //                CbQueue cc; BinStream<CbQueue> bc( &cc );
    //                bc << 123456789u << db->dev_id << db->usr_id;

    //                // encrypted message
    //                std::string dec = db->private_key.decrypt( p.second );
    //                Evp sym_key( dec.data() );
    //                std::string ans = sym_key.encrypt( cc );

    //                // command
    //                BEG_CMD_RAII( bq, ans_identity ) << ans;
    //                return;
    //            }
    //        }
    //        TODO;
    //    }

    //
    TODO;
}

void DbSession::send_corrupted_data( PI8 code ) {
    BEG_CMD_RAII( bq, corrupted_data ) << code;
}

void DbSession::send_graph_cmd( CbStringPtr cq ) {
    BEG_CMD_RAII( bq, graph_cmd ) << cq;
}

void DbSession::send_graph_ans( PI64 num, CbStringPtr cq ) {
    BEG_CMD_RAII( bq, graph_ans ) << num << cq;
}

void DbSession::send_patch_data( Inode loc_inode, CbQueue &&msg ) {
    BEG_CMD_RAII( bq, new_changes ) << loc_inode.glo_dev( db->dev_id ) << std::move( msg );
}

void DbSession::send_quit() {
    BEG_CMD_RAII( bq, quit );

}


void DbSession::set_connection( DbConnection *c ) {
    connection = c;

    if ( not tmp_cb_queue.empty() ) {
        connection->cb_queue()->write_some( std::move( tmp_cb_queue ) );
        connection->flush();
    }
}

void DbSession::recv_req_identity( Vec<std::pair<UsrId, CbString> > &keys ) {
    send_ans_identity( keys );

    // send a request ?
    if ( as_a_server )
        send_req_identity();
}

void DbSession::recv_ans_identity( const std::string &msg ) {
    if ( msg.size() ) {
        Evp evp( ran.data() );
        std::string dec = evp.decrypt( msg );
        CmString cm( &dec[ 0 ], &dec[ 0 ] + dec.size() );
        BinStream<CmString> bm( &cm );
        PI64  checkval = bm.read_unsigned();
        DevId dist_dev = bm.read();
        UsrId dist_usr = bm.read();

        if ( bm.error() == false and checkval == 123456789u and dist_dev and dist_usr ) {
            BEG_CMD_RAII( bq, ack_identity ) << 1u;
            set_dev_and_usr( dist_dev, dist_usr );
        } else {
            BEG_CMD_RAII( bq, ack_identity ) << 0u;
        }
    } else {
        TODO;
    }

    reg_changed();
}

void DbSession::recv_ack_identity( PI32 res ) {
    // PRINT( res );
}

void DbSession::recv_graph_cmd( CbStringPtr msg ) {
    Vec<Rp *> outputs;
    Vec<RpAndUse> objects;
    while ( not msg.empty() ) {
        Rp *n = deserialize( &msg, objects );
        if ( msg.error() ) {
            for( Rp *o : outputs )
                RO_CALL( del, o );
            return;
        }
        objects << n;
        outputs << n;
    }
    *exec_list << outputs;
}

void DbSession::recv_graph_ans( PI64 num, CbStringPtr msg ) {
    exec_list->graph_ans( glo_dev_id, num, msg );
}

void DbSession::recv_quit() {
    gev->stop();
}

void DbSession::try_to_connect_if_not_done() {
    if ( connection )
        return;
    if ( server_mode )
        IMPORTANT_TODO;
    //
    int fd = client_socket( addr.c_str() );

//    int fd = client_socket( addr.c_str(), to_string( port ).c_str(), true, false );
//    if ( fd < 0 ) {
//        if ( num_retry == 0 ) {
//            DISP_ERROR( "Impossible to connect to tracker" );
//            for( const std::function<void()> &f : _on_connection_problem ) f();
//            _on_connection_problem.clear();
//            return false;
//        }
//        // wait a bit and retry to connect
//        in_a_connection_retry = true;
//        gev->add_timer( [ this, num_retry, valid = valid_if_no_destruction_of( this ) ]( int ) {
//            if ( valid )
//                connect_if_not_already_done( num_retry - 1 );
//            return false;
//        }, 1.0 / num_retry );
//        return false;
//    }

    connection = new DbConnection( fd, this );

}

void DbSession::wake_up_if_necessary() {
}

CbQueue *DbSession::cb_queue() {
    return connection ? connection->cb_queue() : &tmp_cb_queue;
}

void DbSession::reg_changed() {
}

void DbSession::set_dev_and_usr( DevId dev, UsrId usr ) {
    glo_dev_id = dev;
    glo_usr_id = usr;

    for( const std::function<void(void)> &f : todo_when_dev_id_known ) f();
    for( const std::function<void(void)> &f : todo_when_usr_id_known ) f();
    todo_when_dev_id_known.clear();
    todo_when_usr_id_known.clear();
}

void DbSession::flush() {
    if ( connection )
        connection->flush();

    //    // else, we have to start a new one.
    //    tracker_client_session->send_get_dev_info( glo_ext_dev, [ this, f ]( std::string addr, unsigned port ) {
    //        PRINTL( addr, port );
    //        int fd = client_socket( addr.c_str(), to_string( port ).c_str() );
    //        PRINT( fd );
    //        if ( fd < 0 ) {
    //            IMPORTANT_TODO;
    //        }

    //        // start a new session with this socket
    //        DbSession *s = new DbSession( this, false );
    //        sessions << s;

    //        DbConnection *c = new DbConnection( fd, s );
    //        s->set_connection( c );
    //        *gev << c;
    //        TODO;
    //    } );
}

} // namespace RiotBase
