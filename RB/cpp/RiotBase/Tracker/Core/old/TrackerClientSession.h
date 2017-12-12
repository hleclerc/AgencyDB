#pragma once

#include "../System/VectorClock.h"
#include "../System/ValidityHook.h"
#include "../System/UsrId.h"
#include "../System/Vec.h"
#include <functional>
#include <string>

namespace RiotBase {
class TrackerClientConnection;

/**
  Session to a tracker
*/
class TrackerClientSession {
public:
    struct TrackerClientDevInfo { std::string addr; unsigned port; };

    TrackerClientSession( std::string addr, unsigned port );
    virtual ~TrackerClientSession();

    void                                 send_credentials            ( std::string user, std::string pass, std::function<void( DevId dev, UsrId usr )> &&f );
    void                                 send_get_dev_info           ( DevId glo_ext_dev, std::function<void( std::string addr, unsigned port )> &&f );
    void                                 send_bind_obj               ( Inode inode, const VectorClock &known ); ///< say that we need the data of object identified by `inode`, and that we may modify it. The tracker will answer by a list of device ids watching/knowing `inode`, AND will send a query to one of them to send the corresponding data
    void                                 send_quit_cmd               (); ///< stop the server. Allowed only for root user.

    void                                 on_connection_problem       ( std::function<void()> &&f );
    void                                 on_identification           ( std::function<void( DevId, UsrId )> &&f );

    void                                 flush                       ();


    DevId                                dev;            ///< device id of the local db instance. obtained from the tracker
    UsrId                                usr;            ///< user id of the local db instance. obtained from the tracker
    ValidityHook                         _validity_hook;

protected:
    using TodoWhenDevInfo = std::map<DevId,Vec<std::function<void( std::string addr, unsigned port )>>>;
    friend class TrackerClientConnection;
    enum { nb_retry_max = 5 };

    bool                                  connect_if_not_already_done ( int num_retry = nb_retry_max );

    //    void                                  _ans_dev_info               ( DevId dev, std::string addr, unsigned port );
    void                                  _send_inode_data            ( Inode inode, std::string addr, unsigned port, const VectorClock &known );

    virtual void                          recv_ans_login              ( DevId dev, UsrId usr, const CbString &addr, unsigned port );
    virtual void                          recv_new_inode_connection   ( Inode inode, DevId dev, const VectorClock &known );
    virtual void                          recv_req_patch_data         ( Inode inode, DevId dev, const VectorClock &known );
    virtual void                          recv_ans_dev_info           ( DevId dev, const CbString &addr, unsigned port );
    virtual void                          recv_corrupted_data         ( PI64 code );

    CbQueue                              *cb_queue                    ();

    Vec<std::function<void()>>            _on_connection_problem;           ///< if errors
    Vec<std::function<void(DevId,UsrId)>> _on_identification;               ///< when connected, with a known dev and a known usr
    TodoWhenDevInfo                       _on_dev_info;
    CbQueue                               to_be_sent;                       ///< tmp CbQueue that can be used if no connection
    TrackerClientConnection              *connection;                       ///<
    std::string                           addr;                             ///< tracker address
    unsigned                              port;                             ///< port used for tracker and peer communication

    bool                                  in_a_connection_retry;
};

} // namespace RiotBase
