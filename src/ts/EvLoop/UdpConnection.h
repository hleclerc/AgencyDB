#pragma once

#include <boost/intrusive/avl_set.hpp>
#include "../System/InetAddress.h"
#include "../System/MsgQueue.h"
#include "../System/Flush.h"

namespace RiotBase {
class UdpSocketAncestor;

/**
*/
class UdpConnection {
public:
    using AvlMemberHook = boost::intrusive::avl_set_member_hook<>;
    template<class Impl,class Conn> friend class UdpSocket;
    friend class UdpSocketAncestor;

    UdpConnection();

    void               init     ( UdpSocketAncestor *udp_socket, bool server ) {}

protected:

    void               out      (); ///< called when there is room in the output buffer AND this has been registered
    bool               send     ( MsgQueue::Msg *msg ); ///< try to send data to udp_socket. if not done, store the data in queued_output and after register the connection in udp_socket->connections_with_pending_output
    bool               send_data( const void *data, PT size ); ///< try to send data to udp_socket. if not done, store the data in queued_output and after register the connection in udp_socket->connections_with_pending_output

    UdpSocketAncestor *udp_socket;
    InetAddress        inet_address;                      ///< needed at least to sort items in udp_socket
    MsgQueue           pending_output;                    ///< output that failed to be sent to udp_socket
    AvlMemberHook      member_hook_addr;
    UdpConnection     *prev_with_pending_output;
};

} // namespace RiotBase
