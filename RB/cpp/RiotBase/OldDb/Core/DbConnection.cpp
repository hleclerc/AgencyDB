#include "../../EvLoop/TcpSslConnection.h"
#include "../../EvLoop/Util/Socket.h"
#include "../../EvLoop/EvLoop.h"

#include "../../System/SetOfPatchId.h"
#include "../../System/TxtStream.h"
#include "../../System/Base64.h"
#include "../../System/Print.h"
#include "../../System/Inode.h"
#include "../../System/Log.h"

#include "DbCorruptedCommand.h"
#include "DbCorruptedData.h"
#include "DbConnection.h"
#include "DbCommands.h"
#include "DbSession.h"

#include "../Db.h"

#include <iostream>

namespace RiotBase {

#define RECV( CMD ) DISP_DEBUG( "Recv <- {} (from {}): {}", session->dev_id, session->db->get_dev_id(), CMD )

struct DbConnection : TcpRawConnection {
    using TP = TcpRawConnection;

    DbConnection( DbSession *session, int fd, bool server ) : TcpRawConnection( fd ), inp_cont( 0 ), pending_cb_queue( 0 ), session( session ), bq( &cq ) {
    }

    ~DbConnection() {
        if ( session )
            session->connection = 0;
    }

protected:
    enum class WebsocketStatusCode {
        normal                                                             = 1000,
        going_away                                                         = 1001,
        protocol_error                                                     = 1002,
        unsupported_data                                                   = 1003,
        reserved                                                           = 1004,
        reserved_for_extensions_0                                          = 1005,
        reserved_for_extensions_1                                          = 1006,
        inconsistent_or_invalid_data                                       = 1007,
        policy_violation                                                   = 1008,
        message_too_big                                                    = 1009,
        extension_handshake_missing                                        = 1010,
        an_unexpected_condition_prevented_the_request_from_being_fulfilled = 1011
    };

