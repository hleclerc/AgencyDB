#pragma once

#include "LazySource.h"

namespace RiotBase {

///
class LazySourceKnownValue : public LazySource<LazySourceKnownValue> {
public:
    LazySourceKnownValue( Rp *rp );
    ~LazySourceKnownValue();

    static std::string type_name         ();
    void               get_lazy_source_info( LazySourceInfo &info, const LazySourceInfo **children_info ) const;
    Rp                *exec_mp           ();

    mutable Rp        *rp;
};

} // namespace RiotBase
