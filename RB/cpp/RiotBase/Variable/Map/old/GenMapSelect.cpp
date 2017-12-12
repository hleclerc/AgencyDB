#include "../../System/Scast.h"
#include "../Core/Surdef.h"
#include "GenMapSelect.h"

namespace RiotBase {

GenMapSelect::GenMapSelect( GenMap::Node *n, GenMap *o) : n( n ), o( o ) {
}

std::string GenMapSelect::type_name() {
    return "GenMapSelect";
}

Rp **GenMapSelect::prox_ptr() {
    o->sig_change_val( n );
    return &n->val;
}

const Rp *GenMapSelect::prox_ref() const {
    return n->val;
}

void GenMapSelect::on_change( Callback *c ) {
    n->on_change_vals << c;
}


namespace {

RiotBase_DEF_FUNC_2( self_set_b, 1, tv0->isa<GenMapSelect>() and tv1->known_term_value() ) {
    GenMapSelect *g = scast( v0 );
    g->o->sig_change_val( g->n );
    RO_CALL( self_set_b, g->n->val, v1 );
}
RiotBase_DEF_FUNC_2( self_set_o, 1, tv0->isa<GenMapSelect>() and tv1->known_term_value() ) {
    GenMapSelect *g = scast( v0 );
    g->o->sig_change_val( g->n );
    RO_CALL( self_set_o, g->n->val, v1 );
}

}


} // namespace RiotBase
