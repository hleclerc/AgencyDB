
RiotBase_DEF_FUNC_2( add_bb, 0, FunctionTable_2<RF_self_add_b>::find_best_func( tv0, tv1 ).second > 0 ) { Rp *res = RO_CALL( copy, v0 ); RO_CALL( self_add_b, res, v1 ); return res; }
RiotBase_DEF_FUNC_2( add_bb, 1, tv0->unknown_term_value() or tv1->unknown_term_value() ) { return Lazyfier::New( SymOp<Op_add>::New( Op_add{}, Op_add::category( v0, v1 ), RO_CALL( copy, v0 ), RO_CALL( copy, v1 ) ) ); }
RiotBase_DEF_FUNC_2( add_ob, 1, tv0->unknown_term_value() or tv1->unknown_term_value() ) { return Lazyfier::New( SymOp<Op_add>::New( Op_add{}, Op_add::category( v0, v1 ), v0, RO_CALL( copy, v1 ) ) ); }
RiotBase_DEF_FUNC_2( add_sb, 1, tv0->unknown_term_value() or tv1->unknown_term_value() ) { return SymOp<Op_add,true>::New( Op_add{}, Op_add::category( v0, v1 ), v0, RO_CALL( copy, v1 ) ); }
        
RiotBase_DEF_FUNC_2( sub_bb, 0, FunctionTable_2<RF_self_sub_b>::find_best_func( tv0, tv1 ).second > 0 ) { Rp *res = RO_CALL( copy, v0 ); RO_CALL( self_sub_b, res, v1 ); return res; }
RiotBase_DEF_FUNC_2( sub_bb, 1, tv0->unknown_term_value() or tv1->unknown_term_value() ) { return Lazyfier::New( SymOp<Op_sub>::New( Op_sub{}, Op_sub::category( v0, v1 ), RO_CALL( copy, v0 ), RO_CALL( copy, v1 ) ) ); }
RiotBase_DEF_FUNC_2( sub_ob, 1, tv0->unknown_term_value() or tv1->unknown_term_value() ) { return Lazyfier::New( SymOp<Op_sub>::New( Op_sub{}, Op_sub::category( v0, v1 ), v0, RO_CALL( copy, v1 ) ) ); }
RiotBase_DEF_FUNC_2( sub_sb, 1, tv0->unknown_term_value() or tv1->unknown_term_value() ) { return SymOp<Op_sub,true>::New( Op_sub{}, Op_sub::category( v0, v1 ), v0, RO_CALL( copy, v1 ) ); }
        
RiotBase_DEF_FUNC_2( mul_bb, 0, FunctionTable_2<RF_self_mul_b>::find_best_func( tv0, tv1 ).second > 0 ) { Rp *res = RO_CALL( copy, v0 ); RO_CALL( self_mul_b, res, v1 ); return res; }
RiotBase_DEF_FUNC_2( mul_bb, 1, tv0->unknown_term_value() or tv1->unknown_term_value() ) { return Lazyfier::New( SymOp<Op_mul>::New( Op_mul{}, Op_mul::category( v0, v1 ), RO_CALL( copy, v0 ), RO_CALL( copy, v1 ) ) ); }
RiotBase_DEF_FUNC_2( mul_ob, 1, tv0->unknown_term_value() or tv1->unknown_term_value() ) { return Lazyfier::New( SymOp<Op_mul>::New( Op_mul{}, Op_mul::category( v0, v1 ), v0, RO_CALL( copy, v1 ) ) ); }
RiotBase_DEF_FUNC_2( mul_sb, 1, tv0->unknown_term_value() or tv1->unknown_term_value() ) { return SymOp<Op_mul,true>::New( Op_mul{}, Op_mul::category( v0, v1 ), v0, RO_CALL( copy, v1 ) ); }
        
RiotBase_DEF_FUNC_2( div_bb, 0, FunctionTable_2<RF_self_div_b>::find_best_func( tv0, tv1 ).second > 0 ) { Rp *res = RO_CALL( copy, v0 ); RO_CALL( self_div_b, res, v1 ); return res; }
RiotBase_DEF_FUNC_2( div_bb, 1, tv0->unknown_term_value() or tv1->unknown_term_value() ) { return Lazyfier::New( SymOp<Op_div>::New( Op_div{}, Op_div::category( v0, v1 ), RO_CALL( copy, v0 ), RO_CALL( copy, v1 ) ) ); }
RiotBase_DEF_FUNC_2( div_ob, 1, tv0->unknown_term_value() or tv1->unknown_term_value() ) { return Lazyfier::New( SymOp<Op_div>::New( Op_div{}, Op_div::category( v0, v1 ), v0, RO_CALL( copy, v1 ) ) ); }
RiotBase_DEF_FUNC_2( div_sb, 1, tv0->unknown_term_value() or tv1->unknown_term_value() ) { return SymOp<Op_div,true>::New( Op_div{}, Op_div::category( v0, v1 ), v0, RO_CALL( copy, v1 ) ); }
        
