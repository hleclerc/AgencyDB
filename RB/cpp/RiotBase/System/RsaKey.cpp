#include <openssl/pem.h>
#include <openssl/err.h>
#include "RsaKey.h"
#include "Assert.h"

#include "Print.h"

#ifdef METIL_COMP_DIRECTIVE
#pragma lib_name crypto
#endif // METIL_COMP_DIRECTIVE

namespace RiotBase {

namespace {

int padding = RSA_PKCS1_PADDING;

void printLastError( const char *msg ) {
    char *err = (char *)malloc(130);
    ERR_load_crypto_strings();
    ERR_error_string( ERR_get_error(), err );
    printf( "%s ERROR: %s\n", msg, err );
    free( err );
}

} // namespace

RsaKey::RsaKey( const char *key, bool pub ) : pub( pub ) {
    rsa = NULL;
    if ( not key )
        return;

    BIO *keybio = BIO_new_mem_buf( (void *)key, -1 );
    if ( not keybio )
        ERROR( "Failed to create key BIO");

    if( pub )
        rsa = PEM_read_bio_RSA_PUBKEY( keybio, &rsa, NULL, NULL );
    else
        rsa = PEM_read_bio_RSAPrivateKey( keybio, &rsa, NULL, NULL );

    BIO_free( keybio );

    if ( not rsa ) {
        printLastError( "Failed to create RSA" );
        ERROR( "Failed to create RSA");
    }
}

RsaKey::~RsaKey() {
    if ( rsa )
        RSA_free( rsa );
}

void RsaKey::operator=( RsaKey &&key ) {
    rsa     = key.rsa;
    pub     = key.pub;
    key.rsa = 0;
}

RiotBase::RsaKey::operator bool() const {
    return rsa;
}

std::string RsaKey::encrypt( const std::string &input ) const {
    #ifdef AVOID_SSL
        return input;
    #endif
    if ( not rsa ) return {};
    std::string res; res.resize( RSA_size( rsa ), 0 );
    int size = pub ? RSA_public_encrypt ( input.size(), (const unsigned char *)&input[ 0 ], (unsigned char *)&res[ 0 ], rsa, padding ) :
                     RSA_private_encrypt( input.size(), (const unsigned char *)&input[ 0 ], (unsigned char *)&res[ 0 ], rsa, padding );
    if ( size < 0 ) {
        printLastError( "Failed to encrypt" );
        res.resize( 0 );
        IMPORTANT_TODO;
    } else
        res.resize( size );
    return res;
}

std::string RsaKey::decrypt( const std::string &input ) const {
    #ifdef AVOID_SSL
        return input;
    #endif
    if ( not rsa ) return {};
    std::string res; res.resize( RSA_size( rsa ) );
    int size = pub ? RSA_public_decrypt ( input.size(), (const unsigned char *)&input[ 0 ], (unsigned char *)&res[ 0 ], rsa, padding ) :
                     RSA_private_decrypt( input.size(), (const unsigned char *)&input[ 0 ], (unsigned char *)&res[ 0 ], rsa, padding );
    if ( size < 0 ) {
        printLastError( "Failed to decrypt" );
        res.resize( 0 );
        IMPORTANT_TODO;
    } else
        res.resize( size );
    return res;
}

} // namespace RiotBase
