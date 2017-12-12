#include "../Core/AutoRpDel.h"
#include "OtWrapper.h"

namespace RiotBase {

namespace {

// defaults methods for rights
RiotBase_DEF_FUNC_1( get_groups_in_acl   , 0, true ) { return {}; }
RiotBase_DEF_FUNC_1( get_users_in_acl    , 0, true ) { return {}; }
RiotBase_DEF_FUNC_1( get_grp_rights      , 0, true, GrpId grp ) { return {}; }
RiotBase_DEF_FUNC_1( get_usr_rights      , 0, true, UsrId usr ) { return {}; }

// methods for which creation of an ot_wrapper would be great
RiotBase_DEF_FUNC_1( add_grp_right       , 1, tv0->has_surdef_for( RF_make_ot_wrapper() ), GrpId grp, const std::string &right ) { v0 = RO_CALL( make_ot_wrapper, v0 ); return RO_CALL( add_grp_right, v0, grp, right ); }
RiotBase_DEF_FUNC_1( rem_grp_right       , 1, tv0->has_surdef_for( RF_make_ot_wrapper() ), GrpId grp, const std::string &right ) { v0 = RO_CALL( make_ot_wrapper, v0 ); return RO_CALL( rem_grp_right, v0, grp, right ); }
RiotBase_DEF_FUNC_1( add_usr_right       , 1, tv0->has_surdef_for( RF_make_ot_wrapper() ), UsrId usr, const std::string &right ) { v0 = RO_CALL( make_ot_wrapper, v0 ); return RO_CALL( add_usr_right, v0, usr, right ); }
RiotBase_DEF_FUNC_1( rem_usr_right       , 1, tv0->has_surdef_for( RF_make_ot_wrapper() ), UsrId usr, const std::string &right ) { v0 = RO_CALL( make_ot_wrapper, v0 ); return RO_CALL( rem_usr_right, v0, usr, right ); }
RiotBase_DEF_FUNC_1( get_possible_rights , 1, tv0->has_surdef_for( RF_make_ot_wrapper() )                                      ) { v0 = RO_CALL( make_ot_wrapper, v0 ); return RO_CALL( get_possible_rights, v0 ); }

RiotBase_DEF_FUNC_1( pull_ot_changes     , 0, tv0->has_surdef_for( RF_make_ot_wrapper() ), BBQ changes                         ) { v0 = RO_CALL( make_ot_wrapper, v0 ); return RO_CALL( pull_ot_changes, v0, changes ); }
RiotBase_DEF_FUNC_1( new_ot_merger       , 0, tv0->has_surdef_for( RF_make_ot_wrapper() )                                      ) { v0 = RO_CALL( make_ot_wrapper, v0 ); return RO_CALL( new_ot_merger, v0 ); }

RiotBase_DEF_FUNC_1( self_on_change      , 1, tv0->has_surdef_for( RF_make_ot_wrapper() ), Callback *c                         ) { v0 = RO_CALL( make_ot_wrapper, v0 ); return RO_CALL( self_on_change, v0, c ); }
// RiotBase_DEF_FUNC_1( on_cha       , 1, tv0->has_surdef_for( RF_make_ot_wrapper() ), UsrId usr, const std::string &right ) { v0 = RO_CALL( make_ot_wrapper, v0 ); return RO_CALL( rem_usr_right, v0, usr, right ); }

}

} // namespace RiotBase
