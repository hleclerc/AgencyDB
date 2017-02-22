#pragma once

#include "../System/VoidStruct.h"
#include "EvLoop.h"
#include "TcpListener.h"

namespace RiotBase {

/**
  TcpListener that creates an object of type EO (with the corresponding fd, and data as constructor arguments) each time there's a new connection
*/
template<class EO,class AdditionnalData=VoidStruct,class PA=TcpListener>
class ConnectionFactoryTcpListener : public PA {
public:
    ConnectionFactoryTcpListener( const char *port, AdditionnalData data = AdditionnalData() ) : PA( port ), data( data ) {}
    ConnectionFactoryTcpListener( typename PA::VtableOnly vo ) : PA( vo ) {}

    virtual bool connection( int fd ) {
        *PA::ev_loop << new EO( data, fd );
        return true;
    }

    AdditionnalData data;
};

} // namespace RiotBase