RiotBase_DEF_FUNC_2( mod_bb, 0, FunctionTable_2<RF_self_mod_b>::find_best_func( tv0, tv1 ).second > 0 ) { Rp *res = RO_CALL( copy, v0 ); RO_CALL( self_mod_b, res, v1 ); return res; }
RiotBase_DEF_FUNC_2( mod_bb, 1, tv0->unknown_term_value() or tv1->unknown_term_value() ) { return Lazyfier::New( SymOp<Op_mod>::New( Op_mod{}, Op_mod::category( v0, v1 ), RO_CALL( copy, v0 ), RO_CALL( copy, v1 ) ) ); }
RiotBase_DEF_FUNC_2( mod_ob, 1, tv0->unknown_term_value() or tv1->unknown_term_value() ) { return Lazyfier::New( SymOp<Op_mod>::New( Op_mod{}, Op_mod::category( v0, v1 ), v0, RO_CALL( copy, v1 ) ) ); }
RiotBase_DEF_FUNC_2( mod_sb, 1, tv0->unknown_term_value() or tv1->unknown_term_value() ) { return SymOp<Op_mod,true>::New( Op_mod{}, Op_mod::category( v0, v1 ), v0, RO_CALL( copy, v1 ) ); }
        
RiotBase_DEF_FUNC_2( add_key_bb, 0, FunctionTable_2<RF_self_add_key_b>::find_best_func( tv0, tv1 ).second > 0 ) { Rp *res = RO_CALL( copy, v0 ); RO_CALL( self_add_key_b, res, v1 ); return res; }
RiotBase_DEF_FUNC_2( add_key_bb, 1, tv0->unknown_term_value() or tv1->unknown_term_value() ) { return Lazyfier::New( SymOp<Op_add_key>::New( Op_add_key{}, Op_add_key::category( v0, v1 ), RO_CALL( copy, v0 ), RO_CALL( copy, v1 ) ) ); }
RiotBase_DEF_FUNC_2( add_key_ob, 1, tv0->unknown_term_value() or tv1->unknown_term_value() ) { return Lazyfier::New( SymOp<Op_add_key>::New( Op_add_key{}, Op_add_key::category( v0, v1 ), v0, RO_CALL( copy, v1 ) ) ); }
RiotBase_DEF_FUNC_2( add_key_sb, 1, tv0->unknown_term_value() or tv1->unknown_term_value() ) { return SymOp<Op_add_key,true>::New( Op_add_key{}, Op_add_key::category( v0, v1 ), v0, RO_CALL( copy, v1 ) ); }
        
RiotBase_DEF_FUNC_2( push_back_bb, 0, FunctionTable_2<RF_self_push_back_b>::find_best_func( tv0, tv1 ).second > 0 ) { Rp *res = RO_CALL( copy, v0 ); RO_CALL( self_push_back_b, res, v1 ); return res; }
RiotBase_DEF_FUNC_2( push_back_bb, 1, tv0->unknown_term_value() or tv1->unknown_term_value() ) { return Lazyfier::New( SymOp<Op_push_back>::New( Op_push_back{}, Op_push_back::category( v0, v1 ), RO_CALL( copy, v0 ), RO_CALL( copy, v1 ) ) ); }
RiotBase_DEF_FUNC_2( push_back_ob, 1, tv0->unknown_term_value() or tv1->unknown_term_value() ) { return Lazyfier::New( SymOp<Op_push_back>::New( Op_push_back{}, Op_push_back::category( v0, v1 ), v0, RO_CALL( copy, v1 ) ) ); }
RiotBase_DEF_FUNC_2( push_back_sb, 1, tv0->unknown_term_value() or tv1->unknown_term_value() ) { return SymOp<Op_push_back,true>::New( Op_push_back{}, Op_push_back::category( v0, v1 ), v0, RO_CALL( copy, v1 ) ); }
        
RiotBase_DEF_FUNC_3( insert_bbb, 0, FunctionTable_3<RF_self_insert_bb>::find_best_func( tv0, tv1, tv2 ).second > 0 ) { Rp *res = RO_CALL( copy, v0 ); RO_CALL( self_insert_bb, res, v1 , v2 ); return res; }
RiotBase_DEF_FUNC_3( insert_bbb, 1, tv0->unknown_term_value() or tv1->unknown_term_value() or tv2->unknown_term_value() ) { return Lazyfier::New( SymOp<Op_insert>::New( Op_insert{}, Op_insert::category( v0, v1, v2 ), RO_CALL( copy, v0 ), RO_CALL( copy, v1 ), RO_CALL( copy, v2 ) ) ); }
RiotBase_DEF_FUNC_3( insert_obb, 1, tv0->unknown_term_value() or tv1->unknown_term_value() or tv2->unknown_term_value() ) { return Lazyfier::New( SymOp<Op_insert>::New( Op_insert{}, Op_insert::category( v0, v1, v2 ), v0, RO_CALL( copy, v1 ), RO_CALL( copy, v2 ) ) ); }
RiotBase_DEF_FUNC_3( insert_sbb, 1, tv0->unknown_term_value() or tv1->unknown_term_value() or tv2->unknown_term_value() ) { return SymOp<Op_insert,true>::New( Op_insert{}, Op_insert::category( v0, v1, v2 ), v0, RO_CALL( copy, v1 ), RO_CALL( copy, v2 ) ); }
        
