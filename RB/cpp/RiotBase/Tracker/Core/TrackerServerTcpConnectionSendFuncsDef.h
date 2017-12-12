
void TrackerServerTcpConnection::send_error_msg( const CbString &msg ) {
    DISP_DEBUG( "send error_msg to {}", session->dev );
    bs_out_auto_close() << 0u << msg;
}
void TrackerServerTcpConnection::send_corrupted_data() {
    DISP_DEBUG( "send corrupted_data to {}", session->dev );
    bs_out_auto_close() << 1u;
}
void TrackerServerTcpConnection::send_ans_login( DevId dev, UsrId usr, const CbString &token ) {
    DISP_DEBUG( "send ans_login to {}", session->dev );
    bs_out_auto_close() << 2u << dev << usr << token;
}
void TrackerServerTcpConnection::send_ans_logout() {
    DISP_DEBUG( "send ans_logout to {}", session->dev );
    bs_out_auto_close() << 3u;
}
void TrackerServerTcpConnection::send_bind_obj( Inode id, DevId dev ) {
    DISP_DEBUG( "send bind_obj to {}", session->dev );
    bs_out_auto_close() << 4u << id << dev;
}
void TrackerServerTcpConnection::send_req_inode_data( Inode id, DevId dev ) {
    DISP_DEBUG( "send req_inode_data to {}", session->dev );
    bs_out_auto_close() << 5u << id << dev;
}
void TrackerServerTcpConnection::send_req_inode_updates( Inode id, DevId dev, const VectorClock &known ) {
    DISP_DEBUG( "send req_inode_updates to {}", session->dev );
    bs_out_auto_close() << 6u << id << dev << known;
}
void TrackerServerTcpConnection::send_req_and_bind_inode_data( Inode id, DevId dev ) {
    DISP_DEBUG( "send req_and_bind_inode_data to {}", session->dev );
    bs_out_auto_close() << 7u << id << dev;
}
void TrackerServerTcpConnection::send_webrtc_offer( DevId dev, const CbString &sdp ) {
    DISP_DEBUG( "send webrtc_offer to {}", session->dev );
    bs_out_auto_close() << 8u << dev << sdp;
}
void TrackerServerTcpConnection::send_webrtc_answer( DevId dev, const CbString &sdp ) {
    DISP_DEBUG( "send webrtc_answer to {}", session->dev );
    bs_out_auto_close() << 9u << dev << sdp;
}
void TrackerServerTcpConnection::send_ice_candidate( DevId dev, PI8 initiator, const CbString &sdp ) {
    DISP_DEBUG( "send ice_candidate to {}", session->dev );
    bs_out_auto_close() << 10u << dev << initiator << sdp;
}
void TrackerServerTcpConnection::send_ans_create_user( const CbString &res ) {
    DISP_DEBUG( "send ans_create_user to {}", session->dev );
    bs_out_auto_close() << 11u << res;
}

