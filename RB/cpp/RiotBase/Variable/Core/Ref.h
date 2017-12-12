#pragma once

#include "Variable.h"

namespace RiotBase {

///
template<class T>
class Ref : public T {
public:
    Ref( Ref<T> &&ref ) : T( ref.rp ) {}
    Ref( const Ref<T> &ref ) = delete;
    Ref( const T &ref ) = delete;
    Ref( Rp *rp ) : T( rp ) {}

    operator T() const { return RO_CALL( copy, this->rp ); }

    using T::operator=;
};


} // namespace RiotBase
