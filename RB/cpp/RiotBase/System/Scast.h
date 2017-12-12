#pragma once

namespace RiotBase {

template<class T>
struct SCast {
    SCast( T p ) : p( p ) {
    }
    template<class O> operator O() const {
        return static_cast<O>( p );
    }
    T p;
};

template<class T>
SCast<T> scast( T p ) {
    return p;
}

template<class T>
struct RSCast {
    RSCast( T *p ) : p( p ) {
    }
    template<class O> operator O&() const {
        return static_cast<O&>( *p );
    }
    mutable T *p;
};

template<class T>
RSCast<T> rscast( T &p ) {
    return &p;
}

}
