#pragma once

#include "../SocketProtocol.h"

namespace RiotBase {

int listening_named_socket( const char *name, bool non_blocking = true ); ///< return a bound listening socket on named socket `name`
int listening_socket      ( SocketProtocol protocol, unsigned port, bool non_blocking = true ); ///< return a bound listening socket on port `port`
int accepting_socket      ( int listen_fd, bool non_blocking = true ); ///< return socket after accept. if != 0, addr must be a INET6_ADDRSTRLEN bytes array

int client_named_socket   ( const char *name, const char **err_msg = 0, bool non_blocking = true ); ///< if port == 0, addr is assumed to be a local named socket
int client_socket         ( SocketProtocol protocol, const char *addr, unsigned port, const char **err_msg = 0, bool non_blocking = true ); ///< if port == 0, addr is assumed to be a local named socket

} // namespace RiotBase
