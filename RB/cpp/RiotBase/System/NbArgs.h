#pragma once

#include "ArgsAsTypeList.h"

namespace RiotBase {

/// nb arg in functions of type TF
template<typename TF>
struct NbArgs {
    enum { value = ArgsAsTypeList<TF>::T::size };
};

} // namespace RiotBase




