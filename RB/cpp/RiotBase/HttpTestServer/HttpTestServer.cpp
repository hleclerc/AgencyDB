/**
  Proposition: require 'toto'

*/
#include "../EvLoop/Util/UrlEncodeAndDecode.h"
#include "../EvLoop/TcpSslConnection.h"
#include "../EvLoop/TcpRawConnection.h"
#include "../EvLoop/Util/SslCtx.h"
#include "../EvLoop/TcpListener.h"
#include "../EvLoop/Gev.h"

#include "../System/TxtStream.h"
#include "../System/Log.h"

#include "HttpTestServer.h"

#include <boost/filesystem.hpp>
#include <leveldb/db.h>
#include <json/json.h>
#include <signal.h>
#include <fstream>
#include <sstream>
#include <set>

#ifdef METIL_COMP_DIRECTIVE
#pragma lib_name boost_filesystem
#pragma lib_name boost_system
#pragma lib_name leveldb
#pragma lib_name jsoncpp
#endif // METIL_COMP_DIRECTIVE

namespace RiotBase {

namespace {
    using String = std::string;

    template<class T_BufferedSocket>
    struct MyHttpRequest : T_BufferedSocket {
        template<class TI> MyHttpRequest( HttpTestServer *http_test_server, SSL_CTX *ssl_ctx, TI fd ) : T_BufferedSocket( ssl_ctx, fd ), http_test_server( http_test_server ) { }
        template<class TI> MyHttpRequest( HttpTestServer *http_test_server, TI fd ) : T_BufferedSocket( fd ), http_test_server( http_test_server ) { }

        virtual bool parse() {
            // fill req upon completion
            req += std::string( this->inp_buff->data, this->inp_buff->data + this->inp_buff->used );
            if ( req.find( "\r\n\r\n" ) < 0 and req.find( "\n\n" ) < 0 )
                return true;

            // GET
            if ( req.substr( 0, 4 ) == "GET " ) {
                get( req.substr( 4, req.find( ' ', 4 ) - 4 ) );
                return false;
            }

            // ??
            return false;
        }

        void get( std::string url ) {
            // commands
            if ( url == "/_quit_server" ) {
                this->ev_loop->stop();
                return;
            }

            if ( url.find( "/_test?" ) == 0 ) {
                std::map<std::string,std::string> p = decode_url_attr( url );
                std::cout << p[ "fl" ] << ": ";
                if ( p[ "se" ] == p[ "sv" ] )
                    std::cout << "(OK) " << p[ "se" ];
                else
                    std::cout << "(BAD) " << p[ "se" ] << " (expected: " << p[ "sv" ] << ")";
                std::cout << std::endl;
                this->cw.write_some( "HTTP/1.0 200 OK\nContent-Type: text/plain\nCache-Control: no-cache\nContent-Length: 0\n\n" );
                return;
            }

            if ( url.find( "/_graph?" ) == 0 ) {
                std::map<std::string,std::string> p = decode_url_attr( url );
                std::ofstream f( ".tmp.dot" );
                f << "digraph my_digraph {\n";
                f << p[ "gr" ];
                f << "}\n";
                f.flush();

                system( "dot -Tpng .tmp.dot > .tmp.png" );
                std::string png = content_of( ".tmp.png" );
                this->cw.write_some( "HTTP/1.0 200 OK\nContent-Type: text/plain\nCache-Control: no-cache\nContent-Length: " + to_string( png.size() ) + "\n\n" + png );
                return;
            }

            if ( url.find( "/_make_local_copy?" ) == 0 ) {
                url = url.substr( url.find( '?' ) + 1 );
                if ( url.size() == 0  or url[ 0 ] != '/' )
                    url = '/' + url;
                make_local_copy_rec( url );
                this->cw.write_some( "HTTP/1.0 200 OK\nContent-Type: text/plain\nCache-Control: no-cache\nContent-Length: 0\n\n" );
                return;
            }

            //
            std::string content, mimetype;
            if ( virtual_content_of( content, mimetype, url ) ) {
                TxtStream<CbQueue> txt( &this->cw );
                txt << "HTTP/1.0 200 OK\nContent-Type: " << mimetype << "\n";
                if ( not starts_with( url, "/node_modules" ) )
                    txt << "Cache-Control: no-cache\n";
                txt << "Content-Length: " << content.size() << "\n\n" << content;
                return;
            }

            // not found :(
            std::string data = "<html><head><title>Not found</title></head><body>Not found</body></html>";
            return this->cw.write_some( "HTTP/1.0 404 Not Found\nContent-Type: text/html\nContent-Length: " + to_string( data.size() ) + "\n\n" + data );
        }


