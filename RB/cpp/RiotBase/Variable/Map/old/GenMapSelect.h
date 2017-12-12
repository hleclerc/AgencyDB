#pragma once

#include "../Core/RpTempl.h"
#include "GenMap.h"

namespace RiotBase {

///
class GenMapSelect : public RpTempl<GenMapSelect> {
public:
    GenMapSelect( GenMap::Node *n, GenMap *o );

    static std::string type_name();
    Rp               **prox_ptr ();
    const Rp          *prox_ref () const;
    void               on_change( Callback *c );

    GenMap::Node      *n;
    GenMap            *o;
};

} // namespace RiotBase
