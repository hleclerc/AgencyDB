#pragma once

#include <Evel/SslCtx.h>
#include <Evel/Gev.h>
#include <memory>
#include <vector>

namespace rocksdb { class DB; }
namespace AgencyDb {

/**
  Uses gev
*/
class Tracker {
public:
    Tracker();

    void open_local_db( const std::string &filename ); ///<
    void start_user   (); ///< read user info in db. If not present, ask for it (cmd line)
    void listen_tls   ( SSL_CTX *ssl_ctx, int port );

private:
    using VE = std::vector<std::unique_ptr<Evel::Event>>;

    VE           to_del;
    rocksdb::DB *db;
};

} // namespace AgencyDb
