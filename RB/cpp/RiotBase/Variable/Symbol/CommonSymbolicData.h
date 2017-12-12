#pragma once

#include "../../System/Vec.h"
#include "../Core/Variable.h"

namespace RiotBase {
class Lazyfier;

///
class CommonSymbolicData {
public:
    CommonSymbolicData() : op_id( 0 ) {}

    Vec<const Rp *> parents;
    Vec<Lazyfier *> lazyfier_parents;

    void           *op_mp;
    mutable PI64    op_id;
    static PI64     cur_op_id;
};

} // namespace RiotBase
