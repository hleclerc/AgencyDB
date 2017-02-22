#include "UdpSocketAncestor.h"
#include "UdpConnection.h"
#include "../System/Log.h"

namespace RiotBase {

UdpConnection::UdpConnection() : prev_with_pending_output( nullptr ) {
}

void UdpConnection::out() {
    // send queued messages (not the pending one, which is not assumed to be finished)
    if ( MsgQueue::Msg *cur = pending_output.first ) {
        for( MsgQueue::Msg *nxt; cur; cur = nxt ) {
            if ( not send_data( cur->data, cur->size ) )
                break;
            nxt = cur->next;
            free( cur );
        }
        if ( not cur ) pending_output.last = cur;
        pending_output.first = cur;
    }
}

bool UdpConnection::send( MsgQueue::Msg *msg ) {
    if ( not send_data( msg->data, msg->size ) ) {
        pending_output << msg;
        return false;
    }
    msg->size = 0;
    return true;
}

bool UdpConnection::send_data( const void *data, PT size ) {
    ssize_t real = sendto( udp_socket->fd, data, size, MSG_NOSIGNAL, (const sockaddr *)&inet_address.sa, sizeof inet_address.sa );
    if ( real <= 0 ) {
        #warning EINTR ?
        if ( real < 0 and ( errno == EAGAIN or errno == EWOULDBLOCK ) ) {
            udp_socket->connections_with_pending_output << this;
            udp_socket->wait_for_more_out();
        } else {
            udp_socket->hup_error();
        }
        return false;
    }
    if ( PT( real ) != size )
        DISP_ERROR( "real != size in {}:{}", __FILE__, __LINE__ );
    return true;
}

} // namespace RiotBase
