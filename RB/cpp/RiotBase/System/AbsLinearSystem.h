#pragma once

#include "Vec.h"

namespace RiotBase {

struct AbsLinearSolver {
    using T = double;
    using V = Vec<T>;

    struct Item {
        PT     num;
        double coef;
    };
    /// something between bars (absolute values)
    struct Term {
        Term( double offset = 0 ) : offset( offset ) {}

        T    operator()  ( const V &x ) const;
        T    s_val       ( const V &x ) const;
        void add_gradient( V &g, const V &x ) const;
        void add_gram_schmidt( Vec<V> &h, const V &g, PT s ) const;
        bool ort_gradient( V &g ) const;
        T    get_alpha   ( const V &g, const V &x ) const;
        bool on_edge     ( const V &x ) const;

        Vec<Item> items;
        double    offset;
    };

    T         residual() const;
    void      solve();

    Vec<Term> terms;
    V         x;
};

} // namespace RiotBase
