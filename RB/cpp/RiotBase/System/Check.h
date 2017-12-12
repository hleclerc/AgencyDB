#pragma once

#include <iostream>
#include <sstream>
#include "Print.h"

namespace RiotBase {

template<class T,class E>
void test( std::string str, const T &out, const E &exp, const char *file, int line ) {
    if ( str.size() > 100 )
        str = "..." + str.substr( str.size() - 100 );
    std::ostringstream ss, se;
    ss << out;
    se << exp;
    std::cout << file << ":" << line << ( ss.str() == se.str() ? ": (OK) " : ": (BAD) " ) << str << " -> " << ss.str();
    std::cout << std::endl;

}

#define CHECK( COND, EXPECTED ) \
    test( #COND, COND, EXPECTED, __FILE__, __LINE__ )

#define CHECK_TXT( TXT, COND, EXPECTED ) \
    test( TXT, COND, EXPECTED, __FILE__, __LINE__ )

}
