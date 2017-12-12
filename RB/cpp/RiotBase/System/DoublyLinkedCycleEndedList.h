#pragma once

#include "CycleEndedList.h"
#include "Assert.h"

namespace RiotBase {

struct DefaultGetNext {
    template<class T> T *&operator()( T *obj ) const { return obj->next; }
    template<class T> const T *operator()( const T *obj ) const { return obj->next; }
};

/** Doubly linked list with last element pointing to itself.
 *  The major interest if this is that it enable to know if an element is in the list or not
 *  ( by looking if item->prev != 0... which does not mean anything in std linked lists )
*/
template<class T,class GetPrev=DefaultGetPrev,class GetNext=DefaultGetNext>
class DoublyLinkedCycleEndedList {
public:
    struct Iterator {
        T   *operator* () { return cur; }
        void operator++() { cur = cur == gn( cur ) ? 0 : gn( cur ); }
        bool operator!=( Iterator iter ) const { return iter.cur != cur; }
        T   *cur;
    };

    struct ConstIterator {
        const T *operator* () const { return cur; }
        void     operator++() { cur = cur == gn( cur ) ? 0 : gn( cur ); }
        bool     operator!=( ConstIterator iter ) const { return iter.cur != cur; }
        const T   *cur;
    };

    struct RIterator {
        T   *operator* () { return cur; }
        void operator++() { cur = cur == gp( cur ) ? 0 : gp( cur ); }
        bool operator!=( RIterator iter ) const { return iter.cur != cur; }
        T   *cur;
    };

    struct RConstIterator {
        const T *operator* () const { return cur; }
        void     operator++() { cur = cur == gp( cur ) ? 0 : gp( cur ); }
        bool     operator!=( RConstIterator iter ) const { return iter.cur != cur; }
        const T   *cur;
    };

    struct Reversed {
        RIterator                   begin          () { return l->rbegin(); }
        RIterator                   end            () { return l->rend(); }

        RConstIterator              begin          () const { return ((const DoublyLinkedCycleEndedList *)l)->rbegin(); }
        RConstIterator              end            () const { return ((const DoublyLinkedCycleEndedList *)l)->rend(); }

        void                        write_to_stream( std::ostream &os ) const { for( const T *v : *this ) os << *v << ( gp( v ) == v ? "" : " " ); }

        DoublyLinkedCycleEndedList *l;
    };

    DoublyLinkedCycleEndedList() : first( 0 ), last( 0 ) {}

    DoublyLinkedCycleEndedList  &operator<<    ( T *node ) { return push_back( node ); }///< behaves like push_back
    DoublyLinkedCycleEndedList  &push_back     ( T *node ); ///< unless node is already in the list (in this case, it is not moved)

    template<class Op> void     apply          ( Op &&op ) const;
    template<class Op> void     apply          ( Op &&op );

    Iterator                    begin          () { return { first }; }
    Iterator                    end            () { return { 0 }; }

    ConstIterator               begin          () const { return { first }; }
    ConstIterator               end            () const { return { 0 }; }

    RIterator                   rbegin         () { return { last }; }
    RIterator                   rend           () { return { 0 }; }

    RConstIterator              rbegin         () const { return { last }; }
    RConstIterator              rend           () const { return { 0 }; }

    Reversed                    reversed       () { return { this }; }

    void                        clear          ();                            ///< Beware: it does not delete anything
    DoublyLinkedCycleEndedList &remove         ( T *val );                    ///< simply remove from the list (no deletion)
    bool                        empty          () const { return first == 0; }
    PT                          size           () const;

    template<class Op> void     clear          ( const Op &op );              ///< apply op on each element and clear. Can be used to delete element

    void                        write_to_stream( std::ostream &os ) const;


    T                          *first;
    T                          *last;
    static GetPrev              gp;
    static GetNext              gn;
};

template<class T,class GetPrev,class GetNext>
GetPrev DoublyLinkedCycleEndedList<T,GetPrev,GetNext>::gp;

template<class T,class GetPrev,class GetNext>
GetNext DoublyLinkedCycleEndedList<T,GetPrev,GetNext>::gn;

template<class T,class GetPrev,class GetNext>
DoublyLinkedCycleEndedList<T,GetPrev,GetNext> &DoublyLinkedCycleEndedList<T,GetPrev,GetNext>::push_back( T *node ) {
    if ( first ) {
        if ( not gp( node ) ) { // if not already in the list
            gn( last ) = node;
            gp( node ) = last;
            gn( node ) = node;
            last = node;
        }
    } else {
        gp( node ) = node;
        gn( node ) = node;
        first      = node;
        last       = node;
    }
    return *this;
}

template<class T,class GetPrev,class GetNext>
void DoublyLinkedCycleEndedList<T,GetPrev,GetNext>::clear() {
    if ( T *c = first ) {
        while ( true ) {
            T *n = gn( c );
            gp( c ) = 0;
            gn( c ) = 0;
            if ( c == n )
                break;
            c = n;
        }
        first = 0;
        last = 0;
    }
}

template<class T,class GetPrev,class GetNext> template<class Op>
void DoublyLinkedCycleEndedList<T,GetPrev,GetNext>::clear( const Op &op ) {
    if ( T *c = first ) {
        while ( true ) {
            T *n = gn( c );
            gp( c ) = 0;
            gn( c ) = 0;
            op( c );
            if ( c == n )
                break;
            c = n;
        }
        first = 0;
        last = 0;
    }
}

template<class T,class GetPrev,class GetNext>
DoublyLinkedCycleEndedList<T,GetPrev,GetNext> &DoublyLinkedCycleEndedList<T,GetPrev,GetNext>::remove( T *node ) {
    // already removed ?
    if ( gp( node ) == 0 )
        return *this;

    // at the beginning of the list
    if ( gp( node ) == node ) {
        // also at the end of the list
        if ( gn( node ) == node ) {
            first = 0;
            last = 0;
        } else {
            first = gn( node );
            gp( first ) = first;
        }
    } else if ( gn( node ) == node ) { // (only) at the end of the list
        last = gp( node );
        gn( last ) = last;
    } else { // in the middle of the list
        gn( gp( node ) ) = gn( node );
        gp( gn( node ) ) = gp( node );
    }

    // say that it is remove from the list
    gp( node ) = 0;
    gn( node ) = 0;
    return *this;
}

template<class T,class GetPrev,class GetNext>
PT DoublyLinkedCycleEndedList<T,GetPrev,GetNext>::size() const {
    PT res = 0;
    apply( [ & ]( const T * ) { ++res; } );
    return res;
}

template<class T,class GetPrev,class GetNext>
template<class Op>
void DoublyLinkedCycleEndedList<T,GetPrev,GetNext>::apply( Op &&op ) const {
    if ( const T *c = first ) {
        while ( true ) {
            op( c );
            if ( c == gn( c ) )
                break;
            c = gn( c );
        }
    }
}

template<class T,class GetPrev,class GetNext>
template<class Op>
void DoublyLinkedCycleEndedList<T,GetPrev,GetNext>::apply( Op &&op ) {
    if ( T *c = first ) {
        while ( true ) {
            op( c );
            if ( c == gn( c ) )
                break;
            c = gn( c );
        }
    }
}

template<class T,class GetPrev,class GetNext>
void DoublyLinkedCycleEndedList<T,GetPrev,GetNext>::write_to_stream( std::ostream &os ) const {
    apply( [&]( const T *node ) {
        if ( gp( node ) != node )
            os << " ";
        os << *node;
    } );
}

} // namespace RiotBase
