#pragma once

#include "../System/LazyNew.h"
#include "../System/Vec.h"
#include "EvLoop.h"
#include <string>
#include <map>

namespace RiotBase {
class DnsSocket;

/**
*/
class Dns {
public:
    struct Ipv4 { PI8  data[ 4 ]; };
    struct Ipv6 { PI16 data[ 8 ]; };

    /// error codes
    enum {
        NO_RESOLV_ENTRY
    };

    Dns( EvLoop *ev = 0 );
    void query( const std::string &addr, std::function<void( const Vec<Ipv4> &ipv4, const Vec<Ipv6> &ipv6 )> ok, std::function<void(int)> err );

protected:
    friend struct DnsConnection;

    struct Wcb {
        Vec<unsigned>                                                            num;
        Vec<std::function<void( int )>>                                          err;
        Vec<std::function<void( const Vec<Ipv4> &ipv4, const Vec<Ipv6> &ipv6 )>> ok;
    };
    struct Entry {
        std::string name;
        Vec<Ipv4>   ipv4;
        Vec<Ipv6>   ipv6;
        unsigned    ttl;
    };

    std::map<std::string,Entry> cached_entries; //
    std::map<std::string,Wcb>   waiting_reqs;   //
    Vec<std::string>            server_ips;     // ip coming from /etc/resolv.conf
    unsigned                    cur_server;     // num user server (in server_ips)
    DnsSocket                  *socket;         // socket to communicate with DNS servers
    EvLoop                     *ev;
};

extern LazyNew<Dns> g_dns;

} // namespace RiotBase
