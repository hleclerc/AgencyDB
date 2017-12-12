#include "../Symbol/LazySourceKnownValue.h"
#include "CallbackFunctor.h"
#include "Deserialize.h"
#include "AutoRpDel.h"
#include "Variable.h"
#include "Surdef.h"
#include "Error.h"

#include "../../System/Ccast.h"
// #include "../../Db/Gev.h"

namespace RiotBase {

void Variable::write_to_stream( std::ostream &os ) const {
    RO_CALL( write_to_stream, rp, os );
}

void Variable::write_structure( std::ostream &os ) const {
    RO_CALL( write_structure, rp, os );
}

void Variable::write_to( BBQ bq ) const {
    RO_CALL( serialize, rp, bq, nullptr );
}

Rp *Variable::read_from( BinStream<CbStringPtr> bq ) {
    Vec<RpAndUse> objects;
    return deserialize( bq, objects );
}

void Variable::on_change( Callback *c ) {
    RO_CALL( self_on_change, rp, c );
}

void Variable::then( Callback *c ) {
    RO_CALL( self_then, rp, c );
}

bool Variable::is_symbolic() {
    return RO_CALL( is_symbolic, rp );
}

Vec<std::string> Variable::get_possible_rights() const {
    return RO_CALL( get_possible_rights, const_cast<Rp *&>( rp ) );
}

Vec<GrpId> Variable::get_groups_in_acl() const {
    return RO_CALL( get_groups_in_acl, rp );
}

Vec<UsrId> Variable::get_users_in_acl() const {
    return RO_CALL( get_users_in_acl, rp );
}

Vec<std::string> Variable::get_grp_rights( GrpId grp ) const {
    return RO_CALL( get_grp_rights, rp, grp );

}

Vec<std::string> Variable::get_usr_rights( UsrId usr ) const {
    return RO_CALL( get_usr_rights, rp, usr );
}

bool Variable::add_grp_right( const std::string &right, GrpId grp ) {
    return RO_CALL( add_grp_right, rp, grp, right );
}

bool Variable::add_usr_right( const std::string &right, UsrId usr ) {
    return RO_CALL( add_usr_right, rp, usr, right );
}

bool Variable::rem_grp_right( const std::string &right, GrpId grp ) {
    return RO_CALL( rem_grp_right, rp, grp, right );
}

bool Variable::rem_usr_right( const std::string &right, UsrId usr ) {
    return RO_CALL( rem_usr_right, rp, usr, right );
}

void Variable::wait() {
    TODO;
//    if ( is_symbolic() ) {
//        struct GevStop { void operator()() const { gev->stop(); } };
//        then( new CallbackFunctor<GevStop> );
//        gev->run();
//    }
}

Rp *_lazyfy( const Variable &var ) {
    Rp *cp = RO_CALL( copy, var.rp );
    return RO_CALL( common_symbolic_data, cp ) ? cp : LazySourceKnownValue::mkl( cp );
}

Rp *_lazyfy( Variable &&var ) {
    Rp *res = RO_CALL( common_symbolic_data, var.rp ) ? var.rp : LazySourceKnownValue::mkl( var.rp );
    var.rp = 0;
    return res;
}

namespace {

RiotBase_DEF_FUNC_2( self_set_b          , 0  , true ) { if ( v0 != v1 ) { Rp *v = RO_CALL( copy, v1 ); RO_CALL( del, v0 ); v0 = v; } }
RiotBase_DEF_FUNC_2( self_set_o          , 0  , true ) { if ( v0 != v1 ) RO_CALL( del, v0 ); v0 = v1; }
RiotBase_DEF_FUNC_1( is_a_s_op           , 0  , true ) { return 0; }
RiotBase_DEF_FUNC_1( common_symbolic_data, 0  , true ) { return 0; }
RiotBase_DEF_FUNC_1( nb_symbolic_children, 0  , true ) { return 0; }
RiotBase_DEF_FUNC_1( symbolic_child      , 0  , true, int ) { return 0; }
RiotBase_DEF_FUNC_1( launch              , 0  , true ) {}
RiotBase_DEF_FUNC_1( self_then           , 0.1, true, Callback *c ) { c->exec( &v0 ); dec_ref( c ); }
RiotBase_DEF_FUNC_1( to_std_string       , 0  , true, std::string *res, PT nb_items ) { if ( nb_items != 1 ) return false; std::ostringstream ss; RO_CALL( write_to_stream, v0, ss ); *res = ss.str(); return true; }

#include "SelfOpIfTermValue.h"
#include "StdOpOxxxx.h"

}

} // namespace RiotBase
