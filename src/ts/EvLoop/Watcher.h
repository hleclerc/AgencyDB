#pragma once

#include "../System/TypeConfig.h"
#include "Event.h"

namespace RiotBase {

/// enable to emit a signal to an event loop
///  -> call event if sig (from any thread) has been called
class Watcher : public Event {
public:
    Watcher();

    void         sig();

protected:
    virtual void inp();
    virtual bool has_out();
    virtual bool event( PI64 n ); ///< return true to continue watching for signal. It you want to stop the program call ev_loop->stop()
};

} // namespace RiotBase
