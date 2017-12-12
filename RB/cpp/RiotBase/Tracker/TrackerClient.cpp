#include "Core/TrackerServerCommands.h"
#include "Core/TrackerClientUdpSocket.h"
#include "TrackerClient.h"
#include "../EvLoop/Gev.h"

#define BEG_CMD_RAII( CMD ) \
    auto bs = connection->bs_out_auto_close(); \
    DISP_DEBUG( "send {} (from Client {})", #CMD, PT( this ) ); \
    bs << TrackerServerCommands::CMD

namespace RiotBase {

TrackerClient::TrackerClient( const std::string &server_addr, unsigned server_port ) {
    udp_socket = new TrackerClientUdpSocket( this );
    //    int n = 9000 + Date::current().nsec % 100; PRINT( n );
    //    udp_socket->bind( n );
    *gev << udp_socket;

    connection = udp_socket->connection( InetAddress{ server_addr, server_port } );
}

TrackerClient::~TrackerClient() {
    delete udp_socket;
}

void TrackerClient::send_credentials( const std::string &user, const std::string &pass ) {
    BEG_CMD_RAII( login ) << user << pass;
}

void TrackerClient::send_bind_obj( Inode loc_inode, const VectorClock &known, DevId cur_dev ) {
    BEG_CMD_RAII( bind_obj );
    loc_inode.write_to( bs, cur_dev );
    known.write_to( bs, cur_dev );
}

void TrackerClient::flush() {
    connection->flush();
}

void TrackerClient::on_ans_credentials( DevId dev_id, UsrId usr_id ) {
    // DISP_INFO( "ans credentials {}, {}", dev_id, usr_id );
}

void TrackerClient::on_bind_obj( Inode inode, DevId dev_id ) {
    // DISP_INFO( "bind obj {} with dev={}", inode, dev_id );
}

void TrackerClient::on_req_inode_data( Inode inode, DevId dev_id, const VectorClock &known ) {
    // DISP_INFO( "req inode data {} for dev={}, known={}", inode, dev_id, known );
}

void TrackerClient::set_dev_and_usr_id( DevId dev_id, UsrId usr_id ) {
    _dev_id = dev_id;
    _usr_id = usr_id;
    on_ans_credentials( dev_id, usr_id );
}


} // namespace RiotBase
