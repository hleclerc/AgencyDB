#include "TrackerServerTcpConnection.h"
#include "TrackerServerUdpConnection.h"
#include "TrackerServerSession.h"
#include "../Tracker.h"

#include "../../System/ReadContHelpers.h"
#include "../../System/VectorClock.h"
#include "../../System/ArgsAsTypeList.h"
#include "../../System/TxtStream.h"
#include "../../System/Base64.h"
#include "../../System/Unref.h"
#include "../../System/UsrId.h"
#include "../../System/Date.h"
#include "../../System/Log.h"

namespace RiotBase {


TrackerServerTcpConnection::TrackerServerTcpConnection( SSL_CTX *ssl_ctx, int fd, Tracker *tracker, const InetAddress &addr ) : TcpSslConnection( ssl_ctx, fd ), inet_address( addr ), tracker( tracker ) {
    session          = new TrackerServerSession( tracker ); session->add_tcp_connection( this );
    pending_cb_queue = nullptr;
    inp_cont         = nullptr;
    websocket_mode   = false;
    framed           = 0;
}

TrackerServerTcpConnection::~TrackerServerTcpConnection() {
    session->rem_tcp_connection( this );
}

bool TrackerServerTcpConnection::parse() {
    if ( not inp_buff->used )
        return true;
    unsigned char *data = inp_buff->data, *end = data + inp_buff->used;
    session->activate();

    // if we were interrupted reading a String
    if ( pending_cb_queue ) {
        pending_cb_queue->next = inc_ref( inp_buff );
        pending_cb_queue = inp_buff;
    }

    // jump to previous "program counter" if previously stopped
    if ( inp_cont )
        goto *inp_cont;

    // first letter of the command
    while ( true ) {
        switch ( *data ) {
        case 'G': goto l_GET;
        default : return method_not_allowed();
        }
    }

    l_GET:
        READ_DATA; if ( *data != 'E' ) return method_not_allowed();
        READ_DATA; if ( *data != 'T' ) return method_not_allowed();
        READ_DATA; if ( *data != ' ' ) return method_not_allowed();

        // read the url
        READ_STRING_UNTIL_SPACE( url );

        // look for things like 'Sec-WebSocket-Key: ', ... until the two end-of-line
        while ( true ) {
            READ_DATA;
            l_READ_GET_HEADER:
                switch ( *data ) {
                case 13:
                    continue; // windows CR
                case 10: {
                    // do we have two successive eol ?
                    while ( true ) {
                        READ_DATA;
                        switch ( *data ) {
                        case 13:
                            continue; // windows CR
                        case 10:
                            if ( websocket_key.size() ) {
                                inp_cont = &&ws_loop;
                                return websocket_handshake();
                            }
                            return http_get(); // two consecutive eol means that the GET request is ended
                        default:
                            goto l_READ_GET_HEADER; // -> go to the previous swich (without waiting for new data)
                        }
                    }
                }
                case 'S':
                    goto GET_S;
                default:
                    continue;
                }
          }

    GET_S: // get with an url and with a first 'S'
        READ_DATA; if ( *data != 'e' ) goto l_READ_GET_HEADER;
        READ_DATA; if ( *data != 'c' ) goto l_READ_GET_HEADER;
        READ_DATA; if ( *data != '-' ) goto l_READ_GET_HEADER;
        READ_DATA; if ( *data != 'W' ) goto l_READ_GET_HEADER;
        READ_DATA; if ( *data != 'e' ) goto l_READ_GET_HEADER;
        READ_DATA; if ( *data != 'b' ) goto l_READ_GET_HEADER;
        READ_DATA; if ( *data != 'S' ) goto l_READ_GET_HEADER;
        READ_DATA; if ( *data != 'o' ) goto l_READ_GET_HEADER;
        READ_DATA; if ( *data != 'c' ) goto l_READ_GET_HEADER;
        READ_DATA; if ( *data != 'k' ) goto l_READ_GET_HEADER;
        READ_DATA; if ( *data != 'e' ) goto l_READ_GET_HEADER;
        READ_DATA; if ( *data != 't' ) goto l_READ_GET_HEADER;
        READ_DATA; if ( *data != '-' ) goto l_READ_GET_HEADER;
        READ_DATA; if ( *data == 'K' ) goto GET_WebSocket_K;
                   if ( *data != 'V' ) goto l_READ_GET_HEADER;
        READ_DATA; if ( *data != 'e' ) goto l_READ_GET_HEADER;
        READ_DATA; if ( *data != 'r' ) goto l_READ_GET_HEADER;
        READ_DATA; if ( *data != 's' ) goto l_READ_GET_HEADER;
        READ_DATA; if ( *data != 'i' ) goto l_READ_GET_HEADER;
        READ_DATA; if ( *data != 'o' ) goto l_READ_GET_HEADER;
        READ_DATA; if ( *data != 'n' ) goto l_READ_GET_HEADER;
        READ_DATA; if ( *data != ':' ) goto l_READ_GET_HEADER;
        READ_DATA; if ( *data != ' ' ) goto l_READ_GET_HEADER;
        READ_TXT_UINT( websocket_version );
        goto l_READ_GET_HEADER;

    GET_WebSocket_K: // 'Sec-WebSocket-K'
        READ_DATA; if ( *data != 'e' ) goto l_READ_GET_HEADER;
        READ_DATA; if ( *data != 'y' ) goto l_READ_GET_HEADER;
        READ_DATA; if ( *data != ':' ) goto l_READ_GET_HEADER;
        READ_DATA; if ( *data != ' ' ) goto l_READ_GET_HEADER;
        READ_STRING_UNTIL_SPACE( websocket_key );
        goto l_READ_GET_HEADER;

    // in websocket mode (read the websocket frames)
    ws_loop:
        // byte 0
        websocket_byte_0 = *data;
        // byte 1
        READ_DATA;
        websocket_payload_length = *data & 0x7F;
        if ( not ( *data & 0x80 ) )
            goto ws_wo_mask;
        // -> we have a mask
        if ( websocket_payload_length == 126 )
            goto ws_payload_length_16_bits;
        if ( websocket_payload_length == 127 )
            goto ws_payload_length_64_bits;
    ws_masking_key:
        if ( websocket_payload_length > 1 * 1024 * 1024 )
            return websocket_connection_close( WebsocketStatusCode::message_too_big );

        READ_DATA; websocket_masking_key[ 0 ] = *data;
        READ_DATA; websocket_masking_key[ 1 ] = *data;
        READ_DATA; websocket_masking_key[ 2 ] = *data;
        READ_DATA; websocket_masking_key[ 3 ] = *data;

        // payload data (with mask)
        //READ_SIZED( websocket_data, websocket_payload_length );

        // remove mask
        if ( websocket_payload_length ) {
            websocket_data = CbString( IKnowWhatIDo(), inp_buff, data + 1 - inp_buff->data, websocket_payload_length );
            pending_cb_queue = inp_buff;
            tmp_shift_read = 0; // it is not used to read an integer, so we can use it for offset
            while( websocket_payload_length ) {
                --websocket_payload_length;
                READ_DATA;
                *data ^= websocket_masking_key[ ( tmp_shift_read++ ) % 4 ];
            }
            pending_cb_queue = 0;
        }

    ws_parse_frame_data:
        // use opcode
        switch ( websocket_byte_0 & 0x0F ) {
        case 0x1: if ( not parse_websocket_msg() ) return false; break; // text
        case 0x2: if ( not parse_websocket_msg() ) return false; break; // binary
        case 0x8: return websocket_connection_closed();
        case 0x9: websocket_ping(); break;
        case 0xA: websocket_pong(); break;
        default: DISP_ERROR( "unknown websocket opcode (websocket_byte_0 & 0x0F = {})", websocket_byte_0 & 0x0F ); return false;
        }

        READ_DATA;
        goto ws_loop;

    ws_payload_length_16_bits:
        READ_DATA; websocket_payload_length = PI32( *data ) << 8;
        READ_DATA; websocket_payload_length += *data;
        goto ws_masking_key;

    ws_payload_length_64_bits:
        READ_DATA; websocket_payload_length  = PI64( *data ) << 56;
        READ_DATA; websocket_payload_length += PI64( *data ) << 48;
        READ_DATA; websocket_payload_length += PI64( *data ) << 40;
        READ_DATA; websocket_payload_length += PI64( *data ) << 32;
        READ_DATA; websocket_payload_length += PI64( *data ) << 24;
        READ_DATA; websocket_payload_length += PI64( *data ) << 16;
        READ_DATA; websocket_payload_length += PI64( *data ) << 8;
        READ_DATA; websocket_payload_length += *data;
        goto ws_masking_key;

    ws_wo_mask:
        if ( websocket_payload_length == 126 )
            goto ws_payload_length_16_bits_wo_mask;
        if ( websocket_payload_length == 127 )
            goto ws_payload_length_64_bits_wo_mask;

    ws_payload_data_wo_mask:
        READ_SIZED( websocket_data, websocket_payload_length );
        goto ws_parse_frame_data;

    ws_payload_length_16_bits_wo_mask:
        READ_DATA; websocket_payload_length  = SI64( *data ) << 8;
        READ_DATA; websocket_payload_length += *data;
        goto ws_payload_data_wo_mask;

    ws_payload_length_64_bits_wo_mask:
        READ_DATA; websocket_payload_length  = SI64( *data ) << 56;
        READ_DATA; websocket_payload_length += SI64( *data ) << 48;
        READ_DATA; websocket_payload_length += SI64( *data ) << 40;
        READ_DATA; websocket_payload_length += SI64( *data ) << 32;
        READ_DATA; websocket_payload_length += SI64( *data ) << 24;
        READ_DATA; websocket_payload_length += SI64( *data ) << 16;
        READ_DATA; websocket_payload_length += SI64( *data ) << 8;
        READ_DATA; websocket_payload_length += *data;
        goto ws_payload_data_wo_mask;

        return true;
}

void TrackerServerTcpConnection::frame_cw() {
    if ( websocket_mode ) {
        // TODO: use room to optimize insertion of header
        PT cw_size = cw.size();
        if ( PT pl = cw_size - framed ) {
            if ( pl <= 125 ) {
                PI8 header[] = { 0x80 /* FIN */ + 0x02 /* binary */, PI8( 0 * 0x80 /* no mask */ + pl /* payload length */ ) };
                cw.insert_some( framed, header, sizeof header );
            } else if ( pl < 0x10000 ) {
                PI8 header[] = { 0x80 /* FIN */ + 0x02 /* binary */, 0 * 0x80 /* no mask */ + 126 /* payload length */,
                                 PI8( pl >> 8 ), PI8( pl >> 0 ) };
                cw.insert_some( framed, header, sizeof header );
            } else {
                PI8 header[] = { 0x80 /* FIN */ + 0x02 /* binary */, 0 * 0x80 /* no mask */ + 127 /* payload length */,
                                 PI8( pl >> 56 ), PI8( pl >> 48 ), PI8( pl >> 40 ), PI8( pl >> 32 ),
                                 PI8( pl >> 24 ), PI8( pl >> 16 ), PI8( pl >>  8 ), PI8( pl >>  0 ) };
                cw.insert_some( framed, header, sizeof header );
            }
            framed = cw.size();
        }
    }
}

void TrackerServerTcpConnection::skip_framed( PT len ) {
    if ( websocket_mode )
        framed -= len;
}

bool TrackerServerTcpConnection::method_not_allowed() {
    cw.write_some( "HTTP/1.1 405 Method Not Allowed\nContent-Length: 0\n\n" );
    return false;
}

bool TrackerServerTcpConnection::websocket_handshake() {
    std::string key = websocket_key;
    key += "258EAFA5-E914-47DA-95CA-C5AB0DC85B11";

    unsigned char hash[ SHA_DIGEST_LENGTH ];
    SHA1( (unsigned char *)key.c_str(), key.size(), hash );
    key = base64_encode( hash, SHA_DIGEST_LENGTH );

    // anwser
    TxtStream<CbQueue> os( &cw );
    os << "HTTP/1.1 101 Switching Protocols\r\n"; // WebSocket Protocol Handshake
    os << "Connection: Upgrade\r\n";
    os << "Sec-WebSocket-Accept: " << key << "\r\n";
    os << "Upgrade: WebSocket\r\n";
    os << "\r\n";

    // this first answer should not be framed
    websocket_mode = true;
    framed         = cw.size();

    return true;
}

bool TrackerServerTcpConnection::http_get() {
    //
    if ( url.starts_with( "/register?token=" ) ) {
        std::string content = tracker->reg_waiting_usr( url.end_from( 16 ) );
        TxtStream<CbQueue>( &this->cw ) << "HTTP/1.0 200 OK\nContent-Type: text/plain\nContent-Length: " << content.size() << "\n\n" << content;
        return false;
    }

    //
    std::string data = "<html><head><title>Not found</title></head><body>Not found</body></html>";
    this->cw.write_some( "HTTP/1.0 404 Not Found\nContent-Type: text/html\nContent-Length: " + to_string( data.size() ) + "\n\n" + data );
    // DISP_ERROR( "TODO: http GET (url={})", std::string( url ) );
    return false;
}

bool TrackerServerTcpConnection::websocket_connection_close( WebsocketStatusCode why ) {
    PI8 data[] = { 0x80 /* FIN */ + 0x08 /* opcode close */, 0x80 /* mask that seems to be mandatory */ + 0x02 /* payload length */,
                   1, 2, 3, 4 /* masking key */, PI8( ( int( why ) >> 8 ) ^ 1 ), PI8( ( int( why ) >> 0 ) ^ 2 ) /* payload */ };
    cw.write_some( data, sizeof( data ) );
    return false;
}

bool TrackerServerTcpConnection::websocket_connection_closed() {
    DISP_INFO( "TODO: rfc6455 says that we MUST answer by a close with the same opcode" );
    return false; ///< will delete the connection
}

void TrackerServerTcpConnection::websocket_ping() {
    PI64 pl = websocket_data.size();
    cw.write_byte( 0x80 /* FIN */ + 0x0A /* opcode pong */ );
    if ( pl <= 125 ) {
        cw.write_byte( 0x80 /* mask that seems to be mandatory */ + pl /* payload length */ );
    } else if ( pl < 0x10000 ) {
        cw.write_byte( 0x80 /* mask that seems to be mandatory */ + 126 /* payload length */ );
        cw.write_byte( pl >> 8 );
        cw.write_byte( pl >> 0 );
    } else {
        cw.write_byte( 0x80 /* mask that seems to be mandatory */ + 127 /* payload length */ );
        cw.write_byte( pl >> 56 );
        cw.write_byte( pl >> 48 );
        cw.write_byte( pl >> 40 );
        cw.write_byte( pl >> 32 );
        cw.write_byte( pl >> 24 );
        cw.write_byte( pl >> 16 );
        cw.write_byte( pl >> 8  );
        cw.write_byte( pl >> 0  );
    }

    // masking key
    cw.write_byte( 0 );
    cw.write_byte( 1 );
    cw.write_byte( 2 );
    cw.write_byte( 3 );

    // payload
    PI64 i = 0;
    websocket_data.data_visitor( [ this, &i ]( const PI8 *b, const PI8 *e ) {
        for( ; b < e; ++b )
            cw.write_byte( *b ^ ( i % 4 ) );
    } );
}

void TrackerServerTcpConnection::websocket_pong() {
}

#include "TrackerServerTcpConnectionSendFuncsDef.h"

bool TrackerServerTcpConnection::check_checksum( CbString &ws_cmd, PT delta, const char *cmd ) {
    PI32 vs = crc_func( ws_cmd.substring( 0, delta ) );
    ws_cmd.skip_some( delta );

    PI32 cs;
    ws_cmd.read_some( &cs, sizeof cs );
    if ( ws_cmd.error() ) {
        DISP_ERROR( "READING ERROR" );
        return false;
    }

    if ( vs != cs ) {
        DISP_ERROR( "CHECKSUM ERROR for {} (wanted={} but crc(substring)={})", cmd, cs, vs );
        ws_cmd.ack_error();
        return false;
    }

    return true;
}

void TrackerServerTcpConnection::recv_error_msg( CbString msg ) {
    DISP_INFO( "Error message from connection {}: {}", inet_address, msg );
}

void TrackerServerTcpConnection::recv_corrupted_data() {
    DISP_INFO( "Connection {} says that we have sent corrupted data", inet_address );
}

void TrackerServerTcpConnection::recv_login( CbString user, CbString pass ) {
    tracker->login( session, user, pass );
    send_ans_login( session->dev, session->usr, session->token );
}

void TrackerServerTcpConnection::recv_logout() {
    tracker->logout( session );
    send_ans_logout();
}

void TrackerServerTcpConnection::recv_bind_obj( Inode obj, const VectorClock &known ) {
    tracker->bind_obj( session, obj, known );
}

void TrackerServerTcpConnection::recv_load_and_bind_obj( Inode obj ) {
    tracker->load_obj( session, obj, true );
}

void TrackerServerTcpConnection::recv_load_obj( Inode obj ) {
    tracker->load_obj( session, obj, false );
}

void TrackerServerTcpConnection::recv_get_dev_info( DevId dev ) {
    TODO;
}

void TrackerServerTcpConnection::recv_ice_candidate( DevId destination, PI8 initiator, const CbString &data ) {
    tracker->send_ice_candidate( destination, session->dev, initiator, data );
}

void TrackerServerTcpConnection::recv_webrtc_offer( DevId destination, const CbString &data ) {
    tracker->send_webrtc_offer( destination, session->dev, data );
}

void TrackerServerTcpConnection::recv_webrtc_answer( DevId destination, const CbString &data ) {
    tracker->send_webrtc_answer( destination, session->dev, data );
}

void TrackerServerTcpConnection::recv_create_user( const CbString &user, const CbString &pass, const CbString &email ) {
    send_ans_create_user( tracker->create_user( user, pass, email ) );
}

//void TrackerServerTcpConnection::recv_ice_candidate( CbString data ) {
//    for( TrackerServerSession &s : tracker->sessions )
//        if ( &s != session )
//            s.apply_on_best_conn( [&]( auto *c ) { c->send_ice_candidate( session->dev, data ); } );
//    //    while ( CbString line = data.read_line() ) {
//    //        if ( line.starts_with( "candidate:" ) ) {
//    //            TxtStream<CbString> ts( &line.skip_some( 10 ) );
//    //            DISP_DEBUG( "WebRTC candidate: {}", std::string( line ) );

//    ////            CbString foundation    = ts.read();
//    ////            unsigned component_id  = ts.read();
//    ////            CbString transport     = ts.read();
//    ////            unsigned priority      = ts.read();
//    ////            CbString addr          = ts.read();
//    ////            unsigned port          = ts.read();
//    ////            CbString sep_cand_type = ts.read();
//    ////            CbString cand_type     = ts.read();
//    ////            if ( not ts.error() )
//    ////                get_session()->try_sctp_candidate( addr, port );

//    //            //            PRINT( std::string(foundation)    );
//    //            //            PRINT( component_id               );
//    //            //            PRINT( std::string(transport)     );
//    //            //            PRINT( priority                   );
//    //            //            PRINT( std::string(addr)          );
//    //            //            PRINT( port                       );
//    //            //            PRINT( std::string(sep_cand_type) );
//    //            //            PRINT( std::string(cand_type)     );
//    //        }
//    //    }

//    //    //    std::string line;
//    //    //    std::istringstream is( std::string( data ) );
//    //    //    while ( std::getline( is, line ) ) {
//    //    //        if ( line.find( "candidate:" ) == 0 ) {
//    //    //            std::istringstream js( line.substr( 10 ) );
//    //    //        }
//    //    //    }
//}

bool TrackerServerTcpConnection::parse_websocket_msg() {
    while ( CbStringPtr ws_ptr = websocket_data ) {
        BinStream<CbStringPtr> bs( &ws_ptr );
        PI8 cmd = bs.read_byte();
        switch( cmd ) {
            #include "TrackerServerTcpConnectionParseWebSocketMsg.h"
            default:
                TODO;
        }
    }

    return true;
}


} // namespace RiotBase


