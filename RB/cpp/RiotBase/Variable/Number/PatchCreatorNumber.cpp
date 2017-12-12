#include "../../System/Rcast.h"
#include "PatchCreatorNumber.h"

namespace RiotBase {

Rp *PatchCreatorNumber_New( Rp *rp ) {
    return PatchCreatorNumber::New( rp );
}

/**
  On a besoin de stocker la façon avec laquelle on est arrivés à la valeur en cours, d'où la nécessité de faire un reg_op dès le début.
  Rq: on stocke des Rp * sans correction de cur_xxx, il faudrait faire une méthode de correction
  Aussi à faire: optimisation des patches
*/
PatchCreatorNumber::PatchCreatorNumber( Rp *val, UsrId usr_id ) : PatchCreator<PatchCreatorNumber>( usr_id ), val( val ) {
    if ( bool( this->val ) )
        reg_op( pcd.bo, op_set_und{}, this->val, Number{} );
}

std::string PatchCreatorNumber::type_name() {
    return "PatchCreatorNumber";
}

const Rp *PatchCreatorNumber::prox_ref() const {
    return val.rp;
}

void PatchCreatorNumber::write_structure( std::ostream &os ) const {
    RO_CALL( write_structure, val.rp, os << type_name() << '(' ); os << ')';
}

void PatchCreatorNumber::self_add( const Number &n ) {
    if ( right_to( op_add{}, {}, n ) ) {
        reg_op( pcd.bo, op_add{}, n );
        val += n;
    }
}

void PatchCreatorNumber::apply_op( op_set,  Aod &aod, const Number &str ) {
    val = str;
    this->sig_change();
}

void PatchCreatorNumber::apply_op( op_set_und, Aod &aod, const Number &str, const Number &old ) {
    apply_op( op_set{}, aod, str );
}

void PatchCreatorNumber::apply_op( op_add, Aod &aod, const Number &str ) {
    val += str;
    this->sig_change();
}

bool PatchCreatorNumber::right_to( op_set, UsrId as_usr, const Number &str ) {
    return str > val ? this->has_right( can_increase{}, as_usr ) : bool( str == val ) or this->has_right( can_decrease{}, as_usr );
}

bool PatchCreatorNumber::right_to( op_set_und, UsrId as_usr, const Number &str, const Number &old ) {
    return right_to( op_set{}, as_usr, str );
}

bool PatchCreatorNumber::right_to( op_add, UsrId as_usr, const Number &str ) {
    return str > 0 ? this->has_right( can_increase{}, as_usr ) : bool( str == 0 ) or this->has_right( can_decrease{}, as_usr );
}

void PatchCreatorNumber::undo_patch( PatchCreatorNumber::op_set, CommonPatchCreatorData::Undo u, const Number &str ) {
    WARNING( "Undo on a non und version should not happen" );
}

void PatchCreatorNumber::undo_patch( PatchCreatorNumber::op_set_und, CommonPatchCreatorData::Undo u, const Number &str, const Number &old ) {
    val = old;
}

void PatchCreatorNumber::undo_patch( PatchCreatorNumber::op_add, CommonPatchCreatorData::Undo u, const Number &str ) {
    val -= str;
}

void PatchCreatorNumber::register_patch_op( op_set, BBQ bq, Number &n ) {
     reg_op( bq, op_set_und{}, n, val );
}

namespace {

RiotBase_DEF_FUNC_2( self_add_b, 1, tv0->isa<PatchCreatorNumber>() and tv1->static_category() == Number::category ) {
    static_cast<PatchCreatorNumber *>( v0 )->self_add( reinterpret_cast<const Number &>( v1 ) );
}
RiotBase_DEF_FUNC_2( self_sub_b, 1, tv0->isa<PatchCreatorNumber>() and tv1->static_category() == Number::category ) {
    static_cast<PatchCreatorNumber *>( v0 )->self_add( - reinterpret_cast<const Number &>( v1 ) );
}

}


} // namespace RiotBase
