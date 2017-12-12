#pragma once

#include "EnableIf.h"

#include <iostream>
#include <iomanip>
#include <sstream>

namespace RiotBase {

/// classes with `write_to_stream` will be displayed by default with the corresponding methods
template<class T>
typename EnableIf<1,std::ostream,decltype(&T::write_to_stream)>::T &operator<<( std::ostream &os, const T &val ) {
    val.write_to_stream( os );
    return os;
}

template<class OS,class T0> void __my_print( OS &os, const T0 &t0 ) { os << t0 << std::endl; }
template<class OS,class T0,class... Args> void __my_print( OS &os, const T0 &t0, const Args &...args ) { os << t0 << ", "; __my_print( os, args... ); }

#ifndef PRINT
    #define PRINT( ... ) \
        __my_print( std::cout << #__VA_ARGS__ " -> ", __VA_ARGS__ );
    #define PRINTE( ... ) \
        __my_print( std::cerr << #__VA_ARGS__ " -> ", __VA_ARGS__ );
    #define PRINTN( ... ) \
        __my_print( std::cout << #__VA_ARGS__ " ->\n", __VA_ARGS__ );
    #define PRINTL( ... ) \
        __my_print( std::cout << __FILE__ << ":" << __LINE__ << ": " << #__VA_ARGS__ " -> ", __VA_ARGS__ );
    #define PRINTF( ... ) \
        __my_print( std::cout << __PRETTY_FUNCTION__ << ": " << #__VA_ARGS__ " -> ", __VA_ARGS__ );
#endif

#define PRINTRP( A ) \
    RO_CALL( write_to_stream, (A), std::cout << "  RP( " << #A << " ) -> " ); std::cout << std::endl

#define PRINTRPL( A ) \
    RO_CALL( write_to_stream, (A), std::cout << __FILE__ << ":" << __LINE__ << ": RP( " << #A << " ) -> " ); std::cout << std::endl

#define PRINTSTR( A ) \
    RO_CALL( write_structure, (A), std::cout << "  STR( " << #A << " ) -> " ); std::cout << std::endl

#define PRINTESTR( A ) \
    RO_CALL( write_structure, (A), std::cerr << "  STR( " << #A << " ) -> " ); std::cerr << std::endl

#define PRINTPD( A, DEVID ) \
    RO_CALL( write_patch_data, (A), std::cout << "  PD( " << #A << " ) -> ", 4, nullptr ); std::cout << std::endl

template<class T>
std::string to_string( const T &val ) {
    std::ostringstream ss;
    ss << val;
    return ss.str();
}

template<class T>
std::string to_string_hex( const T &val ) {
    std::ostringstream ss;
    ss << std::hex << val;
    return ss.str();
}

} // namespace RiotBase

