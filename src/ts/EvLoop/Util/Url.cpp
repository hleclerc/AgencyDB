#include "Url.h"

namespace RiotBase {

Url::Url( const char *url ) {
    for( int o = 0; url[ o ]; ++o ) {
        if ( url[ o ] == ':' ) {
            _host = std::string{ url, url + o };
            _port = std::string{ url + o + 1 };
            return;
        }
    }
    _host = url;
}

const char *Url::host() {
    return _host.c_str();
}

const char *Url::port() {
    return _port.c_str();
}


} // namespace RiotBase
