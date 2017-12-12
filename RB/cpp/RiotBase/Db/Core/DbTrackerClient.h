#pragma once

#include "../../Tracker/TrackerClient.h"

namespace RiotBase {
class Db;

/**
*/
class DbTrackerClient : public TrackerClient {
public:
    DbTrackerClient( Db *db, const std::string &addr, unsigned port );

    virtual void on_ans_credentials( DevId dev_id, UsrId usr_id );
    virtual void on_bind_obj       ( Inode inode, DevId dev_id );
    virtual void on_req_inode_data ( Inode inode, DevId dev_id, const VectorClock &known );

    Db *db;
};

} // namespace RiotBase
