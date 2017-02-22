#include "../System/BinStream.h"
#include "../System/CmQueue.h"
#include "../System/Random.h"
#include "../System/Log.h"
#include "UdpRawConnection.h"
#include "UdpSocket.h"
#include "Dns.h"
#include "Gev.h"

#include <string.h>
#include <fstream>
#include <sstream>

namespace RiotBase {

namespace {

bool is_a_space( char c ) {
    return c == ' ' or c == '\t';
}

bool is_a_curl( char c ) {
    return ( c >= '0' and c <= '9' ) or
           ( c >= 'a' and c <= 'z' ) or
           ( c >= 'A' and c <= 'Z' ) or
           c == '.' or c == ':';
}

std::string read_cname( BinStream<CmString> bs ) {
    std::string name;
    while ( true ) {
        unsigned c = bs.read_byte();
        if ( not bs.buf->ack_read_some( c ) ) {
            DISP_ERROR( "Pb with DNS lookup (reading name in answers)" );
            return "";
        }
        if ( not c )
            break;
        if ( name.size() )
            name += '.';
        name += std::string( bs.ptr(), bs.ptr() + c );
        bs.skip_some( c );
    }
    return name;
}

} // namespace

struct DnsSocket;

// http://www.zytrax.com/books/dns/ch15/
struct DnsConnection : UdpRawConnection<DnsConnection,512> {
    void init( DnsSocket *socket, bool server );

    bool check_name( Dns::Entry *&entry, Dns::Wcb *&wcb, unsigned num_request, const std::string name ) {
        //
        if ( entry )
            return entry->name == name;

        //
        std::map<std::string,Dns::Wcb>::iterator iter = dns->waiting_reqs.find( name );
        if ( iter == dns->waiting_reqs.end() ) {
            DISP_INFO( "Unrequested dns info (can be a late answer for instance)" );
            return false;
        }
        wcb = &iter->second;
        if ( not wcb->num.contains( num_request ) ) {
            DISP_INFO( "Dns num_request answer is not correct (it is an attack ?)" );
            return false;
        }

        entry = &dns->cached_entries[ name ];
        entry->name = name;
        return true;
    }

    void parse( char *data, PT size ) {
        // header
        CmString cm( data, data + size );
        BinStream<CmString> bs( &cm );
        unsigned num_request  = bs.read_be16();
        unsigned flags_0      = bs.read_byte(); // flags 0
        unsigned flags_1      = bs.read_byte(); // flags 1
        unsigned nb_questions = bs.read_be16(); // QDCOUNT
        unsigned nb_answers   = bs.read_be16(); // ANCOUNT
        unsigned nb_domauth   = bs.read_be16(); // NSCOUNT
        unsigned nb_addsect   = bs.read_be16(); // ARCOUNT
        if ( 0 )
            PRINT( flags_0, flags_1 );

        // questions
        Dns::Entry *entry = 0;
        Dns::Wcb   *wcb   = 0;
        while ( nb_questions-- ) {
            std::string name   = read_cname( bs );
            unsigned    qtype  = bs.read_be16();
            unsigned    qclass = bs.read_be16();
            if ( qtype == 255 and qclass == 1 and not check_name( entry, wcb, num_request, name ) )
                return;
        }

        // answers
        for( unsigned na = nb_answers + nb_domauth + nb_addsect * 0; na--; ) {
            std::string name;
            unsigned c = bs.read_byte();
            if ( c >= 128 + 64 ) { // pointer format
                c = ( ( c - ( 128 + 64 ) ) << 8 ) + bs.read_byte();
                CmString nc( data + std::min( c, unsigned( size ) ), data + size );
                name = read_cname( &nc );
            } else { // CNAME format
                name = read_cname( bs );
            }

            unsigned type   = bs.read_be16();
            unsigned qclass = bs.read_be16();
            unsigned ttl    = bs.read_be32();
            unsigned rlen   = bs.read_be16();

            if ( type == 0x01 and rlen == 4 ) { // A
                if ( qclass == 1 and not check_name( entry, wcb, num_request, name ) )
                    return;
                entry->ttl = ttl;
                bs.read_some( entry->ipv4.push_back(), 4 );
            } else if ( type == 28 and rlen == 16 ) { // AAAA
                if ( qclass == 1 and not check_name( entry, wcb, num_request, name ) )
                    return;
                entry->ttl = ttl;
                bs.read_some( entry->ipv6.push_back(), 16 );
            } else
                bs.skip_some( rlen );
        }

        // callbacks
        if ( wcb ) {
            for( std::function<void( const Vec<Dns::Ipv4> &ipv4, const Vec<Dns::Ipv6> &ipv6 )> &f : wcb->ok )
                f( entry->ipv4, entry->ipv6 );
            dns->waiting_reqs.erase( entry->name );
        }
    }

