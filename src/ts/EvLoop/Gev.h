#pragma once

#include "../System/LazyNew.h"
#include "../System/Vec.h"
#include "PostFunc.h"
#include "EvLoop.h"

#include <functional>

namespace RiotBase {
class Db;

///
class Gev : public EvLoop {
public:
    int  run( double timeout = 0.0 ); ///< infinite loop (until timeout if != 0, or gev->stop is called)

    void add_timer( std::function<bool(int)> &&f, double delay, double freq = 0 );

    // 192.168.0.254
    // Vec<void *> lazy_sources; ///< dynobj that are waiting for an Exec to launch something (and that may a have parents waiting for the result)
};

extern LazyNew<Gev> gev;

} // namespace RiotBase
