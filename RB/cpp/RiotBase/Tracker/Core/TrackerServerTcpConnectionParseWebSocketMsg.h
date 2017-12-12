
case 0: {
    DISP_DEBUG( "recv error_msg from {}", session->dev );
    CbString msg = bs.read();
    if ( not this->check_checksum( websocket_data, websocket_data.size() - ws_ptr.size(), "error_msg" ) )
        return false;
    recv_error_msg( std::move( msg ) );
    break;
}
case 1: {
    DISP_DEBUG( "recv corrupted_data from {}", session->dev );
    if ( not this->check_checksum( websocket_data, websocket_data.size() - ws_ptr.size(), "corrupted_data" ) )
        return false;
    recv_corrupted_data();
    break;
}
case 2: {
    DISP_DEBUG( "recv login from {}", session->dev );
    CbString user = bs.read();
    CbString pass = bs.read();
    if ( not this->check_checksum( websocket_data, websocket_data.size() - ws_ptr.size(), "login" ) )
        return false;
    recv_login( std::move( user ), std::move( pass ) );
    break;
}
case 3: {
    DISP_DEBUG( "recv logout from {}", session->dev );
    if ( not this->check_checksum( websocket_data, websocket_data.size() - ws_ptr.size(), "logout" ) )
        return false;
    recv_logout();
    break;
}
case 4: {
    DISP_DEBUG( "recv bind_obj from {}", session->dev );
    Inode id = bs.read();
    VectorClock known = bs.read();
    if ( not this->check_checksum( websocket_data, websocket_data.size() - ws_ptr.size(), "bind_obj" ) )
        return false;
    recv_bind_obj( std::move( id ), std::move( known ) );
    break;
}
case 5: {
    DISP_DEBUG( "recv load_and_bind_obj from {}", session->dev );
    Inode id = bs.read();
    if ( not this->check_checksum( websocket_data, websocket_data.size() - ws_ptr.size(), "load_and_bind_obj" ) )
        return false;
    recv_load_and_bind_obj( std::move( id ) );
    break;
}
case 6: {
    DISP_DEBUG( "recv load_obj from {}", session->dev );
    Inode id = bs.read();
    if ( not this->check_checksum( websocket_data, websocket_data.size() - ws_ptr.size(), "load_obj" ) )
        return false;
    recv_load_obj( std::move( id ) );
    break;
}
case 7: {
    DISP_DEBUG( "recv get_dev_info from {}", session->dev );
    DevId dev = bs.read();
    if ( not this->check_checksum( websocket_data, websocket_data.size() - ws_ptr.size(), "get_dev_info" ) )
        return false;
    recv_get_dev_info( std::move( dev ) );
    break;
}
case 8: {
    DISP_DEBUG( "recv ice_candidate from {}", session->dev );
    DevId destination = bs.read();
    PI8 initiator = bs.read();
    CbString jsep = bs.read();
    if ( not this->check_checksum( websocket_data, websocket_data.size() - ws_ptr.size(), "ice_candidate" ) )
        return false;
    recv_ice_candidate( std::move( destination ), std::move( initiator ), std::move( jsep ) );
    break;
}
case 9: {
    DISP_DEBUG( "recv webrtc_offer from {}", session->dev );
    DevId destination = bs.read();
    CbString data = bs.read();
    if ( not this->check_checksum( websocket_data, websocket_data.size() - ws_ptr.size(), "webrtc_offer" ) )
        return false;
    recv_webrtc_offer( std::move( destination ), std::move( data ) );
    break;
}
case 10: {
    DISP_DEBUG( "recv webrtc_answer from {}", session->dev );
    DevId destination = bs.read();
    CbString data = bs.read();
    if ( not this->check_checksum( websocket_data, websocket_data.size() - ws_ptr.size(), "webrtc_answer" ) )
        return false;
    recv_webrtc_answer( std::move( destination ), std::move( data ) );
    break;
}
case 11: {
    DISP_DEBUG( "recv create_user from {}", session->dev );
    CbString user = bs.read();
    CbString pass = bs.read();
    CbString email = bs.read();
    if ( not this->check_checksum( websocket_data, websocket_data.size() - ws_ptr.size(), "create_user" ) )
        return false;
    recv_create_user( std::move( user ), std::move( pass ), std::move( email ) );
    break;
}

