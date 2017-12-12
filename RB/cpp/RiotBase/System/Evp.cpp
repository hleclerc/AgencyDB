#include <openssl/err.h>
#include "Evp.h"

#ifdef METIL_COMP_DIRECTIVE
#pragma lib_name crypto
#endif // METIL_COMP_DIRECTIVE

namespace RiotBase {

namespace {

void printLastError( const char *msg ) {
    char *err = (char *)malloc(130);
    ERR_load_crypto_strings();
    ERR_error_string( ERR_get_error(), err );
    printf( "%s ERROR: %s\n", msg, err );
    free( err );
}

} // namespace

Evp::Evp( const char *key, const char *iv ) : key( key, key + 32 ), iv( iv, iv + 16 ) {
    if ( not ( ctx = EVP_CIPHER_CTX_new() ) )
        printLastError( "EVP_CIPHER_CTX_new" );
}

Evp::~Evp() {
    if ( ctx )
        EVP_CIPHER_CTX_free( ctx );
}

std::string Evp::encrypt( const std::string &msg ) const {
    #ifdef AVOID_SSL
        return msg;
    #endif
    if ( EVP_EncryptInit_ex( ctx, EVP_aes_256_cbc(), NULL, (const unsigned char *)key.data(), (const unsigned char *)iv.data() ) != 1 ) {
        printLastError( "EVP_EncryptInit_ex" );
        return {};
    }

    int len = 0;
    std::string res; res.resize( msg.size() + EVP_CIPHER_CTX_block_size( ctx ) - 1 );
    if( EVP_EncryptUpdate( ctx, (unsigned char *)&res[ 0 ], &len, (const unsigned char *)&msg[ 0 ], msg.size() ) != 1 ) {
        printLastError( "EVP_EncryptUpdate" );
        return {};
    }

    int res_len = len;
    if ( EVP_EncryptFinal_ex( ctx, (unsigned char *)&res[ 0 ] + res_len, &len ) != 1 ) {
        printLastError( "EVP_EncryptFinal_ex" );
        return {};
    }

    res.resize( res_len + len );
    return res;
}

std::string Evp::decrypt( const std::string &msg ) const {
    #ifdef AVOID_SSL
        return msg;
    #endif
    if ( EVP_DecryptInit_ex( ctx, EVP_aes_256_cbc(), NULL, (const unsigned char *)key.data(), (const unsigned char *)iv.data() ) != 1 ) {
        printLastError( "EVP_EncryptInit_ex" );
        return {};
    }

    int len = 0;
    std::string res; res.resize( msg.size() + EVP_CIPHER_CTX_block_size( ctx ) - 1 );
    if ( EVP_DecryptUpdate( ctx, (unsigned char *)&res[ 0 ], &len, (const unsigned char *)&msg[ 0 ], msg.size() ) != 1 ){
        printLastError( "EVP_DecryptUpdate" );
        return {};
    }

    int res_len = len;
    if( EVP_DecryptFinal_ex( ctx, (unsigned char *)&res[ 0 ] + res_len, &len ) != 1 ) {
        printLastError( "EVP_DecryptFinal_ex" );
        return {};
    }

    res.resize( res_len + len );
    return res;
}

} // namespace RiotBase
