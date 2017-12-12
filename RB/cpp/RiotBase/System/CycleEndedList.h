#pragma once

#include "TypeConfig.h"
#include "Print.h"

namespace RiotBase {
struct DefaultGetPrev {
    template<class T> const T *operator()( const T *obj ) const { return obj->prev; }
    template<class T> T *&operator()( T *obj ) const { return obj->prev; }
};

/** Single linked list with last element pointing to itself.
 *  The major interest if this is that it enable to know if an element is in the list or not
 *  ( by looking if item->prev != 0... which does not mean anything in std linked lists )
*/
template<class T,class GetPrev=DefaultGetPrev>
class CycleEndedList {
public:
    struct Iterator {
        T   *operator* () { return cur; }
        void operator++() { cur = cur == gp( cur ) ? 0 : gp( cur ); }
        bool operator!=( Iterator iter ) const { return iter.cur != cur; }
        T   *cur;
    };

    struct ConstIterator {
        const T *operator* () const { return cur; }
        void     operator++() { cur = cur == gp( cur ) ? 0 : gp( cur ); }
        bool     operator!=( ConstIterator iter ) const { return iter.cur != cur; }
        const T   *cur;
    };

    CycleEndedList() : last( 0 ) {}

    /// behaves like a push front
    CycleEndedList &operator<<( T *node );

    Iterator        begin     () { return { last }; }
    Iterator        end       () { return { 0 }; }

    ConstIterator   begin     () const { return { last }; }
    ConstIterator   end       () const { return { 0 }; }

    void            clear     ();                            ///< Beware: it does not delete anything
    void            remove    ( T *val );                    ///< simply remove from the list (no deletion)
    bool            empty     () const { return last == 0; }
    PT              size      () const;

    template<class Op>
    void            clear     ( const Op &op );              ///< apply op on each element and clear

    T              *last;
    static GetPrev  gp;
};

template<class T,class GetPrev>
GetPrev CycleEndedList<T,GetPrev>::gp;

template<class T,class GetPrev>
CycleEndedList<T,GetPrev> &CycleEndedList<T,GetPrev>::operator<<( T *node ) {
    if ( not gp( node ) ) {
        gp( node ) = last ? last : node;
        last = node;
    }
    return *this;
}

template<class T,class GetPrev>
void CycleEndedList<T,GetPrev>::clear() {
    if ( T *c = last ) {
        while ( true ) {
            T *p = gp( c );
            gp( c ) = 0;
            if ( c == p )
                break;
            c = p;
        }
        last = 0;
    }
}

template<class T,class GetPrev> template<class Op>
void CycleEndedList<T,GetPrev>::clear( const Op &op ) {
    if ( T *c = last ) {
        while ( true ) {
            T *p = gp( c );
            gp( c ) = 0;
            op( c );
            if ( c == p )
                break;
            c = p;
        }
        last = 0;
    }
}

template<class T,class GetPrev>
void CycleEndedList<T,GetPrev>::remove( T *val ) {
    for( T **c = &last; ; ) {
        if ( *c == val ) {
            if ( gp( val ) == val )
                *c = 0;
            else
                *c = gp( val );
            break;
        }
        if ( *c == gp( *c ) )
            break;
        c = &gp( *c );
    }
}

template<class T,class GetPrev>
PT CycleEndedList<T,GetPrev>::size() const {
    PT res = 0;
    if ( T *c = last ) {
        while ( true ) {
            ++res;
            if ( c == gp( c ) )
                break;
            c = gp( c );
        }
    }
    return res;
}


} // namespace RiotBase
