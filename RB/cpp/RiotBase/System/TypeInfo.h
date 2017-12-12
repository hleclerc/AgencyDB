#pragma once

#include <string>

namespace RiotBase {

template<class... T>
struct TypeInfo {
    static std::string name();
    static unsigned    cat_id(); ///< id in the corresponding Category
};

} // namespace RiotBase
