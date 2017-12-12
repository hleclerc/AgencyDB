#pragma once

namespace RiotBase {

///
template<class T>
struct StaticStore {
    static T data;
};

template<class T>
T StaticStore<T>::data;

} // namespace RiotBase
