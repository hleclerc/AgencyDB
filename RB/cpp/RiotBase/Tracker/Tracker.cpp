#include "../System/PasswordKeyDerivation.h"
#include "../System/Random.h"
#include "../System/Base64.h"
#include "../System/Rcast.h"
#include "../System/UsrId.h"
#include "../System/Log.h"

#include "Core/TrackerServerTcpConnection.h"
#include "Core/TrackerServerUdpConnection.h"
#include "Core/TrackerServerTcpListener.h"
#include "Core/TrackerServerUdpSocket.h"

#include "../EvLoop/EvLoop.h"
#include "../EvLoop/Timer.h"
#include "../EvLoop/Smtp.h"
#include "../EvLoop/Dns.h"

#include "Tracker.h"

#include <rocksdb/db.h> // git clone https://github.com/facebook/rocksdb.git && make static_lib && sudo make install
#include <json/json.h> // apt install libjsoncpp-dev

#ifdef METIL_COMP_DIRECTIVE
#pragma lib_name rocksdb
#pragma lib_name jsoncpp
#endif // METIL_COMP_DIRECTIVE

namespace RiotBase {


struct Timer_1s : Timer {
    Timer_1s( Tracker *tracker ) : Timer( 1 ), tracker( tracker ) {
    }

    virtual bool timeout( int ) {
        //
        tracker->timed_task_list_1mn_res_1s.exec();

        //
        for( TrackerServerSession *s : tracker->sessions_to_del )
            delete s;
        tracker->sessions_to_del.clear();

        return true;
    }

    Tracker *tracker;
};

struct Timer_1h : Timer {
    Timer_1h( Tracker *tracker ) : Timer( 3600 ), tracker( tracker ) {
    }

    virtual bool timeout( int ) {
        tracker->timed_task_list_15day_res_1hr.exec();
        return true;
    }

