#include "../../System/NetStream.h"
#include "../../System/Random.h"
#include "../../System/Task.h"

#include "TrackerServerTcpConnection.h"
#include "TrackerServerUdpConnection.h"
#include "TrackerServerSession.h"
#include "../Tracker.h"

namespace RiotBase {

TrackerServerSession::TrackerServerSession( Tracker *tracker ) : tracker( tracker ) {
    verification_tag = 0;

    // caller when we expect an answer but nothing comes at all
    alive_check = task_receiver << Task::New( [ this ]() {
        this->tracker->reg_session_to_del( this );
    }, Task::DO_NOT_DELETE_AFTER_EXECUTION );
}

TrackerServerSession::~TrackerServerSession() {
    for( TrackerServerTcpConnection *c : tcp_connections ) delete c;
    for( TrackerServerUdpConnection *c : udp_connections ) delete c;
    if ( token.size() ) tracker->token_to_usr_id.erase( token );
    delete alive_check;
}

bool TrackerServerSession::connected() const {
    return tcp_connections.size() or udp_connections.size();
}

void TrackerServerSession::activate() {
    alive_check->desactivate();
}

void TrackerServerSession::try_sctp_candidate( const CbString &addr, unsigned port ) {
    // make a new verification_tag
    //    static Random r;
    //    do {
    //        r.get( &verification_tag, sizeof verification_tag );
    //    } while ( verification_tag == 0 );
    //    PRINT( verification_tag );

    //    //
    //    NetStream<TrackerServerUdpConnection> ns( tracker->udp_socket->connection( InetAddress( addr, port ), false ) );
    //    PRINT( InetAddress( addr, port ) );
    //    ns << PI8 ( 1 ); // type
    //    ns << PI8 ( 0 ); // chunk flags
    //    ns << PI16( 0 ); // chunk length

    //    ns << verification_tag; // Initiate Tag
    //    ns << PI32( 0 ); // Advertised Receiver Window Credit (a_rwnd) ???
    //    ns << PI16( 1 ); // Number of outbound streams
    //    ns << PI16( 16 ); // Number of inbound streams
    //    ns << verification_tag; // Initial TSN

    //    ns.buf->flush();
}

void TrackerServerSession::add_tcp_connection( TrackerServerTcpConnection *connection ) {
    bool pc = connected();
    tcp_connections << connection;
    if ( not pc )
        on_reconnect();
}

void TrackerServerSession::add_udp_connection( TrackerServerUdpConnection *connection ) {
    bool pc = connected();
    udp_connections << connection;
    if ( not pc )
        on_reconnect();
}

void TrackerServerSession::rem_tcp_connection( TrackerServerTcpConnection *connection ) {
    tcp_connections.remove_first_unordered( connection );
    if ( not tcp_connections.size() )
        make_alive_check( 5 ); // we expect the client to try to reconnect. If it is not the case after a given time, we will assume that this is the end
}

void TrackerServerSession::rem_udp_connection( TrackerServerUdpConnection *connection ) {
    udp_connections.remove_first_unordered( connection );
}

void TrackerServerSession::on_reconnect() {
    if ( pending_bound_inode.size() )
        IMPORTANT_TODO;
}

void TrackerServerSession::make_alive_check( double delay ) {
    tracker->timed_task_list_1mn_res_1s.add_task( alive_check, delay );
}


} // namespace RiotBase
