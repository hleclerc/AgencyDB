#include "../../System/BinStreamWithOffsets.h"
#include "../../System/AutoPtr.h"
#include "../../System/Log.h"
#include "../../System/Evp.h"

#include "../../Variable/Symbol/CommonSymbolicData.h"
#include "../../Variable/Symbol/SetLazyVal.h"
#include "../../Variable/Symbol/Scheduler.h"
#include "../../Variable/Core/Deserializer.h"
#include "../../Variable/Core/OtMessage.h"
#include "../../EvLoop/TcpRawConnection.h"

#include "DbConnection.h"
#include "DbCommands.h"
#include "DbSession.h"
#include "../Gev.h"
#include "../Db.h"

#include <openssl/rand.h>

#define BEGCMD( CMD ) DISP_DEBUG( "Send to {} (from {}): {}", dev_id, db->dev, #CMD ); BinStream<CbQueue> bq( connection->cb_queue() ); bq << DbCommands::CMD


namespace RiotBase {

DbSession::DbSession( Db *db, int fd, bool server ) : DbSession( db, server ) {
    connection = new_db_connection( this, fd, server );
    *gev << connection;
}

DbSession::DbSession( Db *db, bool server ) : dev_id( 0 ), prev( 0 ), db( db ), cpt_use( 0 ), server( server ) {
}

DbSession::~DbSession() {
    delete connection;
}

void DbSession::flush() {
    if ( connection )
        connection->flush();
}

void DbSession::activate() {
}

void DbSession::_reg_connection() {
    // TODO; add a watchdog ?
}

void DbSession::send_req_identity() {
    if ( not connection ) {
        todo_when_connection_up << [ this ](){
            send_req_identity();
        };
        return _reg_connection();
    }

    struct MyGenerator {
        MyGenerator() { if ( RAND_load_file( "/dev/urandom", 128 ) != 128 ) ERROR( "Pb seeding..." ); }
        void get( unsigned char *buf, int len ) { if ( not RAND_bytes( buf, len ) ) ERROR( "Pb rand..." ); }
    };

    // make a random string that will be encrypted for db->usr and it's parents (because of a important probability that we have one of these in the other side)
    // i.e. we're offering the other side the possibility to make a quick answer if cur_usr or a parent
    static MyGenerator mg;
    ran.resize( 32 );
    mg.get( (unsigned char *)&ran[ 0 ], ran.size() );

    // request for information from the distant instance
    BEGCMD( req_identity );
    // send encrypted version of the random string using the known public keys (TODO: use a selection)
    bq.write_unsigned( Db::public_keys_by_usr_id.size() );
    for( std::pair<const UsrId &,const RsaKey &> p : Db::public_keys_by_usr_id )
        bq << p.first << p.second.encrypt( ran );

    flush();
}

void DbSession::recv_req_identity( const Vec<std::pair<UsrId,std::string> > &vec_usr_enc ) {
    // send answer
    send_ans_identity( vec_usr_enc );
    // send identity request
    if ( not dev_id or not usr_id )
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

        if ( not bm.error() and checkval == 123456789u and dist_dev and dist_usr ) {
            BEGCMD( ack_identity ) << 1u;
            _set_dev_and_usr( dist_dev, dist_usr );
        } else {
            BEGCMD( ack_identity ) << 0u;
        }
    } else {
        TODO;
    }

    _reg_changed();
}

void DbSession::recv_ack_identity( unsigned res ) {
    if ( not res )
        WARNING( "identity not registered" );
}



void DbSession::send_req_auth_std( const std::string &login, const std::string &password ) {
    TODO;
}

void DbSession::send_req_auth_tok( const std::string &token ) {
    TODO;
}

void DbSession::send_req_auth_yub( const std::string &login, const std::string &password, const std::string &yubikey ) {
    TODO;
}

void DbSession::send_ans_auth( DevId dev_id, UsrId usr_id ) {
    if ( not connection ) {
        todo_when_connection_up << [ dev_id, usr_id, this ](){
            send_ans_auth( dev_id, usr_id );
        };
        return _reg_connection();
    }

    BEGCMD( ans_auth ) << dev_id;
    if ( dev_id )
        bq << usr_id.glo_dev( db->get_dev_id(), db->get_usr_id() );
    _reg_changed();
}

void DbSession::send_graph_cmd( PI64 dst_id, const Vec<PT> &out_offset, const CbStringPtr &msg ) {
    if ( not connection ) {
        CbString m = msg;
        todo_when_connection_up << [ dst_id, out_offset, m, this ](){
            send_graph_cmd( dst_id, out_offset, m );
        };
        return _reg_connection();
    }

    BEGCMD( graph_cmd ) << dst_id << out_offset << msg;
    _reg_changed();
}

void DbSession::send_res_graph( PI64 dst_id, PT out_num, const CbStringPtr &msg ) {
    BEGCMD( res_graph ) << dst_id << out_num << msg;
}

void DbSession::send_req_content( Inode inode, const Category *category ) {
    if ( not dev_id_ready() ) {
        todo_when_dev_id_known << [ inode, category, this ](){
            send_req_content( inode, category );
        };
        return _reg_connection();
    }

    BEGCMD( req_content ) << inode.glo_dev( db->get_dev_id() );
    _reg_changed();
}

void DbSession::send_req_changes( Inode inode, const Category *category ) {
    TODO;
}

