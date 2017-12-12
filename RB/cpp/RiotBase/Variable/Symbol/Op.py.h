#pragma once

#include "../Core/Category.h"
#include "../Core/Methods.h"

namespace RiotBase {

% for m in std_ops:
% if not ( m in self_ops ):
struct Op_${m.name} {
    enum { nb_args = ${m.nb_args} };
    static const char *name() { return "${m.name}"; } 
    Rp *exec_${"b"*m.nb_args}( ${string.join([ "const Rp *v" + str( i ) for i in range( m.nb_args ) ],', ')} ) const { return RO_CALL( ${m.name}_${"b"*m.nb_args}, ${m.name_all_args()} ); }
    static const Category *category( ${string.join( [ "const Rp *v" + str( i ) for i in range( m.nb_args ) ], ', ' )} ) { return RO_CALL( category, v0 ); }${m.decl_add_args(j='',b='\n    ',e=';')}
};
% endif
% endfor

% for m in self_ops:
struct Op_${m.name} {
    enum { nb_args = ${m.nb_args} };
    static const char *name() { return "${m.name}"; } 
    Rp *exec_s${"b"*(m.nb_args-1)}( ${string.join( [ "Rp *v0" ] + [ "const Rp *v" + str( i ) for i in range( 1, m.nb_args ) ],', ')} ) const { RO_CALL( self_${m.name}${"_"*(m.nb_args>=2)+"b"*(m.nb_args-1)}, ${m.name_base_args()}${m.name_add_args(j='',b=', ')} ); return v0; }
    Rp *exec_${"b"*m.nb_args}( ${string.join([ "const Rp *v" + str( i ) for i in range( m.nb_args ) ],', ')} ) const { return RO_CALL( ${m.name}_${"b"*m.nb_args}, ${m.name_all_args()} ); }
    Rp *exec_s${"b"*(m.nb_args-1)}_extr( Rp **ch, int ns ) const { Vec<Rp *,-1,2> lst; lst.reserve( ns ); Rp *sel = ch[ 0 ]; for( int i = 0; i < ns; ++i ) lst << ( sel = RO_CALL( select_bb, sel, ch[ ${m.nb_args} + i ] ) ); RO_CALL( self_${m.name}${"_"*(m.nb_args>=2)+"b"*(m.nb_args-1)}, ${string.join( [ "sel" ] + [ "ch[ " + str( i ) + " ]" for i in range( 1, m.nb_args ) ] + [ a.n for a in m.add_args ], ', ' )} ); while( ns-- ) RO_CALL( del, lst[ ns ] ); return ch[ 0 ]; }

    static const Category *category( ${string.join( [ "const Rp *v" + str( i ) for i in range( m.nb_args ) ], ', ' )} ) { return RO_CALL( category, v0 ); }${m.decl_add_args(j='',b='\n    ',e=';')}${ string.join([ "\n    " + attr for attr in m.add_attrs ]) }
};
% endfor

} // namespace RiotBase
