#pragma once

#include "../../EvLoop/TcpConnection_Ssl.h"
#include "../../System/XxHash.h"
#include "../../System/DevId.h"

namespace RiotBase {
class TrackerServerSession;

///
class TrackerConnection : public BufferedSocket {
public:
    TrackerConnection( int fd, TrackerServerSession *session );
    ~TrackerConnection();

    DevId           get_dev_id() const;

protected:
    template<class T> T *beg_cmd() { static_assert( sizeof( T ) <= sizeof( parsing_context ), "No enough size" ); return new ( parsing_context ) T; }
    template<class T> T *pc() { return reinterpret_cast<T *>( parsing_context ); }
    struct ParsingContext { virtual ~ParsingContext() {} };

    virtual bool    parse( Buffer *buff );

    DevId           dev_id();
    UsrId           usr_id();

    char            parsing_context[ 12 * 8 ]; ///< large enough to store all the parsing context types
    Buffer         *pending_cb_queue;         ///< if we need to add the current buffer to a previous one
    PI32            inp_hash;
    void           *inp_cont;
    TrackerServerSession *session;
    XxHash          hash;
};

} // namespace RiotBase
