#pragma once

#include "../System/VectorClock.h"
#include "../System/UsrId.h"
#include "../System/Log.h"
#include <functional>
#include <string>

namespace RiotBase {
class TrackerClientUdpConnection;
class TrackerClientUdpSocket;

/**
  create a socket to trackers
*/
class TrackerClient {
public:
    TrackerClient( const std::string &server_addr, unsigned server_port );
    virtual ~TrackerClient();

    void                             send_credentials  ( const std::string &user, const std::string &pass );
    void                             send_bind_obj     ( Inode loc_inode, const VectorClock &known, DevId cur_dev );

    void                             flush             ();

    DevId                            dev_id            () const { return _dev_id; }
    UsrId                            usr_id            () const { return _usr_id; }

protected:
    friend class TrackerClientUdpConnection;

    virtual void                     on_ans_credentials( DevId dev_id, UsrId usr_id );
    virtual void                     on_bind_obj       ( Inode inode, DevId dev_id );
    virtual void                     on_req_inode_data ( Inode inode, DevId dev_id, const VectorClock &known );

    void                             set_dev_and_usr_id( DevId dev_id, UsrId usr_id );

    DevId                            _dev_id;
    UsrId                            _usr_id;
    TrackerClientUdpSocket             *udp_socket;
    TrackerClientUdpConnection         *connection;
};

} // namespace RiotBase
