#pragma once

#include "../Core/RpAndUse.h"
#include "LazySource.h"

namespace RiotBase {

///
class LazySourceKnownValueExt : public LazySource<LazySourceKnownValueExt> {
public:
    LazySourceKnownValueExt( Rp *rp, DevId loc );
    ~LazySourceKnownValueExt();

    static std::string type_name           ();
    void               get_lazy_source_info( LazySourceInfo &info, const LazySourceInfo **children_info ) const;
    void               serialize           ( BBQ bq, const PT *num_children ) const;
    static Rp         *deserialize         ( BinStream<CbStringPtr> bq, Vec<RpAndUse> &children );
    Rp                *exec_mp             ();

    mutable Rp        *rp;
    DevId              loc;
};

} // namespace RiotBase
