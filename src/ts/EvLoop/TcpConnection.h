#pragma once

#include "../System/CbQueue.h"
#include "Event.h"

namespace RiotBase {

/**
*/
class TcpConnection : public Event {
public:
    TcpConnection();
    TcpConnection( VtableOnly );
    virtual ~TcpConnection();

    virtual void flush() = 0; ///< send on the wire what we can of `cw`

    CbQueue      cw;          ///< where to write the stuff before flush
protected:
    virtual bool parse() = 0; ///< uses inp_buff. return false if we have enough data

    Buffer      *inp_buff;    ///< buffer for input data
};

} // namespace RiotBase
