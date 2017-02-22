#pragma once


namespace RiotBase {

/**
  Each time we have a new address, we make a new instance of ConnectionType

*/
class UdpSocket : public Event {
public:
    UdpSocket();

    void                   bind                  ( unsigned port ); ///< if we want to accept messages

protected:
    virtual void           on_bind_error         ( const char *msg ); ///<
};

} // namespace RiotBase
