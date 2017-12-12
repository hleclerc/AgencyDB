#pragma once

#include "TypeList.h"

namespace RiotBase {

/// extraction of a function argument type. By default, the return type.
template<typename Func>
struct ArgsAsTypeList;

template<typename TR,typename... Args>
struct ArgsAsTypeList<TR( Args... )> {
    using T = TypeList<Args...>;
};

// void (test_DtlsCmdSocket()::MyConnection::*)(long unsigned int, long unsigned int)
template<typename TR,class CL,typename... Args>
struct ArgsAsTypeList<TR(CL::*)( Args... )> {
    using T = TypeList<Args...>;
};


} // namespace RiotBase




