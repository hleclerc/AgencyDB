#pragma once

#include "TypeConfig.h"

namespace RiotBase {
class CbStringPtr;

/**
*/
class Crc32c {
public:
    PI32 operator()( const void *data, PT size, unsigned init = 0 ) const;
    PI32 operator()( const CbStringPtr &cs, unsigned init = 0 ) const;
};

} // namespace RiotBase
