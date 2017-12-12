#include "VectorClock.h"

namespace RiotBase {

VectorClock::operator SetOfPatchId() const {
    SetOfPatchId res;
    for( const auto &p : data )
        res.data[ p.first ].insert( 1, p.second );
    return res;
}

VectorClock &VectorClock::operator+=( PatchId patch_id ) {
    TMap::iterator iter = data.find( patch_id.dev );
    if ( iter == data.end() )
        data.insert( iter, std::make_pair( patch_id.dev, patch_id.num ) );
    else
        iter->second = std::max( iter->second, patch_id.num );
    return *this;
}

void VectorClock::write_to_stream( std::ostream &os ) const {
    int cpt = 0;
    os << "{";
    for( const auto &p : data )
        os << ( cpt++ ? "," : "" ) << p.second << "@" << p.first;
    os << "}";
}

bool VectorClock::contains( PatchId patch_id ) const {
    TMap::const_iterator iter = data.find( patch_id.dev );
    return iter != data.end() ? patch_id.num < iter->second : false;
}

PT VectorClock::size() const {
    PT res = 0;
    for( const auto &p : data )
        res += p.second;
    return res;
}

bool VectorClock::empty() const {
    return data.empty();
}

void VectorClock::clear() {
    data.clear();
}



} // namespace RiotBase