        bool virtual_content_of( std::string &content, std::string &mimetype, std::string url, bool reg = true ) {
            // PRINT( url );
            if ( url.find( "?" ) != std::string::npos )
                url = url.substr( 0, url.find( "?" ) );

            //
            if ( extension_of( url ) == ".js" ) {
                mimetype = "text/javascript";

                // javascript to be transpiled
                if ( exists( url ) ) {
                    content = js_from_js( content_of( url ), url, reg );
                    return true;
                }

                // .coffee to be transpiled
                if ( exists( basename_of( url ) + ".coffee" ) ) {
                    content = js_from_cs( content_of( basename_of( url ) + ".coffee" ), url, reg );
                    return true;
                }

                // .cs to be transpiled
                if ( exists( basename_of( url ) + ".cs" ) ){
                    content = js_from_cs( content_of( basename_of( url ) + ".cs" ), url, reg );
                    return true;
                }

                // .ts to be transpiled
                if ( exists( basename_of( url ) + ".ts" ) ){
                    content = js_from_ts( basename_of( url ) + ".ts", url, reg );
                    return true;
                }

                // .tsx to be transpiled
                if ( exists( basename_of( url ) + ".tsx" ) ){
                    content = js_from_ts( basename_of( url ) + ".tsx", url, reg );
                    return true;
                }
            }

            // xxx from xxx.js
            if ( exists( url + ".js" ) ) {
                content = js_from_js( content_of( url + ".js" ), url, reg );
                return true;
            }

            // xxx from xxx.coffee
            if ( exists( url + ".coffee" ) ) {
                content = js_from_cs( content_of( url + ".coffee" ), url, reg );
                return true;
            }

            // xxx from xxx.cs
            if ( exists( url + ".cs" ) ) {
                content = js_from_cs( content_of( url + ".cs" ), url, reg );
                return true;
            }

            // xxx from xxx.ts
            if ( exists( url + ".ts" ) ) {
                content = js_from_ts( url + ".ts", url, reg );
                return true;
            }

            if ( exists( url + ".tsx" ) ) {
                content = js_from_ts( url + ".tsx", url, reg );
                return true;
            }

            // html from a js or coffee
            if ( extension_of( url ) == ".html" ) {
                mimetype = "text/html";
                if ( exists( basename_of( url ) + ".js" ) or exists( basename_of( url ) + ".coffee" ) or exists( basename_of( url ) + ".cs" ) or exists( basename_of( url ) + ".ts" ) or exists( basename_of( url ) + ".tsx" ) ) {
                    content = html_for_js( basename_of( url ) + ".js" );
                    return true;
                }
            }

            // directory ?
            if ( is_directory( url ) ) {
                url = beautify( url );
                std::ostringstream os;
                os << "<html><head><title>" << url << "</title></head><body>\n";
                os << "<h3>Directories:</h3>\n";
                apply_dir( url, [ & ]( std::string sub_path ) {
                    if ( is_directory( url + "/" + sub_path ) )
                        os << "&nbsp;<a href='" << url + "/" + sub_path << "'>" << sub_path << "</a><br/>\n";
                } );
                if ( url != "/" )
                    os << "&nbsp;<a href='" << parent_path( url ) << "'>..</a><br/>\n";
                os << "<h3>Files:</h3>\n";
                apply_dir( url, [ & ]( std::string sub_path ) {
                    if ( is_regular( url + "/" + sub_path ) ) {
                        std::string ext = extension_of( sub_path );
                        if ( ext == ".html" )
                            os << "&nbsp;<a href='" << url + "/" + sub_path << "'>" << sub_path << "</a><br/>\n";
                        else if ( ext == ".coffee" or ext == ".cs" or ext == ".ts" or ext == ".tsx" or ext == ".js" ) {
                            std::string m = sub_path.substr( 0, sub_path.size() - ext.size() ) + ".html";
                            os << "&nbsp;<a href='" << url + "/" + m << "'>" << m << "</a> (made from <a href='" << url + "/" + sub_path << "'>" << sub_path << "</a>)<br/>\n";
                        }
                    }
                } );
                os << "</body></html>\n";

                mimetype = "text/html";
                content = os.str();
                return true;
            }

            // regular file
            if ( is_regular( url ) ) {
                mimetype = mymetype_for( url );
                content = content_of( url );
                return true;
            }

            // not found
            DISP_INFO( "Not found: {}", url );
            return false;
        }


