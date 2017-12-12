#pragma once

#include "CommonSymbolicData.h"

namespace RiotBase {

template<class Op>
void _symbolic_visitor( const Op &op, const Rp *rp ) {
    // there's already a node ?
    if ( CommonSymbolicData *csd = RO_CALL( common_symbolic_data, rp ) ) {
        if ( csd->op_id == CommonSymbolicData::cur_op_id )
            return;
        csd->op_id = CommonSymbolicData::cur_op_id;
    }

    // else, visit the children
    for( int i = 0, l = RO_CALL( nb_symbolic_children, rp ); i < l; ++i )
        _symbolic_visitor( op, RO_CALL( symbolic_child, rp, i ) );

    // and execute to the operation
    op( rp );
}

template<class Op>
void symbolic_visitor( const Op &op, const Rp *rp ) {
    ++CommonSymbolicData::cur_op_id;
    _symbolic_visitor( op, rp );
}

} // namespace RiotBase
