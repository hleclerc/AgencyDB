#pragma once

#include "TypeConfig.h"
#include "Print.h"
#include <algorithm>

namespace RiotBase {
  
/**
*/
template<PT n>
struct N {
    static constexpr PT val = n;

    template<PT m> N<n+m> operator+( N<m> ) { return {}; }
    template<PT m> N<n-m> operator-( N<m> ) { return {}; }

    PT operator+( PT m ) { return n + m; }
    PT operator-( PT m ) { return n - m; }

    operator PT() const { return n; }

    void write_to_stream( std::ostream &os ) const { os << n; }
};

} // namespace RiotBase

namespace std {

template<RiotBase::PT n,RiotBase::PT m>
RiotBase::N<( n > m ? n : m )> max( RiotBase::N<n>, RiotBase::N<m> ) { return {}; }

template<RiotBase::PT n>
RiotBase::N<n> max( RiotBase::N<n>, RiotBase::N<n> ) { return {}; }

template<RiotBase::PT n>
RiotBase::PT max( RiotBase::N<n>, RiotBase::PT m ) { return max( n, m ); }

template<RiotBase::PT m>
RiotBase::PT max( RiotBase::PT n, RiotBase::N<m> ) { return max( n, m ); }

}
