#pragma once

namespace RiotBase {

/// extraction of a function argument type. By default, the return type.
template<typename Func,int num_arg=-1>
struct ArgType;

template<typename TR,typename... Args>
struct ArgType<TR( Args... ),-1> {
    typedef TR T;
};

template<typename TR,typename TA,typename... Args>
struct ArgType<TR( TA, Args... ),0> {
    typedef TA T;
};

template<typename TR,typename TA,typename TB,typename... Args>
struct ArgType<TR( TA, TB, Args... ),1> {
    typedef TB T;
};

template<typename TR,typename TA,typename TB,typename TC,typename... Args>
struct ArgType<TR( TA, TB, TC, Args... ),2> {
    typedef TC T;
};

template<typename TR,typename TA,typename TB,typename TC,typename TD,typename... Args>
struct ArgType<TR( TA, TB, TC, TD, Args... ),3> {
    typedef TD T;
};

template<typename TR,typename TA,typename TB,typename TC,typename TD,typename TE,typename... Args>
struct ArgType<TR( TA, TB, TC, TD, TE, Args... ),4> {
    typedef TE T;
};

} // namespace RiotBase




