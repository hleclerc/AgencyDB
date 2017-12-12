#pragma once

#include "../System/CycleEndedList.h"
#include "../System/IntervalSet.h"
#include "../System/LazyNew.h"
#include "../System/RcPtr.h"

#include "../Variable/ObjectId.h"
#include "../Variable/String.h"
#include "../Variable/Map.h"

// #include "Core/DbSession.h"
#include "Core/DbTable.h"

#include <map>

namespace RiotBase {
using Directory = Map<String,ObjectId>;
class DbTrackerClient;
class DbPostFunc;

///
class Db {
public:
    struct Binder {
        template<class T> operator T () const { return        RO_CALL( bind_by_path, path.rp, T::category )  ; }
        template<class T> operator T*() const { return new T( RO_CALL( bind_by_path, path.rp, T::category ) ); }
        String path;
    };

    Db();
    ~Db();

    // preparation
    void                                connect              ( const std::string &user, const std::string &pass, const std::string &tracker_addr = {}, unsigned tracker_port = 0 ); ///<

    void                                add_listening_port   ( unsigned listening_port );                          ///< default_listening_port by default

    int                                 run                  ( double timeout = 0.0 );                             ///< run the (gev) event loop

    DevId                               get_dev_id           () const;
    UsrId                               get_usr_id           () const;

    void                                send_graph_cmd       ( DevId location, CbStringPtr dat );
    void                                send_graph_ans       ( DevId location, PI64 num, CbStringPtr dat );

//    Binder                              bind                 ( const String &path ) { return { path }; }

//    // stuff that should be protected somehow
//    Rp                                 *_bind_by_path        ( DaSi path, const Category *cat );
//    Rp                                 *_bind_by_inode       ( Inode inode, const Category *cat );
//    Rp                                 *_make_new_object     ( Rp *&object_id, const Category *cat );
//    Rp                                 *_bind_by_kn_object_id( Rp *&object_id, const Category *cat );

    // public attributes
    // Vec<RcPtr<DbSession>>               sessions;              ///< links to connected instances

protected:
    friend class DbTrackerClient;
    friend class DbPostFunc;
    friend class DbCallback;
    friend class DbSession;
    friend class DbItem;

//    // helpers
//    void                                set_dev_id           ( DevId dev, UsrId usr );
    Inode                               new_loc_inode        ();
//    DbSession                          *find_session         ( DevId glo_ext_dev );

//    void                                req_patch_data       ( Inode inode, DevId glo_ext_dev, const VectorClock &known ); ///< inode and dev are local

//    // user credentials
//    UsrId                               usr_id;                ///< global usr_id. Beware: usr_id.dev is local (it can be null, meaning that usr_id.real_dev == this->dev_id)
//    DevId                               dev_id;                ///< global dev_id of current instance (the only place where current dev_id != 0)

//    // connections
//    Vec<DbTcpListener *>                TcpListeners;
//    unsigned                            listening_port;

//    TrackerList                         tracker_list;
//    unsigned                            tracker_to_try;
    DbTrackerClient             *tracker_client;

    // items
    DbTable                             table;
    RcPtr<DbItem>                       root_dir;
    PI64                                cur_inode_num;

    CycleEndedList<DbItem,DbItem::Ptbs> items_to_be_signaled_to_tracker; ///< items are in this list when created. They are removed once the info is sent to the tracker
    CycleEndedList<DbItem,DbItem::Ptbs> items_with_updates_to_be_sent;   ///<

//    std::set<Inode>                     modified_ext_objects;            ///< external object that has been modified since the beginning (used when we connect to a new instance)


    // events
    DbPostFunc                         *db_post_func;

    // waiting stuff
//    bool                                waiting_for_an_id;
//    Vec<std::function<void(void)>>      todo_when_id_known;
};

extern LazyNew<Db> db; ///< global instance

} // namespace RiotBase
