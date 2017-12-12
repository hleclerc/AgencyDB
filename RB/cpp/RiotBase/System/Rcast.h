#pragma once

namespace RiotBase {

template<class T>
struct RCast {
    RCast( T p ) : p( p ) {
    }
    template<class O> operator O() const {
        return reinterpret_cast<O>( p );
    }
    T p;
};

template<class T>
RCast<T> rcast( T p ) {
    return p;
}

template<class T>
struct RrCast {
    RrCast( T *p ) : p( p ) {
    }
    template<class O> operator O&() const {
        return reinterpret_cast<O&>( *p );
    }
    mutable T *p;
};

template<class T>
RrCast<T> rrcast( T &p ) {
    return &p;
}


}
