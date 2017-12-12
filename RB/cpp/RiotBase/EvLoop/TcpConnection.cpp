#include "TcpConnection.h"

namespace RiotBase {

TcpConnection::TcpConnection() : inp_buff( nullptr ) {
}

TcpConnection::TcpConnection( VtableOnly vo ) : Event( vo ) {
}

TcpConnection::~TcpConnection() {
    if ( inp_buff )
        dec_ref( inp_buff );
}


} // namespace RiotBase
