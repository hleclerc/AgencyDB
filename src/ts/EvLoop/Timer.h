#pragma once

#include "Event.h"

namespace RiotBase {

/**
*/
class Timer : public Event {
public:
    Timer( double delay, double freq = 0 );

    static int make_timer_fd( double delay, double freq );

protected:
    virtual void inp();
    virtual bool has_out();
    virtual bool timeout( int nb_expirations ) = 0; ///< return true to keep the timer running
};

} // namespace RiotBase
