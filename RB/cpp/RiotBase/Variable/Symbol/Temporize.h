#pragma once

#include "Lazyfier.h"
#include "SymOp.h"

namespace RiotBase {

struct Op_temporize {
    enum { nb_args = 1 };
    static const char *name() { return "temporize"; }
    Rp *exec_b( const Rp *v0 ) const { sleep( time_in_sec ); return RO_CALL( copy, v0 ); }
    static const Category *category( const Rp *v0 ) { return RO_CALL( category, v0 ); }
    double time_in_sec;
};

template<class Var>
Var temporize( const Var &var, double time_in_sec ) {
    return make_sym_op( Op_temporize{ time_in_sec }, var.rp );
}

} // namespace RiotBase
