#include "../Core/FunctionTable.h"
#include "Lazyfier.h"
#include "SymOp.h"

namespace RiotBase {

namespace {

% for m in std_ops:
<% cond = string.join( [ "tv" + str( i ) + "->unknown_term_value()" for i in range( m.nb_args ) ], " or " ) %>
<% targ = string.join( [ ", copy_without_lazifier( v" + str( i ) + " )" for i in range( 1, m.nb_args ) ], '' ) %>
RiotBase_DEF_FUNC_${m.nb_args}( ${m.name}_b${"b"*(m.nb_args-1)}, 1, ${cond}${m.decl_add_args(j='',b=', ')} ) { return Lazyfier::New( SymOp<Op_${m.name}>::New( Op_${m.name}{${m.name_add_args()}}, Op_${m.name}::category( ${m.name_base_args()} ), copy_without_lazifier( v0 )${targ} ) ); }
% if m in self_ops:
RiotBase_DEF_FUNC_${m.nb_args}( ${m.name}_b${"b"*(m.nb_args-1)}, 0, FunctionTable_${m.nb_args}<RF_self_${m.name}${"_"*(m.nb_args>=2)+"b"*(m.nb_args-1)}>::find_best_func( ${string.join( [ "tv" + str( i ) for i in range( m.nb_args ) ], ", " )} ).second > 0${m.decl_add_args(j='',b=', ')} ) { Rp *res = RO_CALL( copy, v0 ); RO_CALL( self_${m.name}${"_"*(m.nb_args>=2)+"b"*(m.nb_args-1)}, res${m.name_base_args(j='',b=', ',start=1)}${m.name_add_args(j='',b=', ')} ); return res; }
RiotBase_DEF_FUNC_${m.nb_args}( ${m.name}_o${"b"*(m.nb_args-1)}, 1, ${cond}${m.decl_add_args(j='',b=', ')} ) { return Lazyfier::New( SymOp<Op_${m.name}>::New( Op_${m.name}{${m.name_add_args()}}, Op_${m.name}::category( ${m.name_base_args()} ), ref_without_lazifier( v0 )${targ} ) ); }
RiotBase_DEF_FUNC_${m.nb_args}( ${m.name}_s${"b"*(m.nb_args-1)}, 1, ${cond}${m.decl_add_args(j='',b=', ')} ) { return SymOp<Op_${m.name},true>::New( Op_${m.name}{${m.name_add_args()}}, Op_${m.name}::category( ${m.name_base_args()} ), ref_without_lazifier( v0 )${targ} ); }
% endif
% endfor

} // namespace

bool _Op_then_get_done( const Op_then &op ) { return op.done; }
void _Op_then_set_done( Op_then &op ) { op.done = true; }

} // namespace RiotBase
