#pragma once

#include "../../System/SetOfPatchId.h"
#include "../../System/FqId.h"

namespace RiotBase {
class TcpRawConnection;
class Category;
class Db;

///
class DbSession {
public:
    DbSession( Db *db, int fd, bool server );
    ~DbSession();

    // connection mgmt
    void                           flush               (); ///< do the flush
    void                           activate            (); ///< to say it's not time to close the connection
    bool                           is_a_server_session () const { return server; }
    bool                           dev_id_ready        () const { return connection and dev_id; }

    DevId                          get_dev_id          () const { return dev_id; }
    UsrId                          get_usr_id          () const { return usr_id; }
    FqId                           fqid                () const { return { db, dev_id, usr_id }; }
    bool                           connected           () const { return connection; }

    // messages
    void                           send_req_identity   (); ///< send a request to get the identity of the other side
    void                           send_req_auth_std   ( const std::string &login, const std::string &password ); ///<
    void                           send_req_auth_tok   ( const std::string &token ); ///<
    void                           send_req_auth_yub   ( const std::string &login, const std::string &password, const std::string &yubikey ); ///<
    void                           send_ans_auth       ( DevId dev_id, UsrId usr_id = {} ); ///< dev_id == 0 means bad credentials
    void                           send_graph_cmd      ( PI64 dst_id, const Vec<PT> &out_offset, const CbStringPtr &msg );
    void                           send_res_graph      ( PI64 dst_id, PT out_num, const CbStringPtr &msg );

    void                           send_req_content    ( Inode inode, const Category *category );
    void                           send_req_changes    ( Inode inode, const Category *category );
    void                           send_new_changes    ( Inode inode, const CbString &msg ); // , const Vec<DevId> &dst
    void                           send_ack_changes    ( Inode inode );

    Db                            *get_db              () { return db; }

private:
    template<class T> friend void dec_ref( const T *ptr );
    template<class T> friend T *inc_ref( T *p );
    friend struct RcPtr<DbSession>;
    friend struct DefaultGetPrev;
    friend class DbConnection;

    DbSession( Db *db, bool server );

    // answers
    void                           recv_req_identity   ( const Vec<std::pair<UsrId,std::string>> &vec_usr_enc );
    void                           recv_ans_identity   ( const std::string &msg );
    void                           recv_ack_identity   ( unsigned res );
    void                           recv_req_auth_std   ( const CbString &login, const CbString &password );
    void                           recv_req_auth_tok   ( std::string token );
    void                           recv_req_auth_yub   ( std::string login, std::string password, std::string yubikey );
    void                           recv_ans_auth       ( DevId dev_id, UsrId usr_id );

    void                           recv_req_content    ( Inode inode );
    void                           recv_req_changes    ( DevId dev_id, UsrId usr_id );
    void                           recv_new_changes    ( Inode inode, CbString &msg );
    void                           recv_ack_changes    ( DevId dev_id, UsrId usr_id );

    void                           recv_graph_cmd      ( PI64 dst_id, const Vec<PT> &num_outputs, CbString &graph );

    void                           send_ans_identity   ( const Vec<std::pair<UsrId,std::string>> &vec_usr_enc );

    // internal
    void                           _set_dev_and_usr    ( DevId id, UsrId usr );
    void                           _reg_connection     (); ///< ?
    void                           _reg_changed        (); ///< say that we will need a flush

    Vec<std::function<void(void)>> todo_when_connection_up;
    Vec<std::function<void(void)>> todo_when_dev_id_known;
    TcpRawConnection            *connection;
    DevId                          dev_id;                  ///< dev_id of distant machine
    UsrId                          usr_id;
    DbSession                     *prev;                    ///< prev changed
    Db                            *db;
    std::string                    ran;                     ///< random key used to encrypt the dev_id
    mutable int                    cpt_use;
    bool                           server;
};

} // namespace RiotBase
