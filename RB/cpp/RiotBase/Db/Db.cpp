#include "Core/DbTrackerClient.h"
// #include "Core/DbConnection.h"
#include "Core/DbPostFunc.h"
#include "Core/DbItemPtr.h"
#include "Core/DbItem.h"
#include "Db.h"

#include "../Variable/ObjectId/ObjectId_Inode.h"
#include "../Variable/Core/Error.h"
#include "../Variable/ObjectId.h"
#include "../Variable/Map.h"

#include "../System/Date.h"
#include "../System/Log.h"

#include "../EvLoop/Gev.h"

namespace RiotBase {

// global variables
LazyNew<Db> db;

namespace {

/// make a DbItemPtr, after category check
Rp *new_DbItemPtr( DbItem *obj, const Category *category ) {
    return RO_CALL( category, obj->patch_manager.rp ) == category ? (Rp *)DbItemPtr::New( obj ) : (Rp *)Error::New( "Bad category" );
}

}

Db::Db() {
    // waiting_for_an_id      = false;
    // listening_port         = default_listening_port;
    cur_inode_num          = 0;
    tracker_client         = nullptr;
    // tracker_to_try         = 0;

    root_dir               = new DbItem( this, KnownInodes::root_dir, new_OtWrapperMap( Directory::category, String::category, ObjectId::category ) );
    table.insert( inc_ref( root_dir.ptr() ) );

    // events
    *gev << ( db_post_func = new DbPostFunc( this ) );
}

Db::~Db() {
    // for( DbTcpListener *l : TcpListeners ) delete l;
    delete tracker_client;
    delete db_post_func;
}

void Db::connect( const std::string &user, const std::string &pass, const std::string &tracker_addr, unsigned tracker_port ) {
    if ( tracker_client )
        delete tracker_client;
    if ( tracker_addr.empty() )
        IMPORTANT_TODO;

    //    // choice of the tracker refs to use
    //    if ( tracker_list.empty() ) {
    //        DISP_ERROR( "No id server defined (@see db->add_tracker( \"...\" ))" );
    //        return;
    //    }
    //    std::pair<std::string,unsigned> td = tracker_list[ tracker_to_try++ % tracker_list.size() ];

    // new session (which leads to a retry, or a set_dev_id)
    tracker_client = new DbTrackerClient( this, tracker_addr, tracker_port );
    tracker_client->send_credentials( user, pass );
    tracker_client->flush();
}

int Db::run( double timeout ) {
    return gev->run( timeout );
}

DevId Db::get_dev_id() const {
    return tracker_client->dev_id();
}

UsrId Db::get_usr_id() const {
    return tracker_client->usr_id();
}

//DbSession *Db::connect( const std::string &url ) {
//    req_dev_id_if_needed();

//    // make a client socket
//    int fd;
//    Url d{ url.c_str() };
//    for( double nb_retry = 0, wait = 0.1; ; ++nb_retry, wait *= 1.414 ) {
//        if ( nb_retry == 10 ) {
//            perror( "Pb with connection to socket" );
//            return nullptr;
//        }
//        fd = client_socket( d.host(), d.port() );
//        if ( fd >= 0 )
//            break;
//        Date::sleep( wait );
//    }

//    // make a session, with the corresponding connection
//    DbSession *s = new DbSession( this, false );
//    sessions << s;

//    DbConnection *connection = new DbConnection( fd, s );
//    s->set_connection( connection );
//    *gev << connection;

//    // launch a request to get the dev_id of the corresponding instance
//    s->send_req_identity();
//    s->flush();

//    return s;
//}

void Db::send_graph_cmd( DevId location, CbStringPtr dat ) {
//    for( RcPtr<DbSession> &s : sessions ) {
//        if ( s->dev_id() == location ) {
//            s->send_graph_cmd( dat );
//            return;
//        }
//    }
    //
    TODO; // wait ? Make a new session ?
}

void Db::send_graph_ans( DevId location, PI64 num, CbStringPtr dat ) {
//    for( RcPtr<DbSession> &s : sessions ) {
//        if ( s->dev_id() == location ) {
//            s->send_graph_ans( num, dat );
//            s->flush();
//            return;
//        }
//    }
    //
    TODO; // maybe wait ? Make a new session ?
}

//Rp *Db::_bind_by_path( DaSi path, const Category *cat ) {
//    while ( path.size and path.front() == '/' )
//        --path.size, ++path.data;
//    while ( path.size and path.back() == '/' )
//        --path.size;

//    // only the root ?
//    if ( not path.size )
//        return DbItemPtr::New( root_dir->inc_ref() );

//    // base dir (object behind the last '/')
//    Rp *rp_base_dir;
//    PT s = path.rfind( '/' );
//    if ( s == path.size ) {
//        rp_base_dir = DbItemPtr::New( root_dir );
//    } else {
//        String sub( path.substr( s ) );
//        rp_base_dir = RO_CALL( bind_by_path, sub.rp, Directory::category );
//    }

//    // item
//    String file( s == path.size ? path : path.substr( s + 1, path.size ) );
//    Directory base_dir( rp_base_dir );
//    ObjectId inode = RO_CALL( mod_select, base_dir.rp, file.rp );
//    return RO_CALL( bind_by_object_id, inode.rp, cat );
//}

//Rp *Db::_bind_by_inode( Inode inode, const Category *cat ) {
//    DISP_INFO( "create inode {} with cat {}", inode, cat->name() );

//    // already in the table ?
//    if ( DbItem *res = table.find( inode ) )
//        return new_DbItemPtr( res, cat );
//    return DbItemPtr::New( table.insert( new DbItem( this, inode, cat->create() ) ) );
//}

//Rp *Db::_make_new_object( Rp *&object_id, const Category *cat ) {
//    Inode inode = new_loc_inode();
//    RO_CALL( self_set_o, object_id, ObjectId_Inode::New( inode ) );
//    return DbItemPtr::New( table.insert( new DbItem( this, inode, cat->create() ) ) );
//}

//Rp *Db::_bind_by_kn_object_id( Rp *&object_id, const Category *cat ) {
//    // get inode value
//    Inode inode;
//    if ( not RO_CALL( to_Inode, object_id, &inode, 1 ) )
//        return Error::New( "Pb with inode conversion" );
//    // new item in the directory ?
//    return inode ? _bind_by_inode( inode, cat ) : _make_new_object( object_id, cat );
//}


//void Db::req_dev_id_if_needed() {
//    TODO;
//    if ( dev_id or waiting_for_an_id )
//        return;

//    // make a session to a Tracker (the first to try)
//    if ( not tracker_client ) {
//        if ( tracker_list.empty() ) {
//            DISP_ERROR( "No id server defined (@see db->add_tracker( \"...\" ))" );
//            return;
//        }
//        std::pair<std::string,unsigned> td = tracker_list[ tracker_to_try++ % tracker_list.size() ];
//        tracker_session = new DbTrackerClientSession( td.first, td.second );
//    }

//    // send a request for a dev_id
//    waiting_for_an_id = true;

//    tracker_client->send_credentials( [ this ]( DevId dev, UsrId usr ) {
//        if ( not dev ) {
//            DISP_ERROR( "Bad credentials" );
//            TODO; // wrong credentials
//        }
//        set_dev_id( dev, usr );
//    }, user, pass );

//    tracker_session->flush();
//}

//void Db::set_dev_id( DevId dev, UsrId usr ) {
//    waiting_for_an_id = false;
//    dev_id = dev;
//    usr_id = usr;

//    for( const std::function<void(void)> &f : todo_when_id_known ) f();
//    todo_when_id_known.clear();
//}

Inode Db::new_loc_inode() {
    return { 0, ++cur_inode_num };
}

//DbSession *Db::find_session( DevId glo_ext_dev ) {
//    // we already have this session ?
//    for( RcPtr<DbSession> &s : sessions )
//        if ( s->glo_dev_id == glo_ext_dev )
//            return s.ptr();
//    // else, make a new one
//    DbSession *s = new DbSession( this, false );
//    sessions << s;
//    return s;
//}

//void Db::req_patch_data( Inode inode, DevId glo_ext_dev, const VectorClock &known ) {
//    // find item. We keep it in memory.
//    DbItem *item = table.find( inode );
//    if ( not item )
//        IMPORTANT_TODO; // send message to the tracker

//    // ask for patches that are not known by this
//    if ( SetOfPatchId not_know_by_this = SetOfPatchId( known ) - item->patch_manager.patch_ids ) {
//        //IMPORTANT_TODO; // ask for patch content
//        DISP_ERROR( "IMPORTANT_TODO: ext_dev has data we do no know {}", not_know_by_this );
//    }

//    // pb: on veut que deux trucs soient

//    // send missing patch content
//    item->patch_manager.ext_states[ glo_ext_dev.loc_dev( dev_id ) ].known += SetOfPatchId( known );
//    item->patch_manager.ext_states[ glo_ext_dev.loc_dev( dev_id ) ].sent  += SetOfPatchId( known );
//    if ( AutoPtr<OtMessage> otm = item->patch_manager.get_changes( glo_ext_dev.loc_dev( dev_id ), dev_id, usr_id ) ) {
//        DbSession *s = find_session( glo_ext_dev );
//        PRINT( glo_ext_dev );
//        PRINT( s->dev_id() );
//        s->send_patch_data( item->inode, std::move( otm->msg ) );
//        PRINT( s->dev_id() );
//        s->flush();
//    }
//}

//namespace {

//// bind_by_path
//RiotBase_DEF_FUNC_1( bind_by_path, 1, tv0->has_surdef_for( RF_apply_DaSi() ), const Category *cat ) {
//    Rp *res = 0;
//    RO_CALL( apply_DaSi, v0, [ &res, cat ]( DaSi dasi ) {
//        res = db->_bind_by_path( dasi, cat );
//    } );
//    return res;
//}

//// bind_by_object_id for non symbolic and for symbolic values
//RiotBase_DEF_FUNC_1( bind_by_object_id, 1, tv0->term_value(), const Category *cat ) {
//    return db->_bind_by_kn_object_id( v0, cat );
//}
//RiotBase_DEF_FUNC_1( bind_by_object_id, 2, tv0->unknown_value(), const Category *cat ) {
//    RO_CALL( self_make_inode_if_z, v0, cat );
//    return RO_CALL( bind_by_kn_object_id_b, v0, cat );
//}

//// bind_by_kn_object_id_b
//RiotBase_DEF_FUNC_1( bind_by_kn_object_id_b, 1, tv0->known_value(), const Category *cat ) {
//    Inode inode;
//    if ( not RO_CALL( to_Inode, v0, &inode, 1 ) )
//        return Error::New( "Pb with inode conversion" );
//    return db->_bind_by_inode( inode, cat );
//}

//// self_make_inode_if_z
//RiotBase_DEF_FUNC_1( self_make_inode_if_z, 2, tv0->known_value(), const Category *cat ) {
//    Inode inode;
//    if ( RO_CALL( to_Inode, v0, &inode, 1 ) and not inode )
//        RO_CALL( del, db->_make_new_object( v0, cat ) );
//}

//}

} // namespace RiotBase
