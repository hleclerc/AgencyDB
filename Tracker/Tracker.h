#pragma once

#include <Evel/SslCtx.h>
#include <Evel/Gev.h>
#include <vector>

namespace AgencyDb {

/**
  Uses gev
*/
class Tracker {
public:
    void listen_tls( SSL_CTX *ssl_ctx, int port );

private:
    std::vector<std::unique_ptr<Evel::Event>> to_del;
};

} // namespace AgencyDb
