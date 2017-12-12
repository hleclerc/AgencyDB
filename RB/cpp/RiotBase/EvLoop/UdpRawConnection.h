#pragma once

#include "UdpSocketAncestor.h"
#include "../System/Log.h"

namespace RiotBase {

/**
  Beware: there are no checks about the msg sizes
*/
template<class Impl,PT msg_size=500>
class UdpRawConnection : public UdpConnection {
public:
    template<class _Impl,class _Conn> friend class UdpSocket;
    enum { inp_buf_size = msg_size };
    using TImpl = Impl;

    UdpRawConnection() : output( MsgQueue::Msg::New( msg_size ) ) {}
    ~UdpRawConnection() { free( output ); }

    void           flush                 () { if ( output->size and not send( output ) ) output = MsgQueue::Msg::New( msg_size ); } ///< try to send pending dgram (or push it in the wait queue if the output buffer is full)
    void           write_some            ( const void *data, PT size ) { memcpy( output->data + output->size, data, size ); output->size += size; }
    void           write_byte            ( PI8 val ) { output->data[ output->size++ ] = val; }
    void           write_byte_wo_beg_test( PI8 val ) { write_byte( val ); }

protected:
    void           parse_anc             ( char *data, PT size ) { static_cast<Impl *>( this )->parse( data, size ); } ///< by default, directly calls parse (but can be surdefined e.g. for filtering/decyphering/...)
    const PI8     *cur_output_ptr        () { return output->data + output->size; }

    MsgQueue::Msg *output; ///< where we save the stuff before flush()
};

} // namespace RiotBase
