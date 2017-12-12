#include "AbsLinearSystem.h"

namespace RiotBase {

namespace {

template<class T>
T dot( const Vec<T> &a, const Vec<T> &b ) {
    T res = 0;
    for( PT i = 0; i < a.size(); ++i )
        res += a[ i ] * b[ i ];
    return res;
}

}

AbsLinearSolver::T AbsLinearSolver::Term::operator()( const V &x ) const {
    return std::abs( s_val( x ) );
}

AbsLinearSolver::T AbsLinearSolver::Term::s_val( const V &x ) const { // signed value
    T res = offset;
    for( const Item &i : items )
        res += i.coef * x[ i.num ];
    return res;
}

void AbsLinearSolver::Term::add_gradient( V &g, const V &x ) const {
    if ( s_val( x ) < 0 )
        for( const Item &i : items )
            g[ i.num ] += i.coef;
    else
        for( const Item &i : items )
            g[ i.num ] -= i.coef;
}

void AbsLinearSolver::Term::add_gram_schmidt( Vec<V> &h, const V &g, PT s ) const {
    // new vec
    V &n = *h.push_back( Size(), s, 0.0 );
    for( const Item &i : items )
        n[ i.num ] += i.coef;

    //    //
    //    if ( dot( n, g ) >= 0 ) {
    //        h.pop_back();
    //        return;
    //    }

    // orthogonalization
    for( PT k = h.size() - 1; k--; )
        if ( T d = dot( h[ k ], n ) )
            for( PT i = 0; i < s; ++i )
                n[ i ] -= d * h[ k ][ i ];

    // normalization
    if ( T v = sqrt( dot( n, n ) ) )
        for( PT i = 0; i < s; ++i )
            n[ i ] /= v;
}

AbsLinearSolver::T AbsLinearSolver::Term::get_alpha( const V &g, const V &x ) const {
    T d = 0;
    for( const Item &i : items )
        d += i.coef * g[ i.num ];
    return d ? - s_val( x ) / d : 0;
}

bool AbsLinearSolver::Term::on_edge( const V &x ) const {
    T res = offset, sco = 0;
    for( const Item &i : items ) {
        res += i.coef * x[ i.num ];
        sco += i.coef;
    }
    return std::abs( res ) <= 1e-3 * std::abs( sco ) / items.size();
}

void AbsLinearSolver::solve() {
    // initial vector (if not already given)
    PT s = 0;
    for( const Term &t : terms )
        for( const Item &i : t.items )
            s = std::max( s, i.num + 1 );
    if ( x.size() < s )
        x.resize( s, 0 );

    //
    T old_residual = 0;
    for( PT num_iter = 0; num_iter < 5; ++num_iter) {
        // find gradient, with items not on edge
        Vec<V> h;
        V g( Size(), s, 0.0 );
        for( const Term &t : terms )
            if ( not t.on_edge( x ) )
                t.add_gradient( g, x );

        // ortho gradient if bad direction
        for( const Term &t : terms )
            if ( t.on_edge( x ) )
                t.add_gram_schmidt( h, g, s );

        // orthogonalization of g
        for( V &o : h )
            if ( T d = dot( o, g ) )
                for( PT i = 0; i < s; ++i )
                    g[ i ] -= d * o[ i ];
        if ( T d = dot( g, g ) )
            for( PT i = 0; i < s; ++i )
                g[ i ] /= sqrt( d );

        // find alpha
        T alpha = std::numeric_limits<T>::max();
        for( const Term &t : terms )
            if ( not t.on_edge( x ) )
                if ( T a = t.get_alpha( g, x ) )
                    if ( alpha > a and a >= 1e-3 )
                        alpha = a;
        if ( alpha == std::numeric_limits<T>::max() )
            break;

        PRINT( x, g, alpha, residual() );

        // update vec
        T delta = 0;
        for( PT i = 0; i < s; ++i ) {
            T u = alpha * g[ i ];
            delta += u * u;
            x[ i ] += u;
        }
        if ( delta < 1e-6 )
            break;

        //
        if ( std::abs( old_residual - residual() ) < 1e-3 )
            break;
        old_residual = residual();
    }
}

AbsLinearSolver::T AbsLinearSolver::residual() const {
    T res = 0;
    for( const Term &t : terms )
        res += t( x );
    return res;
}


} // namespace RiotBase
