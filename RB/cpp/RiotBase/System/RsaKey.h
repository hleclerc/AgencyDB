#pragma once

#include <openssl/rsa.h>
#include <string>

namespace RiotBase {

///
class RsaKey {
public:
    RsaKey( const char *key = 0, bool pub = true );
    ~RsaKey();

    void        operator=( RsaKey &&key );
    operator    bool     () const;

    std::string encrypt  ( const std::string &input ) const;
    std::string decrypt  ( const std::string &input ) const;

protected:
    RSA *rsa;
    bool pub;
};

} // namespace RiotBase
