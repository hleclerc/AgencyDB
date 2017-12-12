#pragma once

#include "../../System/UsrId.h"
#include "../../System/Vec.h"
#include <functional>

namespace RiotBase {
class DbConnection;
class Db;

/**
  if send_... function are called manually (i.e. not in a parsing context for instance -- which call flush), flush has to be called manually
*/
class DbSession {
public:
    DbSession( Db *db, std::string addr, unsigned port, int fd ); ///< client mode
    DbSession( Db *db, int fd );                                  ///< server mode
    ~DbSession();

    void                               send_req_identity   ();
    void                               send_ans_identity   ( Vec<std::pair<UsrId, CbString> > &keys );
    void                               send_corrupted_data ( PI8 code );
    void                               send_graph_cmd      ( CbStringPtr cq );
    void                               send_graph_ans      ( PI64 num, CbStringPtr cq );
    void                               send_patch_data     ( Inode loc_inode, CbQueue &&msg );
    void                               send_quit           ();

    void                               flush               ();

    Db                                *db;
    DevId                              glo_dev_id;              ///< device id of the connected instance
    UsrId                              glo_usr_id;              ///< user id of the connected instance
    mutable int                        cpt_use;

protected:
    friend class DbConnection;
    friend class Db;

    void                               recv_req_identity   ( Vec<std::pair<UsrId,CbString>> &keys );
    void                               recv_ans_identity   ( const std::string &msg );
    void                               recv_ack_identity   ( PI32 res );
    void                               recv_graph_cmd      ( CbStringPtr msg );
    void                               recv_graph_ans      ( PI64 num, CbStringPtr msg );
    void                               recv_quit           ();

    void                               try_to_connect_if_not_done();

    void                               when_connection_up  ( const std::function<void(void)> &f );
    void                               set_connection      ( DbConnection *c );
    void                               wake_up_if_necessary();
    CbQueue                           *cb_queue            ();

    void                               reg_changed();
    void                               set_dev_and_usr( DevId dev, UsrId usr );

    // connection
    bool                               server_mode;
    DbConnection                      *connection;
    std::string                        addr;
    unsigned                           port;

    std::string                        ran;                     ///< random key used to encrypt the dev_id
    CbQueue                            tmp_cb_queue;            ///< used if not connection
    mutable Vec<std::function<void()>> todo_when_id_known;      ///< when dev and usr of the other side (ext dev) are known
};



} // namespace RiotBase