        bool exists( const std::string &p ) const {
            return http_test_server->fs.count( p ) or boost::filesystem::exists( "./" + p );
        }

        bool is_directory( const std::string &p ) const {
            return boost::filesystem::is_directory( "./" + p );
        }

        bool is_regular( const std::string &p ) const {
            return http_test_server->fs.count( p ) or boost::filesystem::is_regular( "./" + p );
        }

        void apply_dir( std::string dir, std::function<void( std::string sub_path )> f ) const {
            for( boost::filesystem::directory_iterator itr( "./" + dir ), end; itr != end; ++itr ) {
                f( itr->path().filename().string() );
            }
        }

        std::string content_of( std::string url ) const {
            if ( http_test_server->fs.count( url ) )
                return http_test_server->fs[ url ];
            // else, read file
            std::ifstream f( ( "./" + url ).c_str() );
            std::ostringstream fs;
            fs << f.rdbuf();
            return fs.str();
        }

        static void save( std::string filename, std::string content ) {
            std::ofstream f( filename.c_str() );
            auto s = filename.rfind( '/' );
            if ( s != std::string::npos )
                boost::filesystem::create_directories( filename.substr( 0, s ) );
            f << content << std::flush;
        }

        static std::string shifted_left( std::string ss ) {
            std::istringstream is( ss );
            // nb spaces to remove
            auto m = std::string::npos;
            Vec<std::string> lines;
            std::string tmp;
            while ( std::getline( is, tmp ) ) {
                if ( tmp.size() )
                    m = std::min( m, tmp.find_first_not_of( ' ' ) );
                lines << tmp;
            }
            if ( m == std::string::npos )
                m = 0;

            std::string res;
            for( std::string l : lines )
                res += ( l.size() ? l.substr( m ) : l ) + "\n";
            return res;
        }

        static std::string repl_newlines_by( std::string con, std::string beg ) {
            std::string::size_type pos = 0;
            while ( true ) {
                pos = con.find( '\n', pos );
                if ( pos == std::string::npos )
                    break;
                con.replace( pos, 1, beg );
                pos += beg.size();
            }
            return con;
        }

        static std::string extension_of( std::string path ) {
            size_t n = path.rfind( '.' );
            return n != std::string::npos ? path.substr( n ) : std::string{};
        }

        static std::string basename_of( std::string path ) {
            size_t n = path.rfind( '.' );
            return n != std::string::npos ? path.substr( 0, n  ) : std::string{};
        }

        static std::string parent_path( std::string path ) {
            size_t n = path.rfind( '/' );
            return n != std::string::npos ? ( n ? path.substr( 0, n ) : "/" ) : std::string{};
        }

        static std::string mymetype_for( std::string url ) {
            std::string ext = extension_of( url );
            if ( ext == ".html" ) return "text/html";
            if ( ext == ".js"   ) return "text/javascript";
            if ( ext == ".css"  ) return "text/css";
            if ( ext == ".ico"  ) return "image/x-icon";
            return "text/plain";
        }

        std::string js_from_cs( std::string content, std::string url, bool reg ) {
            // in cache ?
            std::string tmp, key = content + url + to_string( reg );
            if ( http_test_server->gencache->Get( leveldb::ReadOptions{}, key, &tmp ).ok() )
                return tmp;

            // else, preprocess and call coffee
            save( ".inp.cs" , preprocessing( content ) );
            exec( "cjsx -cpb .inp.cs > .out.js # compilation of " + url );
            return js_from_js( content_of( ".out.js" ), key, url, reg );
        }

        std::string js_from_ts( std::string ts_url, std::string url, bool reg ) {
            // in cache ?
            std::string tmp, key = content_of( ts_url ) + url + to_string( reg );
            if ( http_test_server->gencache->Get( leveldb::ReadOptions{}, key, &tmp ).ok() )
                return tmp;

            // else, preprocess and call tsc
            // save( ".inp.ts", preprocessing( content ) );
            exec( "tsc --jsx react --pretty -m commonjs -t ES5 --rootDir . --outDir .out typings/browser.d.ts ." + ts_url ); //  + " # compilation of " + url + " -> .out/" + url
            return js_from_js( content_of( ".out" + url ), key, url, reg );
        }

