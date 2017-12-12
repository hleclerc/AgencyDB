#pragma once

#include "BinStream.h"
#include "Print.h"
#include <set>

namespace RiotBase {

/**
  For integer like types
  Bounds are INcluded (min and max)
*/
template<class T>
class IntervalSet {
public:
    struct Interval {
        Interval( T min, T max ) : min( min ), max( max ) {}
        Interval( T val ) : min( val ), max( val ) {}

        bool operator==( const Interval &a ) const { return min == a.min and max == a.max; }
        bool operator!=( const Interval &a ) const { return min != a.min or  max != a.max; }

        T min; // included beginning
        T max; // included ending
    };
    struct InterComp {
        bool operator()( const Interval &a, const Interval &b ) const { return a.min < b.min; }
    };
    using Set    = std::set<Interval,InterComp>;
    using RCIter = typename Set::const_reverse_iterator;
    using RIter  = typename Set::reverse_iterator;
    using CIter  = typename Set::const_iterator;
    using Iter   = typename Set::iterator;

    struct const_iterator {
        const_iterator( const IntervalSet *is ) : iter( is->data.begin() ), is( is ) { val = iter != is->data.end() ? iter->min : 0; }
        const_iterator( const IntervalSet *is, CIter iter, T val ) : iter( iter ), val( val ), is( is ) {}

        T                  operator* () const { return val; }
        const_iterator    &operator++() { if ( ++val > iter->max ) val = ++iter != is->data.end() ? iter->min : 0; return *this; }
        bool               operator!=( const_iterator i ) const { return iter != i.iter or val != i.val; }

        CIter              iter;
        T                  val;
        const IntervalSet *is;
    };

    IntervalSet( T min, T max ) : IntervalSet() { insert( min, max ); }
    IntervalSet( T val ) : IntervalSet() { insert( val ); }
    IntervalSet() {}

    bool operator==( T val ) const {
        CIter i = data.begin(), e = data.end();
        return i != e and i->min == val and i->max == val and ++i == e;
    }

    bool operator==( const IntervalSet &val ) const {
        for( CIter a = data.begin(), b = val.data.begin(); ; ++a, ++b ) {
            if ( a == data.end() )
                return b == val.data.end();
            if ( b == data.end() )
                return false;
            if ( *a != *b )
                return false;
        }
    }

    bool operator!=( T val ) const {
        return not operator==( val );
    }

    bool operator!=( const IntervalSet &val ) const {
        return not operator==( val );
    }

    bool empty() const {
        return data.empty();
    }

    T min() const {
        return data.begin() != data.end() ? data.begin()->min : T( 0 );
    }

    T max() const {
        return data.rbegin() != data.rend() ? data.rbegin()->max : T( 0 );
    }

    const_iterator begin() const { return this; }
    const_iterator end  () const { return { this, data.end(), 0 }; }

    T get_and_rem_val() {
        Iter iter = data.begin();
        if ( iter == data.end() )
            return T( 0 );
        T res = iter->min;
        if ( res == iter->max )
            data.erase( iter );
        else
            const_cast<T &>( iter->min ) = res + 1;
        return res;
    }

    PT size() const {
        PT res = 0;
        for( CIter iter = data.begin(); iter != data.end(); ++iter )
            res += iter->max + 1 - iter->min;
        return res;
    }

    bool size_is_less_than( T val ) const {
        T res = 0;
        for( CIter iter = data.begin(); iter != data.end(); ++iter ) {
            res += iter->end + 1 - iter->beg;
            if ( res >= val )
                return false;
        }
        return true;
    }

    bool contains( T val ) const {
        if ( data.empty() )
            return false;
        CIter iter = data.upper_bound( Interval( val, val ) );
        if ( iter == data.begin() )
            return false;
        --iter;
        return val <= iter->max;
    }

    bool contains( T min, T max ) const {
        if ( data.empty() )
            return false;
        CIter iter = data.upper_bound( Interval( min, min ) );
        if ( iter == data.begin() )
            return false;
        --iter;
        return min >= iter->min and max <= iter->max;
    }


