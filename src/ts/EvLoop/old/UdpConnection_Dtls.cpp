//#include "UdpConnection_Dtls.h"
//#include "UdpSocket.h"

//#include "../System/Log.h"

//#include <openssl/bio.h>
//#include <openssl/err.h>

//namespace RiotBase {

////namespace {
////    void check_ssl_err() {
////        char buffer[ 256 ];
////        while ( int error = ERR_get_error() )  {
////            ERR_error_string_n( error, buffer, sizeof buffer );
////            PRINT( buffer );
////        }
////    }
////}

//UdpConnection_Dtls::~UdpConnection_Dtls() {
//}

//void UdpConnection_Dtls::flush() {
//    while ( not dtls_to_be_sent.empty() ) {
//        std::pair<const void *,ST> buff = dtls_to_be_sent.contiguous_data_at_the_beginning();
//        int sent = SSL_write( ssl, buff.first, buff.second );
//        if ( sent <= 0 )
//            break;
//        dtls_to_be_sent.skip_some( sent );
//    }

//    send_out_bio();
//}

//void UdpConnection_Dtls::write_some( const void *data, PT size ) {
//    // write pending data to out_bio
//    while ( not dtls_to_be_sent.empty() ) {
//        std::pair<const void *,ST> buff = dtls_to_be_sent.contiguous_data_at_the_beginning();
//        int sent = SSL_write( ssl, buff.first, buff.second );
//        if ( sent <= 0 ) {
//            dtls_to_be_sent.write_some( data, size );
//            send_out_bio();
//            return;
//        }
//        dtls_to_be_sent.skip_some( sent );
//    }

//    // write data to out_bio
//    int sent = SSL_write( ssl, data, size );
//    if ( sent <= 0 ) {
//        dtls_to_be_sent.write_some( data, size );
//        send_out_bio();
//        return;
//    }
//    if ( sent < int( size ) ) {
//        PRINTL( sent, size );
//        TODO;
//        // cw.write_some( data + sent, size );
//    }

//    // send out_bio data to socket
//    send_out_bio();
//}

//void UdpConnection_Dtls::send_out_bio() {
//    char buff[ 1024 ];
//    while ( true ) {
//        int read = BIO_read( out_bio, buff, sizeof buff );
//        if ( read <= 0 )
//            break;

//        ssize_t real = sendto( udp_socket->fd, buff, read, MSG_NOSIGNAL, (const sockaddr *)&inet_address.sa, sizeof inet_address.sa );
//        if ( real != read )
//            TODO;
//        //        if ( real <= 0 ) { // error ?
//        //            if ( real < 0 and ( errno == EAGAIN or errno == EWOULDBLOCK ) )
//        //                return udp_socket->wait_for_more_out();
//        //            return udp_socket->hup_error();
//        //        }
//    }
//}

//bool UdpConnection_Dtls::parse_anc( Buffer *buff ) {
//    // send socket data to inp_bio
//    int sent = BIO_write( inp_bio, buff->data, buff->used );
//    if ( PT( sent ) != buff->used )
//        TODO;

//    // read inp_bio
//    while ( true ) {
//        // Buffer *tmp_buff = Buffer::New();
//        int read = SSL_read( ssl, buff->data, buff->size );
//        if ( read <= 0 )
//            return true;
//        buff->used = read;

//        if ( not parse( buff ) )
//            return false;
//    }
//}

//void UdpConnection_Dtls::ssl_cmd( int code ) {
//    switch ( SSL_get_error( ssl, code ) ) {
//    case SSL_ERROR_NONE:
//        return;
//    case SSL_ERROR_WANT_READ:
//        PRINTL("");
//        return;
//    case SSL_ERROR_WANT_WRITE:
//        PRINTL("");
//        return;
//    case SSL_ERROR_ZERO_RETURN:
//        DISP_ERROR( "rd_hup_error()" );
//        return;
//    case SSL_ERROR_SYSCALL:
//        DISP_ERROR( "hup_error()" );
//        return;
//    default:
//        DISP_ERROR( "ssl_error();" );
//        return;
//    }
//}


//} // namespace RiotBase
