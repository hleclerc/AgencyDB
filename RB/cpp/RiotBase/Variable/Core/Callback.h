#pragma once

#include "../../System/CycleEndedList.h"
#include "../../System/RcPtr.h"

namespace RiotBase {
class Rp;

///
class Callback {
public:
    Callback() : prev( 0 ), cpt_use( 0 ) {}
    virtual ~Callback();

    virtual void                    exec( Rp **rp = 0 ) = 0;
    virtual std::string             name() const;

    static  bool                    exec_all();                     ///< execute all Callbacks in `waiting`
    void                            reg_wait() { waiting << this; } ///< execute all Callbacks in `waiting`

    Callback                       *prev;
    mutable int                     cpt_use;

    static CycleEndedList<Callback> waiting;                        ///< to be executed during the next "idle round"
};

} // namespace RiotBase
