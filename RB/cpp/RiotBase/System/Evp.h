#pragma once

#include <openssl/evp.h>
#include <string>

namespace RiotBase {

/// symmetric encription
class Evp {
public:
    Evp( const char *key, const char *iv = "0123456789ABCDEF" ); ///< key size = 256/8 ( = 32 ), iv size = 128/8 ( = 16 )
    ~Evp();

    std::string     encrypt( const std::string &msg ) const;
    std::string     decrypt( const std::string &msg ) const;

private:
    EVP_CIPHER_CTX *ctx;
    std::string     key;
    std::string     iv;
};

} // namespace RiotBase
