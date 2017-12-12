#pragma once

#include "Print.h"
#include "Vec.h"

namespace RiotBase {

///
class ConfigReader {
public:
    void parse( std::istream &is );
    virtual void onargs( Vec<std::string> &args ) = 0;

    static std::string write( const Vec<std::string> &args );

};

} // namespace RiotBase
