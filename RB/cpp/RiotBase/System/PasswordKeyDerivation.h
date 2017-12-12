#pragma once

#include <string>

namespace RiotBase {

/**
*/
class PasswordKeyDerivation {
public:
    static std::string salt();
    static std::string key( std::string pass, std::string salt );
};

} // namespace RiotBase
