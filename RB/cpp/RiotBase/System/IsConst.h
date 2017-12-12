#pragma once

namespace RiotBase {

template<class R> struct IsConst { enum { v = true }; };

template<class R> struct IsConst<      R &> { enum { v = false }; };
template<class R> struct IsConst<const R &> { enum { v = true  }; };

template<class R> struct IsConst<      R *> { enum { v = false }; };
template<class R> struct IsConst<const R *> { enum { v = true  }; };

} // namespace RiotBase



