#include "../../System/Scast.h"
#include "../Core/Surdef.h"
#include "../Core/Error.h"
#include "GenArraySelect.h"

namespace RiotBase {

static AutoRpDel err_removed_element = Error::New( "Removed element" );

GenArraySelect::GenArraySelect( GenArray *ori, PT pos ) : ori( ori ), pos( pos ) {
    ori->set_array_selects.insert( this );
}

GenArraySelect::~GenArraySelect() {
    ori->set_array_selects.erase( this );
}

std::string GenArraySelect::type_name() {
    return "GenArraySelect";
}

Rp **GenArraySelect::prox_ptr() {
    if ( pos < ori->items.size() ) {
        ori->sig_change();
        return &ori->items[ pos ].rp;
    }
    return &reinterpret_cast<Rp *&>( err_removed_element.rp );
}

const Rp *GenArraySelect::prox_ref() const {
    if ( pos < ori->items.size() )
        return ori->items[ pos ].rp;
    return err_removed_element.rp;
}

namespace {

RiotBase_DEF_FUNC_2( self_set_b, 1, tv0->isa<GenArraySelect>() and tv1->known_term_value() ) {
    GenArraySelect *g = scast( v0 );
    if ( g->pos < g->ori->items.size() ) {
        g->ori->sig_change();
        RO_CALL( self_set_b, g->ori->items[ g->pos ].rp, v1 );
    }
}
RiotBase_DEF_FUNC_2( self_set_o, 1, tv0->isa<GenArraySelect>() and tv1->known_term_value() ) {
    GenArraySelect *g = scast( v0 );
    if ( g->pos < g->ori->items.size() ) {
        g->ori->sig_change();
        RO_CALL( self_set_o, g->ori->items[ g->pos ].rp, v1 );
    }
}

}


} // namespace RiotBase
