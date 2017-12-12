#pragma once

#include <arpa/inet.h>
#include "TypeConfig.h"
#include "Print.h"

namespace RiotBase {

/** IPv6 socket address (with port)
*/
class InetAddress {
public:
    InetAddress( std::string ip, unsigned port ); // IPVx
    InetAddress( const PI16 *ip, unsigned port );       // IPV6 only
    InetAddress( const PI8  *ip, unsigned port );       // IPV4 only
    InetAddress();

    void         write_to_stream( std::ostream &os ) const;
    unsigned     port           () const;
    std::string  ip             () const;

    bool         operator<      ( const InetAddress &b ) const;

    sockaddr_in6 sa;
};

} // namespace RiotBase
