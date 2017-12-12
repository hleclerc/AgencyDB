#pragma once

#include "SetOfPatchId.h"
#include "Inode.h"
#include <map>

namespace RiotBase {

/**
  value stored per dev_id
*/
class VectorClock {
public:
    using TMap = std::map<DevId,PI64>;

    operator               SetOfPatchId   () const;

    VectorClock           &operator+=     ( PatchId patch_id );

    void                   write_to_stream( std::ostream &os ) const;

    template<class Bs>
    void                   write_to       ( Bs &bq, DevId cur_dev = {} ) const;

    template<class Bs>
    static VectorClock     read_from      ( Bs &bq, DevId cur_dev = {} );

    bool                   contains       ( PatchId patch_id ) const;
    PT                     size           () const;
    bool                   empty          () const;

    void                   clear          ();

    TMap                   data; ///< dev_id_of_creator -> max bound (included)
    Vec<Inode>             merges;
};

template<class Bs>
void VectorClock::write_to( Bs &bq, DevId cur_dev ) const {
     bq << data.size();
     for( const auto &p : data )
         bq << p.first.glo_dev( cur_dev ) << p.second;
     bq << merges.size();
     for( Inode p : merges )
         p.write_to( bq, cur_dev );
}

template<class Bs>
VectorClock VectorClock::read_from( Bs &bq, DevId cur_dev ) {
    VectorClock res;
    for( RiotBase::PT size = bq.read(); size--; ) {
        DevId dev = RiotBase::DevId::read_from( bq );
        PI64 num = bq.read();
        if ( bq.error() )
            break;
        res.data[ dev.loc_dev( cur_dev ) ] = num;
    }
    for( RiotBase::PT size = bq.read(); size--; ) {
        Inode inode = RiotBase::Inode::read_from( bq, cur_dev );
        if ( bq.error() )
            break;
        res.merges << inode;
    }
    return res;
}

} // namespace RiotBase
