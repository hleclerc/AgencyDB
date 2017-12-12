#include "Type.h"

namespace RiotBase {

// _surdef_for (helper for TypeTempl<>::surdef_for)
template<class T,class FuncName>
typename FuncName::TF *_surdef_for( FuncName, double &pertinence ) { return 0; }


% for m in methods:
%   if m.nb_args == 1:
template<class T> struct DynMethToStatFunc_${m.name} { static ${m.ret_os()}f( ${m.decl_args()} ) { return static_cast<${ "const " * ( m.args[ 0 ].t.find( "const " ) >= 0 ) }T *>( v0 )->${m.name}( ${m.name_args(start=1)} ); } };
template<class T> typename EnableIf<1,RF_${m.name}::TF,decltype(&T::${m.name})>::T *_surdef_for( RF_${m.name}, double &pertinence ) { pertinence = 2; return &DynMethToStatFunc_${m.name}<T>::f; }
%   endif
% endfor


} // namespace RiotBase
