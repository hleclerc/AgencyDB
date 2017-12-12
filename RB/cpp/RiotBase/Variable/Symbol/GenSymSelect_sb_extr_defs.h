/// This file is generated (by bin/Method_template.py). Do not modify it directly.
static void check_at_least_a_sym_child( const Vec<Rp *> &args, const char *method_name ) {
    for( Rp *a : args )
        if ( RO_CALL( is_symbolic, a ) )
            return;
    WARNING( "%s is not defined for known values", method_name );
}

RiotBase_DEF_FUNC_2( self_set_b, 2, tv0->isa<GenSymSelect>() and tv1->term_value() ) {
    GenSymSelect *s = scast( v0 );
    Lazyfier     *l = s->container;
    if ( CommonSymbolicData *csd = RO_CALL( common_symbolic_data, l->cur ) )
        csd->lazyfier_parents.remove_first_unordered( l );
    l->cur = RO_CALL( set_sb_extr, l->cur, v1, s->keys );
    if ( CommonSymbolicData *csd = RO_CALL( common_symbolic_data, l->cur ) )
        csd->lazyfier_parents << l;
}
RiotBase_DEF_FUNC_2( set_sb_extr, 1, true, const Vec<Rp *> &keys ) {
    Vec<Rp *> args; args << ref_without_lazifier( v0 ) << copy_without_lazifier( v1 ); for( Rp *k : keys ) args << copy_without_lazifier( k );
    check_at_least_a_sym_child( args, "set_sb_extr" );
    return SymOpExtr<Op_set,true>::New( Op_set{  }, Op_set::category( v0, v1 ), args.size(), args.ptr() );
}
RiotBase_DEF_FUNC_2( self_add_b, 2, tv0->isa<GenSymSelect>() and tv1->term_value() ) {
    GenSymSelect *s = scast( v0 );
    Lazyfier     *l = s->container;
    if ( CommonSymbolicData *csd = RO_CALL( common_symbolic_data, l->cur ) )
        csd->lazyfier_parents.remove_first_unordered( l );
    l->cur = RO_CALL( add_sb_extr, l->cur, v1, s->keys );
    if ( CommonSymbolicData *csd = RO_CALL( common_symbolic_data, l->cur ) )
        csd->lazyfier_parents << l;
}
RiotBase_DEF_FUNC_2( add_sb_extr, 1, true, const Vec<Rp *> &keys ) {
    Vec<Rp *> args; args << ref_without_lazifier( v0 ) << copy_without_lazifier( v1 ); for( Rp *k : keys ) args << copy_without_lazifier( k );
    check_at_least_a_sym_child( args, "add_sb_extr" );
    return SymOpExtr<Op_add,true>::New( Op_add{  }, Op_add::category( v0, v1 ), args.size(), args.ptr() );
}
RiotBase_DEF_FUNC_2( self_sub_b, 2, tv0->isa<GenSymSelect>() and tv1->term_value() ) {
    GenSymSelect *s = scast( v0 );
    Lazyfier     *l = s->container;
    if ( CommonSymbolicData *csd = RO_CALL( common_symbolic_data, l->cur ) )
        csd->lazyfier_parents.remove_first_unordered( l );
    l->cur = RO_CALL( sub_sb_extr, l->cur, v1, s->keys );
    if ( CommonSymbolicData *csd = RO_CALL( common_symbolic_data, l->cur ) )
        csd->lazyfier_parents << l;
}
RiotBase_DEF_FUNC_2( sub_sb_extr, 1, true, const Vec<Rp *> &keys ) {
    Vec<Rp *> args; args << ref_without_lazifier( v0 ) << copy_without_lazifier( v1 ); for( Rp *k : keys ) args << copy_without_lazifier( k );
    check_at_least_a_sym_child( args, "sub_sb_extr" );
    return SymOpExtr<Op_sub,true>::New( Op_sub{  }, Op_sub::category( v0, v1 ), args.size(), args.ptr() );
}
RiotBase_DEF_FUNC_2( self_mul_b, 2, tv0->isa<GenSymSelect>() and tv1->term_value() ) {
    GenSymSelect *s = scast( v0 );
    Lazyfier     *l = s->container;
    if ( CommonSymbolicData *csd = RO_CALL( common_symbolic_data, l->cur ) )
        csd->lazyfier_parents.remove_first_unordered( l );
    l->cur = RO_CALL( mul_sb_extr, l->cur, v1, s->keys );
    if ( CommonSymbolicData *csd = RO_CALL( common_symbolic_data, l->cur ) )
        csd->lazyfier_parents << l;
}
RiotBase_DEF_FUNC_2( mul_sb_extr, 1, true, const Vec<Rp *> &keys ) {
    Vec<Rp *> args; args << ref_without_lazifier( v0 ) << copy_without_lazifier( v1 ); for( Rp *k : keys ) args << copy_without_lazifier( k );
    check_at_least_a_sym_child( args, "mul_sb_extr" );
    return SymOpExtr<Op_mul,true>::New( Op_mul{  }, Op_mul::category( v0, v1 ), args.size(), args.ptr() );
}
RiotBase_DEF_FUNC_2( self_div_b, 2, tv0->isa<GenSymSelect>() and tv1->term_value() ) {
    GenSymSelect *s = scast( v0 );
    Lazyfier     *l = s->container;
    if ( CommonSymbolicData *csd = RO_CALL( common_symbolic_data, l->cur ) )
        csd->lazyfier_parents.remove_first_unordered( l );
    l->cur = RO_CALL( div_sb_extr, l->cur, v1, s->keys );
    if ( CommonSymbolicData *csd = RO_CALL( common_symbolic_data, l->cur ) )
        csd->lazyfier_parents << l;
}
RiotBase_DEF_FUNC_2( div_sb_extr, 1, true, const Vec<Rp *> &keys ) {
    Vec<Rp *> args; args << ref_without_lazifier( v0 ) << copy_without_lazifier( v1 ); for( Rp *k : keys ) args << copy_without_lazifier( k );
    check_at_least_a_sym_child( args, "div_sb_extr" );
    return SymOpExtr<Op_div,true>::New( Op_div{  }, Op_div::category( v0, v1 ), args.size(), args.ptr() );
}
RiotBase_DEF_FUNC_2( self_mod_b, 2, tv0->isa<GenSymSelect>() and tv1->term_value() ) {
    GenSymSelect *s = scast( v0 );
    Lazyfier     *l = s->container;
    if ( CommonSymbolicData *csd = RO_CALL( common_symbolic_data, l->cur ) )
        csd->lazyfier_parents.remove_first_unordered( l );
    l->cur = RO_CALL( mod_sb_extr, l->cur, v1, s->keys );
    if ( CommonSymbolicData *csd = RO_CALL( common_symbolic_data, l->cur ) )
        csd->lazyfier_parents << l;
}
RiotBase_DEF_FUNC_2( mod_sb_extr, 1, true, const Vec<Rp *> &keys ) {
    Vec<Rp *> args; args << ref_without_lazifier( v0 ) << copy_without_lazifier( v1 ); for( Rp *k : keys ) args << copy_without_lazifier( k );
    check_at_least_a_sym_child( args, "mod_sb_extr" );
    return SymOpExtr<Op_mod,true>::New( Op_mod{  }, Op_mod::category( v0, v1 ), args.size(), args.ptr() );
}
RiotBase_DEF_FUNC_2( self_add_key_b, 2, tv0->isa<GenSymSelect>() and tv1->term_value() ) {
    GenSymSelect *s = scast( v0 );
    Lazyfier     *l = s->container;
    if ( CommonSymbolicData *csd = RO_CALL( common_symbolic_data, l->cur ) )
        csd->lazyfier_parents.remove_first_unordered( l );
    l->cur = RO_CALL( add_key_sb_extr, l->cur, v1, s->keys );
    if ( CommonSymbolicData *csd = RO_CALL( common_symbolic_data, l->cur ) )
        csd->lazyfier_parents << l;
}
RiotBase_DEF_FUNC_2( add_key_sb_extr, 1, true, const Vec<Rp *> &keys ) {
    Vec<Rp *> args; args << ref_without_lazifier( v0 ) << copy_without_lazifier( v1 ); for( Rp *k : keys ) args << copy_without_lazifier( k );
    check_at_least_a_sym_child( args, "add_key_sb_extr" );
    return SymOpExtr<Op_add_key,true>::New( Op_add_key{  }, Op_add_key::category( v0, v1 ), args.size(), args.ptr() );
}
RiotBase_DEF_FUNC_2( self_rem_key_b, 2, tv0->isa<GenSymSelect>() and tv1->term_value() ) {
    GenSymSelect *s = scast( v0 );
    Lazyfier     *l = s->container;
    if ( CommonSymbolicData *csd = RO_CALL( common_symbolic_data, l->cur ) )
        csd->lazyfier_parents.remove_first_unordered( l );
    l->cur = RO_CALL( rem_key_sb_extr, l->cur, v1, s->keys );
    if ( CommonSymbolicData *csd = RO_CALL( common_symbolic_data, l->cur ) )
        csd->lazyfier_parents << l;
}
RiotBase_DEF_FUNC_2( rem_key_sb_extr, 1, true, const Vec<Rp *> &keys ) {
    Vec<Rp *> args; args << ref_without_lazifier( v0 ) << copy_without_lazifier( v1 ); for( Rp *k : keys ) args << copy_without_lazifier( k );
    check_at_least_a_sym_child( args, "rem_key_sb_extr" );
    return SymOpExtr<Op_rem_key,true>::New( Op_rem_key{  }, Op_rem_key::category( v0, v1 ), args.size(), args.ptr() );
}
RiotBase_DEF_FUNC_2( self_push_back_b, 2, tv0->isa<GenSymSelect>() and tv1->term_value() ) {
    GenSymSelect *s = scast( v0 );
    Lazyfier     *l = s->container;
    if ( CommonSymbolicData *csd = RO_CALL( common_symbolic_data, l->cur ) )
        csd->lazyfier_parents.remove_first_unordered( l );
    l->cur = RO_CALL( push_back_sb_extr, l->cur, v1, s->keys );
    if ( CommonSymbolicData *csd = RO_CALL( common_symbolic_data, l->cur ) )
        csd->lazyfier_parents << l;
}
RiotBase_DEF_FUNC_2( push_back_sb_extr, 1, true, const Vec<Rp *> &keys ) {
    Vec<Rp *> args; args << ref_without_lazifier( v0 ) << copy_without_lazifier( v1 ); for( Rp *k : keys ) args << copy_without_lazifier( k );
    check_at_least_a_sym_child( args, "push_back_sb_extr" );
    return SymOpExtr<Op_push_back,true>::New( Op_push_back{  }, Op_push_back::category( v0, v1 ), args.size(), args.ptr() );
}
RiotBase_DEF_FUNC_3( self_insert_bb, 2, tv0->isa<GenSymSelect>() and tv1->term_value() and tv2->term_value() ) {
    GenSymSelect *s = scast( v0 );
    Lazyfier     *l = s->container;
    if ( CommonSymbolicData *csd = RO_CALL( common_symbolic_data, l->cur ) )
        csd->lazyfier_parents.remove_first_unordered( l );
    l->cur = RO_CALL( insert_sbb_extr, l->cur, v1, v2, s->keys );
    if ( CommonSymbolicData *csd = RO_CALL( common_symbolic_data, l->cur ) )
        csd->lazyfier_parents << l;
}
RiotBase_DEF_FUNC_3( insert_sbb_extr, 1, true, const Vec<Rp *> &keys ) {
    Vec<Rp *> args; args << ref_without_lazifier( v0 ) << copy_without_lazifier( v1 ) << copy_without_lazifier( v2 ); for( Rp *k : keys ) args << copy_without_lazifier( k );
    check_at_least_a_sym_child( args, "insert_sbb_extr" );
    return SymOpExtr<Op_insert,true>::New( Op_insert{  }, Op_insert::category( v0, v1, v2 ), args.size(), args.ptr() );
}
RiotBase_DEF_FUNC_3( self_remove_bb, 2, tv0->isa<GenSymSelect>() and tv1->term_value() and tv2->term_value() ) {
    GenSymSelect *s = scast( v0 );
    Lazyfier     *l = s->container;
    if ( CommonSymbolicData *csd = RO_CALL( common_symbolic_data, l->cur ) )
        csd->lazyfier_parents.remove_first_unordered( l );
    l->cur = RO_CALL( remove_sbb_extr, l->cur, v1, v2, s->keys );
    if ( CommonSymbolicData *csd = RO_CALL( common_symbolic_data, l->cur ) )
        csd->lazyfier_parents << l;
}
RiotBase_DEF_FUNC_3( remove_sbb_extr, 1, true, const Vec<Rp *> &keys ) {
    Vec<Rp *> args; args << ref_without_lazifier( v0 ) << copy_without_lazifier( v1 ) << copy_without_lazifier( v2 ); for( Rp *k : keys ) args << copy_without_lazifier( k );
    check_at_least_a_sym_child( args, "remove_sbb_extr" );
    return SymOpExtr<Op_remove,true>::New( Op_remove{  }, Op_remove::category( v0, v1, v2 ), args.size(), args.ptr() );
}
RiotBase_DEF_FUNC_1( self_on_change, 2, tv0->isa<GenSymSelect>(), Callback *c ) {
    GenSymSelect *s = scast( v0 );
    Lazyfier     *l = s->container;
    if ( CommonSymbolicData *csd = RO_CALL( common_symbolic_data, l->cur ) )
        csd->lazyfier_parents.remove_first_unordered( l );
    l->cur = RO_CALL( on_change_s_extr, l->cur, c, s->keys );
    if ( CommonSymbolicData *csd = RO_CALL( common_symbolic_data, l->cur ) )
        csd->lazyfier_parents << l;
}
RiotBase_DEF_FUNC_1( on_change_s_extr, 1, true, Callback *c, const Vec<Rp *> &keys ) {
    Vec<Rp *> args; args << ref_without_lazifier( v0 ); for( Rp *k : keys ) args << copy_without_lazifier( k );
    check_at_least_a_sym_child( args, "on_change_s_extr" );
    return SymOpExtr<Op_on_change,true>::New( Op_on_change{ c }, Op_on_change::category( v0 ), args.size(), args.ptr() );
}
RiotBase_DEF_FUNC_1( self_then, 2, tv0->isa<GenSymSelect>(), Callback *c ) {
    GenSymSelect *s = scast( v0 );
    Lazyfier     *l = s->container;
    if ( CommonSymbolicData *csd = RO_CALL( common_symbolic_data, l->cur ) )
        csd->lazyfier_parents.remove_first_unordered( l );
    l->cur = RO_CALL( then_s_extr, l->cur, c, s->keys );
    if ( CommonSymbolicData *csd = RO_CALL( common_symbolic_data, l->cur ) )
        csd->lazyfier_parents << l;
}
RiotBase_DEF_FUNC_1( then_s_extr, 1, true, Callback *c, const Vec<Rp *> &keys ) {
    Vec<Rp *> args; args << ref_without_lazifier( v0 ); for( Rp *k : keys ) args << copy_without_lazifier( k );
    check_at_least_a_sym_child( args, "then_s_extr" );
    return SymOpExtr<Op_then,true>::New( Op_then{ c }, Op_then::category( v0 ), args.size(), args.ptr() );
}
RiotBase_DEF_FUNC_1( self_make_inode_if_z, 2, tv0->isa<GenSymSelect>(), const Category *cat ) {
    GenSymSelect *s = scast( v0 );
    Lazyfier     *l = s->container;
    if ( CommonSymbolicData *csd = RO_CALL( common_symbolic_data, l->cur ) )
        csd->lazyfier_parents.remove_first_unordered( l );
    l->cur = RO_CALL( make_inode_if_z_s_extr, l->cur, cat, s->keys );
    if ( CommonSymbolicData *csd = RO_CALL( common_symbolic_data, l->cur ) )
        csd->lazyfier_parents << l;
}
RiotBase_DEF_FUNC_1( make_inode_if_z_s_extr, 1, true, const Category *cat, const Vec<Rp *> &keys ) {
    Vec<Rp *> args; args << ref_without_lazifier( v0 ); for( Rp *k : keys ) args << copy_without_lazifier( k );
    check_at_least_a_sym_child( args, "make_inode_if_z_s_extr" );
    return SymOpExtr<Op_make_inode_if_z,true>::New( Op_make_inode_if_z{ cat }, Op_make_inode_if_z::category( v0 ), args.size(), args.ptr() );
}
RiotBase_DEF_FUNC_1( self_special_func, 2, tv0->isa<GenSymSelect>(), const char *func, Rp **parm, int nb_parms ) {
    GenSymSelect *s = scast( v0 );
    Lazyfier     *l = s->container;
    if ( CommonSymbolicData *csd = RO_CALL( common_symbolic_data, l->cur ) )
        csd->lazyfier_parents.remove_first_unordered( l );
    l->cur = RO_CALL( special_func_s_extr, l->cur, func, parm, nb_parms, s->keys );
    if ( CommonSymbolicData *csd = RO_CALL( common_symbolic_data, l->cur ) )
        csd->lazyfier_parents << l;
}
RiotBase_DEF_FUNC_1( special_func_s_extr, 1, true, const char *func, Rp **parm, int nb_parms, const Vec<Rp *> &keys ) {
    Vec<Rp *> args; args << ref_without_lazifier( v0 ); for( Rp *k : keys ) args << copy_without_lazifier( k );
    check_at_least_a_sym_child( args, "special_func_s_extr" );
    return SymOpExtr<Op_special_func,true>::New( Op_special_func{ func, parm, nb_parms }, Op_special_func::category( v0 ), args.size(), args.ptr() );
}
