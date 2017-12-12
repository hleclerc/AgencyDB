#include "Callbackifier.h"
#include "Surdef.h"

namespace RiotBase {

Callbackifier::Callbackifier( Rp *src, Callback *c ) : RpTemplProxy( src ) {
    on_change_list << c;
}

std::string Callbackifier::type_name() {
    return "Callbackifier";
}

Rp **Callbackifier::prox_ptr() {
    sig_change();
    return &src.rp;
}

void Callbackifier::on_change( Callback *c ) {
    on_change_list << c;
}

void Callbackifier::write_structure( std::ostream &os ) const {
    os << "Callbackifier["; int cpt = 0;
    for( const RcPtr<Callback> &c : on_change_list )
        os << ( cpt++ ? "," : "" ) << c->name();
    os << "](";
    RO_CALL( write_structure, src.rp, os );
    os << ')';
}

void Callbackifier::sig_change() {
    for( RcPtr<Callback> &c : on_change_list )
        c->reg_wait();
}

namespace {

RiotBase_DEF_FUNC_1( self_on_change, 1, tv0->term_value(), Callback *c ) {
    v0 = Callbackifier::New( v0, c );
    c->reg_wait();
}

RiotBase_DEF_FUNC_1( self_on_change, 2, tv0->isa<Callbackifier>(), Callback *c ) {
    static_cast<Callbackifier *>( v0 )->on_change_list << c;
    c->reg_wait();
}

} // namespace


} // namespace RiotBase
