% for m in self_ops:
RiotBase_DEF_FUNC_${m.nb_args}( self_${m.name}${"_"*(m.nb_args>=2)+"b"*(m.nb_args-1)}, 1, ${"( " + string.join( [ "tv" + str( i ) + "->unknown_value()" for i in range( 0, m.nb_args ) ], " or " ) + " )" + string.join( [ " and tv" + str( i ) + "->term_value()" for i in range( 1, m.nb_args ) ], "" )}${m.decl_add_args(j='',b=', ')} ) {
    v0 = Lazyfier::New( RO_CALL( ${m.name}_s${"b"*(m.nb_args-1)}, v0${m.name_base_args( j='', b=', ', start = 1 )}${m.name_add_args( j='', b=', ' )} ) );
}
RiotBase_DEF_FUNC_${m.nb_args}( self_${m.name}${"_"*(m.nb_args>=2)+"b"*(m.nb_args-1)}, 2, ${string.join( ["tv0->isa<Lazyfier>()"] + [ "tv" + str( i ) + "->term_value()" for i in range( 1, m.nb_args ) ], " and " )}${m.decl_add_args(j='',b=', ')} ) {
    Lazyfier *l = scast( v0 );
    if ( CommonSymbolicData *csd = RO_CALL( common_symbolic_data, l->cur ) )
        csd->lazyfier_parents.remove_first_unordered( l );
    l->cur = RO_CALL( ${m.name}_s${"b"*(m.nb_args-1)}, ${string.join( [ "l->cur" ] + [ "v" + str( i ) for i in range( 1, m.nb_args ) ] + [ a.n for a in m.add_args ], ', ' )} );
    if ( CommonSymbolicData *csd = RO_CALL( common_symbolic_data, l->cur ) )
        csd->lazyfier_parents << l;
}
% endfor
