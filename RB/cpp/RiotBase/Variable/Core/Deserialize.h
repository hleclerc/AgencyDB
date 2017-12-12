#pragma once

#include "RpAndUse.h"
#include "Methods.h"

namespace RiotBase {

Rp *deserialize( BinStream<CbStringPtr> bq, Vec<RpAndUse> &objects );

} // namespace RiotBase
