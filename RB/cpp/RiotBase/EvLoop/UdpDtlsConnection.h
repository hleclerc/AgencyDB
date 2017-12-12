#pragma once

#include "../System/CbString.h"
#include "../System/Assert.h"
#include "UdpRawConnection.h"

#include <openssl/ssl.h>
#include <openssl/bio.h>
#include <openssl/err.h>

namespace RiotBase {

/**
  pending_output will contain cyphered data
*/
template<class Impl,PT msg_size=500>
class UdpDtlsConnection : public UdpConnection {
public:
    template<class _Impl,class _Conn> friend class UdpSocket;
    enum { inp_buf_size = msg_size + 96 >= 1012 ? msg_size + 96 : 1012 }; // SSL may use more than msg_size during handshakes
    using TImpl = Impl;

    UdpDtlsConnection();
    ~UdpDtlsConnection();

    template<class UdpSocketType>
    void           init                  ( UdpSocketType *us, bool server );

    void           flush                 (); ///< send unfiltered data to SSL
    void           write_some            ( const void *data, PT size ) { memcpy( unfiltered_output->data + unfiltered_output->size, data, size ); unfiltered_output->size += size; }
    void           write_some            ( const CbString &data ) { data.data_visitor( [&]( const PI8 *beg, const PI8 *end ) { write_some( beg, end - beg ); } ); }
    void           write_byte            ( PI8 val ) { unfiltered_output->data[ unfiltered_output->size++ ] = val; }
    void           write_byte_wo_beg_test( PI8 val ) { write_byte( val ); }

protected:
    bool           write_pending_unfilt  ();
    void           flush_out_bio         ();
    void           parse_anc             ( char *data, PT size );
    const PI8     *cur_output_ptr        () { return unfiltered_output->data + unfiltered_output->size; }

    SSL           *ssl;
    BIO           *inp_bio;
    BIO           *out_bio;
    MsgQueue::Msg *filtered_output;   ///< cyphered stuff that has not been sent. Used in flush_out_bio()
    MsgQueue::Msg *unfiltered_output; ///< not cyphered (before SSL_write)
    MsgQueue       pending_unfiltered_output;
};

template<class Impl,PT msg_size>
UdpDtlsConnection<Impl,msg_size>::UdpDtlsConnection() : filtered_output( nullptr ), unfiltered_output( MsgQueue::Msg::New( msg_size ) ) {
}

template<class Impl,PT msg_size>
UdpDtlsConnection<Impl,msg_size>::~UdpDtlsConnection() {
    if ( filtered_output ) free( filtered_output );
    if ( ssl ) SSL_free( ssl );
    free( unfiltered_output );
}

template<class Impl,PT msg_size> template<class UdpSocketType>
void UdpDtlsConnection<Impl,msg_size>::init( UdpSocketType *udp_socket, bool server ) {
    // parent init
    UdpConnection::init( udp_socket, server );

    // create SSL
    if ( not ( ssl = SSL_new( server ? udp_socket->ctx_server : udp_socket->ctx_client ) ) ) {
        DISP_ERROR( "Cannot create new SSL.\n" );
        return;
    }

    if ( server )
        SSL_set_accept_state( ssl );
    else
        SSL_set_connect_state( ssl );

    // bios
    if( not ( inp_bio = BIO_new( BIO_s_mem() ) ) ) { DISP_ERROR( "Cannot allocate inp bio.\n" ); SSL_free( ssl ); ssl = NULL; return; }
    if( not ( out_bio = BIO_new( BIO_s_mem() ) ) ) { DISP_ERROR( "Cannot allocate out bio.\n" ); SSL_free( ssl ); ssl = NULL; return; }
    BIO_set_mem_eof_return( inp_bio, -1 ); // to allow asynchronous IO
    BIO_set_mem_eof_return( out_bio, -1 ); // to allow asynchronous IO
    SSL_set_bio( ssl, inp_bio, out_bio );

    SSL_do_handshake( ssl ); // SSL_is_init_finished(ssl)
    flush_out_bio();
}

template<class Impl,PT msg_size>
void UdpDtlsConnection<Impl,msg_size>::flush() {
    if ( not unfiltered_output->size )
        return;

    // we have some old message to send to SSL_write ?
    if ( not write_pending_unfilt() ) {
        pending_unfiltered_output << unfiltered_output;
        unfiltered_output = MsgQueue::Msg::New( msg_size );
        return;
    }

    // now it's time for unfiltered_output
    int sent = SSL_write( ssl, unfiltered_output->data, unfiltered_output->size );
    flush_out_bio();
    if ( sent <= 0 ) {
        pending_unfiltered_output << unfiltered_output;
        unfiltered_output = MsgQueue::Msg::New( msg_size );
    } else
        unfiltered_output->size = 0;
}

template<class Impl,PT msg_size>
bool UdpDtlsConnection<Impl,msg_size>::write_pending_unfilt() {
    if ( MsgQueue::Msg *cur = pending_unfiltered_output.first ) {
        for( MsgQueue::Msg *nxt; cur; cur = nxt ) {
            int sent = SSL_write( ssl, cur->data, cur->size );
            flush_out_bio();
            if ( sent <= 0 )
                return false;
            nxt = cur->next;
            free( cur );
        }
        if ( not cur ) pending_unfiltered_output.last = cur;
        pending_unfiltered_output.first = cur;
    }
    return true;
}

template<class Impl,PT msg_size>
void UdpDtlsConnection<Impl,msg_size>::flush_out_bio() {
    // make room to receive the data
    if ( not filtered_output )
        filtered_output = MsgQueue::Msg::New( inp_buf_size );

    // read bio
    int read = BIO_read( out_bio, filtered_output->data, inp_buf_size );

    // try to send the content
    if ( read >= 0  ) {
        filtered_output->size = read;
        if ( not send( filtered_output ) )
            filtered_output = nullptr;
    }
}

template<class Impl,PT msg_size>
void UdpDtlsConnection<Impl,msg_size>::parse_anc( char *data, PT size ) {
    PRINTL( size, inet_address );
    // send socket data to inp_bio
    BIO_write( inp_bio, data, size );

    // read inp_bio
    char decyph[ msg_size ];
    while ( true ) {
        int read = SSL_read( ssl, decyph, sizeof decyph );
        flush_out_bio();
        if ( read <= 0 )
            break;
        static_cast<Impl *>( this )->parse( decyph, read );
    }

    // we have some old message to send to SSL_write ?
    write_pending_unfilt();
}


} // namespace RiotBase
