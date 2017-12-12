#include "PasswordKeyDerivation.h"
#include "Random.h"
#include "Assert.h"

#include <openssl/evp.h>

namespace RiotBase {

std::string PasswordKeyDerivation::salt() {
    static Random mg;

    std::string res;
    res.resize( 32 );
    mg.get( (unsigned char *)&res[ 0 ], res.size() );
    return res;
}

std::string PasswordKeyDerivation::key( std::string pass, std::string salt ) {
    std::string res;
    res.resize( 64 );
    PKCS5_PBKDF2_HMAC_SHA1( pass.data(), pass.size(), (const unsigned char *)salt.data(), salt.size(), 10000, res.size(), (unsigned char *)&res[ 0 ] );
    return res;
}


} // namespace RiotBase
