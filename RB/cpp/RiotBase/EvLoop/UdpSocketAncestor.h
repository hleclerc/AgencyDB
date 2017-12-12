#pragma once

#include "../System/CycleEndedList.h"
#include "UdpConnection.h"
#include "Event.h"

namespace RiotBase {

/**
*/
class UdpSocketAncestor : public Event {
public:
    struct Prev_to_be_flushed { UdpConnection *&operator()( UdpConnection *obj ) const { return obj->prev_with_pending_output; } };
    using TConnFlushList = CycleEndedList<UdpConnection,Prev_to_be_flushed>;

    struct Cmp {
        bool operator()( const UdpConnection &a, const UdpConnection &b ) const { return a.inet_address < b.inet_address; }
        bool operator()( const UdpConnection &a, const InetAddress   &b ) const { return a.inet_address < b             ; }
        bool operator()( const InetAddress   &a, const UdpConnection &b ) const { return a              < b.inet_address; }
    };
    using CMap = boost::intrusive::avl_multiset<UdpConnection,
        boost::intrusive::compare<Cmp>,
        boost::intrusive::member_hook<
            UdpConnection,
            boost::intrusive::avl_set_member_hook<>,
            &UdpConnection::member_hook_addr
        >
    >;

    UdpSocketAncestor();

    void           bind         ( unsigned port );

    virtual void   on_bind_error( const char *msg );

    CMap           connection_map;   ///< address -> UdpConnection *
    TConnFlushList connections_with_pending_output;
};

} // namespace RiotBase
