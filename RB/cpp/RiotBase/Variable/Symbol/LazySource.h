#pragma once

#include "ActiveLazySources.h"
#include "RpTemplSym.h"
#include "Lazyfier.h"

namespace RiotBase {

/// objects that must be created using mkl (to be registered in active_lazy_sources)
/// -> enable registering in active_lazy_sources
/// during the event loop, active_lazy_sources.exec() will parse the graphs to see if there is something to split
template<class T>
class LazySource : public RpTemplSym<T> {
public:
    /// Beware: symbol and lazy_sources must be wrapped by a Lazyfier object for modifications (@see mkl function)
    LazySource( const Category *category ) : RpTemplSym<T>( category ) {}
    ~LazySource() { active_lazy_sources->sources.remove_first_unordered( this ); }

    /// make a lazifier around a T object (which is assumed to inherit from LazySource<T>)
    template<class... Args>
    static Rp *mkl( Args&& ...args ) {
        T *r = T::New( std::forward<Args>( args )... );
        *active_lazy_sources << r;
        return Lazyfier::New( r );
    }
};

} // namespace RiotBase
