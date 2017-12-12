#pragma once

#include "RpTempl.h"

namespace RiotBase {

///
template<class T>
class RpTemplRefCounted : public RpTempl<T> {
public:
    RpTemplRefCounted() : cpt_use( 0 ) {}

    void        del() { if ( cpt_use ) --cpt_use; else RpTempl<T>::_del( this ); }

    mutable int cpt_use;
};

} // namespace RiotBase
