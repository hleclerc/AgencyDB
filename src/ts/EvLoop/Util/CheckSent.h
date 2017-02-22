#pragma once

#include "../Timer.h"

namespace RiotBase {

/**
*/
class CheckSent : public Timer {
public:
    using Timer::Timer;

    virtual bool timeout( int );
};

} // namespace RiotBase
