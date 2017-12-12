#include "../../System/Scast.h"
#include "OtWrapperMapSelect.h"
#include "../Core/Surdef.h"

namespace RiotBase {

OtWrapperMapSelect::OtWrapperMapSelect( OtWrapperMap::Node *n, OtWrapperMap *o ) : n( n ), o( o ) {
}

std::string OtWrapperMapSelect::type_name() {
    return "OtWrapperMapSelect";
}

Rp **OtWrapperMapSelect::prox_ptr() {
    o->sig_change_val( n );
    return &n->val;
}

const Rp *OtWrapperMapSelect::prox_ref() const {
    return n->val;
}

void OtWrapperMapSelect::on_change( Callback *c ) {
    RO_CALL( self_on_change, n->val, c );
}


namespace {

RiotBase_DEF_FUNC_2( self_set_b, 1, tv0->isa<OtWrapperMapSelect>() and tv1->known_term_value() ) {
    OtWrapperMapSelect *g = scast( v0 );
    g->o->sig_change_val( g->n );
    RO_CALL( self_set_b, g->n->val, v1 );
}

RiotBase_DEF_FUNC_2( self_set_o, 1, tv0->isa<OtWrapperMapSelect>() and tv1->known_term_value() ) {
    OtWrapperMapSelect *g = scast( v0 );
    g->o->sig_change_val( g->n );
    RO_CALL( self_set_o, g->n->val, v1 );
}

}


} // namespace RiotBase
