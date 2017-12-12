#pragma once

#include "../Core/RpTempl.h"
#include "GenArray.h"

namespace RiotBase {

///
class GenArraySelect : public RpTempl<GenArraySelect> {
public:
    GenArraySelect( GenArray *ori, PT pos );
    ~GenArraySelect();

    static std::string type_name();
    Rp               **prox_ptr ();
    const Rp          *prox_ref () const;
    void               on_change( Callback *c );

    GenArray          *ori;
    PT                 pos;
};

} // namespace RiotBase
