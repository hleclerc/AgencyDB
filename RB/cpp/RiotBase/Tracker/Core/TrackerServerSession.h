#pragma once

#include <boost/intrusive/avl_set.hpp>
#include "../../System/TaskReceiver.h"
#include "../../System/UsrId.h"
#include "../../System/Vec.h"

namespace RiotBase {
class TrackerServerTcpConnection;
class TrackerServerUdpConnection;
class CbString;
class Tracker;

/**
*/
class TrackerServerSession {
public:
    using AvlMemberHook = boost::intrusive::avl_set_member_hook<>;

    struct DevIdCmp {
        bool operator()( const TrackerServerSession &a, const TrackerServerSession &b ) const { return a.dev < b.dev; }
        bool operator()( const TrackerServerSession &a, const DevId                &b ) const { return a.dev < b    ; }
        bool operator()( const DevId                &a, const TrackerServerSession &b ) const { return a     < b.dev; }
    };

    TrackerServerSession( Tracker *tracker );
    ~TrackerServerSession();

    //
    template<class Op> bool           apply_on_best_conn( const Op &op );
    bool                              connected         () const;
    void                              activate          ();               ///< say that this session is alive

    void                              try_sctp_candidate( const CbString &addr, unsigned port );

    void                              add_tcp_connection( TrackerServerTcpConnection *connection );
    void                              add_udp_connection( TrackerServerUdpConnection *connection );
    void                              rem_tcp_connection( TrackerServerTcpConnection *connection );
    void                              rem_udp_connection( TrackerServerUdpConnection *connection );
    void                              on_reconnect      ();

    void                              make_alive_check  ( double delay );

    Vec<std::pair<DevId,Inode>>       pending_bound_inode;
    AvlMemberHook                     member_hook_dev_id;
    PI32                              verification_tag;
    Vec<TrackerServerTcpConnection *> tcp_connections;
    Vec<TrackerServerUdpConnection *> udp_connections;
    TaskReceiver                      task_receiver;
    Task                             *alive_check;
    Tracker                          *tracker;
    std::string                       token;
    DevId                             dev;
    UsrId                             usr;
};

template<class Op>
bool TrackerServerSession::apply_on_best_conn( const Op &op ) {
    if ( udp_connections.size() ) {
        op( udp_connections[ 0 ] );
        return true;
    }
    if ( tcp_connections.size() ) {
        op( tcp_connections[ 0 ] );
        return true;
    }
    return false;
}

} // namespace RiotBase
