#pragma once

namespace RiotBase { class EvLoop; }
namespace leveldb { class DB; }

#include "../System/Vec.h"
#include <string>
#include <map>

namespace RiotBase {

/**
*/
class HttpTestServer {
public:
    HttpTestServer();
    ~HttpTestServer();

    void                              bind           ( EvLoop *ev_loop, unsigned port, const char *cert_file, const char *key_file ); ///< https
    void                              bind           ( EvLoop *ev_loop, unsigned port ); ///< http
    int                               launch_browser ( const std::string &url ); ///< return the pid
    void                              run            ( unsigned port, const char *cert_file = 0, const char *key_file = 0, double timeout = 0.0 ); ///< start an event loop, with an active listener, defined by port, perm.
    void                              run            ( const std::string &url, unsigned port, const char *cert_file = 0, const char *key_file = 0, double timeout = 0.0 ); ///< launch a browser on address `url` and start the event loop (that can be stoped using quit_server in the javascript side)

    Vec<int>                          to_kill_at_the_end;
    Vec<std::string>                  log, err; ///< log and err of the javascript application
    std::map<std::string,std::string> result;   ///<
    std::map<std::string,std::string> fs;       ///< fake filesystem
    leveldb::DB                      *gencache;
};

} // namespace RiotBase
