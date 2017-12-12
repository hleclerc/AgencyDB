#pragma once

#include "../../EvLoop/UdpDtlsConnection.h"
//#include "../../System/ApplyVa.h"
//#include "../../System/Inode.h"
#include "../../System/UsrId.h"
//#include "../../System/Vec.h"
//#include <functional>

namespace RiotBase {
//class VectorClock;
//class CbString;
class Tracker;

/**
*/
class TrackerServerSession : public UdpDtlsConnection {
public:
    TrackerServerSession( Tracker *tracker, InetAddress inet_address, UdpSocket *udp_socket, SSL_CTX *ctx );
    ~TrackerServerSession();

    virtual bool parse( Buffer *buff );

    //    void                       add_connection           ( TrackerConnection *connection );

    //    void                       send_corrupted_data      ( PI64 code = 0 );
    //    void                       send_new_inode_connection( Inode inode, DevId dev, const VectorClock &known );
    //    void                       send_req_patch_data      ( Inode inode, DevId dev, const VectorClock &known );

    //    template<class... Args>
    //    void                       send_error_msg           ( PI32 msg_id, const Args& ...data );

    //    void                       flush                    ();

    //    // DevId                      dev_id                   () { return dev; }
    //    // UsrId                      usr_id                   () { return usr; }

    //protected:
    //    friend class TrackerConnection;
    //    friend class Tracker;

    //    void                       recv_login               ( const CbString &user, const CbString &pass );
    //    void                       recv_bind_obj            ( Inode inode, const VectorClock &known );
    //    void                       recv_get_dev_info        ( DevId dev );
    //    void                       recv_corrupted_data      ( PI64 code );

    //    void                       _send_error_msg          ( CbQueue &&data );

    DevId                      dev;
    UsrId                      usr;
    //    std::string                addr;
    //    unsigned                   port;
    //    Tracker                   *tracker;
    //    Vec<TrackerConnection *>   connections;
    //    Vec<std::function<void()>> todo_when_connected;
    Tracker     *tracker;
};


//template<class... Args>
//void TrackerSession::send_error_msg( PI32 msg_id, const Args& ...data ) {
//    CbQueue cq;
//    BBQ bq( &cq );
//    bq << msg_id;
//    apply_va( [ &bq ]( const auto &val ) { bq << val; }, data... );
//    _send_error_msg( std::move( cq ) );
//}

} // namespace RiotBase
