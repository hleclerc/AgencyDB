#pragma once

#include "../System/ArgsAsTypeList.h"
#include "../System/BinStream.h"
#include "../System/Crc32c.h"
#include "../System/Log.h"
#include "UdpConnection.h"
#include <functional>

namespace RiotBase {

/**
  BaseUdpConn can be for instance a UdpDtlsConnection<Impl,msg_size>

  Sets of command will automatically be splitted if size > msg_size.
*/
template<class BaseUdpConn,class CheckSumType=Crc32c>
class UdpHashedCmdsConnection : public BaseUdpConn {
public:
    using TImpl = typename BaseUdpConn::TImpl;
    struct BsAc { ~BsAc() { bs.buf->close_cmd(); } template<class T> BsAc &operator<<( const T &val ) { bs << val; return *this; } BinStream<TImpl> bs; };

    UdpHashedCmdsConnection();

    void             close_cmd          (); ///< end of current command -> will add a hash, and set a marker for the next command
    void             flush              () { close_cmd(); BaseUdpConn::flush(); beg_cmd = this->cur_output_ptr(); }

    BinStream<TImpl> bs_out             () { return static_cast<TImpl *>( this ); }
    BsAc             bs_out_auto_close  () { return { bs_out() }; }

    void             parse              ( char *data, PT size );

protected:
    void             on_bin_stream_error( const char *cmd ) { DISP_ERROR( "BinStream error while reading cmd '{}'", cmd ); }
    void             on_checksum_error  ( const char *cmd ) { DISP_ERROR( "Checksum error while reading cmd '{}'", cmd ); }

    template<class TF>
    bool             parse_cmd          ( BinStream<CmString> bq, const TF &f, const char *cmd );
    bool             check_checksum     ( BinStream<CmString> bq, const PI8 *beg_cmd, const char *cmd );


    const PI8       *beg_cmd;      ///< beginning of the currrent cmd in msg_queue.pending
    CheckSumType     check_func;   ///< check sum function
};

/**
Helper to call close_cmd on a given connection
*/
struct CloseCmd {
    template<class Bq>
    void write_to( Bq bq ) const {
        bq.buf->close_cmd();
    }
};
constexpr CloseCmd close_cmd;

template<class BaseUdpConn,class CheckSumType>
UdpHashedCmdsConnection<BaseUdpConn,CheckSumType>::UdpHashedCmdsConnection() {
    beg_cmd = this->cur_output_ptr();
}

template<class BaseUdpConn,class CheckSumType>
void UdpHashedCmdsConnection<BaseUdpConn,CheckSumType>::parse( char *data, PT size ) {
    CmString cm( data, data + size );
    static_cast<TImpl *>( this )->parse_bs( &cm );
    flush();
}

template<class BaseUdpConn,class CheckSumType>
void UdpHashedCmdsConnection<BaseUdpConn,CheckSumType>::close_cmd() {
    if ( beg_cmd != this->cur_output_ptr() ) {
        auto cs = check_func( beg_cmd, this->cur_output_ptr() - beg_cmd );
        this->write_some( &cs, sizeof cs );
        beg_cmd = this->cur_output_ptr();
    }
}

template<class BaseUdpConn,class CheckSumType>
template<class TF>
bool UdpHashedCmdsConnection<BaseUdpConn,CheckSumType>::parse_cmd( BinStream<CmString> bq, const TF &f, const char *cmd ) {
    DISP_DEBUG( "recv {} from {}", cmd, this->inet_address );

    bool res;
    const PI8 *beg_cmd = bq.buf->ptr() - 1; // HUM: only works for 1 byte commands
    ArgsAsTypeList<TF>::T::map( [&]( auto ot ) {
        using OT = typename Unref<typename decltype( ot )::T>::T;
        return bq.read().operator OT();
    }, [&]( auto&& ...args ) {
        if (( res = this->check_checksum( bq, beg_cmd, cmd ) ))
            ( static_cast<TImpl *>( this )->*f )( std::forward<decltype(args)>( args )... );
    } );
    return res;
}

template<class BaseUdpConn,class CheckSumType>
bool UdpHashedCmdsConnection<BaseUdpConn,CheckSumType>::check_checksum( BinStream<CmString> bq, const PI8 *beg_cmd, const char *cmd ) {
    if ( bq.error() ) {
        static_cast<TImpl *>( this )->on_bin_stream_error( cmd );
        return false;
    }
    auto vs = check_func( beg_cmd, bq.ptr() - beg_cmd );
    auto cs = vs;
    bq.read_some( &cs, sizeof cs );
    if ( bq.error() or vs != cs ) {
        bq.buf->ack_error();
        on_checksum_error( cmd );
        return false;
    }
    return true;
}

} // namespace RiotBase