    IntervalSet &insert( T val ) {
        if ( data.empty() ) {
            data.insert( Interval( val, val ) );
            return *this;
        }

        Iter iter = data.upper_bound( Interval( val, val ) );
        if ( iter == data.begin() ) {
            if ( val + 1 == iter->min )
                const_cast<T &>( iter->min ) = val;
            else
                data.insert( Interval( val, val ) );
            return *this;
        }

        Iter next = iter--;
        if ( val <= iter->max )
            return *this;

        if ( val == iter->max + 1 ) {
            // touching intervals
            if ( next != data.end() and val + 1 == next->min ) {
                const_cast<T &>( iter->max ) = next->max;
                data.erase( next );
            } else
                const_cast<T &>( iter->max ) = val;
        } else if ( next != data.end() and val + 1 == next->min )
            const_cast<T &>( next->min ) = val;
        else
            data.insert( iter, Interval( val, val ) );
        return *this;
    }

    IntervalSet &insert( T min, T max ) {
        if ( data.empty() ) {
            data.insert( Interval( min, max ) );
            return *this;
        }

        Iter base = data.upper_bound( Interval( min, min ) );
        if ( base == data.begin() ) {
            if ( max + 1 < base->min ) {
                data.insert( Interval( min, max ) );
                return *this;
            }
            const_cast<T &>( base->min ) = min;
        } else {
            --base;
            if ( min > base->max + 1 ) {
                ++base;
                if ( base == data.end() or max + 1 < base->min ) {
                    data.insert( --base, Interval( min, max ) );
                    return *this;
                }
                const_cast<T &>( base->min ) = min;
            }
        }

        Iter iter = base, last = base;
        while ( ++iter != data.end() and max + 1 >= iter->min )
            last = iter;
        const_cast<T &>( base->max ) = std::max( last->max, max );
        data.erase( ++base, iter );
        return *this;
    }

    IntervalSet &insert( const IntervalSet &i ) {
        for( const Interval &inter : i.data )
            insert( inter.min, inter.max );
        return *this;
    }

    IntervalSet &operator+=( const IntervalSet &i ) {
        return insert( i );
    }

    IntervalSet &clear() {
        data.clear();
        return *this;
    }

    IntervalSet &remove( T val ) {
        if ( data.empty() )
            return *this;

        Iter iter = data.upper_bound( Interval( val, val ) );
        if ( iter == data.begin() )
            return *this;

        --iter;
        if ( val > iter->max )
            return *this;

        if ( iter->min == val ) {
            if ( iter->max == val )
                data.erase( iter );
            else
                const_cast<T &>( iter->min ) = ++val;
        } else if ( iter->max == val ) {
            const_cast<T &>( iter->max ) = --val;
        } else {
            T old_max = iter->max;
            const_cast<T &>( iter->max ) = val - 1;
            data.insert( iter, Interval( val + 1, old_max ) );
        }
        return *this;
    }

    IntervalSet &remove( T min, T max ) {
        if ( data.empty() )
            return *this;

        //
        Iter iter = data.upper_bound( Interval( min, min ) );
        if ( iter != data.begin() ) {
            --iter;
            // we can keep something in this interval ?
            if ( min > iter->min ) {
                T iter_max = iter->max;
                if ( min <= iter_max )
                    const_cast<T &>( iter->max ) = min - 1;
                if ( max < iter_max ) {
                    data.insert( iter, Interval( max + 1, iter_max ) );
                    return *this;
                }
                ++iter;
            }
        }

        // iter will be the first interval to keep (but perhaps that will be modified)
        Iter tced = iter;
        while ( iter != data.end() and iter->max <= max )
            ++iter;
        data.erase( tced, iter );
        if ( iter != data.end() and iter->min <= max )
            const_cast<T &>( iter->min ) = max + 1;
        return *this;
    }

    IntervalSet &remove( const IntervalSet &i ) {
        for( const Interval &inter : i.data )
            remove( inter.min, inter.max );
        return *this;
    }

    IntervalSet &operator-=( const IntervalSet &i ) {
        return remove( i );
    }

    void write_to_stream( std::ostream &os ) const {
        int cpt = 0;
        for( CIter iter = data.begin(); iter != data.end(); ++iter ) {
            if ( cpt++ )
                os << ",";
            os << iter->min;
            if ( iter->min != iter->max )
                os << "--" << iter->max;
        }
    }

    template<class Cq>
    void write_to( BinStream<Cq> &bq ) const {
        bq << data.size();
        for( CIter iter = data.begin(); iter != data.end(); ++iter )
            bq << iter->min << iter->max;
    }

    template<class Cq>
    static IntervalSet read_from( BinStream<Cq> &bq ) {
        IntervalSet res;
        for( PT size = bq.read(); not bq.error() and size; --size ) {
            T beg = bq.read();
            T end = bq.read();
            if ( not bq.error() )
                res.insert( beg, end );
        }
        return res;
    }

    Set data;
};


} // namespace RiotBase



