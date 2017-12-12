#pragma once

#include "TypeList.h"

namespace RiotBase {

///
template<class TypeList,class... Args>
class TypeListAppender;

///
template<class... TupleArgs,class... Args>
class TypeListAppender<TypeList<TupleArgs...>,Args...> {
public:
    using T = TypeList<TupleArgs...,Args...>;
};

} // namespace RiotBase
