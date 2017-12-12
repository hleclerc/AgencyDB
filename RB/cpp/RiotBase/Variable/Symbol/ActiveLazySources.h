#pragma once

#include "../../System/LazyNew.h"
#include "../Core/Methods.h"

namespace RiotBase {

struct ActiveLazysources {
    ActiveLazysources();

    void operator<<( Rp *rp );
    bool exec(); ///< return true if something has been done

    Vec<Rp *>  sources;
};

extern LazyNew<ActiveLazysources> active_lazy_sources;

} // namespace RiotBase