RiotBase_DEF_FUNC_3( remove_bbb, 0, FunctionTable_3<RF_self_remove_bb>::find_best_func( tv0, tv1, tv2 ).second > 0 ) { Rp *res = RO_CALL( copy, v0 ); RO_CALL( self_remove_bb, res, v1 , v2 ); return res; }
RiotBase_DEF_FUNC_3( remove_bbb, 1, tv0->unknown_term_value() or tv1->unknown_term_value() or tv2->unknown_term_value() ) { return Lazyfier::New( SymOp<Op_remove>::New( Op_remove{}, Op_remove::category( v0, v1, v2 ), RO_CALL( copy, v0 ), RO_CALL( copy, v1 ), RO_CALL( copy, v2 ) ) ); }
RiotBase_DEF_FUNC_3( remove_obb, 1, tv0->unknown_term_value() or tv1->unknown_term_value() or tv2->unknown_term_value() ) { return Lazyfier::New( SymOp<Op_remove>::New( Op_remove{}, Op_remove::category( v0, v1, v2 ), v0, RO_CALL( copy, v1 ), RO_CALL( copy, v2 ) ) ); }
RiotBase_DEF_FUNC_3( remove_sbb, 1, tv0->unknown_term_value() or tv1->unknown_term_value() or tv2->unknown_term_value() ) { return SymOp<Op_remove,true>::New( Op_remove{}, Op_remove::category( v0, v1, v2 ), v0, RO_CALL( copy, v1 ), RO_CALL( copy, v2 ) ); }
        
RiotBase_DEF_FUNC_2( equ_bb, 1, tv0->unknown_term_value() or tv1->unknown_term_value() ) { return Lazyfier::New( SymOp<Op_equ>::New( Op_equ{}, Op_equ::category( v0, v1 ), RO_CALL( copy, v0 ), RO_CALL( copy, v1 ) ) ); }
        
RiotBase_DEF_FUNC_2( neq_bb, 1, tv0->unknown_term_value() or tv1->unknown_term_value() ) { return Lazyfier::New( SymOp<Op_neq>::New( Op_neq{}, Op_neq::category( v0, v1 ), RO_CALL( copy, v0 ), RO_CALL( copy, v1 ) ) ); }
        
RiotBase_DEF_FUNC_2( bor_bb, 1, tv0->unknown_term_value() or tv1->unknown_term_value() ) { return Lazyfier::New( SymOp<Op_bor>::New( Op_bor{}, Op_bor::category( v0, v1 ), RO_CALL( copy, v0 ), RO_CALL( copy, v1 ) ) ); }
        
RiotBase_DEF_FUNC_2( select_bb, 1, tv0->unknown_term_value() or tv1->unknown_term_value() ) { return Lazyfier::New( SymOp<Op_select>::New( Op_select{}, Op_select::category( v0, v1 ), RO_CALL( copy, v0 ), RO_CALL( copy, v1 ) ) ); }
        
RiotBase_DEF_FUNC_1( bind_by_kn_object_id_b, 1, tv0->unknown_term_value(), Db *db, const Category *cat ) { return Lazyfier::New( SymOp<Op_bind_by_kn_object_id>::New( Op_bind_by_kn_object_id{db, cat}, Op_bind_by_kn_object_id::category( v0 ), RO_CALL( copy, v0 ) ) ); }
        
RiotBase_DEF_FUNC_1( make_inode_if_z_b, 0, FunctionTable_1<RF_self_make_inode_if_z>::find_best_func( tv0 ).second > 0, Db *db, const Category *cat ) { Rp *res = RO_CALL( copy, v0 ); RO_CALL( self_make_inode_if_z, res, db, cat ); return res; }
RiotBase_DEF_FUNC_1( make_inode_if_z_b, 1, tv0->unknown_term_value(), Db *db, const Category *cat ) { return Lazyfier::New( SymOp<Op_make_inode_if_z>::New( Op_make_inode_if_z{db, cat}, Op_make_inode_if_z::category( v0 ), RO_CALL( copy, v0 ) ) ); }
RiotBase_DEF_FUNC_1( make_inode_if_z_o, 1, tv0->unknown_term_value(), Db *db, const Category *cat ) { return Lazyfier::New( SymOp<Op_make_inode_if_z>::New( Op_make_inode_if_z{db, cat}, Op_make_inode_if_z::category( v0 ), v0 ) ); }
RiotBase_DEF_FUNC_1( make_inode_if_z_s, 1, tv0->unknown_term_value(), Db *db, const Category *cat ) { return SymOp<Op_make_inode_if_z,true>::New( Op_make_inode_if_z{db, cat}, Op_make_inode_if_z::category( v0 ), v0 ); }
        