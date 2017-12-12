#pragma once

#include "UsrId.h"

namespace RiotBase {

///
class Gateway {
public:
    virtual ~Gateway();

    DevId dev;
    UsrId usr;
};

} // namespace RiotBase
