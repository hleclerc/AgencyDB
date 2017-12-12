#pragma once

#include "../../EvLoop/PostFunc.h"

namespace RiotBase {
class Db;

/// executed at the beginning of the ev_loop, and every n events
class DbPostFunc : public PostFunc {
public:
    DbPostFunc( Db *db );
    virtual bool exec();
    Db *db;
};

} // namespace RiotBase
