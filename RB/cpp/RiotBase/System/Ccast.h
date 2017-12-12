#pragma once

namespace RiotBase {

template<class T> T *ccast( const T *p ) { return const_cast<T *>( p ); }
template<class T> T *ccast( T *p ) { return const_cast<T *>( p ); }

template<class T> T &ccast( const T &p ) { return const_cast<T &>( p ); }
template<class T> T &ccast( T &p ) { return const_cast<T &>( p ); }


}
