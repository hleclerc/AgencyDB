#pragma once

#include "../../EvLoop/UdpHashedCmdsConnection.h"
#include "../../EvLoop/UdpDtlsConnection.h"
#include "../../System/UsrId.h"

namespace RiotBase {
class VectorClock;
class TrackerClient;

#ifdef AVOID_SSL
#define BASE_TrackerClientConnection UdpRawConnection
#else
#define BASE_TrackerClientConnection UdpDtlsConnection
#endif // AVOID_SSS

/**
*/
class TrackerClientUdpConnection : public UdpHashedCmdsConnection<BASE_TrackerClientConnection<TrackerClientUdpConnection,512>> {
public:
    void           recv_error_msg           ( CmString msg );
    void           recv_corrupted_data      ();
    void           recv_ans_login           ( DevId dev_id, UsrId usr_id );
    void           recv_bind_obj            ( Inode inode, DevId dev_id );
    void           recv_req_inode_data      ( Inode inode, DevId dev_id, const VectorClock &known );
    void           recv_ans_dev_info        () { TODO; }

    void           parse_bs                 ( BinStream<CmString> bs );

    TrackerClient *tracker_client           ();
};

} // namespace RiotBase
