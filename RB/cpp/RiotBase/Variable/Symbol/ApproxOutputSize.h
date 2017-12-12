#pragma once

#include "LazySourceInfo.h"
#include "Op.h"

namespace RiotBase {

template<class Op>
struct ApproxOutputSize {
    static PT value( const LazySourceInfo **children_info, const Category **children_cat ) { PT res = Op::nb_args; for( PT i = 0; i < Op::nb_args; ++i ) res += children_info[ i ]->output_size; return res / Op::nb_args; }
};

template<> struct ApproxOutputSize<Op_split> { static PT value( const LazySourceInfo **children_info, const Category **children_cat ) { return children_info[ 0 ]->output_size; } };
template<> struct ApproxOutputSize<Op_join>  { static PT value( const LazySourceInfo **children_info, const Category **children_cat ) { return children_info[ 0 ]->output_size; } };

}