        std::string preprocessing( std::string precon ) {
            while ( true ) {
                auto b = precon.find( "###~", 0 ); if ( b == std::string::npos ) break;
                auto e = precon.find( "~###", b ); if ( e == std::string::npos ) break;

                std::string msg = precon.substr( b + 4, e - ( b + 4 ) );
                std::string tmp = ".tmp_res_cmd";
                auto sxe = msg.find( '\n' );
                if ( sxe == std::string::npos ) { // only one command
                    exec( msg + " > " + tmp );
                } else {
                    std::string exe = msg.substr( 0, sxe );
                    std::string dat = ".tmp_dat_cmd";
                    if ( exe.find( "g++" ) != std::string::npos )
                        dat += ".h";
                    save( dat, shifted_left( msg.substr( sxe + 1 ) ) );
                    exec( exe + " " + dat + " > " + tmp );
                }
                auto bl = precon.rfind( '\n', b );
                precon = precon.substr( 0, b ) + repl_newlines_by( content_of( tmp ), precon.substr( bl, b - bl ) ) + precon.substr( e + 4 );
            }

            return precon;
        }

        std::string js_from_js( std::string content, std::string url, bool reg ) {
            // in cache ?
            std::string tmp, key = content + url + to_string( reg );
            if ( http_test_server->gencache->Get( leveldb::ReadOptions{}, key, &tmp ).ok() )
                return tmp;

            // else, call babel
            return js_from_js( content, key, url, reg );
        }

        std::string js_from_js( std::string content, std::string key, std::string url, bool reg ) {
            // call babel -> c
            // std::ofstream cs( ".inp.js" ); cs << content << std::flush;
            // exec( "node_modules/.bin/babel .inp.js --out-file .out.js" ); // --source-maps inline

            std::ostringstream c;

            bool main = url.rfind( ".main.js" ) == url.size() - 8 and url.size() > 8;
            if ( main ) {
                Vec<std::string> req;
                std::set<std::string> parsed;
                std::map<std::string,std::string> req_as;
                find_required( req, content, parent_path( url ), parsed, req_as );

                std::ostringstream os;
                c << "var __required_as = {};\n";
                for( std::pair<std::string,std::string> p : req_as )
                    if ( p.first != p.second )
                        c << "__required_as[ '" << p.first << "' ] = '" << p.second << "';\n";

                std::string header_script, mt_header_script;
                virtual_content_of( header_script, mt_header_script, "/cpp/RiotBase/HttpTestServer/header_script.js", false );
                c << header_script + "\n";

                c << "var __dependencies = [\n";
                for( std::string r : req ) {
                    //std::string njs, mt;
                    //virtual_content_of( njs, mt, r );
                    //c << njs;
                    c << "    '" << r << "',\n";
                }
                c << "]\n";
            }

            // std::string c = content_of( ".out.js" );
            if ( reg ) {
                c << "__register( '" + url + "', function( module, exports, require ) { \n";
                c << "    " << repl_newlines_by( content, "\n    " )<< "\n";
                c << "} );\n";
            } else
                c << content;


            if ( main ) {
                std::string header_script, mt_header_script;
                virtual_content_of( header_script, mt_header_script, "/cpp/RiotBase/HttpTestServer/dep_loader.js", false );
                c << header_script + "\n";
                c << "__dep_loader( __dependencies, function() { __req('','" << url << "'); } );\n";
            }

            // output
            http_test_server->gencache->Put( leveldb::WriteOptions{}, key, c.str() );
            return c.str();
        }

