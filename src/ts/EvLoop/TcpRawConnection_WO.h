#pragma once

#include "../System/VoidStruct.h"
#include "TcpRawConnection.h"

namespace RiotBase {

/**
  Event_obj which calls obj->parse( beg, end ) each time new data comes in

*/
template<class ObjWithParse,class AdditionalData=VoidStruct,bool del=false>
class TcpRawConnection_WO : public TcpRawConnection {
public:
    TcpRawConnection_WO( ObjWithParse *obj, int fd, AdditionalData data = AdditionalData() ) : TcpRawConnection( fd ), data( data ), obj( obj ) {
    }

    ~TcpRawConnection_WO() {
        if ( del )
            delete obj;
    }

    virtual bool parse( RcPtr<Buffer> buff ) {
        return obj->parse( this, buff );
    }

    AdditionalData data;

protected:
    ObjWithParse *obj;
};

} // namespace RiotBase
