#include "../System/TxtStream.h"
#include "../System/LazyNew.h"
#include "../System/Base64.h"
#include "../System/Log.h"
#include "TcpSslConnection.h"
#include "Util/SslCtx.h"
#include "Smtp.h"
#include "Dns.h"
#include "Gev.h"

#include <fstream>
#include <time.h>

#define TEST_ANS( LABEL, CODE ) if ( code != CODE ) { cb( SmtpStatus::pb_after_##LABEL ); return false; }
#define WAIT_ANS( LABEL, CODE ) do { inp_cont = &&LABEL; return true; LABEL:; TEST_ANS( LABEL, CODE ); } while ( 0 ) // DISP_INFO( "Waiting for answer of " #LABEL );

namespace RiotBase {

/**
*/
class SmtpClient {
public:
    SmtpClient() : ssl_ctx{ SslCtx::Method::SSLv23 } {
        read_crea();
    }

    void send_mail( const std::string &to, const std::string &subject, const std::string &msg, std::function<void(SmtpStatus)> cb, EvLoop *evl, Dns *dns ) {
        // IMPORTANT_TODO: timeout
        if ( not dns ) dns = g_dns.ptr();
        if ( not evl ) evl = gev.ptr();

        // get ip
        dns->query( addr, [ this, to, subject, msg, cb, evl, dns ]( const Vec<Dns::Ipv4> &i4, const Vec<Dns::Ipv6> &i6 ) {
            if ( i4.empty() && i6.empty() )
                return cb( SmtpStatus::no_dns_entry );
            InetAddress inaddr = i6.size() ? InetAddress( i6[ 0 ].data, port ) : InetAddress( i4[ 0 ].data, port );

            //
            MyConnection *c = new MyConnection( ssl_ctx.ctx, inaddr, this, to, subject, msg, cb );
            c->flush();
            *evl << c;
        }, []( int code ) {
            DISP_ERROR( "Dns query pb code={}", code );
        } );
    }

private:
    struct MyConnection : TcpSslConnection {
        MyConnection( SSL_CTX *ssl_ctx, const InetAddress &addr, SmtpClient *client, const std::string &to, const std::string &subject, const std::string &msg, std::function<void(SmtpStatus)> cb ) : TcpSslConnection( ssl_ctx, addr ), client( client ), to( to ), subject( subject ), msg( msg ), cb( cb ) {
            inp_cont = 0;
        }

        virtual bool parse() {
            CmString cm( inp_buff->data, inp_buff->data + inp_buff->used );
            TxtStream<CmString> rd( &cm );
            PI32 code = rd.read_unsigned();

            if ( inp_cont )
                goto *inp_cont;

            // GREETING from the server
            TEST_ANS( GREETING, 220 );

            // HELO
            cw.write_some( "HELO test.example.org\n" );
            WAIT_ANS( HELO, 250 );

            // LOGIN
            cw.write_some( "AUTH LOGIN\n" );
            WAIT_ANS( AUTH_LOGIN, 334 );

            cw.write_some( base64_encode( client->user ) + "\n" );
            WAIT_ANS( AUTH_USER, 334 );


            cw.write_some( base64_encode( client->pass ) + "\n" );
            WAIT_ANS( AUTH_PASS, 235 );

            //
            cw.write_some( "MAIL FROM:<" + client->user + ">\n" );
            WAIT_ANS( MAIL_FROM, 250 );

            cw.write_some( "RCPT TO:<" + to + ">\n" );
            WAIT_ANS( RCPT_TO, 250 );

            cw.write_some( "DATA\n" );
            WAIT_ANS( DATA, 354 );

            cw.write_some( "From: <" + client->user+ ">\n" );
            cw.write_some( "To: <" + to + ">\n" );

            time_t t = time( NULL );
            if ( struct tm *tmp = localtime( &t ) ) {
                char date[ 100 ];
                size_t size = strftime( date, sizeof( date ), "%a, %d %b %Y %T %z", tmp );

                cw.write_some( "Date: " );
                cw.write_some( date, size );
                cw.write_some( "\n" );
            } else {
                perror( "localtime" );
                cw.write_some( "Date: Mon, 04 April 2016 21:45:03 +0200\n" );
            }

            cw.write_some( "Subject: " + subject + "\n" );
            cw.write_some( "\n" );

            for( PT i = 0; i < msg.size(); ++i ) {
                if ( msg[ i ] == '.' && ( i == 0 || msg[ i - 1 ] == '\n' ) ) // line starting with a .
                    cw.write_byte( '.' );
                cw.write_byte( msg[ i ] );
            }

            cw.write_some( "\n.\n" );
            WAIT_ANS( CONTENT, 250 );

            cw.write_some( "QUIT\n" );
            return false;
        }

        void                           *inp_cont;
        SmtpClient                     *client;
        std::string                     to;
        std::string                     subject;
        std::string                     msg;
        std::function<void(SmtpStatus)> cb;
    };

    friend struct MyConnection;

    void read_crea() {
        std::ifstream f( "credmail" );
        std::ostringstream fs;
        fs << f.rdbuf();

        std::istringstream is( fs.str() );
        is >> addr >> port >> user >> pass;
    }

    SslCtx      ssl_ctx;
    std::string addr;
    unsigned    port;
    std::string user;
    std::string pass;
};


void send_mail( const std::string &to, const std::string &subject, const std::string &msg, std::function<void(SmtpStatus)> cb, EvLoop *evl, Dns *dns ) {
    static SmtpClient smtp_client;
    smtp_client.send_mail( to, subject, msg, cb, evl, dns );
}


} // namespace RiotBase
