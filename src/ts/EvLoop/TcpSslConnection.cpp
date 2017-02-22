#include "TcpSslConnection.h"
#include "../System/Assert.h"
#include "../System/Log.h"

#include <openssl/err.h>
#include <sys/sendfile.h>
#include <sys/socket.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>

namespace RiotBase {

//namespace {
//void ShowCerts( SSL* ssl ) {
//    X509 *cert;
//    char *line;

//    cert = SSL_get_peer_certificate( ssl ); /* get the server's certificate */
//    if ( cert != NULL ) {
//        printf( "Server certificates:\n" );
//        line = X509_NAME_oneline( X509_get_subject_name(cert), 0, 0 );
//        printf( "Subject: %s\n", line );
//        free( line );      /* free the malloc'ed string */
//        line = X509_NAME_oneline( X509_get_issuer_name(cert), 0, 0 );
//        printf( "Issuer: %s\n", line );
//        free( line );      /* free the malloc'ed string */
//        X509_free( cert ); /* free the malloc'ed certificate copy */
//    }
//    else
//        printf( "No certificates.\n" );
//}
//}

TcpSslConnection::TcpSslConnection( SSL_CTX *ssl_ctx, const InetAddress &addr ) : TcpRawConnection( addr ), server( false ) {
    ssl = SSL_new( ssl_ctx );
    SSL_set_fd( ssl, fd );
    SSL_set_connect_state( ssl ); // client
    SSL_set_mode( ssl, SSL_MODE_ENABLE_PARTIAL_WRITE );
}

TcpSslConnection::TcpSslConnection( SSL_CTX *ssl_ctx, int fd ) : TcpRawConnection( fd ), server( true ) {
    ssl = SSL_new( ssl_ctx );
    SSL_set_fd( ssl, fd );
    SSL_set_accept_state( ssl ); // server
    SSL_set_mode( ssl, SSL_MODE_ENABLE_PARTIAL_WRITE );
}


TcpSslConnection::TcpSslConnection( VtableOnly vo ) : TcpRawConnection( vo ) {
}

TcpSslConnection::~TcpSslConnection() {
    if ( ssl )
        SSL_free( ssl );
}

void TcpSslConnection::inp() {
    if ( not ssl )
        return;

    if ( state_SSL_write == wants_inp ) // we need to start with SSL_write: if we start with SSL_read, we will obtain state_SSL_read == wants_inp
        call_SSL_write();

    if ( state_SSL_read != wants_out ) // wants_out means that inp some data will be worthless
        call_SSL_read();
}

void TcpSslConnection::out() {
    // update of waiting_for_connection
    if ( waiting_for_connection ) {
        waiting_for_connection = false;
        state_SSL_write = ok;
        state_SSL_read = ok;

        int error;
        socklen_t len = sizeof error;
        if ( getsockopt( fd, SOL_SOCKET, SO_ERROR, &error, &len ) ) {
            DISP_ERROR( "getsockopt SO_ERROR: {}", strerror( errno ) );
            return;
        }
        if ( error )
            on_connect_error( error );
        else
            on_connect();

        if ( not server )
            SSL_do_handshake( ssl );
    }

    if ( state_SSL_read == wants_out )
        call_SSL_read();

    if ( state_SSL_write != wants_inp ) // wants_inp means the possibility to output data is worthless
        call_SSL_write();
}

void TcpSslConnection::flush() {
    if ( not waiting_for_connection and ssl and state_SSL_write == ok )
        call_SSL_write();
}

void TcpSslConnection::call_SSL_write() {
    if ( cw.empty() )
        return;

    frame_cw();
    while ( not cw.empty() ) {
        std::pair<const void *,ST> buff = cw.contiguous_data_at_the_beginning();
        ST ruff = SSL_write( ssl, buff.first, buff.second );

        switch ( SSL_get_error( ssl, ruff ) ) {
        case SSL_ERROR_NONE: // PRINTL("...");
            state_SSL_write = ok;
            cw.skip_some( ruff );
            skip_framed( ruff );
            break;
        case SSL_ERROR_WANT_READ: // PRINTL("...");
            state_SSL_write = wants_inp;
            return wait_for_more_inp();
        case SSL_ERROR_WANT_WRITE: // PRINTL("...");
            state_SSL_write = wants_out;
            return wait_for_more_out();
        case SSL_ERROR_ZERO_RETURN: // PRINTL("...");
            return rd_hup_error();
        case SSL_ERROR_SYSCALL: // PRINTL("...");
            return hup_error();
        default: // PRINTL("...");
            return ssl_error();
        }
    }
}

void TcpSslConnection::call_SSL_read() {
    while ( true ) {
        // try to re-use the old buffer
        if ( not inp_buff )
            inp_buff = Buffer::New();
        else if ( inp_buff->cpt_use > 0 ) {
            --inp_buff;
            inp_buff = Buffer::New();
        }

        // read some data
        int ruff = SSL_read( ssl, inp_buff->data, inp_buff->size );

        // error/OK cases
        switch ( SSL_get_error( ssl, ruff ) ) {
        case SSL_ERROR_NONE: // PRINTL("...");
            state_SSL_read = ok;
            inp_buff->used = ruff;
            if ( ruff ) {
                bool cont = parse();
                flush();
                if ( not cont )
                    return;
            }
            break;
        case SSL_ERROR_WANT_READ: // PRINTL("...");
            state_SSL_read = wants_inp;
            return wait_for_more_inp();
        case SSL_ERROR_WANT_WRITE: // PRINTL("...");
            state_SSL_read = wants_out;
            return wait_for_more_out();
        case SSL_ERROR_ZERO_RETURN: // PRINTL("...");
            return rd_hup_error(); // The TLS/SSL connection has been closed
        case SSL_ERROR_SYSCALL: // PRINTL("...");
            if ( errno ) {
                ERR_print_errors_fp( stderr );
                DISP_ERROR( "SSL_ERROR_SYSCALL: {}", strerror( errno ) );
            }
            return hup_error();
        default: // PRINTL("...");
            return ssl_error();
        }
    }
}

void TcpSslConnection::ssl_error() {
    DISP_ERROR( "Ssl error" );

    ERR_print_errors_fp( stderr );
    errors |= EF_SSL_error;
    reg_for_deletion();
}

void TcpSslConnection::frame_cw() {
}

void TcpSslConnection::skip_framed( PT len ) {
}

} // namespace RiotBase
