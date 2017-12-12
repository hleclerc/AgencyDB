#pragma once

#include "N.h"

namespace RiotBase {

///
template<class R>
struct Unref { using T = R; };

template<class R>
struct Unref<R &> { using T = R; };

template<class R>
struct Unref<const R &> { using T = R; };

}
