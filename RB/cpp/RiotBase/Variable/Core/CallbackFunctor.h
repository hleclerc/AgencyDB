#pragma once

#include "../../System/IsSame.h"
#include "../../System/N.h"
#include "Callback.h"

namespace RiotBase {

///
template<class Functor,class T=void>
class CallbackFunctor : public Callback {
public:
    CallbackFunctor( const Functor &functor = Functor() ) : functor( functor ) {}

    virtual void        exec( Rp **rp = 0 ) { _exec( N<IsSame<T,void>::v>(), rp ); }
    virtual std::string name() const { return "Func"; }

protected:
    void                _exec( N<0>, Rp **rp ) { functor( reinterpret_cast<T &>( *rp ) ); }
    void                _exec( N<1>, Rp **rp ) { functor(); }

    Functor             functor;
};

} // namespace RiotBase
