/// This file is generated (by bin/Method_template.py). Do not modify it directly.
#include "../Core/FunctionTable.h"
#include "Lazyfier.h"
#include "SymOp.h"

namespace RiotBase {

namespace {



RiotBase_DEF_FUNC_2( set_bb, 1, tv0->unknown_term_value() or tv1->unknown_term_value() ) { return Lazyfier::New( SymOp<Op_set>::New( Op_set{}, Op_set::category( v0, v1 ), copy_without_lazifier( v0 ), copy_without_lazifier( v1 ) ) ); }
RiotBase_DEF_FUNC_2( set_bb, 0, FunctionTable_2<RF_self_set_b>::find_best_func( tv0, tv1 ).second > 0 ) { Rp *res = RO_CALL( copy, v0 ); RO_CALL( self_set_b, res, v1 ); return res; }
RiotBase_DEF_FUNC_2( set_ob, 1, tv0->unknown_term_value() or tv1->unknown_term_value() ) { return Lazyfier::New( SymOp<Op_set>::New( Op_set{}, Op_set::category( v0, v1 ), ref_without_lazifier( v0 ), copy_without_lazifier( v1 ) ) ); }
RiotBase_DEF_FUNC_2( set_sb, 1, tv0->unknown_term_value() or tv1->unknown_term_value() ) { return SymOp<Op_set,true>::New( Op_set{}, Op_set::category( v0, v1 ), ref_without_lazifier( v0 ), copy_without_lazifier( v1 ) ); }


RiotBase_DEF_FUNC_2( add_bb, 1, tv0->unknown_term_value() or tv1->unknown_term_value() ) { return Lazyfier::New( SymOp<Op_add>::New( Op_add{}, Op_add::category( v0, v1 ), copy_without_lazifier( v0 ), copy_without_lazifier( v1 ) ) ); }
RiotBase_DEF_FUNC_2( add_bb, 0, FunctionTable_2<RF_self_add_b>::find_best_func( tv0, tv1 ).second > 0 ) { Rp *res = RO_CALL( copy, v0 ); RO_CALL( self_add_b, res, v1 ); return res; }
RiotBase_DEF_FUNC_2( add_ob, 1, tv0->unknown_term_value() or tv1->unknown_term_value() ) { return Lazyfier::New( SymOp<Op_add>::New( Op_add{}, Op_add::category( v0, v1 ), ref_without_lazifier( v0 ), copy_without_lazifier( v1 ) ) ); }
RiotBase_DEF_FUNC_2( add_sb, 1, tv0->unknown_term_value() or tv1->unknown_term_value() ) { return SymOp<Op_add,true>::New( Op_add{}, Op_add::category( v0, v1 ), ref_without_lazifier( v0 ), copy_without_lazifier( v1 ) ); }


RiotBase_DEF_FUNC_2( sub_bb, 1, tv0->unknown_term_value() or tv1->unknown_term_value() ) { return Lazyfier::New( SymOp<Op_sub>::New( Op_sub{}, Op_sub::category( v0, v1 ), copy_without_lazifier( v0 ), copy_without_lazifier( v1 ) ) ); }
RiotBase_DEF_FUNC_2( sub_bb, 0, FunctionTable_2<RF_self_sub_b>::find_best_func( tv0, tv1 ).second > 0 ) { Rp *res = RO_CALL( copy, v0 ); RO_CALL( self_sub_b, res, v1 ); return res; }
RiotBase_DEF_FUNC_2( sub_ob, 1, tv0->unknown_term_value() or tv1->unknown_term_value() ) { return Lazyfier::New( SymOp<Op_sub>::New( Op_sub{}, Op_sub::category( v0, v1 ), ref_without_lazifier( v0 ), copy_without_lazifier( v1 ) ) ); }
RiotBase_DEF_FUNC_2( sub_sb, 1, tv0->unknown_term_value() or tv1->unknown_term_value() ) { return SymOp<Op_sub,true>::New( Op_sub{}, Op_sub::category( v0, v1 ), ref_without_lazifier( v0 ), copy_without_lazifier( v1 ) ); }


RiotBase_DEF_FUNC_2( mul_bb, 1, tv0->unknown_term_value() or tv1->unknown_term_value() ) { return Lazyfier::New( SymOp<Op_mul>::New( Op_mul{}, Op_mul::category( v0, v1 ), copy_without_lazifier( v0 ), copy_without_lazifier( v1 ) ) ); }
RiotBase_DEF_FUNC_2( mul_bb, 0, FunctionTable_2<RF_self_mul_b>::find_best_func( tv0, tv1 ).second > 0 ) { Rp *res = RO_CALL( copy, v0 ); RO_CALL( self_mul_b, res, v1 ); return res; }
RiotBase_DEF_FUNC_2( mul_ob, 1, tv0->unknown_term_value() or tv1->unknown_term_value() ) { return Lazyfier::New( SymOp<Op_mul>::New( Op_mul{}, Op_mul::category( v0, v1 ), ref_without_lazifier( v0 ), copy_without_lazifier( v1 ) ) ); }
RiotBase_DEF_FUNC_2( mul_sb, 1, tv0->unknown_term_value() or tv1->unknown_term_value() ) { return SymOp<Op_mul,true>::New( Op_mul{}, Op_mul::category( v0, v1 ), ref_without_lazifier( v0 ), copy_without_lazifier( v1 ) ); }


RiotBase_DEF_FUNC_2( div_bb, 1, tv0->unknown_term_value() or tv1->unknown_term_value() ) { return Lazyfier::New( SymOp<Op_div>::New( Op_div{}, Op_div::category( v0, v1 ), copy_without_lazifier( v0 ), copy_without_lazifier( v1 ) ) ); }
RiotBase_DEF_FUNC_2( div_bb, 0, FunctionTable_2<RF_self_div_b>::find_best_func( tv0, tv1 ).second > 0 ) { Rp *res = RO_CALL( copy, v0 ); RO_CALL( self_div_b, res, v1 ); return res; }
RiotBase_DEF_FUNC_2( div_ob, 1, tv0->unknown_term_value() or tv1->unknown_term_value() ) { return Lazyfier::New( SymOp<Op_div>::New( Op_div{}, Op_div::category( v0, v1 ), ref_without_lazifier( v0 ), copy_without_lazifier( v1 ) ) ); }
RiotBase_DEF_FUNC_2( div_sb, 1, tv0->unknown_term_value() or tv1->unknown_term_value() ) { return SymOp<Op_div,true>::New( Op_div{}, Op_div::category( v0, v1 ), ref_without_lazifier( v0 ), copy_without_lazifier( v1 ) ); }


RiotBase_DEF_FUNC_2( mod_bb, 1, tv0->unknown_term_value() or tv1->unknown_term_value() ) { return Lazyfier::New( SymOp<Op_mod>::New( Op_mod{}, Op_mod::category( v0, v1 ), copy_without_lazifier( v0 ), copy_without_lazifier( v1 ) ) ); }
RiotBase_DEF_FUNC_2( mod_bb, 0, FunctionTable_2<RF_self_mod_b>::find_best_func( tv0, tv1 ).second > 0 ) { Rp *res = RO_CALL( copy, v0 ); RO_CALL( self_mod_b, res, v1 ); return res; }
RiotBase_DEF_FUNC_2( mod_ob, 1, tv0->unknown_term_value() or tv1->unknown_term_value() ) { return Lazyfier::New( SymOp<Op_mod>::New( Op_mod{}, Op_mod::category( v0, v1 ), ref_without_lazifier( v0 ), copy_without_lazifier( v1 ) ) ); }
RiotBase_DEF_FUNC_2( mod_sb, 1, tv0->unknown_term_value() or tv1->unknown_term_value() ) { return SymOp<Op_mod,true>::New( Op_mod{}, Op_mod::category( v0, v1 ), ref_without_lazifier( v0 ), copy_without_lazifier( v1 ) ); }


RiotBase_DEF_FUNC_2( add_key_bb, 1, tv0->unknown_term_value() or tv1->unknown_term_value() ) { return Lazyfier::New( SymOp<Op_add_key>::New( Op_add_key{}, Op_add_key::category( v0, v1 ), copy_without_lazifier( v0 ), copy_without_lazifier( v1 ) ) ); }
RiotBase_DEF_FUNC_2( add_key_bb, 0, FunctionTable_2<RF_self_add_key_b>::find_best_func( tv0, tv1 ).second > 0 ) { Rp *res = RO_CALL( copy, v0 ); RO_CALL( self_add_key_b, res, v1 ); return res; }
RiotBase_DEF_FUNC_2( add_key_ob, 1, tv0->unknown_term_value() or tv1->unknown_term_value() ) { return Lazyfier::New( SymOp<Op_add_key>::New( Op_add_key{}, Op_add_key::category( v0, v1 ), ref_without_lazifier( v0 ), copy_without_lazifier( v1 ) ) ); }
RiotBase_DEF_FUNC_2( add_key_sb, 1, tv0->unknown_term_value() or tv1->unknown_term_value() ) { return SymOp<Op_add_key,true>::New( Op_add_key{}, Op_add_key::category( v0, v1 ), ref_without_lazifier( v0 ), copy_without_lazifier( v1 ) ); }


RiotBase_DEF_FUNC_2( rem_key_bb, 1, tv0->unknown_term_value() or tv1->unknown_term_value() ) { return Lazyfier::New( SymOp<Op_rem_key>::New( Op_rem_key{}, Op_rem_key::category( v0, v1 ), copy_without_lazifier( v0 ), copy_without_lazifier( v1 ) ) ); }
RiotBase_DEF_FUNC_2( rem_key_bb, 0, FunctionTable_2<RF_self_rem_key_b>::find_best_func( tv0, tv1 ).second > 0 ) { Rp *res = RO_CALL( copy, v0 ); RO_CALL( self_rem_key_b, res, v1 ); return res; }
RiotBase_DEF_FUNC_2( rem_key_ob, 1, tv0->unknown_term_value() or tv1->unknown_term_value() ) { return Lazyfier::New( SymOp<Op_rem_key>::New( Op_rem_key{}, Op_rem_key::category( v0, v1 ), ref_without_lazifier( v0 ), copy_without_lazifier( v1 ) ) ); }
RiotBase_DEF_FUNC_2( rem_key_sb, 1, tv0->unknown_term_value() or tv1->unknown_term_value() ) { return SymOp<Op_rem_key,true>::New( Op_rem_key{}, Op_rem_key::category( v0, v1 ), ref_without_lazifier( v0 ), copy_without_lazifier( v1 ) ); }


RiotBase_DEF_FUNC_2( push_back_bb, 1, tv0->unknown_term_value() or tv1->unknown_term_value() ) { return Lazyfier::New( SymOp<Op_push_back>::New( Op_push_back{}, Op_push_back::category( v0, v1 ), copy_without_lazifier( v0 ), copy_without_lazifier( v1 ) ) ); }
RiotBase_DEF_FUNC_2( push_back_bb, 0, FunctionTable_2<RF_self_push_back_b>::find_best_func( tv0, tv1 ).second > 0 ) { Rp *res = RO_CALL( copy, v0 ); RO_CALL( self_push_back_b, res, v1 ); return res; }
RiotBase_DEF_FUNC_2( push_back_ob, 1, tv0->unknown_term_value() or tv1->unknown_term_value() ) { return Lazyfier::New( SymOp<Op_push_back>::New( Op_push_back{}, Op_push_back::category( v0, v1 ), ref_without_lazifier( v0 ), copy_without_lazifier( v1 ) ) ); }
RiotBase_DEF_FUNC_2( push_back_sb, 1, tv0->unknown_term_value() or tv1->unknown_term_value() ) { return SymOp<Op_push_back,true>::New( Op_push_back{}, Op_push_back::category( v0, v1 ), ref_without_lazifier( v0 ), copy_without_lazifier( v1 ) ); }


RiotBase_DEF_FUNC_3( insert_bbb, 1, tv0->unknown_term_value() or tv1->unknown_term_value() or tv2->unknown_term_value() ) { return Lazyfier::New( SymOp<Op_insert>::New( Op_insert{}, Op_insert::category( v0, v1, v2 ), copy_without_lazifier( v0 ), copy_without_lazifier( v1 ), copy_without_lazifier( v2 ) ) ); }
RiotBase_DEF_FUNC_3( insert_bbb, 0, FunctionTable_3<RF_self_insert_bb>::find_best_func( tv0, tv1, tv2 ).second > 0 ) { Rp *res = RO_CALL( copy, v0 ); RO_CALL( self_insert_bb, res, v1, v2 ); return res; }
RiotBase_DEF_FUNC_3( insert_obb, 1, tv0->unknown_term_value() or tv1->unknown_term_value() or tv2->unknown_term_value() ) { return Lazyfier::New( SymOp<Op_insert>::New( Op_insert{}, Op_insert::category( v0, v1, v2 ), ref_without_lazifier( v0 ), copy_without_lazifier( v1 ), copy_without_lazifier( v2 ) ) ); }
RiotBase_DEF_FUNC_3( insert_sbb, 1, tv0->unknown_term_value() or tv1->unknown_term_value() or tv2->unknown_term_value() ) { return SymOp<Op_insert,true>::New( Op_insert{}, Op_insert::category( v0, v1, v2 ), ref_without_lazifier( v0 ), copy_without_lazifier( v1 ), copy_without_lazifier( v2 ) ); }


RiotBase_DEF_FUNC_3( remove_bbb, 1, tv0->unknown_term_value() or tv1->unknown_term_value() or tv2->unknown_term_value() ) { return Lazyfier::New( SymOp<Op_remove>::New( Op_remove{}, Op_remove::category( v0, v1, v2 ), copy_without_lazifier( v0 ), copy_without_lazifier( v1 ), copy_without_lazifier( v2 ) ) ); }
RiotBase_DEF_FUNC_3( remove_bbb, 0, FunctionTable_3<RF_self_remove_bb>::find_best_func( tv0, tv1, tv2 ).second > 0 ) { Rp *res = RO_CALL( copy, v0 ); RO_CALL( self_remove_bb, res, v1, v2 ); return res; }
RiotBase_DEF_FUNC_3( remove_obb, 1, tv0->unknown_term_value() or tv1->unknown_term_value() or tv2->unknown_term_value() ) { return Lazyfier::New( SymOp<Op_remove>::New( Op_remove{}, Op_remove::category( v0, v1, v2 ), ref_without_lazifier( v0 ), copy_without_lazifier( v1 ), copy_without_lazifier( v2 ) ) ); }
RiotBase_DEF_FUNC_3( remove_sbb, 1, tv0->unknown_term_value() or tv1->unknown_term_value() or tv2->unknown_term_value() ) { return SymOp<Op_remove,true>::New( Op_remove{}, Op_remove::category( v0, v1, v2 ), ref_without_lazifier( v0 ), copy_without_lazifier( v1 ), copy_without_lazifier( v2 ) ); }


RiotBase_DEF_FUNC_2( equ_bb, 1, tv0->unknown_term_value() or tv1->unknown_term_value() ) { return Lazyfier::New( SymOp<Op_equ>::New( Op_equ{}, Op_equ::category( v0, v1 ), copy_without_lazifier( v0 ), copy_without_lazifier( v1 ) ) ); }


RiotBase_DEF_FUNC_2( neq_bb, 1, tv0->unknown_term_value() or tv1->unknown_term_value() ) { return Lazyfier::New( SymOp<Op_neq>::New( Op_neq{}, Op_neq::category( v0, v1 ), copy_without_lazifier( v0 ), copy_without_lazifier( v1 ) ) ); }


RiotBase_DEF_FUNC_2( inf_bb, 1, tv0->unknown_term_value() or tv1->unknown_term_value() ) { return Lazyfier::New( SymOp<Op_inf>::New( Op_inf{}, Op_inf::category( v0, v1 ), copy_without_lazifier( v0 ), copy_without_lazifier( v1 ) ) ); }


RiotBase_DEF_FUNC_2( sup_bb, 1, tv0->unknown_term_value() or tv1->unknown_term_value() ) { return Lazyfier::New( SymOp<Op_sup>::New( Op_sup{}, Op_sup::category( v0, v1 ), copy_without_lazifier( v0 ), copy_without_lazifier( v1 ) ) ); }


RiotBase_DEF_FUNC_2( inf_eq_bb, 1, tv0->unknown_term_value() or tv1->unknown_term_value() ) { return Lazyfier::New( SymOp<Op_inf_eq>::New( Op_inf_eq{}, Op_inf_eq::category( v0, v1 ), copy_without_lazifier( v0 ), copy_without_lazifier( v1 ) ) ); }


RiotBase_DEF_FUNC_2( sup_eq_bb, 1, tv0->unknown_term_value() or tv1->unknown_term_value() ) { return Lazyfier::New( SymOp<Op_sup_eq>::New( Op_sup_eq{}, Op_sup_eq::category( v0, v1 ), copy_without_lazifier( v0 ), copy_without_lazifier( v1 ) ) ); }


RiotBase_DEF_FUNC_2( bor_bb, 1, tv0->unknown_term_value() or tv1->unknown_term_value() ) { return Lazyfier::New( SymOp<Op_bor>::New( Op_bor{}, Op_bor::category( v0, v1 ), copy_without_lazifier( v0 ), copy_without_lazifier( v1 ) ) ); }


RiotBase_DEF_FUNC_2( select_bb, 1, tv0->unknown_term_value() or tv1->unknown_term_value() ) { return Lazyfier::New( SymOp<Op_select>::New( Op_select{}, Op_select::category( v0, v1 ), copy_without_lazifier( v0 ), copy_without_lazifier( v1 ) ) ); }


RiotBase_DEF_FUNC_1( neg_b, 1, tv0->unknown_term_value() ) { return Lazyfier::New( SymOp<Op_neg>::New( Op_neg{}, Op_neg::category( v0 ), copy_without_lazifier( v0 ) ) ); }


RiotBase_DEF_FUNC_1( on_change_b, 1, tv0->unknown_term_value(), Callback *c ) { return Lazyfier::New( SymOp<Op_on_change>::New( Op_on_change{c}, Op_on_change::category( v0 ), copy_without_lazifier( v0 ) ) ); }
RiotBase_DEF_FUNC_1( on_change_b, 0, FunctionTable_1<RF_self_on_change>::find_best_func( tv0 ).second > 0, Callback *c ) { Rp *res = RO_CALL( copy, v0 ); RO_CALL( self_on_change, res, c ); return res; }
RiotBase_DEF_FUNC_1( on_change_o, 1, tv0->unknown_term_value(), Callback *c ) { return Lazyfier::New( SymOp<Op_on_change>::New( Op_on_change{c}, Op_on_change::category( v0 ), ref_without_lazifier( v0 ) ) ); }
RiotBase_DEF_FUNC_1( on_change_s, 1, tv0->unknown_term_value(), Callback *c ) { return SymOp<Op_on_change,true>::New( Op_on_change{c}, Op_on_change::category( v0 ), ref_without_lazifier( v0 ) ); }


RiotBase_DEF_FUNC_1( then_b, 1, tv0->unknown_term_value(), Callback *c ) { return Lazyfier::New( SymOp<Op_then>::New( Op_then{c}, Op_then::category( v0 ), copy_without_lazifier( v0 ) ) ); }
RiotBase_DEF_FUNC_1( then_b, 0, FunctionTable_1<RF_self_then>::find_best_func( tv0 ).second > 0, Callback *c ) { Rp *res = RO_CALL( copy, v0 ); RO_CALL( self_then, res, c ); return res; }
RiotBase_DEF_FUNC_1( then_o, 1, tv0->unknown_term_value(), Callback *c ) { return Lazyfier::New( SymOp<Op_then>::New( Op_then{c}, Op_then::category( v0 ), ref_without_lazifier( v0 ) ) ); }
RiotBase_DEF_FUNC_1( then_s, 1, tv0->unknown_term_value(), Callback *c ) { return SymOp<Op_then,true>::New( Op_then{c}, Op_then::category( v0 ), ref_without_lazifier( v0 ) ); }


RiotBase_DEF_FUNC_2( split_bb, 1, tv0->unknown_term_value() or tv1->unknown_term_value() ) { return Lazyfier::New( SymOp<Op_split>::New( Op_split{}, Op_split::category( v0, v1 ), copy_without_lazifier( v0 ), copy_without_lazifier( v1 ) ) ); }


RiotBase_DEF_FUNC_2( join_bb, 1, tv0->unknown_term_value() or tv1->unknown_term_value() ) { return Lazyfier::New( SymOp<Op_join>::New( Op_join{}, Op_join::category( v0, v1 ), copy_without_lazifier( v0 ), copy_without_lazifier( v1 ) ) ); }


RiotBase_DEF_FUNC_1( bind_by_kn_object_id_b, 1, tv0->unknown_term_value(), const Category *cat ) { return Lazyfier::New( SymOp<Op_bind_by_kn_object_id>::New( Op_bind_by_kn_object_id{cat}, Op_bind_by_kn_object_id::category( v0 ), copy_without_lazifier( v0 ) ) ); }


RiotBase_DEF_FUNC_1( make_inode_if_z_b, 1, tv0->unknown_term_value(), const Category *cat ) { return Lazyfier::New( SymOp<Op_make_inode_if_z>::New( Op_make_inode_if_z{cat}, Op_make_inode_if_z::category( v0 ), copy_without_lazifier( v0 ) ) ); }
RiotBase_DEF_FUNC_1( make_inode_if_z_b, 0, FunctionTable_1<RF_self_make_inode_if_z>::find_best_func( tv0 ).second > 0, const Category *cat ) { Rp *res = RO_CALL( copy, v0 ); RO_CALL( self_make_inode_if_z, res, cat ); return res; }
RiotBase_DEF_FUNC_1( make_inode_if_z_o, 1, tv0->unknown_term_value(), const Category *cat ) { return Lazyfier::New( SymOp<Op_make_inode_if_z>::New( Op_make_inode_if_z{cat}, Op_make_inode_if_z::category( v0 ), ref_without_lazifier( v0 ) ) ); }
RiotBase_DEF_FUNC_1( make_inode_if_z_s, 1, tv0->unknown_term_value(), const Category *cat ) { return SymOp<Op_make_inode_if_z,true>::New( Op_make_inode_if_z{cat}, Op_make_inode_if_z::category( v0 ), ref_without_lazifier( v0 ) ); }


RiotBase_DEF_FUNC_1( special_func_b, 1, tv0->unknown_term_value(), const char *func, Rp **parm, int nb_parms ) { return Lazyfier::New( SymOp<Op_special_func>::New( Op_special_func{func, parm, nb_parms}, Op_special_func::category( v0 ), copy_without_lazifier( v0 ) ) ); }
RiotBase_DEF_FUNC_1( special_func_b, 0, FunctionTable_1<RF_self_special_func>::find_best_func( tv0 ).second > 0, const char *func, Rp **parm, int nb_parms ) { Rp *res = RO_CALL( copy, v0 ); RO_CALL( self_special_func, res, func, parm, nb_parms ); return res; }
RiotBase_DEF_FUNC_1( special_func_o, 1, tv0->unknown_term_value(), const char *func, Rp **parm, int nb_parms ) { return Lazyfier::New( SymOp<Op_special_func>::New( Op_special_func{func, parm, nb_parms}, Op_special_func::category( v0 ), ref_without_lazifier( v0 ) ) ); }
RiotBase_DEF_FUNC_1( special_func_s, 1, tv0->unknown_term_value(), const char *func, Rp **parm, int nb_parms ) { return SymOp<Op_special_func,true>::New( Op_special_func{func, parm, nb_parms}, Op_special_func::category( v0 ), ref_without_lazifier( v0 ) ); }

} // namespace

bool _Op_then_get_done( const Op_then &op ) { return op.done; }
void _Op_then_set_done( Op_then &op ) { op.done = true; }

} // namespace RiotBase
