#pragma once

#include "../../System/DevId.h"
#include "../../System/Vec.h"

namespace RiotBase {
class Gateway;

/// prob presence on a given machine
/// size estimation
/// estimation of the delay to obtain the data
class LazySourceInfo {
public:
    struct LocInfo {
        DevId  loc;
        double prob;
    };

    LazySourceInfo();

    void         write_to_stream( std::ostream &os ) const;

    Vec<LocInfo> prob_is_on;       ///< probability to have object on a given instance
    double       output_size;      ///< estimation, in bytes
    double       time_to_complete; ///<
};

} // namespace RiotBase
