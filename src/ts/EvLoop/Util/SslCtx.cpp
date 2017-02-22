#include "SslCtx.h"

#ifdef METIL_COMP_DIRECTIVE
#pragma lib_name crypto
#pragma lib_name ssl
#endif // METIL_COMP_DIRECTIVE

namespace RiotBase {

namespace {
    struct InitSsl {
        InitSsl() {
            SSL_load_error_strings();
            ERR_load_BIO_strings();
            OpenSSL_add_all_algorithms(); // load & register all cryptos, etc.
            SSL_library_init();
        }
        ~InitSsl() {
            ERR_free_strings();
            EVP_cleanup();
            // sk_SSL_COMP_free( SSL_COMP_get_compression_methods() );
            CRYPTO_cleanup_all_ex_data();
        }
    };
    InitSsl &init_ssl_if_necessary() {
        static InitSsl res;
        return res;
    }
}

SslCtx::SslCtx( Method method_name, const char *cert_file, const char *key_file ) {
    init_ssl_if_necessary();

    // method
    const SSL_METHOD *method = 0;
    switch ( method_name ) {
    case Method::SSLv23: method = SSLv23_server_method(); break;
    case Method::DTLSv1: method = DTLSv1_server_method(); break;
    }

    // server ctx
    ctx = SSL_CTX_new( method ); // create new context from method
    if ( not ctx ) {
        ERR_print_errors_fp( stderr );
        abort();
    }

    // set the local certificate from cert_file
    if ( SSL_CTX_use_certificate_file( ctx, cert_file, SSL_FILETYPE_PEM ) <= 0 ) {
        ERR_print_errors_fp( stderr );
        abort();
    }
    // set the private key from key_file (may be the same as CertFile)
    if ( SSL_CTX_use_PrivateKey_file( ctx, key_file, SSL_FILETYPE_PEM ) <= 0 ) {
        ERR_print_errors_fp( stderr );
        abort();
    }
    // verify private key
    if ( not SSL_CTX_check_private_key( ctx ) ) {
        fprintf( stderr, "Private key does not match the public certificate\n" );
        abort();
    }
}

SslCtx::SslCtx( Method method_name ) {
    init_ssl_if_necessary();

    // method
    const SSL_METHOD *method = 0;
    switch ( method_name ) {
    case Method::SSLv23: method = SSLv23_client_method(); break;
    case Method::DTLSv1: method = DTLSv1_client_method(); break;
    }

    // ctx
    ctx = SSL_CTX_new( method ); // create new context from method
    if ( not ctx ) {
        ERR_print_errors_fp( stderr );
        abort();
    }
}

SslCtx::SslCtx() {
    ctx = 0;
}

SslCtx::~SslCtx() {
    SSL_CTX_free( ctx );
}

SslCtx::operator bool() const {
    return ctx;
}

} // namespace RiotBase

