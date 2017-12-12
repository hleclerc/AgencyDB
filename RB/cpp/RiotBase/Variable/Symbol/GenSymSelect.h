#pragma once

#include "../Core/RpTempl.h"

namespace RiotBase {
class Lazyfier;

///
class GenSymSelect : public RpTempl<GenSymSelect> {
public:
    enum { unknown_value = true, };

    GenSymSelect( Lazyfier *container, Rp *key );
    GenSymSelect( Lazyfier *container );
    ~GenSymSelect();

    static std::string     type_name();
    Rp                    *prox_val () const;

    Lazyfier              *container;
    Vec<Rp *>              keys;
};

} // namespace RiotBase
