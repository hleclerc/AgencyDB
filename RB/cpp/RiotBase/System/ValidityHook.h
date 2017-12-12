#pragma once

#include "ApplyVa.h"
#include "Vec.h"

#include <tuple>

namespace RiotBase {

///
struct ValidIfNoDestructionOf_Ancestor {
    ValidIfNoDestructionOf_Ancestor() : valid( true ) {}

    operator     bool      () const { return valid; }
    void         invalidate() { valid = false; }
    virtual void clear_objs() = 0;

    bool         valid;
};

/**
  If ValidityHook is destroyed first, instances of dependent ValidIfNoDestructionOf become invalid. We clear the pointers because ~ValidIfNoDestructionOf() won't be able to use obj->_validity_hook.items anymore

  If ValidIfNoDestructionOf is destroyed first, we say that it not longer has to be watched (e.g. nothing to do when ~ValidityHook())
*/
struct ValidityHook {
    ~ValidityHook() {
        for( ValidIfNoDestructionOf_Ancestor *i : items )
            i->clear_objs();
    }

    Vec<ValidIfNoDestructionOf_Ancestor *> items;
};

///
template<PT n>
struct ValidIfNoDestructionOf : ValidIfNoDestructionOf_Ancestor {
    ValidIfNoDestructionOf( const ValidIfNoDestructionOf &v ) {
        for( PT i = 0; i < n; ++i ) {
            hooks[ i ] = v.hooks[ i ];
            hooks[ i ]->items << this;
        }
    }

    ValidIfNoDestructionOf( ValidIfNoDestructionOf &&v ) {
        for( PT i = 0; i < n; ++i ) {
            hooks[ i ] = v.hooks[ i ];
            if ( hooks[ i ] ) {
                hooks[ i ]->items.replace( &v, this );
                v.hooks[ i ] = 0;
            }
        }
    }

    template<class... Args>
    ValidIfNoDestructionOf( Args ...args ) {
        apply_van( [ this ]( auto *obj, auto i ) {
            hooks[ i.val ] = &obj->_validity_hook;
            obj->_validity_hook.items << this;
        }, args... );
    }

    ~ValidIfNoDestructionOf() {
        for( ValidityHook *hook : hooks )
            if ( hook )
                hook->items.remove_first_unordered( this );
    }

    virtual void clear_objs() {
        valid = false;
        for( ValidityHook *&hook : hooks )
            hook = 0;
    }

    ValidityHook *hooks[ n ];
};

///
template<class... Args>
ValidIfNoDestructionOf<sizeof...(Args)> valid_if_no_destruction_of( Args ...args ) {
    return { args... };
}

}
