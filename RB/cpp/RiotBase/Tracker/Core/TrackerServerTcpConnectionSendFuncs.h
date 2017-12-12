
void send_error_msg              ( const CbString &msg );
void send_corrupted_data         ();
void send_ans_login              ( DevId dev, UsrId usr, const CbString &token );
void send_ans_logout             ();
void send_bind_obj               ( Inode id, DevId dev );
void send_req_inode_data         ( Inode id, DevId dev );
void send_req_inode_updates      ( Inode id, DevId dev, const VectorClock &known );
void send_req_and_bind_inode_data( Inode id, DevId dev );
void send_webrtc_offer           ( DevId dev, const CbString &sdp );
void send_webrtc_answer          ( DevId dev, const CbString &sdp );
void send_ice_candidate          ( DevId dev, PI8 initiator, const CbString &sdp );
void send_ans_create_user        ( const CbString &res );

