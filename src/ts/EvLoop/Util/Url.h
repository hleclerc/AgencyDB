#pragma once

#include <string>

namespace RiotBase {

///
class Url {
public:
    Url( const char *url );

    const char *host();
    const char *port();

protected:
    std::string _host;
    std::string _port;
};

} // namespace RiotBase
