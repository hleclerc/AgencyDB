#include "Core/DbTcpListener.h"
#include "Core/DbFileReader.h"
#include "Core/DbCommands.h"
#include "Core/DbSession.h"
#include "Core/DbItemPtr.h"

#include "../EvLoop/TcpRawConnection.h"
#include "../EvLoop/Util/Socket.h"

#include "../Variable/ObjectId/ObjectId_Inode.h"

#include "../Variable/Symbol/ActiveLazySources.h"
#include "../Variable/Symbol/SetLazyVal.h"
#include "../Variable/Symbol/SymOpExtr.h"
#include "../Variable/Symbol/Lazyfier.h"
#include "../Variable/Symbol/Symbol.h"

#include "../Variable/Core/AutoRpDel.h"
#include "../Variable/Core/Error.h"

#include "../System/ConfigReader.h"

#include "Gev.h"
#include "Db.h"

#include <boost/filesystem.hpp>
#include <sys/stat.h>
#include <fstream>
#include <fcntl.h>

#ifdef METIL_COMP_DIRECTIVE
#pragma lib_name boost_filesystem
#pragma lib_name boost_system
#endif // METIL_COMP_DIRECTIVE


namespace RiotBase {

class PostFuncDb : public PostFunc {
public:
    PostFuncDb( Db *db ) : db( db ) {
    }
    virtual bool exec() {
        bool ret = false;
        
        //
        ret |= DbItem  ::exec_all();
        ret |= Callback::exec_all();

        //
        if ( not db->changed_sessions.empty() ) {
            db->changed_sessions.clear( []( DbSession *s ) { s->flush(); } );
            ret = true;
        }

        return ret;
    }
    Db *db;
};

//class StorageFuncDb : public PostFunc {
//public:
//    PostFuncDb( Db *db ) : db( db ) {
//    }
//    virtual bool exec() {
//        Callback::exec_all();

//        bool ret = not db->changed_sessions.empty();
//        for( DbSession *s : db->changed_sessions )
//            s->flush();
//        db->changed_sessions.clear();

//        return ret;
//    }
//    Db *db;
//};

std::map<UsrId,RsaKey> Db::public_keys_by_usr_id;

Rp *new_DbItemPtr( DbItem *obj, const Category *category ) {
    return obj->rp == 0 or RO_CALL( category, obj->rp ) == category ? (Rp *)DbItemPtr::New( obj ) : (Rp *)Error::New( "Bad category" );
}


Db::Db( const std::string &login, const std::string &password, DevId dev_id ) : login( login ), password( password ) {
    dev             = dev_id;
    cur_inode_num   = KnownInodes::beg_num_bootstrap_inodes;
    main_file       = -1;
    cur_datafile_id = 0;

    // event loop
    *gev << ( post_func = new PostFuncDb( this ) );

    // password
    if ( password.find( "-----BEGIN RSA PRIVATE KEY-----" ) == 0 )
        private_key = RsaKey( password.c_str(), false );

    // predefined objects
    root_dir = table.insert( new DbItem( Directory::category, KnownInodes::root_dir, this ) );

    //
    if ( login == "root" ) // hum
        usr = KnownUsrId::root_usr;
}

Db::~Db() {
    flush();
    if ( main_file >= 0 )
        close( main_file );

    table.clear();

    for( DataFile *d : data_files )
        delete d;

    for( TcpListener *l : TcpListeners )
        delete l;

    *gev >> post_func;
    delete post_func;
}

int Db::connect( const std::string &ref, const std::string &port ) {
    DbSession *s = new DbSession( this, client_socket( ref.c_str(), port.size() ? port.c_str() : 0 ), false );
    if ( not s->connected() ) {
        delete s;
        return 1;
    }
    root_dir->sessions << inc_ref( s );
    sessions << s;

    s->send_req_identity();

    for( std::function<void()> f : what_to_do_when_connected_sessions )
        f();
    what_to_do_when_connected_sessions.clear();
    return 0;
}

void Db::listen( const std::string &ref, bool is_a_named_socket ) {
    if ( ref.empty() ) {
        WARNING( "Please provide correct ref" );
        return;
    }
    TcpListener *dc = new_DbTcpListener( this, ref, is_a_named_socket ? true : ref[ 0 ] < '0' or ref[ 0 ] > '9' ); // unencrypted connections
    TcpListeners << dc;
    *gev << dc;
}

Db::SetMainDirStatusCode Db::set_main_dir( const std::string &dir ) {
    boost::filesystem::path p( dir );
    if ( not boost::filesystem::is_directory( p ) )
        return Db::SetMainDirStatusCode::not_a_directory;

    // look for an existing main file
    std::string best_file;
    for( boost::filesystem::directory_iterator itr( p ), end; itr != end; ++itr ) {
        std::string n = itr->path().string();
        if ( itr->path().filename().string().find( "main_" ) == 0 and best_file < n ) {
            if ( not boost::filesystem::is_regular( itr->path() ) )
                return Db::SetMainDirStatusCode::contains_a_non_a_regular_file;
            best_file = n;
        }
    }

    // data to read ?
    if ( best_file.size() ) {
        struct DbConfigReader : ConfigReader {
            DbConfigReader( Db *db ) : db( db ), ret( Db::SetMainDirStatusCode::ok ) {
            }
            virtual void onargs( Vec<std::string> &args ) {
                if ( args[ 0 ] == "NewDataFile" and args.size() > 2 ) {
                    int fd = open( args[ 1 ].c_str(), O_RDWR | ( O_DIRECT * DataFile::use_dio ), S_IRUSR | S_IWUSR );
                    if ( fd < 0 )
                        ret = Db::SetMainDirStatusCode::unable_to_open_data_file;
                    else
                        db->data_files << new DataFile{ args[ 1 ], fd, atoi( args[ 2 ].c_str() ) };
                }
            }
            Db *db;
            Db::SetMainDirStatusCode ret;
        };
        std::ifstream r{ best_file };
        DbConfigReader m{ this };
        m.parse( r );
        if ( m.ret != Db::SetMainDirStatusCode::ok )
            return m.ret;

        for( DataFile *d : data_files ) {
            Buffer *inp_buff = Buffer::New();
            DbFileReader dfr( this, d );
            PI64 offset = 0;
            //lseek( d.fd, 0, SEEK_SET );
            while ( true ) {
                // if not possible to reuse the old buffer, create a new one
                if ( inp_buff->cpt_use > 0 ) {
                    dec_ref( inp_buff );
                    inp_buff = Buffer::New();
                }

                inp_buff->used = read( d->fd, inp_buff->data, inp_buff->size );
                if ( int( inp_buff->used ) <= 0 )
                    break;
                dfr.parse( inp_buff, offset );
                offset += inp_buff->used;
            }
            dec_ref( inp_buff );
        }
    } else
        best_file = dir + "/main_0";

    //
    if ( main_file >= 0 )
        close( main_file );
    main_file = open( best_file.c_str(), O_WRONLY | O_APPEND | O_CREAT, S_IRUSR | S_IWUSR );
    if ( main_file < 0 )
        ERROR( "Pb while opening main file: %s", strerror( errno ) );

    main_dir = dir;
    return Db::SetMainDirStatusCode::ok;
}

Db::AddDataFileStatusCode Db::add_data_file( const std::string &file ) {
    // already opened ?
    for( const DataFile *d : data_files )
        if ( d->path == file )
            return Db::AddDataFileStatusCode::ok;

    // else, try to create and open the file
    int fd = open( file.c_str(), O_RDWR | O_EXCL | O_CREAT | ( O_DIRECT * DataFile::use_dio ), S_IRUSR | S_IWUSR );
    if ( fd < 0 ) {
        if ( errno == EEXIST )
            return Db::AddDataFileStatusCode::already_existing_file;
        return Db::AddDataFileStatusCode::unable_to_create_file;
    }

    // add info in the config file
    if ( main_file < 0 ) {
        close( fd );
        return Db::AddDataFileStatusCode::please_define_a_main_file;
    }
    std::string msg = ConfigReader::write( Vec<std::string>{ "NewDataFile", file, to_string( ++cur_datafile_id ) } );
    write( main_file, msg.data(), msg.size() );

    // register the data file
    data_files << new DataFile{ file, fd, cur_datafile_id };
    return Db::AddDataFileStatusCode::ok;
}

void Db::flush() {
    post_func->exec();
}

DevId Db::get_dev_id() const {
    return dev;
}

UsrId Db::get_usr_id() const {
    return usr;
}

FqId Db::fqid() const {
    return { ccast( this ), dev, usr };
}

Vec<UsrId> Db::connected_users() const {
    Vec<UsrId> res;
    for( const RcPtr<DbSession> &s : sessions )
        res << s->get_usr_id();
    return res;
}

int Db::ev_loop() {
    return gev->run();
}

void Db::send_graph_cmd( DevId dev_id, PI64 dst_id, const Vec<PT> &out_offset, const CbStringPtr &msg ) {
    if ( not sessions.size() )
        ERROR( "Weird: send_graph_cmd is normally created when connected somewhere" );
    // if we can find the dev_id in a session, go ahead
    for( PT i = 0; i < sessions.size(); ++i ) {
        DbSession *s = sessions[ i ].ptr();
        if ( s->get_dev_id() == dev_id ) {
            s->send_graph_cmd( dst_id, out_offset, msg );
            return;
        }
    }
    // -> dev_id not found, try in a random connected instance. TODO: select a connected instance with a connection and good capacities
    PT n = rand() % sessions.size();
    sessions[ n ]->send_graph_cmd( dst_id, out_offset, msg );
}

Inode Db::_newlocinode() {
    return { 0, cur_inode_num++ };
}

// Rp *Db::_get_ext_obj( ObjectId key, const Category *expected_category ) {
//     Inode inode;
//     if ( not key.to_Inode( inode ) )
//         return Error::New( "Pb with an inode conversion" );
//     return _get_ext_obj( inode, expected_category );
// }
// 
// Rp *Db::_get_ext_obj( Inode inode, const Category *expected_category ) {
//     // make the call, register the symbol
//     if ( sessions.size() ) {
//         DbSession *s = sessions[ 0 ].ptr();
//         s->req_for_content( inode, expected_category );
//     } else {
//         what_to_do_when_connected_sessions << [ this, inode, expected_category ]{
//             DbSession *s = sessions[ 0 ].ptr();
//             s->req_for_content( inode, expected_category );
//         };
//     }
//     return new_Symbol( "GetExtObject(" + to_string( inode ) + ")", expected_category );
// }

Rp *Db::_bind_by_path( DaSi path, const Category *expected_category ) {
    while ( path.size and path.front() == '/' )
        --path.size, ++path.data;
    while ( path.size and path.back() == '/' )
        --path.size;

    // only the root ?
    if ( not path.size )
        return DbItemPtr::New( root_dir );

    // base dir
    Rp *rp_base_dir;
    PT s = path.rfind( '/' );
    if ( s == path.size )
        rp_base_dir = DbItemPtr::New( root_dir );
    else {
        String sub( path.substr( s ) );
        rp_base_dir = RO_CALL( bind_by_path, sub.rp, this, Directory::category );
    }

    // item
    String file( s == path.size ? path : path.substr( s + 1, path.size ) );
    Directory base_dir( rp_base_dir );
    Ref<ObjectId> inode = RO_CALL( mod_select, base_dir.rp, file.rp );
    return RO_CALL( bind_by_object_id, inode.rp, this, expected_category );
}

Rp *Db::_bind_by_object_id( Rp *&object_id, const Category *expected_category ) {
    // get inode value
    Inode inode;
    if ( not RO_CALL( to_Inode, object_id, &inode, 1 ) )
        return Error::New( "Pb with inode conversion" );

    // new item in the directory ?
    if ( not inode )
        return _make_new_object( object_id, expected_category );

    //
    return _bind_by_inode( inode, expected_category );
}

Rp *Db::_bind_by_inode( Inode inode, const Category *expected_category ) {
    // already in the table ?
    if ( DbItem *res = table.find( inode ) )
        return new_DbItemPtr( res, expected_category );

    // not found -> create an entry in the table, with a symbol and a re
    if ( sessions.empty() )
        TODO;
    sessions[ 0 ]->send_req_content( inode, expected_category );
    TODO; // ::mkl
    //Rp *rp = new_Symbol( "Sym<ExtS," + to_string( inode ) + ">", expected_category );
    Rp *rp = 0;
    return DbItemPtr::New( table.insert( new DbItem( expected_category, inode, this, rp ) ) );
}

Rp *Db::_make_new_object( Rp *&object_id, const Category *expected_category ) {
    Inode inode = _newlocinode();
    RO_CALL( self_set_o, object_id, ObjectId_Inode::New( inode ) );
    return DbItemPtr::New( table.insert( new DbItem( expected_category, inode, this, expected_category->create() ) ) );
}

namespace {

RiotBase_DEF_FUNC_1( bind_by_path, 1, tv0->known_term_value(), Db *db, const Category *cat ) {
    std::string path; if ( not RO_CALL( to_std_string, v0, &path, 1 ) ) return Error::New( "bind_by_path path pb conversion" );
    return db->_bind_by_path( path, cat );
}
RiotBase_DEF_FUNC_1( bind_by_path, 1.5, tv0->has_surdef_for( RF_dasi() ), Db *db, const Category *cat ) {
    return db->_bind_by_path( RO_CALL( dasi, v0 ), cat );
}

RiotBase_DEF_FUNC_1( bind_by_object_id, 1, true, Db *db, const Category *cat ) {
    return db->_bind_by_object_id( v0, cat );
}
RiotBase_DEF_FUNC_1( bind_by_object_id, 2, tv0->unknown_value(), Db *db, const Category *cat ) {
    RO_CALL( self_make_inode_if_z, v0, db, cat );
    return RO_CALL( bind_by_kn_object_id_b, v0, db, cat );
}

RiotBase_DEF_FUNC_1( bind_by_kn_object_id_b, 1, tv0->known_value(), Db *db, const Category *cat ) {
    Inode inode;
    if ( not RO_CALL( to_Inode, v0, &inode, 1 ) )
        return Error::New( "Pb with inode conversion" );
    return db->_bind_by_inode( inode, cat );
}

RiotBase_DEF_FUNC_1( self_make_inode_if_z, 2, tv0->known_value(), Db *db, const Category *cat ) {
    Inode inode;
    if ( RO_CALL( to_Inode, v0, &inode, 1 ) and not inode )
        RO_CALL( del, db->_make_new_object( v0, cat ) );
}

} // namespace

} // namespace RiotBase
