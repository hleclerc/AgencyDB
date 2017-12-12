#pragma once

#include "AutoRpDel.h"
#include "RpTempl.h"

namespace RiotBase {

///
template<class T>
class RpTemplProxy : public RpTempl<T> {
public:
    RpTemplProxy( Rp *src = 0 ) : src( src ) {}

    const Rp           *prox_ref       () const { return src.rp; }
    void                write_structure( std::ostream &os ) const { RO_CALL( write_structure, src.rp, os << T::type_name() << '(' ); os << ')'; }

    AutoRpDel           src;
};


} // namespace RiotBase
