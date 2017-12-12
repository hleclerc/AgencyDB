#pragma once

#include "OtWrapperMap.h"

namespace RiotBase {

///
class OtWrapperMapSelect : public RpTempl<OtWrapperMapSelect> {
public:
    OtWrapperMapSelect( OtWrapperMap::Node *n, OtWrapperMap *o );

    static std::string     type_name();
    Rp                   **prox_ptr ();
    const Rp              *prox_ref () const;
    void                   on_change( Callback *c );

    OtWrapperMap::Node    *n;
    OtWrapperMap          *o;
};

} // namespace RiotBase
