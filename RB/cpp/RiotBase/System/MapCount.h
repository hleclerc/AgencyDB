#pragma once

#include <map>

namespace RiotBase {

template<class T>
struct MapCount {
    MapCount &operator<<( const T &val ) {
        typename std::map<T,unsigned>::iterator iter = m.insert( std::make_pair( val, 0u ) ).first;
        ++iter->second;
        return *this;
    }

    T max_count() const {
        typename std::map<T,unsigned>::const_iterator best_iter = m.begin(), iter = best_iter;
        for( ++iter; iter != m.end(); ++iter )
            if ( best_iter->second < iter->second )
                best_iter = iter;
        return best_iter->first;
    }

    std::map<T,unsigned> m;
};

} // namespace RiotBase
