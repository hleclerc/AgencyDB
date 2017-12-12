#include "DbTrackerClient.h"
#include "../Db.h"

namespace RiotBase {

DbTrackerClient::DbTrackerClient( Db *db, const std::string &addr, unsigned port ) : TrackerClient( addr, port ), db( db ) {
}

void DbTrackerClient::on_ans_credentials( DevId dev_id, UsrId usr_id ) {

}

void DbTrackerClient::on_bind_obj( Inode inode, DevId dev_id ) {
    TODO;
}

void DbTrackerClient::on_req_inode_data( Inode inode, DevId dev_id, const VectorClock &known ) {
    // db->req_patch_data( inode.loc_dev( db->dev_id ), dev.loc_dev( db->dev_id ), known );
    TODO;
}

} // namespace RiotBase
