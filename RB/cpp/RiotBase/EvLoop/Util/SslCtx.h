#pragma once

#include "openssl/ssl.h"
#include "openssl/err.h"
#include "openssl/bio.h"

namespace RiotBase {

/**
*/
class SslCtx {
public:
    enum class Method {
        SSLv23,
        DTLSv1
    };

    SslCtx( Method method_name, const char *cert_file, const char *key_file ); ///< for a server connection
    SslCtx( Method method_name ); ///< for a client connection
    SslCtx(); ///< for no connection
    ~SslCtx();

    operator bool     () const; ///< true if ok
    operator SSL_CTX *() { return ctx; }

    SSL_CTX *ctx;
};

} // namespace RiotBase
