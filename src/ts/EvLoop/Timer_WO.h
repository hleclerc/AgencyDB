#pragma once

#include "../System/VoidStruct.h"
#include "../System/Stream.h"
#include "Timer.h"

namespace RiotBase {

/**
*/
template<class ObjWithTimeout,class AdditionalData=VoidStruct,bool del=false>
class Timer_WO : public Timer {
public:
    Timer_WO( ObjWithTimeout *obj, double delay, AdditionalData data = AdditionalData() ) : Timer( delay ), data( data ), obj( obj ) {
    }

    ~Timer_WO() {
        if ( del )
            delete obj;
    }

    virtual bool timeout( int nb_expirations ) { ///< return false to delete this
        return obj->timeout( this, nb_expirations );
    }

    AdditionalData data;

protected:
    ObjWithTimeout *obj;
};

} // namespace RiotBase
