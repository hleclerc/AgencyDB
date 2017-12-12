#include "ConfigReader.h"

namespace RiotBase {

std::string _string( const char *o, const char *d ) {
    std::string res;
    res.reserve( d - o );
    for( ; o < d; ++o ) {
        if ( *o == '\\' ) {
            switch ( *( ++o ) ) {
            case '\\': res += '\\'; break;
            case 'n':  res += '\n'; break;
            case 't':  res += '\t'; break;
            default: res += '\\'; res += *o;
            }
        } else
            res += *o;
    }
    return res;
}

void ConfigReader::parse( std::istream &is ) {
    std::string line;
    while ( std::getline( is, line ) ) {
        const char *d = line.c_str();

        // find the arguments
        bool ok = true;
        Vec<std::string> args;
        for( const char *o = d; ; ++d ) {
            if ( *d == ' ' or *d == '\n' or *d == '\t' or *d == '"' or *d == '#' or *d == 0 ) {
                if ( d != o )
                    args << _string( o, d );
                if ( *d == 0 or *d == '#' )
                    break;
                if ( *d == '"' ) {
                    o = ++d;
                    while ( *d != '"' and *d != 0 )
                        ++d;
                    if ( not *d ) {
                        ok = false;
                        break;
                    }
                    if ( d != o )
                        args << _string( o, d );
                }
                o = d + 1;
            }
        }

        // callback
        if ( ok )
            onargs( args );
    }

}

static bool need_quotes( const std::string &s ) {
    for( char v : s )
        if ( v == ' ' or v == '\n' or v == '\t' )
            return true;
    return false;
}

std::string ConfigReader::write( const Vec<std::string> &args ) {
    std::string os;
    for( const std::string &s : args ) {
        bool nq = need_quotes( s );
        if ( nq )
            os += '"';
        for( char v : s ) {
            switch ( v ) {
            case '\n': os += "\\n"; break;
            case '\t': os += "\\t"; break;
            default: os += v;
            }
        }
        if ( nq )
            os += '"';
        os += ( &s == &args.back() ? '\n' : ' ' );
    }
    return os;
}



} // namespace RiotBase