        std::string html_for_js( std::string js_filename ) {
            std::string js, mt;
            Vec<std::string> req;
            std::map<std::string,std::string> req_as;
            if ( virtual_content_of( js, mt, js_filename ) ) {
                std::set<std::string> parsed;
                find_required( req, js, parent_path( js_filename ), parsed, req_as );
            }

            std::string header_script, mt_header_script;
            virtual_content_of( header_script, mt_header_script, "cpp/RiotBase/HttpTestServer/header_script.js", false );

            std::ostringstream os;
            os << "<html><head><meta charset='UTF-8'><title>" << js_filename << "</title>\n";
            os << "    <script>\n";
            os << "        __required_as = {};\n";
            for( std::pair<std::string,std::string> p : req_as )
                if ( p.first != p.second )
                    os << "        __required_as[ '" << p.first << "' ] = '" << p.second << "';\n";
            os << "        " << repl_newlines_by( header_script, "\n        " ) + "\n";
            os << "    </script>\n";
            for( std::string r : req )
                os << "    <script src='" << r << "'></script>\n";
            os << "    <script src='" << js_filename << "'></script>\n";
            os << "</head><body>\n";
            os << "    <div id='test_div'>\n";
            os << "    </div>\n";
            os << "        <div id='info_0'/>\n";
            os << "        <div id='info_1'/>\n";
            os << "    <script>__req('','" << js_filename << "');</script>\n";
            os << "</body></html>\n";

            return os.str();
        }

        void make_local_copy_rec( std::string js_filename ) {
            std::string js, mt;
            Vec<std::string> req;
            std::map<std::string,std::string> req_as;
            if ( virtual_content_of( js, mt, js_filename, false ) ) {
                std::set<std::string> parsed;
                find_required( req, js, parent_path( js_filename ), parsed, req_as );
                DISP_INFO( "Generating in {} (size={})", "local" + js_filename, js.size() );
                save( "./local" + js_filename, js );
            }

            for( std::string r : req ) {
                if ( virtual_content_of( js, mt, r, false ) ) {
                    DISP_INFO( "Generating in {}", "local" + r );
                    save( "local" + r, js );
                }
            }
        }

        /*
          see type_char.py
        */
        static unsigned char type_char( char c ) {
            static const unsigned char type_char_[] = {
                0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 3, 0, 0, 0, 0, 3,
                0, 0, 0, 0, 0, 0, 0, 0,
                2, 2, 2, 2, 2, 2, 2, 2,
                2, 2, 0, 0, 0, 0, 0, 0,
                0, 1, 1, 1, 1, 1, 1, 1,
                1, 1, 1, 1, 1, 1, 1, 1,
                1, 1, 1, 1, 1, 1, 1, 1,
                1, 1, 1, 0, 0, 0, 0, 1,
                0, 1, 1, 1, 1, 1, 1, 1,
                1, 1, 1, 1, 1, 1, 1, 1,
                1, 1, 1, 1, 1, 1, 1, 1,
                1, 1, 1, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0,
            };
            return type_char_[ unsigned( c ) ];
        }

        #define TYPE_CHAR_unknown 0
        #define TYPE_CHAR_letter 1
        #define TYPE_CHAR_number 2
        #define TYPE_CHAR_string 3

        static bool is_num     ( char c ) { return type_char( c ) == TYPE_CHAR_number;   }
        static bool is_letter  ( char c ) { return type_char( c ) == TYPE_CHAR_letter;   }

        enum {
            AST_VARIABLE,
            AST_STRING,
            AST_NUMBER,
            AST_OTHER,
        };

        static void simple_js_ast_parser( const char *s, const char *end, std::function<void( int ast_type, const char *beg, int len )> f ) {
            while ( s < end ) {
                switch ( type_char( *s ) ) {
                case TYPE_CHAR_letter: {
                    const char *old_str = s++;
                    while ( is_num( *s ) or is_letter( *s ) )
                        ++s;
                    f( AST_VARIABLE, old_str, s - old_str );
                    break;
                }
                case TYPE_CHAR_string: {
                    const char t = *s, *old_str = s++;
                    if ( t == '"' )
                        while ( *s and ( *s != '"' or ( s[-1] == '\\' and s[-2] != '\\' ) ) )
                            ++s;
                    else
                        while ( *s and ( *s != '\'' or ( s[-1] == '\\' and s[-2] != '\\' ) ) )
                            ++s;
                    if ( *s )
                        f( AST_STRING, old_str + 1, s++ - old_str - 1 );
                    break;
                }
                case TYPE_CHAR_number: {
                    const char *old_str = s;

                    if ( s[ 0 ] == '0' and ( s[ 1 ] == 'x' or s[ 1 ] == 'X' ) ) { // 0xFFE -> hexadecimal
                        for( s += 2; is_num( s[0] == 0 ) or ( s[0]>='a' and s[0]<='f' ) or ( s[0]>='A' and s[0]<='F' ); ++s );
                        f( AST_NUMBER, old_str, s - old_str );
                    } else { // decimal number
                        while ( is_num( *( ++s ) ) );
                        if ( s[ 0 ] == '.' and is_num( s[ 1 ] ) )
                            while ( is_num( *( ++s ) ) );
                        if ( s[0] == 'e' or s[0] == 'E' ) { // numbers after [eE]
                            s += ( s[ 1 ] == '-' or s[ 1 ] == '+' );
                            while ( is_num( *( ++s ) ) );
                        }
                        // attributes (unsigned, float, pointer, ...)
                        for( ; s[0] == 'u' or s[0] == 'i' or s[0] == 'p' or s[0] == 'l'; ++s );
                        f( AST_NUMBER, old_str, s - old_str );
                    }

                    break;
                }
                default:
                    if ( *s == '/' ) { // comment ?
                        if ( s[ 1 ] == '/' ) { // -> // ...
                            s += 2;
                            while( *s and *s != '\n' ) // looking for a carriage return
                                ++s;
                            break;
                        } else if ( s[ 1 ] == '*' ) { // -> /* pouet */
                            s += 2;
                            while ( *s and ( s[ -1 ] != '*' or s[ 0 ] != '/' ) )
                                ++s;
                            if ( *s )
                                ++s;
                            break;
                        }
                    }
                    f( AST_OTHER, s++, 1 );
                    break;
                }
            }
        }