    Dns *dns;
};

struct DnsSocket : UdpSocket<DnsSocket,DnsConnection> {
    DnsSocket( Dns *dns ) : dns( dns ) {}
    Dns *dns;
};

void DnsConnection::init(DnsSocket *socket, bool server) {
    if ( server )
        DISP_ERROR( "Dns socket should be a client one" );
    dns = socket->dns;
}


Dns::Dns( EvLoop *ev ) : cur_server( 0 ), ev( ev ? ev : gev.ptr() ) {
    // read entries in /etc/resolv.conf
    std::string line;
    std::ifstream f( "/etc/resolv.conf" );
    if ( not f )
        DISP_ERROR( "Pb opening /etc/resolv.conf" );
    while ( std::getline( f, line ) ) {
        const char *d = line.data(), *e = d + line.size();
        while ( d < e and is_a_space( *d ) )
            ++d;
        if ( d == e or *d == '#' or *d == ';' )
            continue;
        if ( e - d > 11 && strncmp( d, "nameserver ", 11 ) == 0 ) {
            const char *b = d += 11;
            while ( d < e && is_a_curl( *d ) )
                ++d;
            server_ips.push_back( b, d );
        }
    }
    if ( server_ips.empty() )
        DISP_ERROR( "/etc/resolv.conf seems to be empty" );

    // open a socket
    socket = new DnsSocket( this );
    *this->ev << socket;
}

void Dns::query( const std::string &addr, std::function<void(const Vec<Dns::Ipv4> &, const Vec<Dns::Ipv6> &)> ok, std::function<void(int)> err ) {
    // we already have the entry ?
    std::map<std::string,Entry>::iterator iter_entries = cached_entries.find( addr );
    if ( iter_entries != cached_entries.end() ) {
        // PRINT( "cached" );
        ok( iter_entries->second.ipv4, iter_entries->second.ipv6 );
        return;
    }

    // IMPORTANT_TODO: try several servers, add timeout !!
    std::map<std::string,Wcb>::iterator iter_waiting = waiting_reqs.find( addr );
    if ( iter_waiting != waiting_reqs.end() ) {
        // PRINT( "added" );
        iter_waiting->second.err << err;
        iter_waiting->second.ok  << ok;
        return;
    }

    // else, make a query
    if ( server_ips.empty() )
        return err( NO_RESOLV_ENTRY );
    BinStream<DnsConnection> bs( socket->connection( { server_ips[ cur_server ], 53 } ) );

    // make a num_request
    static Random rand;
    unsigned num_request = 0;
    rand.get( &num_request, 2 );

    // header
    bs.write_be16( num_request ); // num_request
    bs.write_byte( ( 1 << 0 ) + // recursivity
                   ( 0 << 1 ) + // truncated msg
                   ( 0 << 2 ) + // authoritative answer
                   ( 0 << 3 ) + // type ( -> std request )
                   ( 0 << 7 ) );  // answer ? )
    bs.write_byte( 0x00 ); // !RA, Z=000, RCODE=NOERROR(0000)
    bs.write_be16( 1 ); // QDCOUNT
    bs.write_be16( 0 ); // ANCOUNT
    bs.write_be16( 0 ); // NSCOUNT
    bs.write_be16( 0 ); // ARCOUNT

    // question
    for( const char *b = addr.data(), *e = b; ; ++e ) { // QNAME
        if ( *e == '.' or *e == '@' or *e == 0 ) {
            if ( e - b ) {
                bs.write_byte( e - b );
                bs.write_some( b, e - b );
            }
            if ( *e == 0 )
                break;
            b = e + 1;
        }
    }
    bs.write_byte( 0 );

    bs.write_be16( 0xFF ); /* QTYPE (any) */
    bs.write_be16( 0x01 ); /* QCLASS (internet) */

    bs.flush();

    //
    Wcb &wcb = waiting_reqs[ addr ];
    wcb.num << num_request;
    wcb.err << err;
    wcb.ok  << ok;
}

LazyNew<Dns> g_dns;

} // namespace RiotBase
