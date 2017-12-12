#pragma once

#include "Vec.h"

#include <functional>

namespace RiotBase {

/// helper for patch arrays-like data (see for instance GenString, GenArray, ...)
template<class T,class Id>
class SplittedVecRef {
public:
    struct Item {
        void write_to_stream( std::ostream &os ) const {
            os << "p=" << pos << ",l=" << len;
        }
        T *vec; ///< data
        PT pos;
        PT len;
        Id id;
    };

    void insert( T *vec, PT pos, PT len, Id id = Id() ) {
        if ( not len )
            return;
        PT index = find_item_containing( pos );
        if ( index == items.size() )
            items << Item{ vec, pos, len, id };
        else {
            PT ol = items[ index ].len, dp = pos - items[ index ].pos;
            T *od = items[ index ].vec;
            if ( dp ) {
                items[ index++ ].len = dp;
                items.insert( index++, Item{ vec, pos, len, id } );
            } else {
                items[ index ].vec = vec;
                items[ index ].pos = pos;
                items[ index ].len = len;
                items[ index ].id  = id;
                ++index;
            }
            if ( PT nl = ol - dp )
                items.insert( index++, Item{ od + dp, pos + len, nl } );
            for( ; index < items.size(); ++index )
                items[ index ].pos += len;
        }
    }

    void remove( PT pos, PT len ) {
        if ( not len )
            return;
        PT index = find_item_containing( pos );
        if ( index == items.size() )
            return;

        PT ol = len;
        if ( pos > items[ index ].pos ) {
            if ( pos + len <= items[ index ].pos + items[ index ].len ) {
                PT rem = items[ index ].pos + items[ index ].len - ( pos + len );
                T *vec = items[ index ].vec;
                Id id  = items[ index ].id;
                PT nl = pos - items[ index ].pos;
                items[ index ].len = nl;
                if ( rem )
                    items.insert( ++index, Item{ vec + nl + len, pos, rem, id } );
                while( ++index < items.size() )
                    items[ index ].pos -= ol;
                return;
            }
            len -= items[ index ].pos + items[ index ].len - pos;
            items[ index ].len = pos - items[ index ].pos;
            ++index;
        }
        while( len >= items[ index ].len ) {
            len -= items[ index ].len;
            items.remove( index );
            if ( index == items.size() )
                return;
        }
        if ( len ) {
            items[ index ].vec += len;
            items[ index ].len -= len;
            ++index;
        }
        for( ; index < items.size(); ++index )
            items[ index ].pos -= ol;
    }

    void replace( PT pos, T *val, Id id = Id() ) {
        PT ii = find_item_containing( pos );
        if ( ii < items.size() ) {
            PT d = pos - items[ ii ].pos;
            if ( d == 0 ) {
                if ( items[ ii ].len == 1 ) {
                    items[ ii ].vec = val;
                    items[ ii ].id  = id;
                } else {
                    ++items[ ii ].vec;
                    ++items[ ii ].pos;
                    --items[ ii ].len;
                    items.insert( ii, Item{ val, pos, 1, id } );
                }
            } else if ( d == items[ ii ].len - 1 ) {
                items[ ii ].len = d;
                items.insert( ++ii, Item{ val, pos, 1, id } );
            } else {
                items.insert( ii + 1, Item{ items[ ii ].vec + d + 1, pos + 1, items[ ii ].len - ( d + 1 ), items[ ii ].id } );
                items.insert( ii + 1, Item{ val, pos, 1, id } );
                items[ ii ].len = d;
            }
        }
    }

    PT find_item_containing( PT pos ) const {
        PT beg = 0, end = items.size();
        while ( beg < end ) {
            PT mid = ( beg + end ) / 2;
            if ( items[ mid ].pos > pos )
                end = mid;
            else if ( items[ mid ].pos + items[ mid ].len <= pos )
                beg = mid + 1;
            else
                return mid;
        }
        return items.size();
    }

    const T *at( PT index, Id *id = 0 ) const {
        PT i = find_item_containing( index );
        if ( i >= items.size() )
            return 0;
        if ( id )
            *id = items[ i ].id;
        return items[ i ].vec + index - items[ i ].pos;
    }

    T *at( PT index, Id *id = 0 ) {
        PT i = find_item_containing( index );
        if ( i >= items.size() )
            return 0;
        if ( id )
            *id = items[ i ].id;
        return items[ i ].vec + index - items[ i ].pos;
    }

    const T *back( Id *id = 0 ) const {
        if ( id )
            *id = items.back().id;
        return items.back().vec + items.back().len - 1;
    }

    void apply( std::function<void(const T &)> f ) const {
        for( const Item &item : items )
            for( PT i = 0; i < item.len; ++i )
                f( item.vec[ i ] );
    }

    void write_to_stream( std::ostream &os ) const {
        int cpt = 0;
        apply( [&]( const T &d ) { os << ( cpt++ ? " " : "" ) << d; } );
    }

    PT size() const {
        PT res = 0;
        for( const Item &item : items )
            res += item.len;
        return res;
    }

    bool empty() const {
        for( const Item &item : items )
            if ( item.len )
                return false;
        return true;
    }

    Vec<Item> items;
};

} // namespace RiotBase