        static bool starts_with( std::string str, std::string pat ) {
            return str.size() >= pat.size() and str.substr( 0, pat.size() ) == pat;
        }

        bool check_package( std::string &file, std::string path ) {
            if ( exists( path + "/package.json" ) ) {
                Json::Reader reader;
                Json::Value json_data;
                if ( reader.parse( content_of( path + "/package.json" ), json_data ) ) {
                    Json::Value main = json_data[ "main" ];
                    file = path + ( main.type() != Json::nullValue ? "/" + main.asString() : "/index.js" );
                }
                return true;
            }
            return false;
        }

        void find_required( Vec<std::string> &required, std::string content, const std::string path, std::set<std::string> &parsed, std::map<std::string,std::string> &req_as ) {
            if ( parsed.count( content ) )
                return;
            parsed.insert( content );

            // vec of required filenames
            Vec<std::string> req_vec;
            bool prev_is_require = false;
            simple_js_ast_parser( content.c_str(), content.data() + content.size(), [ & ]( int ast_type, const char *beg, int len ) {
                if ( ast_type == AST_STRING and prev_is_require )
                    req_vec << std::string( beg, beg + len );
                else if ( *beg != '(' and *beg != ' ' )
                    prev_is_require = ast_type == AST_VARIABLE and len == strlen( "require" ) and strncmp( beg, "require", len ) == 0;
            } );

            // register and parse rec
            for( std::string module : req_vec ) {
                // module name. If ./foo -> /pa/th/foo, ./../foo -> /pa/foo
                bool rel = false;
                std::string loc_path = path;
                while ( true ) {
                    if ( starts_with( module, "./" ) ) {
                        module = module.substr( 2 );
                        rel = true;
                    } else if ( starts_with( module, "../" ) ) {
                        loc_path = loc_path.substr( 0, loc_path.rfind( '/' ) );
                        module = module.substr( 3 );
                        rel = true;
                    } else
                        break;
                }
                if ( rel )
                    module = loc_path + '/' + module;


                // file will be the "real" filename (in the virtual file system)
                std::string file = module;

                auto test = [&]( std::string p, std::string v = {} ) {
                    if ( exists( p ) ) {
                        file = v.size() ? v : p;
                        return true;
                    }
                    return false;
                };

                if ( file[ 0 ] == '/' ) { // absolute
                    check_package( file, file )            or
                    test( file + "/index.js" )             or
                    test( file + ".coffee", file + ".js" ) or
                    test( file + ".cs", file + ".js" )     or
                    test( file + ".ts", file + ".js" )     or
                    test( file + ".tsx", file + ".js" )    or
                    test( file + ".js" );
                } else {
                    check_package( file, "/node_modules/" + file ) or
                    test( "/node_modules/" + file + "/index.js" )  or
                    test( "/node_modules/" + file + ".js" )        or
                    test( "/node_modules/" + file );
                }

                // register how the file was asked
                file = beautify( file );
                req_as[ module ] = file;

                // reg + rec parse
                if ( not required.contains( file ) ) {
                    std::string content, mt;
                    if ( virtual_content_of( content, mt, file ) )
                        find_required( required, content, parent_path( file ), parsed, req_as );
                    if ( not required.contains( file ) )
                        required << file;
                }
            }
        }