void DbSession::send_new_changes( Inode inode, const CbString &msg ) { // const Vec<DevId> &dst,
    if ( not dev_id_ready() ) {
        todo_when_dev_id_known << [ inode, msg, this ](){
            send_new_changes( inode, msg );
        };
        return _reg_connection();
    }

    BEGCMD( new_changes ) << inode.glo_dev( db->get_dev_id() ) << msg;
    _reg_changed();
}

void DbSession::send_ack_changes( Inode inode ) {
    TODO;
}

void DbSession::recv_req_auth_std( const CbString &login, const CbString &password ) {
    if ( login.find_utf8( []( unsigned c ) { return c == '/'; } ) )
        return send_ans_auth( 0 );
    String val = db->bind( "/home/" + std::string( login ) + "/.user_info/password" );
    inc_ref( this );
    val.then( [ this ]( const String &val ) {
        PRINT( db->bind( "/" ).operator Directory() );
        PRINT( val );
        send_ans_auth( 101, UsrId( 101, 17 ) );
        dec_ref( this );
    } );
}

void DbSession::recv_req_auth_tok( std::string token ) {
    TODO;
}

void DbSession::recv_req_auth_yub( std::string login, std::string password, std::string yubikey ) {
    TODO;
}

void DbSession::recv_ans_auth( DevId dev_id, UsrId usr_id ) {
    TODO;
}

void DbSession::recv_req_content( Inode inode ) {
    TODO; // this function should be deleted

    //    if ( not dev_id_ready() ) {
    //        todo_when_dev_id_known << [ inode, this ](){
    //            recv_req_content( inode );
    //        };
    //        return _reg_connection();
    //    }

    //    DbItem *item = db->table.find( inode );
    //    if ( not item )
    //        TODO;
    //    item->sessions << inc_ref( this );
    //    Rp *rp = item->check_rp();
    //    if ( RO_CALL( is_symbolic, rp ) )
    //        return; // wait for the value (registering in item->sessions shoud do the job)

    //    //
    //    AutoPtr<OtMessage> om = RO_CALL( get_changes_for, item->rp, f, f );
    //    static CbQueue cq;
    //    send_new_changes( inode, om ? om->msg : cq );
    //    _reg_changed();
}

void DbSession::recv_req_changes( DevId dev_id, UsrId usr_id ) {
    TODO;
}

void DbSession::recv_new_changes( Inode inode, CbString &msg ) {
    if ( DbItem *item = db->table.find( inode ) ) {
        Rp *lazyfier = item->check_rp();

        const Category *category = RO_CALL( category, lazyfier );
        item->rp = category->create();

        CbQueue ans;
        IMPORTANT_TODO;
        // RO_CALL( app_changes, item->rp, ans, msg, db->fqid(), dev_id, &db->dev, 1 );

        // make the waiting operations
        set_lazy_val( lazyfier, item->rp );
        item->rp = lazyfier;

        // object is watched by this
        ++cpt_use;
        item->sessions.push_back_unique( this );

        // signal change
        RO_CALL( sig_change, item->rp );
    } else {
        IMPORTANT_TODO;
    }
}

void DbSession::recv_ack_changes( DevId dev_id, UsrId usr_id ) {
    TODO;
}

void DbSession::recv_graph_cmd( PI64 dst_id, const Vec<PT> &num_outputs, CbString &graph ) {
    // make a Rp * graph representation. -> global representation (Deserializer is used in read_from which does not take cur_dev... as parameters)
    Deserializer d;
    d.read( BinStream<CbString>( &graph ), num_outputs );

    // local representation
    TODO;
    //    for( Rp *rp : d.outputs )
    //        RO_CALL( glo_to_loc, rp, db );

    // comment décider s'il faut exécuter dans le thread ou pas ?
    // -> on peut imaginer lancer des exec dans le thread principal, et ce sont les exec eux même qui décident s'il faut retarder
    // -> du coup, on pourrait imaginer lancer des exec (qui font comme set_lazy_val) sur toutes les feuilles, une par une.
    // Pb: ça implique de refaire le graph un grand nombre de fois.

    // send data to scheduler
    auto adder = global_scheduler.raii_adder();
    for( Rp *rp : d.outputs ) {
        PRINTRP( rp );
        adder << rp;
    }
}

void DbSession::send_ans_identity( const Vec<std::pair<UsrId,std::string>> &vec_usr_enc ) {
    _reg_changed();

    // anonymous
    if ( db->usr == KnownInodes::anon_usr ) {
        BEGCMD( ans_identity ) << 0u;
        return;
    }

    // encoded by a public key which private counterpart is known ?
    if ( db->private_key ) {
        for( const std::pair<UsrId,std::string> &p : vec_usr_enc ) {
            if ( p.first.glo_dev( db->dev, db->usr ) == db->usr ) { //
                // content that will be encrypted
                CbQueue cc; BinStream<CbQueue> bc( &cc );
                bc << 123456789u << db->dev << db->usr;

                // encrypted message
                std::string dec = db->private_key.decrypt( p.second );
                Evp sym_key( dec.data() );
                std::string ans = sym_key.encrypt( std::string( cc ) );

                // command
                BEGCMD( ans_identity );
                bq << ans;
                return;
            }
        }
    }

    // look for a server which should have a relevant private key
    TODO;
}


void DbSession::_reg_changed() {
    db->changed_sessions << this;
}


void DbSession::_set_dev_and_usr( DevId id, UsrId usr ) {
    usr_id = usr;
    dev_id = id;

    for( const std::function<void(void)> &f : todo_when_dev_id_known )
        f();
    todo_when_dev_id_known.clear();
}

} // namespace RiotBase
