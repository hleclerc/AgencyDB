/// This file is generated (by bin/Method_template.py). Do not modify it directly.
RiotBase_DEF_FUNC_2( self_set_b, 1, ( tv0->unknown_value() or tv1->unknown_value() ) and tv1->term_value() ) {
    v0 = Lazyfier::New( RO_CALL( set_sb, v0, v1 ) );
}
RiotBase_DEF_FUNC_2( self_set_b, 2, tv0->isa<Lazyfier>() and tv1->term_value() ) {
    Lazyfier *l = scast( v0 );
    if ( CommonSymbolicData *csd = RO_CALL( common_symbolic_data, l->cur ) )
        csd->lazyfier_parents.remove_first_unordered( l );
    l->cur = RO_CALL( set_sb, l->cur, v1 );
    if ( CommonSymbolicData *csd = RO_CALL( common_symbolic_data, l->cur ) )
        csd->lazyfier_parents << l;
}
RiotBase_DEF_FUNC_2( self_add_b, 1, ( tv0->unknown_value() or tv1->unknown_value() ) and tv1->term_value() ) {
    v0 = Lazyfier::New( RO_CALL( add_sb, v0, v1 ) );
}
RiotBase_DEF_FUNC_2( self_add_b, 2, tv0->isa<Lazyfier>() and tv1->term_value() ) {
    Lazyfier *l = scast( v0 );
    if ( CommonSymbolicData *csd = RO_CALL( common_symbolic_data, l->cur ) )
        csd->lazyfier_parents.remove_first_unordered( l );
    l->cur = RO_CALL( add_sb, l->cur, v1 );
    if ( CommonSymbolicData *csd = RO_CALL( common_symbolic_data, l->cur ) )
        csd->lazyfier_parents << l;
}
RiotBase_DEF_FUNC_2( self_sub_b, 1, ( tv0->unknown_value() or tv1->unknown_value() ) and tv1->term_value() ) {
    v0 = Lazyfier::New( RO_CALL( sub_sb, v0, v1 ) );
}
RiotBase_DEF_FUNC_2( self_sub_b, 2, tv0->isa<Lazyfier>() and tv1->term_value() ) {
    Lazyfier *l = scast( v0 );
    if ( CommonSymbolicData *csd = RO_CALL( common_symbolic_data, l->cur ) )
        csd->lazyfier_parents.remove_first_unordered( l );
    l->cur = RO_CALL( sub_sb, l->cur, v1 );
    if ( CommonSymbolicData *csd = RO_CALL( common_symbolic_data, l->cur ) )
        csd->lazyfier_parents << l;
}
RiotBase_DEF_FUNC_2( self_mul_b, 1, ( tv0->unknown_value() or tv1->unknown_value() ) and tv1->term_value() ) {
    v0 = Lazyfier::New( RO_CALL( mul_sb, v0, v1 ) );
}
RiotBase_DEF_FUNC_2( self_mul_b, 2, tv0->isa<Lazyfier>() and tv1->term_value() ) {
    Lazyfier *l = scast( v0 );
    if ( CommonSymbolicData *csd = RO_CALL( common_symbolic_data, l->cur ) )
        csd->lazyfier_parents.remove_first_unordered( l );
    l->cur = RO_CALL( mul_sb, l->cur, v1 );
    if ( CommonSymbolicData *csd = RO_CALL( common_symbolic_data, l->cur ) )
        csd->lazyfier_parents << l;
}
RiotBase_DEF_FUNC_2( self_div_b, 1, ( tv0->unknown_value() or tv1->unknown_value() ) and tv1->term_value() ) {
    v0 = Lazyfier::New( RO_CALL( div_sb, v0, v1 ) );
}
RiotBase_DEF_FUNC_2( self_div_b, 2, tv0->isa<Lazyfier>() and tv1->term_value() ) {
    Lazyfier *l = scast( v0 );
    if ( CommonSymbolicData *csd = RO_CALL( common_symbolic_data, l->cur ) )
        csd->lazyfier_parents.remove_first_unordered( l );
    l->cur = RO_CALL( div_sb, l->cur, v1 );
    if ( CommonSymbolicData *csd = RO_CALL( common_symbolic_data, l->cur ) )
        csd->lazyfier_parents << l;
}
RiotBase_DEF_FUNC_2( self_mod_b, 1, ( tv0->unknown_value() or tv1->unknown_value() ) and tv1->term_value() ) {
    v0 = Lazyfier::New( RO_CALL( mod_sb, v0, v1 ) );
}
RiotBase_DEF_FUNC_2( self_mod_b, 2, tv0->isa<Lazyfier>() and tv1->term_value() ) {
    Lazyfier *l = scast( v0 );
    if ( CommonSymbolicData *csd = RO_CALL( common_symbolic_data, l->cur ) )
        csd->lazyfier_parents.remove_first_unordered( l );
    l->cur = RO_CALL( mod_sb, l->cur, v1 );
    if ( CommonSymbolicData *csd = RO_CALL( common_symbolic_data, l->cur ) )
        csd->lazyfier_parents << l;
}
RiotBase_DEF_FUNC_2( self_add_key_b, 1, ( tv0->unknown_value() or tv1->unknown_value() ) and tv1->term_value() ) {
    v0 = Lazyfier::New( RO_CALL( add_key_sb, v0, v1 ) );
}
RiotBase_DEF_FUNC_2( self_add_key_b, 2, tv0->isa<Lazyfier>() and tv1->term_value() ) {
    Lazyfier *l = scast( v0 );
    if ( CommonSymbolicData *csd = RO_CALL( common_symbolic_data, l->cur ) )
        csd->lazyfier_parents.remove_first_unordered( l );
    l->cur = RO_CALL( add_key_sb, l->cur, v1 );
    if ( CommonSymbolicData *csd = RO_CALL( common_symbolic_data, l->cur ) )
        csd->lazyfier_parents << l;
}
RiotBase_DEF_FUNC_2( self_rem_key_b, 1, ( tv0->unknown_value() or tv1->unknown_value() ) and tv1->term_value() ) {
    v0 = Lazyfier::New( RO_CALL( rem_key_sb, v0, v1 ) );
}
RiotBase_DEF_FUNC_2( self_rem_key_b, 2, tv0->isa<Lazyfier>() and tv1->term_value() ) {
    Lazyfier *l = scast( v0 );
    if ( CommonSymbolicData *csd = RO_CALL( common_symbolic_data, l->cur ) )
        csd->lazyfier_parents.remove_first_unordered( l );
    l->cur = RO_CALL( rem_key_sb, l->cur, v1 );
    if ( CommonSymbolicData *csd = RO_CALL( common_symbolic_data, l->cur ) )
        csd->lazyfier_parents << l;
}
RiotBase_DEF_FUNC_2( self_push_back_b, 1, ( tv0->unknown_value() or tv1->unknown_value() ) and tv1->term_value() ) {
    v0 = Lazyfier::New( RO_CALL( push_back_sb, v0, v1 ) );
}
RiotBase_DEF_FUNC_2( self_push_back_b, 2, tv0->isa<Lazyfier>() and tv1->term_value() ) {
    Lazyfier *l = scast( v0 );
    if ( CommonSymbolicData *csd = RO_CALL( common_symbolic_data, l->cur ) )
        csd->lazyfier_parents.remove_first_unordered( l );
    l->cur = RO_CALL( push_back_sb, l->cur, v1 );
    if ( CommonSymbolicData *csd = RO_CALL( common_symbolic_data, l->cur ) )
        csd->lazyfier_parents << l;
}
RiotBase_DEF_FUNC_3( self_insert_bb, 1, ( tv0->unknown_value() or tv1->unknown_value() or tv2->unknown_value() ) and tv1->term_value() and tv2->term_value() ) {
    v0 = Lazyfier::New( RO_CALL( insert_sbb, v0, v1, v2 ) );
}
RiotBase_DEF_FUNC_3( self_insert_bb, 2, tv0->isa<Lazyfier>() and tv1->term_value() and tv2->term_value() ) {
    Lazyfier *l = scast( v0 );
    if ( CommonSymbolicData *csd = RO_CALL( common_symbolic_data, l->cur ) )
        csd->lazyfier_parents.remove_first_unordered( l );
    l->cur = RO_CALL( insert_sbb, l->cur, v1, v2 );
    if ( CommonSymbolicData *csd = RO_CALL( common_symbolic_data, l->cur ) )
        csd->lazyfier_parents << l;
}
RiotBase_DEF_FUNC_3( self_remove_bb, 1, ( tv0->unknown_value() or tv1->unknown_value() or tv2->unknown_value() ) and tv1->term_value() and tv2->term_value() ) {
    v0 = Lazyfier::New( RO_CALL( remove_sbb, v0, v1, v2 ) );
}
RiotBase_DEF_FUNC_3( self_remove_bb, 2, tv0->isa<Lazyfier>() and tv1->term_value() and tv2->term_value() ) {
    Lazyfier *l = scast( v0 );
    if ( CommonSymbolicData *csd = RO_CALL( common_symbolic_data, l->cur ) )
        csd->lazyfier_parents.remove_first_unordered( l );
    l->cur = RO_CALL( remove_sbb, l->cur, v1, v2 );
    if ( CommonSymbolicData *csd = RO_CALL( common_symbolic_data, l->cur ) )
        csd->lazyfier_parents << l;
}
RiotBase_DEF_FUNC_1( self_on_change, 1, ( tv0->unknown_value() ), Callback *c ) {
    v0 = Lazyfier::New( RO_CALL( on_change_s, v0, c ) );
}
RiotBase_DEF_FUNC_1( self_on_change, 2, tv0->isa<Lazyfier>(), Callback *c ) {
    Lazyfier *l = scast( v0 );
    if ( CommonSymbolicData *csd = RO_CALL( common_symbolic_data, l->cur ) )
        csd->lazyfier_parents.remove_first_unordered( l );
    l->cur = RO_CALL( on_change_s, l->cur, c );
    if ( CommonSymbolicData *csd = RO_CALL( common_symbolic_data, l->cur ) )
        csd->lazyfier_parents << l;
}
RiotBase_DEF_FUNC_1( self_then, 1, ( tv0->unknown_value() ), Callback *c ) {
    v0 = Lazyfier::New( RO_CALL( then_s, v0, c ) );
}
RiotBase_DEF_FUNC_1( self_then, 2, tv0->isa<Lazyfier>(), Callback *c ) {
    Lazyfier *l = scast( v0 );
    if ( CommonSymbolicData *csd = RO_CALL( common_symbolic_data, l->cur ) )
        csd->lazyfier_parents.remove_first_unordered( l );
    l->cur = RO_CALL( then_s, l->cur, c );
    if ( CommonSymbolicData *csd = RO_CALL( common_symbolic_data, l->cur ) )
        csd->lazyfier_parents << l;
}
RiotBase_DEF_FUNC_1( self_make_inode_if_z, 1, ( tv0->unknown_value() ), const Category *cat ) {
    v0 = Lazyfier::New( RO_CALL( make_inode_if_z_s, v0, cat ) );
}
RiotBase_DEF_FUNC_1( self_make_inode_if_z, 2, tv0->isa<Lazyfier>(), const Category *cat ) {
    Lazyfier *l = scast( v0 );
    if ( CommonSymbolicData *csd = RO_CALL( common_symbolic_data, l->cur ) )
        csd->lazyfier_parents.remove_first_unordered( l );
    l->cur = RO_CALL( make_inode_if_z_s, l->cur, cat );
    if ( CommonSymbolicData *csd = RO_CALL( common_symbolic_data, l->cur ) )
        csd->lazyfier_parents << l;
}
RiotBase_DEF_FUNC_1( self_special_func, 1, ( tv0->unknown_value() ), const char *func, Rp **parm, int nb_parms ) {
    v0 = Lazyfier::New( RO_CALL( special_func_s, v0, func, parm, nb_parms ) );
}
RiotBase_DEF_FUNC_1( self_special_func, 2, tv0->isa<Lazyfier>(), const char *func, Rp **parm, int nb_parms ) {
    Lazyfier *l = scast( v0 );
    if ( CommonSymbolicData *csd = RO_CALL( common_symbolic_data, l->cur ) )
        csd->lazyfier_parents.remove_first_unordered( l );
    l->cur = RO_CALL( special_func_s, l->cur, func, parm, nb_parms );
    if ( CommonSymbolicData *csd = RO_CALL( common_symbolic_data, l->cur ) )
        csd->lazyfier_parents << l;
}
