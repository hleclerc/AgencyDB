#include "../../System/SplittedVec.h"
#include "../../System/Rcast.h"
#include "../../System/Ccast.h"

#include "../Symbol/CommonSymbolicData.h"
#include "../Symbol/Lazyfier.h"
#include "../Symbol/Executed.h"
#include "../Symbol/Symbol.h"

#include "SetLazyVal.h"

#include <set>

namespace RiotBase {

namespace {

struct ExecData {
    ExecData() : val( 0 ), to_be_used( 0 ), own( true ) {}

    Rp     *val;
    int     to_be_used; ///< nb times we expect this to be used
    bool    own;
};

void get_lazyfiers( std::set<Lazyfier *> &lazyfiers, const Rp *rp ) {
    if ( CommonSymbolicData *csd = RO_CALL( common_symbolic_data, rp ) ) {
        if ( csd->op_id == CommonSymbolicData::cur_op_id )
            return;
        csd->op_id = CommonSymbolicData::cur_op_id;

        for( Lazyfier *p : csd->lazyfier_parents )
            lazyfiers.insert( p );
        for( const Rp *p : csd->parents )
            get_lazyfiers( lazyfiers, p );
    }
}

void set_exec_data( SplittedVec<ExecData,32> &exec_data, const Rp *rp ) {
    if ( CommonSymbolicData *csd = RO_CALL( common_symbolic_data, rp ) ) {
        if ( csd->op_id == CommonSymbolicData::cur_op_id )
            return;
        csd->op_id = CommonSymbolicData::cur_op_id;

        ExecData *ed = exec_data.push_back();
        ed->to_be_used = csd->parents.size() + csd->lazyfier_parents.size();
        csd->op_mp = ed;

        for( int i = 0, n = RO_CALL( nb_symbolic_children, rp ); i < n; ++i )
            set_exec_data( exec_data, RO_CALL( symbolic_child, rp, i ) );
    }
}

bool all_children_are_done( const Rp *o ) {
    if ( CommonSymbolicData *csd = RO_CALL( common_symbolic_data, o ) )
        if ( csd->op_id != CommonSymbolicData::cur_op_id )
            return false;
    for( int i = 0, n = RO_CALL( nb_symbolic_children, o ); i < n; ++i ) {
        const Rp *ch = RO_CALL( symbolic_child, o, i );
        if ( CommonSymbolicData *csd = RO_CALL( common_symbolic_data, ch ) )
            if ( csd->op_id != CommonSymbolicData::cur_op_id or not reinterpret_cast<ExecData *>( csd->op_mp )->val )
                return false;
    }
    return true;
}

const Rp *get_leaf_symbol( const Rp *rp ) {
    if ( RO_CALL( is_a_s_op, rp ) and RO_CALL( nb_symbolic_children, rp ) )
        return get_leaf_symbol( RO_CALL( symbolic_child, rp, 0 ) );
    return rp;
}

} // namespace

void set_lazy_val( const Rp *symbol, Rp *value ) {
    // if symbol is a Lazyfier, user want actually to modify the ref
    if ( symbol->type_num == StaticStore<TypeTempl<Lazyfier>>::data.type_num )
        return set_lazy_val( get_leaf_symbol( static_cast<const Lazyfier *>( symbol )->cur ), value );

    // find the target Lazyfiers (value that will be changed with symbol)
    std::set<Lazyfier *> lazyfiers;
    ++CommonSymbolicData::cur_op_id;
    get_lazyfiers( lazyfiers, symbol );

    // if not target lazifier, simply replace the symbol
    if ( lazyfiers.empty() )
        WARNING( "Weird..." );
    //return RO_CALL( self_set_o, symbol, value );

    // set up op_mp for all the children
    SplittedVec<ExecData,32> exec_data;
    ++CommonSymbolicData::cur_op_id;
    for( Lazyfier *l : lazyfiers )
        set_exec_data( exec_data, l->cur );

    // `symbol` arg => `value`
    CommonSymbolicData *csd = RO_CALL( common_symbolic_data, symbol );
    if ( not csd ) {
        WARNING( "First arg of a set_lazy_val must be a symbolic expr" );
        return;
    }
    reinterpret_cast<ExecData *>( csd->op_mp )->val = value;

    // set up the front
    Vec<const Rp *> front;
    for( const Rp *p : csd->parents )
        if ( all_children_are_done( p ) )
            front.push_back_unique( p );

    // IMPORTANT_TODO: copy expressions with several self_op as parents

    //
    Vec<Rp *> children;
    while ( front.size() ) {
        // first, look for a non "_s" item (because "_s" will definitively modify it)
        const Rp *item = 0;
        for( PT i = front.size(); i--; ) {
            if ( not RO_CALL( is_a_s_op, front[ i ] ) ) {
                item = front[ i ];
                front.remove_unordered( i );
                break;
            }
        }
        // else, take the last item in the front (no particular property)
        if ( not item ) {
            item = front.back();
            front.pop_back();
        }

        // get symbolic data
        CommonSymbolicData *csd = RO_CALL( common_symbolic_data, item );
        if ( not csd )
            continue; // ok but that's weird

        // exec
        PT n = RO_CALL( nb_symbolic_children, item );
        BitFieldExtSize own( 2 * n, true ); // 2 * i + 0 -> true if expected to be used/owned by the exec call (true if not used elsewhere)
                                            // 2 * i + 1 -> true if a different object than the corresponding operation child
        children.resize( n );
        for( PT i = 0; i < n; ++i ) {
            children[ i ] = ccast( RO_CALL( symbolic_child, item, i ) );
            CommonSymbolicData *csc = RO_CALL( common_symbolic_data, children[ i ] );
            if ( csc and csc->op_id == CommonSymbolicData::cur_op_id ) {
                ExecData *ed = rcast( csc->op_mp );
                children[ i ] = ed->val;
                if ( --ed->to_be_used )
                    own.set( 2 * n, 2 * i + 0, false );
            } else
                own.set( 2 * n, 2 * i + 1, false );
        }
        reinterpret_cast<ExecData *>( csd->op_mp )->val = RO_CALL( exec, item, own, children.ptr() );

        // new items to add in the front ?
        for( const Rp *p : csd->parents )
            if ( all_children_are_done( p ) )
                front.push_back_unique( p );
    }

    // set final values
    for( Lazyfier *l : lazyfiers ) {
        Rp *old_val = l->cur;
        if ( CommonSymbolicData *csd = RO_CALL( common_symbolic_data, old_val ) ) {
            ExecData *ed = rcast( csd->op_mp );
            if ( ed->val ) {
                Rp *new_val = ed->val;
                Executed::assign_type_to( l )->val.rp = --ed->to_be_used ? RO_CALL( copy, new_val ) : new_val;
                RO_CALL( del, old_val );
            }
        }
    }
}

} // namespace RiotBase
