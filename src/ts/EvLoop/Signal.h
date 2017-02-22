#pragma once

#include "Event.h"

namespace RiotBase {

/**
*/
class Signal : public Event {
public:
    Signal( const int *sigs );

protected:
    virtual void inp();
    virtual bool has_out();
    virtual bool signal( int s ) = 0; ///< return true to continue watching for signal. It you want to stop the program call ev_loop->stop()
};

} // namespace RiotBase
