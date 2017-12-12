#pragma once

namespace RiotBase {
class Rp;

///
class LazyOpScheduler {
public:
    void operator<<( Rp *rp );
};

extern LazyOpScheduler lazy_op_scheduler;

} // namespace RiotBase
