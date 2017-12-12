#include "../../System/Rcast.h"
#include "OtWrapperNumber.h"

namespace RiotBase {

Rp *OtWrapperNumber_New( Rp *rp ) {
    return OtWrapperNumber::New( rp );
}

/**
  On a besoin de stocker la façon avec laquelle on est arrivés à la valeur en cours, d'où la nécessité de faire un reg_op dès le début.
  Rq: on stocke des Rp * sans correction de cur_xxx, il faudrait faire une méthode de correction
  Aussi à faire: optimisation des patches
*/
OtWrapperNumber::OtWrapperNumber( Rp *val, UsrId usr_id ) : OtWrapper<OtWrapperNumber>( usr_id ), val( val ) {
    if ( bool( this->val ) )
        reg_op( op_set_und{}, this->val, Number{} );
}

std::string OtWrapperNumber::type_name() {
    return "OtWrapperNumber";
}

const Rp *OtWrapperNumber::prox_ref() const {
    return val.rp;
}

void OtWrapperNumber::write_structure( std::ostream &os ) const {
    RO_CALL( write_structure, val.rp, os << type_name() << '(' ); os << ')';
}

void OtWrapperNumber::self_add( const Number &n ) {
    if ( this->right_to( op_add{}, {}, n ) ) {
        reg_op( op_add{}, n );
        val += n;
    }
}

void OtWrapperNumber::apply_op( op_set, Aod &aod, const Number &str ) {
    if ( val != str ) {
        val = str;
        this->sig_change();
    }
}

void OtWrapperNumber::apply_op( op_set_und, Aod &aod, const Number &str, const Number &old ) {
    apply_op( op_set{}, aod, str );
}

void OtWrapperNumber::apply_op( op_add, Aod &aod, const Number &str ) {
    if ( str ) {
        val += str;
        this->sig_change();
    }
}

bool OtWrapperNumber::right_to( op_set, UsrId as_usr, const Number &str ) {
    return str > val ? this->has_right( can_increase{}, as_usr ) : bool( str == val ) or this->has_right( can_decrease{}, as_usr );
}

bool OtWrapperNumber::right_to( op_set_und, UsrId as_usr, const Number &str, const Number &old ) {
    return right_to( op_set{}, as_usr, str );
}

bool OtWrapperNumber::right_to( op_add, UsrId as_usr, const Number &str ) {
    return str > 0 ? this->has_right( can_increase{}, as_usr ) : bool( str == 0 ) or this->has_right( can_decrease{}, as_usr );
}

void OtWrapperNumber::undo_patch( op_set, Aod &aod, Number &str ) {
    WARNING( "Undo on a non und version should not happen" );
}

void OtWrapperNumber::undo_patch( op_set_und, Aod &aod, Number &str, Number &old ) {
    val = old;
}

void OtWrapperNumber::undo_patch( op_add, Aod &aod, Number &str ) {
    val -= str;
}

void OtWrapperNumber::transmission( op_set_und, BBQ br, Number &str, Number &old ) {
    transmission( op_set{}, br, str );
}

void OtWrapperNumber::register_patch_op( op_set, BBQ bq, Number &n ) {
    register_patch_op( op_set_und{}, bq, n, val );
}

namespace {

RiotBase_DEF_FUNC_2( self_add_b, 1, tv0->isa<OtWrapperNumber>() and tv1->static_category() == Number::category ) {
    static_cast<OtWrapperNumber *>( v0 )->self_add( reinterpret_cast<const Number &>( v1 ) );
}
RiotBase_DEF_FUNC_2( self_sub_b, 1, tv0->isa<OtWrapperNumber>() and tv1->static_category() == Number::category ) {
    static_cast<OtWrapperNumber *>( v0 )->self_add( - reinterpret_cast<const Number &>( v1 ) );
}

}


} // namespace RiotBase
