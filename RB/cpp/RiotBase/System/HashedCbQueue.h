#pragma once

#include "CbString.h"
#include "XxHash.h"

namespace RiotBase {

///
template<class Cq=CbQueue,class Hash=XxHash,bool raii=false>
struct HashedCbQueue {
    HashedCbQueue( Cq *buf ) : buf( buf ) {}
    ~HashedCbQueue() { if ( raii ) { PI32 h = hash.digest(); write_some( &h, sizeof( h ) ); } }

    bool  error                 () const { return buf->error(); }
    bool  ack_error             () { return buf->ack_error(); }

    // size
    bool  empty                 () const { return buf->empty(); }
    PT    size                  () const { return buf->size(); }

    void  clear                 () { hash.reset(); buf->clear(); }
    void  free                  () { hash.reset(); buf->free (); }

    void  write_some            ( const void *data, PT size ) { hash.update( data, size ); buf->write_some( data, size ); }
    void  write_some            ( const CbString &s ) { s.data_visitor( [ & ]( const PI8 *beg, const PI8 *end ){ hash.update( beg, end - beg ); } ); buf->write_some( s ); }
    void  write_some            ( const CbQueue  &s ) { s.data_visitor( [ & ]( const PI8 *beg, const PI8 *end ){ hash.update( beg, end - beg ); } ); buf->write_some( s ); }
    void  write_some            ( const std::string &s ) { hash.update( s.data(), s.size() ); buf->write_some( s ); }
    void  write_some            ( CbQueue &&s ) { s.data_visitor( [ & ]( const PI8 *beg, const PI8 *end ){ hash.update( beg, end - beg ); } ); buf->write_some( std::move( s ) ); }

    void  write_byte            ( PI8 val ) { hash << val; buf->write_byte( val ); }

    void  write_byte_wo_beg_test( PI8 val ) { hash << val; buf->write_byte( val ); }

    void *write_cont            ( const void *data, PT size ) { hash.update( data, size ); return buf->write_cont( data, size ); }

    template<class T>
    T    *write_cont            ( const T &data ) { return write_cont( &data, sizeof( T ) ); }

    Cq   *buf;
    Hash  hash;
};

} // namespace RiotBase
