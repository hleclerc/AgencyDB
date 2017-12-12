% for m in methods:
virtual RF_${m.name}::TF *surdef_for( RF_${m.name} f, double &pertinence ) { return _surdef_for<T>( f, pertinence ); }
% endfor
