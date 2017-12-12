#pragma once

#include "BinStream.h"
#include "Inode.h"

namespace RiotBase {

/**
  To read or write to BufferQueue, CmQueue, CbString, CmString, ...

  with the RiotBase default binary format (compressed unsigned, ...)
*/
template<class TB=CbQueue>
struct BinStreamWithDevId : BinStream {
    BinStreamWithDevId( TB *buf, DevId dev ) : BinStream( buf ), dev( dev ) {}

    // write
    BinStream &operator<<( Inode s ) { s.write_to( *this, dev ); }

    // read
    BinStream &operator>>( Inode &s ) { s.read_from( *this, dev ); }
    void       read      ( Inode &s ) { s.read_from( *this, dev ); }

    TB   *buf;
    DevId dev;
};

}
