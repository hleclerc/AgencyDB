#pragma once

#include "../System/VoidStruct.h"
#include "Signal.h"

namespace RiotBase {

/**
*/
template<class ObjWithSignal,class AdditionalData=VoidStruct,bool del=false>
class Signal_WO : public Signal {
public:
    Signal_WO( ObjWithSignal *obj, const int *sigs, AdditionalData data = AdditionalData() ) : Signal( sigs ), data( data ), obj( obj ) {
    }

    ~Signal_WO() {
        if ( del )
            delete obj;
    }

    virtual bool signal( int s ) {
        return obj->signal( this, s );
    }

    AdditionalData data;

protected:
    ObjWithSignal *obj;
};

} // namespace RiotBase
