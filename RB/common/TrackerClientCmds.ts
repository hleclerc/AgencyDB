import as_a_function from "./as_a_function";

export default [
    "error_msg              ( String msg )", // msg begins by unsigned error code (@see TrackerCommands)
    "corrupted_data         ()",
    "ans_login              ( DevId dev, UsrId  usr, String token )", //
    "ans_logout             ()",
    "bind_obj               ( Inode id , DevId dev )", // say that a peer has a given object
    "req_inode_data         ( Inode id , DevId dev )", // request and forget
    "req_inode_updates      ( Inode id , DevId dev, VectorClock known )", // request and forget
    "req_and_bind_inode_data( Inode id , DevId dev )", //
    "webrtc_offer           ( DevId dev, String sdp )", //
    "webrtc_answer          ( DevId dev, String sdp )", //
    "ice_candidate          ( DevId dev, PI8 initiator, String sdp )", //
    "ans_create_user        ( String res )", //
].map( as_a_function );
