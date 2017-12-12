#pragma once

#include "../Core/Methods.h"
#include <mutex>

namespace RiotBase {
class Lazyfier;

/**
  Goal: cut the graphs from the current thread (lazyfiers will have symbols)
  This alternative representation enable to compute the stuff in parallel
*/
class Scheduler {
public:
    struct Adder {
        Adder( Scheduler *s );
        ~Adder();

        Adder          &operator<<( Rp *rp );

        Vec<Lazyfier *> lazyfiers;
        Scheduler       *s;
    };

    Adder      raii_adder();

protected:
    friend struct Adder;

    // Vec<>
    std::mutex m;
};

extern Scheduler global_scheduler;

} // namespace RiotBase
