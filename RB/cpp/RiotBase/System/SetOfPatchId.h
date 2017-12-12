#pragma once

#include "IntervalSet.h"
#include "PatchId.h"
#include "DevId.h"
#include <map>

namespace RiotBase {

///
class SetOfPatchId {
public:
    using TMap = std::map<DevId,IntervalSet<PI64>>;

    template<class Cq>
    SetOfPatchId( BinStream<Cq> bq, DevId cur_dev );

    SetOfPatchId() {}

    SetOfPatchId       &operator-=     ( const SetOfPatchId &s );
    SetOfPatchId       &operator+=     ( const SetOfPatchId &s );
    SetOfPatchId       &operator+=     ( PatchId patch_id );
    SetOfPatchId       &operator-=     ( PatchId patch_id );
                      
    SetOfPatchId        operator-      ( const SetOfPatchId &s ) const;
                      
    bool                operator==     ( const SetOfPatchId &s ) const;
                      
    void                write_to_stream( std::ostream &os ) const;

    template<class Cq>
    void                write_to       ( BinStream<Cq> bq, DevId cur_dev ) const;

    template<class Cq>
    static SetOfPatchId read_from      ( BinStream<Cq> bq, DevId cur_dev );

    bool                contains       ( PatchId patch_id ) const;
    PT                  size           () const;
    bool                empty          () const;

    operator            bool           () const;
                        
    void                clear          ();

    template<class Op>
    void                apply          ( const Op &op ) const;
                        
    TMap                data;                                                       ///< dev_id_of_creator -> num_range ()
};

template<class Cq>
SetOfPatchId::SetOfPatchId( BinStream<Cq> bq, DevId cur_dev ) {
    for( PT size = bq.read(); size and not bq.error(); --size ) {
        DevId dev = bq.read();
        data[ dev.loc_dev( cur_dev ) ] = bq.read();
    }
}

template<class Cq>
void SetOfPatchId::write_to( BinStream<Cq> bq, DevId cur_dev ) const {
    bq.write_unsigned( data.size() );
    for( const auto &p : data )
        bq << p.first.glo_dev( cur_dev ) << p.second;
}

template<class Cq>
SetOfPatchId SetOfPatchId::read_from( BinStream<Cq> bq, DevId cur_dev ) {
    SetOfPatchId res;
    for( RiotBase::PT size = bq.read(); not bq.error() and size; --size ) {
        RiotBase::DevId dev = bq.read();
        if ( not bq.error() )
            res.data[ dev.loc_dev( cur_dev ) ] = bq.read();
    }
    return res;
}

template<class Op>
void SetOfPatchId::apply( const Op &op ) const {
    for( std::pair<const DevId &,const IntervalSet<PI64> &> p : data )
        for( PI64 v : p.second )
            op( PatchId{ p.first, v } );

}

} // namespace RiotBase

