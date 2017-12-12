#include "../../System/ReadContHelpers.h"
#include "../../System/VectorClock.h"
#include "../../System/Log.h"
#include "../../EvLoop/Gev.h"

#include "TrackerConnection.h"
#include "TrackerCommands.h"
#include "TrackerSession.h"
#include "../Tracker.h"

namespace RiotBase {

TrackerConnection::TrackerConnection( int fd, TrackerSession *session ) : BufferedSocket( fd ), pending_cb_queue( 0 ), inp_cont( 0 ), session( session ) {
    new ( parsing_context ) ParsingContext;
}

TrackerConnection::~TrackerConnection() {
    session->connections.remove_first_unordered( this );
}

DevId TrackerConnection::get_dev_id() const {
    return session->dev;
}

bool TrackerConnection::parse( Buffer *buffer ) {
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
        case PI8( TrackerCommands::login ): {
            struct PC : ParsingContext { CbString user, pass; PT string_size, shift; };
            BEG_CMD_PC_READ( login );
            READ_STRING_HASHED( PCP->user, PCP->string_size, PCP->shift );
            READ_STRING_HASHED( PCP->pass, PCP->string_size, PCP->shift );
            CHK_CMD_PC_READ;
            session->recv_login( PCP->user, PCP->pass );
            END_CMD_PC_READ;
            break;
        }
        case PI8( TrackerCommands::bind_obj ): {
            struct PC : ParsingContext { Inode inode; VectorClock spm; DevId dev; PT size, shift; };
            BEG_CMD_PC_READ( bind_obj );
            READ_INODE_HASHED( PCP->inode, PCP->shift );
            READ_VectorClock_HASHED( PCP->spm, PCP->dev, PCP->size, PCP->shift );
            CHK_CMD_PC_READ;
            session->recv_bind_obj( PCP->inode, PCP->spm );
            END_CMD_PC_READ;
            break;
        }
        case PI8( TrackerCommands::get_dev_info ): {
            struct PC : ParsingContext { DevId dev; PT shift; };
            BEG_CMD_PC_READ( get_dev_info );
            READ_DEV_ID_HASHED( PCP->dev, PCP->shift );
            CHK_CMD_PC_READ;
            session->recv_get_dev_info( PCP->dev );
            END_CMD_PC_READ;
            break;
        }
        case PI8( TrackerCommands::quit ): {
            struct PC : ParsingContext {};
            BEG_CMD_PC_READ( quit );
            CHK_CMD_PC_READ;
            if ( session->usr == KnownUsrId::root_usr )
                gev->stop();
            END_CMD_PC_READ;
            break;
        }
        case PI8( TrackerCommands::corrupted_data ): {
            struct PC : ParsingContext { PI64 code; PT shift; };
            BEG_CMD_PC_READ( corrupted_data );
            READ_UINT_HASHED( PCP->code, PCP->shift );
            CHK_CMD_PC_READ;
            session->recv_corrupted_data( PCP->code );
            END_CMD_PC_READ;
            break;
        }
        default:
            DISP_ERROR( "tracker server says: corrupted data" );
            session->send_corrupted_data();
            return false;
        }

        READ_DATA;
    }

    return true;
}

DevId TrackerConnection::dev_id() {
    return session->dev;
}

UsrId TrackerConnection::usr_id() {
    return session->usr;
}


} // namespace RiotBase
