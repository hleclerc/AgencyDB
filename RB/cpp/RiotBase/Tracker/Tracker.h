#pragma once

#include "../System/TimedTaskList.h"
#include "../System/Inode.h"
#include "../System/DevId.h"

#include "Core/TrackerServerSession.h"
#include "Core/IdGenerator.h"

#include <map>

namespace rocksdb {
class DB;
}

namespace RiotBase {
class  TrackerServerTcpListener;
class  TrackerServerUdpSocket;
class  VectorClock;
struct Timer_1s;
struct Timer_1h;
class  EvLoop;
class  Dns;

/**
*/
class Tracker {
public:
    /// base_id must be >= 1 and < ( 1 << IdGenerator::nb_bits_base_id ) (and must be unique amongst all the other instances).
    /// pass can be provided for convenience...
    Tracker( EvLoop *ev_loop, Dns *dns, unsigned base_id, std::string db_file );
    ~Tracker();

    void                                   tcp_bind          ( unsigned port, const char *cert_file, const char *key_file ); ///<
    void                                   udp_bind          ( unsigned port, const char *cert_file, const char *key_file ); ///<

    void                                   send_ice_candidate( DevId destination, DevId orig, PI8 initiator, const CbString &data );
    void                                   send_webrtc_offer ( DevId destination, DevId orig, const CbString &data );
    void                                   send_webrtc_answer( DevId destination, DevId orig, const CbString &data );

protected:
    friend class  TrackerServerUdpConnection;
    friend class  TrackerServerTcpConnection;
    friend class  TrackerServerTcpListener;
    friend class  TrackerServerUdpSocket;
    friend class  TrackerServerSession;
    friend struct Timer_1s;
    friend struct Timer_1h;

    using SessionMap = boost::intrusive::avl_multiset<TrackerServerSession,
        boost::intrusive::compare<TrackerServerSession::DevIdCmp>,
        boost::intrusive::member_hook<TrackerServerSession,boost::intrusive::avl_set_member_hook<>,&TrackerServerSession::member_hook_dev_id>
    >;

    struct InodeData {
        Vec<DevId,-1,1> connected_devs;
    };

    struct WaitingConfirmation {
        std::string user;
        std::string pass;
        std::string email;
    };

    void                                      login             ( TrackerServerSession *session, const std::string &user, const std::string &pass, DevId proposition = {} );
    void                                      logout            ( TrackerServerSession *session );
    void                                      bind_obj          ( TrackerServerSession *session, Inode obj, const VectorClock &known );
    void                                      load_obj          ( TrackerServerSession *session, Inode obj, bool bind );

    void                                      reg_session_to_del( TrackerServerSession *session );
    std::string                               create_user       ( const std::string &user, const CbString &pass, const CbString &email );
    std::string                               reg_waiting_usr   ( std::string tok );


    Vec<TrackerServerTcpListener *>           tcp_listeners;
    Vec<TrackerServerUdpSocket *>             udp_sockets;
    SessionMap                                sessions;
    EvLoop                                   *ev_loop;
    IdGenerator                               id_gen;
    Dns                                      *dns;

    rocksdb::DB                              *waiting_user_creation_db; ///< token -> user, pass, email
    rocksdb::DB                              *session_db;               ///< token -> dev, usr
    rocksdb::DB                              *inodes_db;                ///< inode -> bound devices
    rocksdb::DB                              *users_db;                 ///< usr_id -> home_inode, salted pass, login, email


    DevId                                     dev;
    PI64                                      nb_created_users;

    std::map<Inode,InodeData>                 inode_table;
    std::map<std::string,UsrId>               token_to_usr_id;
    std::map<std::string,WaitingConfirmation> usr_waiting_confirmation;

    TimedTaskList                             timed_task_list_1mn_res_1s; ///< 1 minute of tasks. Resolution = 1 sec.
    TimedTaskList                             timed_task_list_15day_res_1hr; ///< 1 minute of tasks. Resolution = 1 sec.
    Vec<TrackerServerSession *>               sessions_to_del;
    Timer_1s                                 *timer_1s;
    Timer_1h                                 *timer_1h;
};

} // namespace RiotBase
