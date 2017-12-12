% for m in methods:
%   if m.nb_args == 1:
virtual RF_${m.name}::TF *surdef_for( RF_${m.name}, double &pertinence ) = 0;
virtual bool has_surdef_for( RF_${m.name} ); //
%   endif
% endfor