    Tracker *tracker;
};

Tracker::Tracker( EvLoop *ev_loop, Dns *dns, unsigned base_id, std::string db_file ) :
        ev_loop( ev_loop ), id_gen( base_id ), dns( dns ), timed_task_list_1mn_res_1s( 1, 60 ), timed_task_list_15day_res_1hr( 3600, 24 * 15 ) {

    // assign a device number
    dev.val          = id_gen.gen();
    nb_created_users = 0;

    // open the password db
    rocksdb::Options options;
    options.create_if_missing = true;
    rocksdb::Status status = rocksdb::DB::Open( options, db_file, &users_db );
    if ( not status.ok() ) {
        DISP_ERROR( "Impossible to open the password database" );
    }

    //    // update root password if asked or necessary
    //    if ( root_pass ) {
    //        std::string tmp;
    //        if ( force_root_pass or passwords->Get( rocksdb::ReadOptions{}, "root", &tmp ).IsNotFound() ) {
    //            Json::Value user_data( Json::objectValue );
    //            std::string salt = PasswordKeyDerivation::salt();
    //            user_data[ "pass"   ] = base64_encode( PasswordKeyDerivation::key( root_pass, salt ) );
    //            user_data[ "salt"   ] = base64_encode( salt );
    //            user_data[ "id_num" ] = Json::UInt64( KnownUsrId::root_usr.num );
    //            user_data[ "id_dev" ] = Json::UInt64( KnownUsrId::root_usr.dev );
    //            passwords->Put( rocksdb::WriteOptions{}, "root", user_data.toStyledString() );
    //        }
    //    }

    // event loop
    *ev_loop << ( timer_1s = new Timer_1s( this ) );
    *ev_loop << ( timer_1h = new Timer_1h( this ) );
}

Tracker::~Tracker() {
    for( TrackerServerTcpListener *s : tcp_listeners ) { s->tracker = nullptr; delete s; }
    for( TrackerServerUdpSocket *s : udp_sockets ) { s->tracker = nullptr; delete s; }
    sessions.clear_and_dispose( []( TrackerServerSession *p ) { delete p; } );
    delete users_db;
    delete timer_1s;
    delete timer_1h;
}

void Tracker::tcp_bind( unsigned port, const char *cert_file, const char *key_file ) {
    TrackerServerTcpListener *tcp_listener = new TrackerServerTcpListener( this, port, cert_file, key_file );
    tcp_listeners << tcp_listener;
    *ev_loop << tcp_listener;
}

void Tracker::udp_bind( unsigned port, const char *cert_file, const char *key_file ) {
    TrackerServerUdpSocket *udp_socket = new TrackerServerUdpSocket( this, port, cert_file, key_file );
    udp_sockets << udp_socket;
    *ev_loop << udp_socket;
}

void Tracker::send_ice_candidate( DevId destination, DevId orig, PI8 initiator, const CbString &data ) {
    auto s = sessions.find( destination, TrackerServerSession::DevIdCmp{} );
    if ( s == sessions.end() )
        TODO;
    if ( not s->apply_on_best_conn( [&]( auto *c ) { c->send_ice_candidate( orig, initiator, data ); c->flush(); } ) )
        TODO;
}

void Tracker::send_webrtc_offer( DevId destination, DevId orig, const CbString &data ) {
    auto s = sessions.find( destination, TrackerServerSession::DevIdCmp{} );
    if ( s == sessions.end() )
        TODO;
    if ( not s->apply_on_best_conn( [&]( auto *c ) { c->send_webrtc_offer( orig, data ); c->flush(); } ) )
        TODO;
}

void Tracker::send_webrtc_answer( DevId destination, DevId orig, const CbString &data ) {
    auto s = sessions.find( destination, TrackerServerSession::DevIdCmp{} );
    if ( s == sessions.end() )
        TODO;
    if ( not s->apply_on_best_conn( [&]( auto *c ) { c->send_webrtc_answer( orig, data ); c->flush(); } ) )
        TODO;
}

void Tracker::login( TrackerServerSession *session, const std::string &user, const std::string &pass, DevId proposition ) {
    bool ok = false;
    if ( pass.size() ) { // user/pass
        std::string user_data_string;
        rocksdb::Status s = users_db->Get( rocksdb::ReadOptions{}, user, &user_data_string );
        if ( s.ok() ) {
            Json::Reader reader;
            Json::Value user_data;
            if ( reader.parse( user_data_string, user_data ) and user_data[ "pass" ] == base64_encode( PasswordKeyDerivation::key( pass, base64_decode( user_data[ "salt" ].asString() ) ) ) ) {
                session->usr.num = user_data[ "id_num" ].asUInt64();
                session->usr.dev = user_data[ "id_dev" ].asUInt64();
                ok = true;
            }
        }
    } else { // token
        std::map<std::string,UsrId>::const_iterator iter = token_to_usr_id.find( user );
        if ( iter != token_to_usr_id.end() ) {
            session->usr = iter->second;
            ok = true;
        } else
            DISP_INFO( "Use of a bad or dated token" );
    }

    if ( ok ) {
        // assign dev and register the session (we can do it now, because we have a dev)
        session->dev = proposition ? proposition : DevId{ id_gen.gen() };
        sessions.insert( *session );

        // make a token, invalid after 24 hours
        do {
            static Random mg;
            session->token.resize( 32 );
            mg.get( &session->token[ 0 ], session->token.size() );
            session->token = base64_encode( session->token );
        } while ( token_to_usr_id.count( session->token ) );
        token_to_usr_id[ session->token ] = session->usr;

        session->task_receiver << timed_task_list_15day_res_1hr.add_oper( [ this, session ]() {
            token_to_usr_id.erase( session->token );
        }, 3600 * 24 );
    }
}

void Tracker::logout( TrackerServerSession *session ) {
    reg_session_to_del( session );
}

void Tracker::bind_obj( TrackerServerSession *session, Inode obj, const VectorClock &known ) {
    if ( not session->dev ) {
        IMPORTANT_TODO; /// msg to say that bind_obj needs authentication
    }

    InodeData &data = inode_table[ obj ];
    if ( data.connected_devs.contains( session->dev ) )
        return;

    // devices connected to obj ?
    Vec<TrackerServerSession *> conn_sess;
    for( PT n = 0; n < data.connected_devs.size(); ++n ) {
        auto iter = sessions.find( data.connected_devs[ n ], TrackerServerSession::DevIdCmp{} );
        if ( iter != sessions.end() )
            conn_sess << &*iter;
        else
            data.connected_devs.remove_unordered( n-- );
    }

    if ( conn_sess.size() ) {
        // answer to session (connection devices)
        for( TrackerServerSession *s : conn_sess )
            session->apply_on_best_conn( [&]( auto *c ) { c->send_bind_obj( obj, s->dev ); } );

        // choose connection for a send_inode_data
        int index_best_session_for_req_inode = -1;
        for( PT n = 0; n < conn_sess.size(); ++n ) {
            if ( conn_sess[ n ]->connected() ) {
                index_best_session_for_req_inode = n;
                break;
            }
        }

        if ( index_best_session_for_req_inode >= 0 ) {
            TrackerServerSession *s = conn_sess[ index_best_session_for_req_inode ];
            conn_sess.remove_unordered( index_best_session_for_req_inode );
            s->apply_on_best_conn( [&]( auto *c ) {
                c->send_req_inode_updates( obj, session->dev, known );
                c->flush();
            } );

            // signal bound object to other conn_sess
            for( TrackerServerSession *s : conn_sess ) {
                if ( not s->apply_on_best_conn( [&]( auto *c ) {
                    c->send_bind_obj( obj, session->dev );
                    c->flush();
                } ) )
                    s->pending_bound_inode << std::make_pair( session->dev, obj );
            }
        }
    }

    // register
    data.connected_devs << session->dev;
}

void Tracker::load_obj( TrackerServerSession *session, Inode obj, bool bind ) {
    if ( not session->dev ) {
        IMPORTANT_TODO; /// msg to say that bind_obj needs authentication
    }

    InodeData &data = inode_table[ obj ];

    // devices connected to obj
    Vec<TrackerServerSession *> conn_sess;
    for( PT n = 0; n < data.connected_devs.size(); ++n ) {
        auto iter = sessions.find( data.connected_devs[ n ], TrackerServerSession::DevIdCmp{} );
        if ( iter != sessions.end() and iter->connected() )
            conn_sess << &*iter;
        else
            data.connected_devs.remove_unordered( n-- );
    }
    if ( not conn_sess.size() )
        IMPORTANT_TODO;

    // choose connection for a send_inode_data
    int index_best_session_for_req_inode = 0;
    TrackerServerSession *s = conn_sess[ index_best_session_for_req_inode ];
    s->apply_on_best_conn( [&]( auto *c ) {
        c->send_req_inode_data( obj, session->dev );
        c->flush();
    } );

    // register
    if ( bind )
        data.connected_devs << session->dev;
}

void Tracker::reg_session_to_del( TrackerServerSession *session ) {
    sessions_to_del.push_back_unique( session );
    sessions.erase( session->dev, TrackerServerSession::DevIdCmp{} ); // do not dispose
}

std::string Tracker::create_user( const std::string &user, const CbString &pass, const CbString &email ) {
    static Random rand;

    // check that user does not exist. If yes, return "Existing " + proposals
    std::string user_data_string;
    rocksdb::Status s = users_db->Get( rocksdb::ReadOptions{}, user, &user_data_string );
    if ( not s.IsNotFound() ) {
        std::string res = "Existing";
        for( PT i = 1, cpt = 0; cpt < 4; ++i ) {
            std::string trial = user + '_' + to_string( i );
            if ( users_db->Get( rocksdb::ReadOptions{}, trial, &user_data_string ).IsNotFound() ) {
                res += " " + trial;
                ++cpt;
            }
        }
        return res;
    }

    // make a token
    std::string tok;
    tok.resize( 32 );
    rand.get( &tok[ 0 ], tok.size() );
    tok = base64_encode( tok );

    // store data to register the user information that will be user during confirmation
    WaitingConfirmation &conf = usr_waiting_confirmation[ tok ];
    conf.user  = user;
    conf.pass  = pass;
    conf.email = email;

    //
    send_mail( email, "Please confirm registration", ".\nClick on https://localhost:8676/register?token=" + tok, []( SmtpStatus status ) { PRINT( int( status ) ); }, ev_loop, dns );
    return {};
}

std::string Tracker::reg_waiting_usr( std::string tok ) {
    DISP_DEBUG( "reg_waiting_usr" );

    std::map<std::string,WaitingConfirmation>::iterator iter = usr_waiting_confirmation.find( tok );
    if ( iter == usr_waiting_confirmation.end() ) {
        DISP_INFO( "There's an attempt to register an user using a wrong token" );
        return "Wrong token";
    }

    // check for existence
    std::string user_data_string;
    WaitingConfirmation &wc = iter->second;
    rocksdb::Status s = users_db->Get( rocksdb::ReadOptions{}, wc.user, &user_data_string );
    if ( s.ok() ) {
        usr_waiting_confirmation.erase( iter );
        return "Already existing user";
    }

    // create the user n the database
    UsrId usr_id( dev, ++nb_created_users );
    Json::Value user_data( Json::objectValue );
    std::string salt = PasswordKeyDerivation::salt();
    user_data[ "pass"   ] = base64_encode( PasswordKeyDerivation::key( wc.pass, salt ) );
    user_data[ "salt"   ] = base64_encode( salt );
    user_data[ "email"  ] = base64_encode( wc.email );
    user_data[ "id_num" ] = Json::UInt64( usr_id.num );
    user_data[ "id_dev" ] = Json::UInt64( usr_id.dev );
    users_db->Put( rocksdb::WriteOptions{}, wc.user, user_data.toStyledString() );

    usr_waiting_confirmation.erase( iter );
    return "Thank you for registering";
}

} // namespace RiotBase
