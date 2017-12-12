static void check_at_least_a_sym_child( const Vec<Rp *> &args, const char *method_name ) {
    for( Rp *a : args )
        if ( RO_CALL( is_symbolic, a ) )
            return;
    WARNING( "%s is not defined for known values", method_name );
}

% for m in self_ops:
RiotBase_DEF_FUNC_${m.nb_args}( self_${m.name}${"_"*(m.nb_args>=2)+"b"*(m.nb_args-1)}, 2, ${string.join( [ "tv0->isa<GenSymSelect>()" ] + [ "tv" + str( i ) + "->term_value()" for i in range( 1, m.nb_args ) ], ' and ' )}${m.decl_add_args(j='',b=', ')} ) {
    GenSymSelect *s = scast( v0 );
    Lazyfier     *l = s->container;
    if ( CommonSymbolicData *csd = RO_CALL( common_symbolic_data, l->cur ) )
        csd->lazyfier_parents.remove_first_unordered( l );
    l->cur = RO_CALL( ${m.name}_s${"b"*(m.nb_args-1)}_extr, ${ string.join( [ "l->cur" ] + [ "v" + str( i ) for i in range( 1, m.nb_args ) ] + [ a.n for a in m.add_args ] + [ "s->keys" ], ', ' ) } );
    if ( CommonSymbolicData *csd = RO_CALL( common_symbolic_data, l->cur ) )
        csd->lazyfier_parents << l;
}
RiotBase_DEF_FUNC_${m.nb_args}( ${m.name}_s${"b"*(m.nb_args-1)}_extr, 1, true${m.decl_add_args(j='',b=', ')}, const Vec<Rp *> &keys ) {
    Vec<Rp *> args; args << ref_without_lazifier( v0 )${ string.join( [ " << copy_without_lazifier( v" + str( i ) + " )" for i in range( 1, m.nb_args ) ], '' ) }; for( Rp *k : keys ) args << copy_without_lazifier( k );
    check_at_least_a_sym_child( args, "${m.name}_s${"b"*(m.nb_args-1)}_extr" );
    return SymOpExtr<Op_${m.name},true>::New( Op_${m.name}{ ${ m.name_add_args() } }, Op_${m.name}::category( ${m.name_base_args()} ), args.size(), args.ptr() );
}
% endfor
