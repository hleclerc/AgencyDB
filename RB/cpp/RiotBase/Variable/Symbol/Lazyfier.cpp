#include "../../System/Scast.h"
#include "../../EvLoop/Gev.h"
#include "../Core/Surdef.h"
#include "ActiveLazySources.h"
#include "GenSymSelect.h"
#include "Lazyfier.h"
#include "Symbol.h"

#include "SymOp.h"

namespace RiotBase {

Lazyfier::Lazyfier( Rp *cur ) : cur( cur ) {
    if ( CommonSymbolicData *csd = RO_CALL( common_symbolic_data, cur ) )
        csd->lazyfier_parents << this;
}

Lazyfier::~Lazyfier() {
    if ( cur ) {
        if ( CommonSymbolicData *csd = RO_CALL( common_symbolic_data, cur ) )
            csd->lazyfier_parents.remove_first_unordered( this );
        RO_CALL( del, cur );
    }
    for( GenSymSelect *s : sym_selects )
        s->container = 0;
}

void Lazyfier::write_to_stream( std::ostream &os ) const {
    RO_CALL( write_to_stream, _wait_for_result(), os );
}

void Lazyfier::write_graphviz( std::ostream &os ) const {
    os << "node_" << this << " [label=\"LZ\" shape=box];";
}

void Lazyfier::write_structure( std::ostream &os ) const {
    RO_CALL( write_structure, cur, os << "Lazyfier(" );
    os << ')';
}

std::string Lazyfier::type_name() {
    return "Lazyfier";
}

const Rp *Lazyfier::prox_ref() const {
    return cur;
}

Rp *Lazyfier::copy() const {
    return Lazyfier::New( RO_CALL( copy, cur ) );
}

const Lazyfier *Lazyfier::_wait_for_result() const {
    exec_list->result_watcher.stop_gev_at.insert( this );
    gev->run();
    return this;
}

Lazyfier *Lazyfier::_wait_for_result() {
    exec_list->result_watcher.stop_gev_at.insert( this );
    gev->run();
    return this;
}

namespace {

#include "Lazyfier_surdefs.h"

RiotBase_DEF_FUNC_1( del_or_cancel_exec, 0, true ) {
    RO_CALL( del, v0 );
}


} // namespace

} // namespace RiotBase
