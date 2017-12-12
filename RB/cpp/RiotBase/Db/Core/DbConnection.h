#pragma once

#include "../../EvLoop/TcpSslConnection.h"
#include "../../System/XxHash.h"
#include "../../System/UsrId.h"

namespace RiotBase {
class DbSession;

///
class DbConnection : public TcpRawConnection {
public:
    DbConnection( int fd, DbSession *session );
    ~DbConnection();

protected:
    friend class DbSession;

    template<class T> T *beg_cmd() { static_assert( sizeof( T ) <= sizeof( parsing_context ), "No enough size" ); return new ( parsing_context ) T; }
    template<class T> T *pc() { return reinterpret_cast<T *>( parsing_context ); }
    struct ParsingContext { virtual ~ParsingContext() {} };

    virtual bool parse ( Buffer *buffer );

    DevId        dev_id() const;
    UsrId        usr_id() const;

    Buffer      *pending_cb_queue;         ///< if we need to add the current buffer to a previous one
    char         parsing_context[ 8 * 8 ]; ///< large enough to store all the parsing context types
    PI32         inp_hash;
    void        *inp_cont;
    DbSession   *session;
    XxHash       hash;
};

} // namespace RiotBase
