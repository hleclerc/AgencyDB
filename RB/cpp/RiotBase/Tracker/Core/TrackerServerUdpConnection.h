#pragma once

#include "../../EvLoop/UdpHashedCmdsConnection.h"
#include "../../EvLoop/UdpDtlsConnection.h"
#include "../../System/VectorClock.h"
#include "../../System/Inode.h"
#include "../../System/UsrId.h"

namespace RiotBase {
class TrackerServerUdpSocket;
class TrackerServerSession;
class Tracker;

/**
*/
class TrackerServerUdpConnection : public UdpHashedCmdsConnection<UdpDtlsConnection<TrackerServerUdpConnection,512>> {
public:
    ~TrackerServerUdpConnection();

    void                  init                ( TrackerServerUdpSocket *us, bool server );

    // server -> client
    void                  send_error_msg              ( const CbString &msg );
    void                  send_corrupted_data         ();
    void                  send_ans_login              ( DevId dev, UsrId usr, const CbString &token );
    void                  send_ans_logout             ();
    void                  send_bind_obj               ( Inode id, DevId dev );
    void                  send_req_inode_data         ( Inode id, DevId dev );
    void                  send_req_inode_updates      ( Inode id, DevId dev, const VectorClock &known );
    void                  send_req_and_bind_inode_data( Inode id, DevId dev );
    void                  send_webrtc_offer           ( DevId dev, const CbString &sdp );
    void                  send_webrtc_answer          ( DevId dev, const CbString &sdp );
    void                  send_ice_candidate          ( DevId dev, PI8 initiator, const CbString &sdp );
    void                  send_ans_create_user        ( const CbString &res );

    void                  parse_bs                    ( BinStream<CmString> bs );

protected:
    // client -> server
    void                  recv_error_msg              ( CmString msg );
    void                  recv_corrupted_data         ();
    void                  recv_login                  ( CmString user, CmString pass );
    void                  recv_logout                 ();
    void                  recv_bind_obj               ( Inode obj, const VectorClock &known );
    void                  recv_get_dev_info           ( DevId dev );
    void                  recv_ice_candidate          ( PI8 initiator, CmString data );
    void                  recv_webrtc_offer           ( DevId destination, CmString data );
    void                  recv_webrtc_answer          ( DevId destination, CmString data );

    Tracker              *tracker                     ();

    TrackerServerSession *session;
    DevId                 dev_id;
    UsrId                 usr_id;
};

} // namespace RiotBase
