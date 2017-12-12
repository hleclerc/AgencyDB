% for m in std_ops:
%   for v0 in itertools.product( ['o','b'], repeat = m.nb_args ):
%     if "o" in v0:
RiotBase_DEF_FUNC_${m.nb_args}( ${m.name}${ "_" + string.join( v0, "" ) }, 0, true${m.decl_add_args(j='',b=', ')} ) { Rp *res = RO_CALL( ${m.name}_${ "b" * m.nb_args }${m.name_base_args(j='',b=', ')}${m.name_add_args(j='',b=', ')} ); \
%       for i in range( len( v0 ) ):
%         if v0[ i ] == "o":
RO_CALL( del, v${ str( i ) } ); \
%         endif
%       endfor
return res; \
}
%     endif
%   endfor
% endfor

 
