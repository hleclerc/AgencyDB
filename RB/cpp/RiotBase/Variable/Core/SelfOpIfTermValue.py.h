% for m in self_ops:
RiotBase_DEF_FUNC_${m.nb_args}( self_${m.name}${ "_" * ( m.nb_args > 1 ) + "b" * ( m.nb_args - 1 ) }, 0, tv0->term_value()${m.decl_add_args(j='',b=', ')} ) { v0 = RO_CALL( ${m.name}_o${ "b" * ( m.nb_args - 1 ) }${m.name_base_args(j='',b=', ')}${m.name_add_args(j='',b=', ')} ); }
% endfor

