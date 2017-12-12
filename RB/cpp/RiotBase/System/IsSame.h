#pragma once

namespace RiotBase {

///
template<class T0,class T1>
struct IsSame {
    enum { v = false };
};

///
template<class T0>
struct IsSame<T0,T0> {
    enum { v = true };
};


} // namespace RiotBase
