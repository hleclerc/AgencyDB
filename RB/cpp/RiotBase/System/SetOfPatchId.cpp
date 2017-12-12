#include "SetOfPatchId.h"

namespace RiotBase {

SetOfPatchId &SetOfPatchId::operator-=( const SetOfPatchId &s ) {
    Vec<TMap::iterator> to_rem;
    for( TMap::iterator p = data.begin(); p != data.end(); ++p ) {
        auto iter = s.data.find( p->first );
        if ( iter != s.data.end() ) {
            p->second -= iter->second;
            if ( p->second.empty() )
                to_rem << p;
        }
    }
    for( TMap::iterator p : to_rem )
        data.erase( p );
    return *this;
}

SetOfPatchId &SetOfPatchId::operator+=( const SetOfPatchId &s ) {
    for( auto &p : s.data )
        if ( not p.second.empty() )
            data[ p.first ] += p.second;
    return *this;
}

SetOfPatchId &SetOfPatchId::operator+=( PatchId patch_id ) {
    data[ patch_id.dev ].insert( patch_id.num );
    return *this;
}

SetOfPatchId &SetOfPatchId::operator-=( PatchId patch_id ) {
    IntervalSet<PI64> &inter = data[ patch_id.dev ];
    inter.remove( patch_id.num );
    if ( inter.empty() )
        data.erase( patch_id.dev );
    return *this;
}

SetOfPatchId SetOfPatchId::operator-( const SetOfPatchId &s ) const {
    SetOfPatchId res = *this;
    return res -= s;
}

bool SetOfPatchId::operator==( const SetOfPatchId &s ) const {
    if ( data.size() != s.data.size() )
        return false;
    for( auto iter_a = data.begin(), iter_b = s.data.begin(); iter_a != data.end(); ++iter_a, ++iter_b )
        if ( iter_a->first != iter_b->first or iter_a->second != iter_b->second )
            return false;
    return true;
}

void SetOfPatchId::write_to_stream( std::ostream &os ) const {
    int cpt = 0;
    for( const auto &p : data )
        os << ( cpt++ ? "," : "" ) << p.second << "@" << p.first;
}

PT SetOfPatchId::size() const {
    PT res = 0;
    for( const auto &p : data )
        res += p.second.size();
    return res;
}

bool SetOfPatchId::empty() const {
    for( const auto &p : data )
        if ( not p.second.empty() )
            return false;
    return true;
}

RiotBase::SetOfPatchId::operator bool() const {
    return not empty();
}

void SetOfPatchId::clear() {
    data.clear();
}

bool SetOfPatchId::contains( PatchId patch_id ) const {
    TMap::const_iterator iter = data.find( patch_id.dev );
    return iter != data.end() ? iter->second.contains( patch_id.num ) : false;
}

} // namespace RiotBase


