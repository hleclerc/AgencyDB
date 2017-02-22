#pragma once

#include "../../System/VoidStruct.h"
#include "TcpListener.h"

namespace RiotBase {

/**
*/
template<class ObjWithConnection,class AdditionalData=VoidStruct,bool del=false>
class TcpListener_WO : public TcpListener {
public:
    TcpListener_WO( ObjWithConnection *obj, const char *port, AdditionalData data = AdditionalData() ) : TcpListener( port ), obj( obj ), data( data ) {
    }

    ~TcpListener_WO() {
        if ( del )
            delete obj;
    }

    virtual bool connection( int fd ) {
        return obj->connection( this, fd );
    }

    AdditionalData data;

protected:
    ObjWithConnection *obj;
};

} // namespace RiotBase
