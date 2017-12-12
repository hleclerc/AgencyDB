#pragma once

#include "TypeConfig.h"
#include <limits>

namespace RiotBase {

template<class R,class T>
bool convert( R &res, const T &val ) { res = val; return true; }

inline bool convert( PI64 &res, const SI64 &val ) { res = val; return val >= 0;                                             }
inline bool convert( PI64 &res, const FP32 &val ) { res = val; return val >= 0 and val <= std::numeric_limits<PI64>::max(); }
inline bool convert( PI64 &res, const FP64 &val ) { res = val; return val >= 0 and val <= std::numeric_limits<PI64>::max(); }

inline bool convert( PI32 &res, const SI32 &val ) { res = val; return val >= 0;                                             }
inline bool convert( PI32 &res, const SI64 &val ) { res = val; return val >= 0 and val <= std::numeric_limits<PI32>::max(); }
inline bool convert( PI32 &res, const PI64 &val ) { res = val; return              val <= std::numeric_limits<PI32>::max(); }
inline bool convert( PI32 &res, const FP32 &val ) { res = val; return val >= 0 and val <= std::numeric_limits<PI32>::max(); }
inline bool convert( PI32 &res, const FP64 &val ) { res = val; return val >= 0 and val <= std::numeric_limits<PI32>::max(); }

} // namespace RiotBase