    virtual bool parse( Buffer *buffer ) {
        if ( session )
            session->activate();
        if ( not buffer->used )
            return true;
        unsigned char *data = buffer->data, *end = data + buffer->used;

        // if we were interrupted reading a String
        if ( pending_cb_queue ) {
            pending_cb_queue->next = inc_ref( buffer );
            pending_cb_queue = buffer;
        }

        // jump to previous "program counter" if previously stopped
        if ( inp_cont )
            goto *inp_cont;

        // macro to ask for more. If not available, wait for more (modification of inp_cont)
        #define READ_DATA( N ) \
            if ( ++data >= end ) { inp_cont = &&m_##N; return true; } m_##N:

        #define READ_STRING_UNTIL_SPACE( VAR ) \
            READ_DATA( TAG_##VAR##_0 ); \
            if ( *data != ' ' ) { \
                VAR = CbString( IKnowWhatIDo(), buffer, data - buffer->data, 1 ); \
                pending_cb_queue = buffer; \
                while ( true ) { \
                    READ_DATA( TAG_##VAR##_1 ); \
                    if ( *data == ' ' or *data == 10 or *data == 13 ) \
                        break; \
                    VAR.inc_length_wo_cr(); \
                } \
                pending_cb_queue = 0; \
                if ( data == buffer->data ) \
                    dec_ref( buffer ); /* not going to be ref counted in CbString VAR */ \
                --data; /* to compensate the next READ_DATA (that may be included in READ_UINT, READ_...) */ \
            }

        // Read ascii repr of a uint number.
        #define READ_TXT_UINT( VAR ) \
            VAR = 0; \
            while ( true ) { \
                READ_DATA( TAG_##VAR ); \
                if ( *data < '0' or *data > '9' ) { \
                    --data; \
                    break; \
                } \
                VAR = 10 * VAR + ( *data - '0' ); \
            }

        // Read compressed repr of a uint number.
        #define READ_UINT( VAR, LAB ) \
            READ_DATA( LAB##_0 ); \
            if ( *data >= 128 ) { \
                VAR = *data - 128; \
                tmp_shift_read = 7; \
                while ( true ) { \
                    READ_DATA( LAB##_1 ); \
                    if ( *data < 128 ) { \
                        VAR += *data << tmp_shift_read; \
                        break; \
                    } \
                    VAR += ( *data - 128 ) << tmp_shift_read; \
                    tmp_shift_read += 7; \
                } \
            } else \
                VAR = *data

        // Read compressed repr of a uint number.
        #define READ_INODE( VAR, LAB ) \
            READ_UINT( VAR.num, LAB##_num ); \
            if ( VAR.num ) { READ_UINT( VAR.dev.val, LAB##_dev ); if ( VAR.dev == session->db->get_dev_id() ) VAR.dev = 0; } else VAR.dev = 0

        // Read compressed repr of a UsrId
        #define READ_USR_ID( VAR, LAB ) \
            READ_UINT( VAR.num, LAB##_num ); \
            if ( VAR.num ) { READ_UINT( VAR.dev.val, LAB##_dev ); if ( VAR.dev == session->db->get_dev_id() ) VAR.dev = 0; if ( VAR == session->db->get_usr_id() ) VAR = UsrId{}; } else VAR.dev = 0

        //        #define READ_SET_OF_PATCH_ID( VAR, LAB )
        //            READ_UINT( tmp_nb_dev, LAB##_nb_dev );
        //            VAR.clear();
        //            while ( tmp_nb_dev-- ) {
        //                READ_UINT( tmp_dev, LAB##_dev );
        //                if ( tmp_dev == session->db->get_dev_id() )
        //                    tmp_dev = 0;
        //                READ_UINT( tmp_nb_int, LAB##_nb_int );
        //                while ( tmp_nb_int-- ) {
        //                     READ_UINT( tmp_min, LAB##_min );
        //                     READ_UINT( tmp_max, LAB##_max );
        //                     VAR.data[ tmp_dev ].insert( tmp_min, tmp_max );
        //                }
        //            }

        //        #define READ_DO_TYPE( VAR, LAB )
        //            READ_STRING( tmp_name, LAB##_name );
        //            VAR = DoType::find( tmp_name );

        // read LENGTH bytes, in OUTPUT which should be a CbReader
        #define READ_SIZED( VAR, LENGTH, LABEL ) \
            if ( LENGTH ) { \
                READ_DATA( m_##VAR##_##LENGTH##_##LABEL##_0 ); /* we need at least one byte */ \
                VAR = CbString( IKnowWhatIDo(), buffer, data - buffer->data, LENGTH );\
                unsigned remaining_data = buffer->used - ( data - buffer->data ); \
                if ( LENGTH > remaining_data ) { \
                    LENGTH -= remaining_data; \
                    pending_cb_queue = buffer; \
                    inp_cont = &&m_##VAR##_##LENGTH##_##LABEL; \
                    return true; \
                  m_##VAR##_##LENGTH##_##LABEL: \
                    if ( LENGTH > buffer->used ) { \
                        LENGTH -= buffer->used; \
                        return true; /* we loop with the same inp_cont */ \
                    } \
                    pending_cb_queue = 0; \
                } \
                data += LENGTH - 1; \
            } else \
                VAR.free()

        // read String (-> CbReader) in binary format (cuint for length then data)
        #define READ_STRING( OUTPUT, LABEL ) \
            READ_UINT( tmp_str_length, LABEL ); \
            READ_SIZED( OUTPUT, tmp_str_length, LABEL ) \

        //
        while ( true ) {
            switch ( *data ) {
            case PI8( DbCommands::end ): return false; // gracefull end of the session
            case PI8( DbCommands::G   ): goto l_GET;
            #define COMMAND_DECL( CMD ) case DbCommands::CMD: RECV(#CMD); goto l_##CMD;
            #include "DbCommandsDecl.h"
            #undef COMMAND_DECL
            default: return corrupted_data( CorruptedData::unknown_command );
            }

            // ------------------------------------------------------------------------------------
          l_GET:
            READ_DATA( GET1 ); if ( *data != 'E' ) return corrupted_data( CorruptedData::unknown_command );
            READ_DATA( GET2 ); if ( *data != 'T' ) return corrupted_data( CorruptedData::unknown_command );
            READ_DATA( GET3 ); if ( *data != ' ' ) return corrupted_data( CorruptedData::unknown_command );

            // read the url
            READ_STRING_UNTIL_SPACE( url );

            // look for things like 'Sec-WebSocket-Key: ', ... until the two end of line
            while ( true ) {
                READ_DATA( GET4 );
                switch ( *data ) {
                case 13:
                    continue; // windows CR
                case 10: {
                    // do we have two successive eol ?
                    while ( true ) {
                        READ_DATA( GET5 );
                        switch ( *data ) {
                        case 13:
                            continue; // windows CR
                        case 10:
                            if ( websocket_key.size() ) {
                                inp_cont = &&ws_loop;
                                return websocket_handshake();
                            }
                            return http_get( url ); // two consecutive eol means that the GET request is ended
                        default:
                            goto m_GET4; // -> go to the previous swich (without waiting for new data)
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
            READ_DATA( GET_S1 ); if ( *data != 'e' ) goto m_GET4;
            READ_DATA( GET_S2 ); if ( *data != 'c' ) goto m_GET4;
            READ_DATA( GET_S3 ); if ( *data != '-' ) goto m_GET4;
            READ_DATA( GET_S4 ); if ( *data != 'W' ) goto m_GET4;
            READ_DATA( GET_S5 ); if ( *data != 'e' ) goto m_GET4;
            READ_DATA( GET_S6 ); if ( *data != 'b' ) goto m_GET4;
            READ_DATA( GET_S7 ); if ( *data != 'S' ) goto m_GET4;
            READ_DATA( GET_S8 ); if ( *data != 'o' ) goto m_GET4;
            READ_DATA( GET_S9 ); if ( *data != 'c' ) goto m_GET4;
            READ_DATA( GET_SA ); if ( *data != 'k' ) goto m_GET4;
            READ_DATA( GET_SB ); if ( *data != 'e' ) goto m_GET4;
            READ_DATA( GET_SC ); if ( *data != 't' ) goto m_GET4;
            READ_DATA( GET_SD ); if ( *data != '-' ) goto m_GET4;
            READ_DATA( GET_SE ); if ( *data == 'K' ) goto GET_WebSocket_K;
                                 if ( *data != 'V' ) goto m_GET4;
            READ_DATA( GET_SF ); if ( *data != 'e' ) goto m_GET4;
            READ_DATA( GET_SG ); if ( *data != 'r' ) goto m_GET4;
            READ_DATA( GET_SH ); if ( *data != 's' ) goto m_GET4;
            READ_DATA( GET_SI ); if ( *data != 'i' ) goto m_GET4;
            READ_DATA( GET_SJ ); if ( *data != 'o' ) goto m_GET4;
            READ_DATA( GET_SK ); if ( *data != 'n' ) goto m_GET4;
            READ_DATA( GET_SL ); if ( *data != ':' ) goto m_GET4;
            READ_DATA( GET_SM ); if ( *data != ' ' ) goto m_GET4;
            READ_TXT_UINT( websocket_version );
            goto m_GET4;

          GET_WebSocket_K: // 'Sec-WebSocket-K'
            READ_DATA( GET_SO ); if ( *data != 'e' ) goto m_GET4;
            READ_DATA( GET_SP ); if ( *data != 'y' ) goto m_GET4;
            READ_DATA( GET_SQ ); if ( *data != ':' ) goto m_GET4;
            READ_DATA( GET_SR ); if ( *data != ' ' ) goto m_GET4;
            READ_STRING_UNTIL_SPACE( websocket_key );
            goto m_GET4;

          // commands in raw (non websocket) mode ------------------------------------------------------------------------
          l_req_identity:
            READ_UINT( tmp_PI64_0, l_req_identity_0 );
            tmp_VecPairUsrIdString.reserve( tmp_PI64_0 );
            tmp_VecPairUsrIdString.resize( 0 );
            while ( tmp_VecPairUsrIdString.size() < tmp_PI64_0 ) {
                READ_USR_ID( tmp_UsrId_0, l_req_identity_3 );
                READ_STRING( tmp_CbString_1, l_req_identity_4 );
                tmp_VecPairUsrIdString << std::make_pair( tmp_UsrId_0, std::string( tmp_CbString_1 ) );
            }

            session ? session->recv_req_identity( tmp_VecPairUsrIdString ) :
                      corrupted_command( CorruptedCommand::command_needs_a_session, {} );
            READ_DATA( l_req_identity_c );
            continue;

          l_ans_identity:
            READ_STRING( tmp_CbString_0, l_ans_identity_0 );
            session ? session->recv_ans_identity( tmp_CbString_0 ) :
                      corrupted_command( CorruptedCommand::command_needs_a_session, {} );
            READ_DATA( l_ans_identity_c );
            continue;

          l_ack_identity:
            READ_UINT( tmp_str_length, l_ack_identity_0 );
            session ? session->recv_ack_identity( tmp_str_length ) :
                      corrupted_command( CorruptedCommand::command_needs_a_session, {} );
            READ_DATA( l_ack_identity_c );
            continue;

          l_req_auth_std:
            READ_STRING( tmp_CbString_0, l_req_auth_std_0 );
            READ_STRING( tmp_CbString_1, l_req_auth_std_1 );
            session ? session->recv_req_auth_std( tmp_CbString_0, tmp_CbString_1 ) :
                      corrupted_command( CorruptedCommand::command_needs_a_session, {} );
            READ_DATA( l_req_auth_std_c );
            continue;

          l_req_auth_tok:
            TODO;
            READ_DATA( l_req_auth_tok_c );
            continue;

          l_req_auth_yub:
            TODO;
            READ_DATA( l_req_auth_yub_c );
            continue;

          l_ans_auth:
            TODO;
            READ_DATA( l_ans_auth_c );
            continue;

          l_graph_cmd:
            READ_UINT( tmp_PI64_0, l_graph_cmd_0 );
            READ_UINT( tmp_PI64_1, l_graph_cmd_1 );
            tmp_VecPT.clear();
            tmp_VecPT.reserve( tmp_PI64_1 );
            while ( tmp_PI64_1-- ) {
                READ_UINT( tmp_PI64_2, l_graph_cmd_2 );
                tmp_VecPT << tmp_PI64_2;
            }
            READ_STRING( tmp_CbString_0, l_graph_cmd_3 );
            session ? session->recv_graph_cmd( tmp_PI64_0, tmp_VecPT, tmp_CbString_0 ) :
                      corrupted_command( CorruptedCommand::command_needs_a_session, {} );
            READ_DATA( l_graph_cmd_c );
            continue;

          l_res_graph:
            TODO;
            READ_DATA( l_res_graph_c );
            continue;

          l_req_content:
            READ_INODE( tmp_Inode_0, l_req_content_0 );
            session ? session->recv_req_content( tmp_Inode_0 ) :
                      corrupted_command( CorruptedCommand::command_needs_a_session, tmp_Inode_0 );
            READ_DATA( l_req_content_c );
            continue;

          l_req_changes:
            TODO;
            READ_DATA( l_req_changes_c );
            continue;

          l_new_changes:
            READ_INODE ( tmp_Inode_0        , l_new_changes_0 );
            READ_STRING( tmp_CbString_0          , l_new_changes_1 );
            session ? session->recv_new_changes( tmp_Inode_0, tmp_CbString_0 ) :
                      corrupted_command( CorruptedCommand::command_needs_a_session, tmp_Inode_0 );
            READ_DATA( l_new_changes_c );
            continue;

          l_ack_changes:
            TODO;
            READ_DATA( l_ack_changes_c );
            continue;

          l_corrupted_data:
            READ_UINT( tmp_PI64_0, l_corrupted_data_0 );
            std::cerr << "Corrupted data code = " << tmp_PI64_0 << std::endl;
            READ_DATA( l_corrupted_data_c );
            continue;

          l_corrupted_command:
            TODO;
            READ_DATA( l_corrupted_command_c );
            continue;

          l_incompatible_do_type:
            TODO;
            READ_DATA( l_incompatible_do_type_c );
            continue;

          l_unknown_do_type:
            TODO;
            READ_DATA( l_unknown_do_type_c );
            continue;

          l_inode_does_not_exist:
            READ_INODE( tmp_Inode_0, l_inode_does_not_exist_0 );
            WARNING( "inode_does_not_exist" );
            READ_DATA( l_inode_does_not_exist_c );
            continue;


            //          l_new_changes:
            //            READ_INODE  ( tmp_Inode_0, l_new_changes_0 );
            //            READ_UINT   ( tmp_nb_int , l_new_changes_1 );
            //            tmp_dst.clear(); tmp_dst.reserve( tmp_nb_int + 1 );
            //            while ( tmp_nb_int-- ) {
            //                READ_UINT( tmp_dev, l_new_changes_2 );
            //                tmp_dst << tmp_dev;
            //            }
            //            tmp_dst << DevId( 0 );
            //            READ_STRING ( tmp_CbString_0  , l_new_changes_3 );

            //            session ? session->use_changes( tmp_Inode_0, tmp_dst, tmp_CbString_0 ) :
            //                      corrupted_command( CorruptedCommand::command_needs_a_session, tmp_Inode_0 );
            //            READ_DATA( l_new_changes_c );
            //            continue;

        } // end of while

      // read the websocket frames
      ws_loop:
        // byte 0
        websocket_byte_0 = *data;
        // byte 1
        READ_DATA( ws_0 );
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

        READ_DATA( ws_1 ); websocket_masking_key[ 0 ] = *data;
        READ_DATA( ws_2 ); websocket_masking_key[ 1 ] = *data;
        READ_DATA( ws_3 ); websocket_masking_key[ 2 ] = *data;
        READ_DATA( ws_4 ); websocket_masking_key[ 3 ] = *data;

        // payload data (with mask)
        if ( websocket_payload_length ) {
            websocket_data = CbString( IKnowWhatIDo(), buffer, data + 1 - buffer->data, websocket_payload_length );
            pending_cb_queue = buffer;
            tmp_shift_read = 0; // it is not used to read an integer, so we can use it for offset
            while( websocket_payload_length ) {
                --websocket_payload_length;
                READ_DATA( ws_5 );
                *data ^= websocket_masking_key[ ( tmp_shift_read++ ) % 4 ];
            }
            pending_cb_queue = 0;
        }

      ws_parse_frame_data:
        // use opcode
        switch ( websocket_byte_0 & 0x0F ) {
        case 0x1: if ( not websocket_msg() ) return false; break; // text
        case 0x2: if ( not websocket_msg() ) return false; break; // binary
        case 0x8: websocket_connection_closed(); return false;
        case 0x9: websocket_ping(); break;
        case 0xA: websocket_pong(); break;
        default:
            PRINT( websocket_byte_0 & 0x0F );
            WARNING( "unknown websocket opcode" );
            return false;
        }

        READ_DATA( ws_7 );
        goto ws_loop;

      ws_payload_length_16_bits:
        READ_DATA( ws_8 ); websocket_payload_length = SI64( *data ) << 8;
        READ_DATA( ws_9 ); websocket_payload_length += *data;
        goto ws_masking_key;

      ws_payload_length_64_bits:
        READ_DATA( ws_A ); websocket_payload_length  = SI64( *data ) << 56;
        READ_DATA( ws_B ); websocket_payload_length += SI64( *data ) << 48;
        READ_DATA( ws_C ); websocket_payload_length += SI64( *data ) << 40;
        READ_DATA( ws_D ); websocket_payload_length += SI64( *data ) << 32;
        READ_DATA( ws_E ); websocket_payload_length += SI64( *data ) << 24;
        READ_DATA( ws_F ); websocket_payload_length += SI64( *data ) << 16;
        READ_DATA( ws_G ); websocket_payload_length += SI64( *data ) << 8;
        READ_DATA( ws_H ); websocket_payload_length += *data;
        goto ws_masking_key;

      ws_wo_mask:
        if ( websocket_payload_length == 126 )
            goto ws_payload_length_16_bits_wo_mask;
        if ( websocket_payload_length == 127 )
            goto ws_payload_length_64_bits_wo_mask;

      payload_data_wo_mask:
        READ_SIZED( websocket_data, websocket_payload_length, payload_data_wo_mask );
        goto ws_parse_frame_data;

      ws_payload_length_16_bits_wo_mask:
        READ_DATA( ws_I ); websocket_payload_length  = SI64( *data ) << 8;
        READ_DATA( ws_J ); websocket_payload_length += *data;
        goto payload_data_wo_mask;

      ws_payload_length_64_bits_wo_mask:
        READ_DATA( ws_K ); websocket_payload_length  = SI64( *data ) << 56;
        READ_DATA( ws_L ); websocket_payload_length += SI64( *data ) << 48;
        READ_DATA( ws_M ); websocket_payload_length += SI64( *data ) << 40;
        READ_DATA( ws_N ); websocket_payload_length += SI64( *data ) << 32;
        READ_DATA( ws_O ); websocket_payload_length += SI64( *data ) << 24;
        READ_DATA( ws_P ); websocket_payload_length += SI64( *data ) << 16;
        READ_DATA( ws_Q ); websocket_payload_length += SI64( *data ) << 8;
        READ_DATA( ws_R ); websocket_payload_length += *data;
        goto payload_data_wo_mask;
    }

    ///
    bool websocket_handshake() {
        std::string key = websocket_key;
        key += "258EAFA5-E914-47DA-95CA-C5AB0DC85B11";

        unsigned char hash[ SHA_DIGEST_LENGTH ];
        SHA1( (unsigned char *)key.c_str(), key.size(), hash );
        key = base64_encode( hash, SHA_DIGEST_LENGTH );

        // anwser
        TxtStream<CbQueue> os( &cq );
        os << "HTTP/1.1 101 Switching Protocols\r\n"; // WebSocket Protocol Handshake
        os << "Connection: Upgrade\r\n";
        // os << "Sec-WebSocket-Origin: https://localhost\r\n";
        // os << "Sec-WebSocket-Location: wss://localhost\r\n";
        os << "Sec-WebSocket-Accept: " << key << "\r\n";
        // os << "Sec-WebSocket-Protocol: myprotocol\r\n";
        os << "Upgrade: WebSocket\r\n";
        os << "\r\n";

        return true;
    }

    /// called when parsed found that the data is corrupted
    bool corrupted_data( PI8 code ) {
        bq << DbCommands::corrupted_data;
        bq << code;
        return false;
    }

    void corrupted_command( PI8 code, Inode inode ) {
        bq << DbCommands::corrupted_command
           << code
           << inode.dev
           << inode.num;
    }

    bool websocket_msg() {
        BinStream<CbString> br( &websocket_data );
        while ( not br.empty() ) {
            PI8 cmd = br.read_byte();

            switch( cmd ) {
            case PI8( DbCommands::end ):
                return websocket_connection_close( WebsocketStatusCode::normal );

            case PI8( DbCommands::req_auth_std ): {
                CbString user = br.read();
                CbString pass = br.read();
                session ? session->recv_req_auth_std( user, pass ) :
                          corrupted_command( CorruptedCommand::command_needs_a_session, {} );
                continue;
            }

            default:
                PRINT( int( cmd ) );
                WARNING( "Unexpected command" );
                return websocket_connection_close( WebsocketStatusCode::normal );
            }
        }
        return true;
    }

    bool websocket_connection_close( WebsocketStatusCode why ) {
        PI8 data[] = { 0x80 /* FIN */ + 0x08 /* opcode close */, 0x80 /* mask that seems to be mandatory */ + 0x02 /* payload length */,
                       1, 2, 3, 4 /* masking key */, PI8( ( int( why ) >> 8 ) ^ 1 ), PI8( ( int( why ) >> 0 ) ^ 2 ) /* payload */ };
        this->cw.write_some( data, sizeof( data ) );
        return false;
    }

    bool websocket_connection_closed() {
        // WARNING( "TODO: rfc6455 says that we MUST answer by a close with the same opcode" );
        return false;
    }

    void websocket_ping() {
        PI64 pl = websocket_data.size();
        this->cw.write_byte( 0x80 /* FIN */ + 0x0A /* opcode pong */ );
        if ( pl <= 125 ) {
            this->cw.write_byte( 0x80 /* mask that seems to be mandatory */ + pl /* payload length */ );
        } else if ( pl < 0x10000 ) {
            this->cw.write_byte( 0x80 /* mask that seems to be mandatory */ + 126 /* payload length */ );
            this->cw.write_byte( pl >> 8 );
            this->cw.write_byte( pl >> 0 );
        } else {
            this->cw.write_byte( 0x80 /* mask that seems to be mandatory */ + 127 /* payload length */ );
            this->cw.write_byte( pl >> 56 );
            this->cw.write_byte( pl >> 48 );
            this->cw.write_byte( pl >> 40 );
            this->cw.write_byte( pl >> 32 );
            this->cw.write_byte( pl >> 24 );
            this->cw.write_byte( pl >> 16 );
            this->cw.write_byte( pl >> 8  );
            this->cw.write_byte( pl >> 0  );
        }

        // masking key
        this->cw.write_byte( 0 );
        this->cw.write_byte( 1 );
        this->cw.write_byte( 2 );
        this->cw.write_byte( 3 );

        // payload
        PI64 i = 0;
        websocket_data.data_visitor( [ this, &i ]( const PI8 *b, const PI8 *e ) {
            for( ; b < e; ++b )
                this->cw.write_byte( *b ^ ( i % 4 ) );
        } );
    }

    void websocket_pong() {
    }

    bool websocket_mode() const {
        return websocket_key.size();
    }

    //    void set_username_and_password( std::string user, std::string pass ) {
    //        if ( session and session->connection == this )
    //            session->connection = 0;

    //        // check
    //        if ( String( user ) == "Satanas" ) {
    //            // ko
    //            session = 0;

    //            // ko answer
    //            bq << PI8( Answer::set_user );
    //            bq << 0u;
    //            return;
    //        }

    //        // ok
    //        session = new Session( Inode( KnownInodes::root_home ) );

    //        // ok answer
    //        bq << PI8( Answer::set_user );
    //        bq << session->home_dir;
    //    }

    bool http_get( CbString url ) {
        std::cout << "TODO: GET requests that are not upgraded to websockets" << std::endl;
        return false;
    }


    virtual void flush() {
        if ( not cq.empty() ) {
            if ( websocket_mode() ) {
                PI64 pl = cq.size();
                if ( pl <= 125 ) {
                    PI8 header[] = { 0x80 /* FIN */ + 0x02 /* binary */, PI8( 0 * 0x80 /* no mask */ + pl /* payload length */ ) };
                    this->cw.write_some( header, sizeof( header ) );
                } else if ( pl < 0x10000 ) {
                    PI8 header[] = { 0x80 /* FIN */ + 0x02 /* binary */, 0 * 0x80 /* no mask */ + 126 /* payload length */,
                                     PI8( pl >> 8 ), PI8( pl >> 0 ) };
                    this->cw.write_some( header, sizeof( header ) );
                } else {
                    PI8 header[] = { 0x80 /* FIN */ + 0x02 /* binary */, 0 * 0x80 /* no mask */ + 127 /* payload length */,
                                     PI8( pl >> 56 ), PI8( pl >> 48 ), PI8( pl >> 40 ), PI8( pl >> 32 ),
                                     PI8( pl >> 24 ), PI8( pl >> 16 ), PI8( pl >>  8 ), PI8( pl >>  0 ) };
                    this->cw.write_some( header, sizeof( header ) );
                }
            }

            this->cw.write_some( std::move( cq ) );
        }

        TP::flush();
    }

    virtual CbQueue *cb_queue() {
        return &cq;
    }


    // parsing
    CbString            url;
    CbString            websocket_key;
    int                 websocket_version;
    int                 websocket_byte_0;
    unsigned            websocket_payload_length;
    char                websocket_masking_key[ 4 ];
    CbString            websocket_data;

    // helpers for continuation
    void               *inp_cont;
    Buffer             *pending_cb_queue;    ///< if we need to add the current buffer to a previous one
    unsigned            tmp_shift_read;      ///< used by READ_UINT, READ_INT, ...
    unsigned            tmp_str_length;      ///< used by READ_STRING

    //    unsigned            tmp_nb_dev;          ///< used by READ_SET_OF_PATCH_ID
    //    unsigned            tmp_nb_int;          ///< used by READ_SET_OF_PATCH_ID
    //    PI64                tmp_dev;             ///< used by READ_SET_OF_PATCH_ID
    //    PI64                tmp_min;             ///< used by READ_SET_OF_PATCH_ID
    //    PI64                tmp_max;             ///< used by READ_SET_OF_PATCH_ID
    //    Vec<DevId>          tmp_dst;             ///<
    //    CbString            tmp_name;            ///< used by READ_DO_TYPE
    //    SetOfPatchId        tmp_set_of_patch_id; ///<

    CbString            tmp_CbString_0;      ///<
    CbString            tmp_CbString_1;      ///<
    Inode               tmp_Inode_0;         ///<
    Inode               tmp_Inode_1;         ///<
    UsrId               tmp_UsrId_0;         ///<
    PI64                tmp_PI64_0;          ///<
    PI64                tmp_PI64_1;          ///<
    PI64                tmp_PI64_2;          ///<
    Vec<std::pair<UsrId,std::string>>
                        tmp_VecPairUsrIdString; ///<
    Vec<PT>             tmp_VecPT;           ///<

    // tasks that depend on this (if this is deleted, we need to remove those tasks)
    // TaskReceiver        task_receiver;    ///<

    // Connection stuff
    DbSession          *session;
    CbQueue             cq;               ///< temporary storage for this->cw (we cannot use it directly due to websocket, with the need to prepend headers)
    BinStream<CbQueue>  bq;               ///< to write in cq in binary format
};

TcpRawConnection *new_db_connection( DbSession *s, int fd, bool server ) {
    return new DbConnection( s, fd, server );
}


} // namespace RiotBase