        static std::map<std::string,std::string> decode_url_attr( std::string url ) {
            url = url.substr( url.find( '?' ) + 1 );

            std::map<std::string,std::string> res;
            std::istringstream iss( url );
            std::string str;
            while ( std::getline( iss, str, '&' ) ) {
                int p = str.find( '=' );
                res[ url_decode( str.substr( 0, p ) ) ] = url_decode( str.substr( p + 1 ) );
            }
            return res;
        }

        static int exec( std::string cmd ) {
            std::cout << cmd << std::endl;
            return system( cmd.c_str() );
        }

        static std::string beautify( std::string url ) {
            Vec<std::string> items;
            for( unsigned i = 0, o = 0; ; ++i ) {
                if ( i == url.size() or url[ i ] == '/' ) {
                    if ( i - o == 2 and url[ o + 0 ] == '.' and url[ o + 1 ] == '.' )
                        items.resize( items.size() ? items.size() - 1 : 0 );
                    else if ( i - o != 0 and ( i - o != 1 or url[ o ] != '.' ) )
                        items << url.substr( o, i - o );

                    if ( i == url.size() )
                        break;
                    o = i + 1;
                }
            }

            if ( items.size() == 0 )
                return ".";

            url.clear();
            for( std::string item : items )
                url += '/' + item;
            return url;
        }

        std::string     req;
        HttpTestServer *http_test_server;
    };

    struct MyListener : TcpListener {
        MyListener( HttpTestServer *http_test_server, unsigned port, const char *cert_file, const char *key_file ) : TcpListener( port ), http_test_server( http_test_server ), ssl_ctx( SslCtx::Method::SSLv23, cert_file, key_file ), port( port ) {}
        MyListener( HttpTestServer *http_test_server, unsigned port ) : TcpListener( port ), http_test_server( http_test_server ), port( port ) {}

        virtual bool    connection( int fd, const InetAddress & ) { *ev_loop << ( ssl_ctx ? (Event *)new MyHttpRequest<TcpSslConnection>( http_test_server, ssl_ctx, fd ) : (Event *)new MyHttpRequest<TcpRawConnection>( http_test_server, fd ) ); return true; }
        virtual void    rdy       () { DISP_INFO( "{} ready on port {}", ( ssl_ctx ? "https" : "http" ), port ); }

        HttpTestServer *http_test_server;
        SslCtx          ssl_ctx;
        unsigned        port;
    };
}

HttpTestServer::HttpTestServer() {
    // open cache db
    leveldb::Options options;
    options.create_if_missing = true;
    leveldb::DB::Open( options, ".cache_http_test_server", &gencache );
}

HttpTestServer::~HttpTestServer() {
    // close cache db
    delete gencache;

    // kill browser instances
    for( int pid : to_kill_at_the_end )
        kill( pid, SIGTERM );
}

void HttpTestServer::bind( EvLoop *ev_loop, unsigned port, const char *cert_file, const char *key_file ) {
    *ev_loop << new MyListener( this, port, cert_file, key_file );
}

void HttpTestServer::bind( EvLoop *ev_loop, unsigned port ) {
    *ev_loop << new MyListener( this, port );
}

int HttpTestServer::launch_browser( const std::string &url ) {
    if ( int pid = fork() )
        return pid;

    // yes it is really bad but haven't seen how to launch firefox in a separate process if we already have at least one running instance
    system( "killall firefox 2> /dev/null" );
    int res = execlp( "firefox", "firefox", url.c_str(), 0 ); //  "--safe-mode",
    DISP_ERROR( "execlp (res={}): {}", res, strerror( errno ) );
    return 0;
}

void HttpTestServer::run( unsigned port, const char *cert_file, const char *key_file, double timeout ) {
    Gev ev;
    bind( &ev, port, cert_file, key_file );
    ev.run( timeout );
}

void HttpTestServer::run( const std::string &url, unsigned port, const char *cert_file, const char *key_file, double timeout ) {
    int pid = launch_browser( url );
    run( port, cert_file, key_file, timeout );
    kill( pid, SIGTERM );
}

} // namespace RiotBase
