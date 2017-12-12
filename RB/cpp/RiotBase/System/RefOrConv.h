#pragma once

namespace RiotBase {

template<class T>
T &&ref_of_conv( T &&val ) { return std::forward<T>( val ); }

template<class T,class O>
T ref_of_conv( const O &val ) { return val; }


} // namespace RiotBase
