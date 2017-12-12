% for m in methods:
%   if m.nb_args == 1:
bool Type::has_surdef_for( RF_${m.name} r ) { double tmp; return surdef_for( r, tmp ); }
%   endif
% endfor
