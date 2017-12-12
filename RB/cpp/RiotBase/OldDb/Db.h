#pragma once

#include "../System/CycleEndedList.h"
#include "../System/RsaKey.h"
#include "../System/FqId.h"

#include "../Variable/ObjectId.h"
#include "../Variable/String.h"
#include "../Variable/Map.h"

#include "Core/DataFile.h"
#include "Core/DbTable.h"

namespace RiotBase {
using Directory = Map<String,ObjectId>;
class PostFuncDb;
class DbSession;
class TcpListener;

///
class Db {
public:
    struct InodeBinder {
        template<class T> operator T () const { return        RO_CALL( bind_by_object_id, key.rp, db, T::category )  ; }
        template<class T> operator T*() const { return new T( RO_CALL( bind_by_object_id, key.rp, db, T::category ) ); }
        Db      *db;
        ObjectId key;
    };

    struct UrlBinder   {
        template<class T> operator T () const { return        RO_CALL( bind_by_path, path.rp, db, T::category )  ; }
        template<class T> operator T*() const { return new T( RO_CALL( bind_by_path, path.rp, db, T::category ) ); }
        Db    *db;
        String path;
    };

    enum class SetMainDirStatusCode {
        #define DECL_ERR( NE, TXT ) NE,
        #include "Core/SetMainDirStatusCodesDecl.h"
        #undef DECL_ERR
    };

    enum class AddDataFileStatusCode {
        #define DECL_ERR( NE, TXT ) NE,
        #include "Core/AddDataFileStatusCodesDecl.h"
        #undef DECL_ERR
    };

    Db( const std::string &login = "anonymous", const std::string &password = "", DevId dev = 0 ); ///< password or private key
    ~Db();

    int                            connect           ( const std::string &ref, const std::string &port = {} ); ///< if port is not defined, we assumed that ref is a named socket
    void                           listen            ( const std::string &ref, bool is_a_named_socket = false );

    SetMainDirStatusCode           set_main_dir      ( const std::string &dir );    ///< directory of the file describing the bloc devices, position of the index, ...
    AddDataFileStatusCode          add_data_file     ( const std::string &file );   ///< add a file path, to store data (main dir will only contain "meta-data")

    InodeBinder                    bind              ( const ObjectId    &key  ) { return { this, key  }; }
    UrlBinder                      bind              ( const String      &path ) { return { this, path }; }
    UrlBinder                      bind              ( const std::string &path ) { return { this, path }; }
    UrlBinder                      bind              ( const char        *path ) { return { this, path }; }

    void                           flush             ();

    DevId                          get_dev_id        () const;
    UsrId                          get_usr_id        () const;
    FqId                           fqid              () const;

    Vec<UsrId>                     connected_users   () const;

    int                            ev_loop           ();

    void                           send_graph_cmd    ( DevId dev_id, PI64 dst_id, const Vec<PT> &out_offset, const CbStringPtr &msg );

    Rp                            *_bind_by_path     ( DaSi  path     , const Category *expected_category );
    Rp                            *_bind_by_object_id( Rp  *&object_id, const Category *expected_category );
    Rp                            *_make_new_object  ( Rp  *&object_id, const Category *expected_category );
    Rp                            *_bind_by_inode    ( Inode inode    , const Category *expected_category );

    static std::map<UsrId,RsaKey>  public_keys_by_usr_id; ///< beware: UsrId => global value

protected:
    friend void                    sched_th          ( DataFile *d );

    friend class  DbTcpListener;
    friend class  LazySourceExtValue;
    friend struct DbConfigReader;
    friend class  DbFileReader;
    friend class  DbCallback;
    friend class  PostFuncDb;
    friend class  DbSession;

    void                           _base_init        ( DevId dev_id, UsrId usr_id );
    // Rp                            *_get_ext_obj      ( ObjectId key, const Category *expected_category );
    // Rp                            *_get_ext_obj      ( Inode    key, const Category *expected_category );
    Inode                          _newlocinode      ();

    DbItem                        *root_dir;

    DevId                          dev;
    UsrId                          usr;

    std::string                    login;
    std::string                    password;
    RsaKey                         private_key;

    std::string                    main_dir;
    int                            main_file;
    Vec<DataFile *>                data_files;
    int                            cur_datafile_id;  ///< new num for local inode

    PI64                           cur_inode_num;    ///< new num for local inode
    DbTable                        table;            ///< Inode -> DbItem
    Vec<RcPtr<DbSession> >         sessions;         ///<
    PostFuncDb                    *post_func;
    Vec<TcpListener *>                TcpListeners;
    CycleEndedList<DbSession>      changed_sessions;
    CycleEndedList<DbItem>         changed_db_items;
    Vec<std::function<void()>>     what_to_do_when_connected_sessions;
};


} // namespace RiotBase
