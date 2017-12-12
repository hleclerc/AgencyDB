import as_a_function from "./as_a_function";

export default [
    'error_msg        ( String msg )', // msg begins by unsigned error code (@see TrackerCommand)
    'corrupted_data   ()',
    'login            ( String user, String pass )',
    'logout           ()',
    'bind_obj         ( Inode id, VectorClock known )',
    'load_and_bind_obj( Inode id )',
    'load_obj         ( Inode id )',
    'get_dev_info     ( DevId dev )',
    'ice_candidate    ( DevId destination, PI8 initiator, String jsep )',
    'webrtc_offer     ( DevId destination, String data )',
    'webrtc_answer    ( DevId destination, String data )',
    'create_user      ( String user, String pass, String email )',
].map( as_a_function );
