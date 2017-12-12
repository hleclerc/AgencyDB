#include "../../System/ReadContHelpers.h"
#include "../../System/Log.h"
#include "DbCorruptedData.h"
#include "DbConnection.h"
#include "DbCommands.h"
#include "DbSession.h"
#include "../Db.h"

//#define RECV( CMD ) DISP_DEBUG( "Recv <- {} (from {}): {}", session->dev_id, session->db->get_dev_id(), CMD )


namespace RiotBase {

DbConnection::DbConnection( int fd, DbSession *session ) : TcpRawConnection( fd ), pending_cb_queue( 0 ), inp_cont( 0 ), session( session ) {
    new ( parsing_context ) ParsingContext;
}

DbConnection::~DbConnection() {
    DISP_DEBUG( "delete connection" );
    reinterpret_cast<ParsingContext *>( parsing_context )->~ParsingContext();
    session->connection = 0;
}

bool DbConnection::parse( Buffer *buffer ) {
    session->wake_up_if_necessary();

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

    //
    while ( true ) {
        switch ( *data ) {
        // auth
        case PI8( DbCommands::req_identity ): {
            struct PC : ParsingContext { Vec<std::pair<UsrId,CbString>> keys; PT string_size, shift, size; };
            BEG_CMD_PC_READ( req_identity );

            READ_UINT_HASHED( PCP->size, PCP->shift );

            PCP->keys.reserve( PCP->size );
            while ( PCP->keys.size() < PCP->size ) {
                PCP->keys.push_back();
                READ_USR_ID_HASHED( PCP->keys.back().first , PCP->shift );
                READ_STRING_HASHED( PCP->keys.back().second, PCP->string_size, PCP->shift );
            }

            CHK_CMD_PC_READ;
            session->recv_req_identity( PCP->keys );
            END_CMD_PC_READ;
            break;
        }
        case PI8( DbCommands::ans_identity ): {
            struct PC : ParsingContext { CbString msg; PT string_size, shift; };
            BEG_CMD_PC_READ( ans_identity );
            READ_STRING_HASHED( PCP->msg, PCP->string_size, PCP->shift );
            CHK_CMD_PC_READ;
            session->recv_ans_identity( PCP->msg );
            END_CMD_PC_READ;
            break;
        }
        case PI8( DbCommands::ack_identity ): {
            struct PC : ParsingContext { PI32 res; PT shift; };
            BEG_CMD_PC_READ( ack_identity );
            READ_UINT_HASHED( PCP->res, PCP->shift );
            CHK_CMD_PC_READ;
            session->recv_ack_identity( PCP->res );
            END_CMD_PC_READ;
            break;
        }
        case PI8( DbCommands::req_auth_std ):
            TODO;
            break;
        case PI8( DbCommands::req_auth_tok ):
            TODO;
            break;
        case PI8( DbCommands::req_auth_yub ):
            TODO;
            break;
        case PI8( DbCommands::ans_auth ):
            TODO;
            break;


        // content / patches
        case PI8( DbCommands::req_content ): 
            TODO;
            break;
        case PI8( DbCommands::req_changes ):
            TODO;
            break;
        case PI8( DbCommands::new_changes ):
            TODO;
            break;
        case PI8( DbCommands::ack_changes ):
            TODO;
            break;

        // graphs
        case PI8( DbCommands::graph_cmd ): {
            struct PC : ParsingContext { CbString msg; PT string_size, shift; };
            BEG_CMD_PC_READ( graph_cmd );
            READ_STRING_HASHED( PCP->msg, PCP->string_size, PCP->shift );
            CHK_CMD_PC_READ;
            session->recv_graph_cmd( PCP->msg );
            END_CMD_PC_READ;
            break;
        }
        case PI8( DbCommands::graph_ans ): {
            struct PC : ParsingContext { PI64 num; CbString msg; PT string_size, shift; };
            BEG_CMD_PC_READ( graph_ans );
            READ_UINT_HASHED( PCP->num, PCP->shift );
            READ_STRING_HASHED( PCP->msg, PCP->string_size, PCP->shift );
            CHK_CMD_PC_READ;
            session->recv_graph_ans( PCP->num, PCP->msg );
            END_CMD_PC_READ;
            break;
        }
        // errors
        case PI8( DbCommands::corrupted_data ):      
            TODO;
            break;
        case PI8( DbCommands::corrupted_command ):
            TODO;
            break;
        case PI8( DbCommands::incompatible_do_type ):
            TODO;
            break;
        case PI8( DbCommands::unknown_do_type ):
            TODO;
            break;
        case PI8( DbCommands::inode_does_not_exist ):
            TODO;
            break;

        case PI8( DbCommands::G ):
            TODO;
            break;
        case PI8( DbCommands::P ):
            TODO;
            break;

        // helpers
        case PI8( DbCommands::nop ):
        case PI8( DbCommands::nnop ):
            TODO;
            break;
        case PI8( DbCommands::end ):
            DISP_ERROR( "end" );
            return false;
        case PI8( DbCommands::quit ): {
            struct PC : ParsingContext {};
            BEG_CMD_PC_READ( quit );
            CHK_CMD_PC_READ;
            session->recv_quit();
            END_CMD_PC_READ;
            return false;
        }
        default:
            DISP_ERROR( "corrupted data" );
            session->send_corrupted_data( CorruptedData::unknown_command );
            return false;
        }

        READ_DATA;
    }

    return true;
}

DevId DbConnection::dev_id() const {
    return session->glo_dev_id;
}

UsrId DbConnection::usr_id() const {
    return session->glo_usr_id;
}


} // namespace RiotBase
