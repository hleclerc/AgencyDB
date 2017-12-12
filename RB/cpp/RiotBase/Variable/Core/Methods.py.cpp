#include "FunctionTable.h"
#include "Methods.h"

namespace RiotBase {

% for m in methods:
%   if m.nb_args <= 2:
RF_${m.name}::TF **RF_${m.name}::table = 0;
Surdef_${m.nb_args}<RF_${m.name}> *RF_${m.name}::last_surdef = 0;
static FunctionTable_${m.nb_args}<RF_${m.name}> FT_${m.name};
%   else:
Vec<RF_${m.name}::Item> *RF_${m.name}::table = 0;
Surdef_3<RF_${m.name}> *RF_${m.name}::last_surdef = 0;
static FunctionTable_3<RF_${m.name}> FT_${m.name};
RF_${m.name}::TF *&RF_${m.name}::get_f( const Rp *v0, const Rp *v1, const Rp *v2 ) {
    Vec<Item> &vec = table[ ( v0->type_off + v1->type_off + v2->type_off ) % 128 ];
    for( Item &item : vec )
        if ( item.t0 == v0->type_off and item.t1 == v1->type_off and item.t2 == v2->type_off )
            return item.f;
    return vec.push_back( Item{ FunctionTable_3<RF_${m.name}>::IncompleteFunc<TF>::f, v0->type_off, v1->type_off, v2->type_off } )->f;
}
%   endif
% endfor

} // namespace RiotBase
