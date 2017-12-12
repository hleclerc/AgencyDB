<%import string%>
#pragma once

#include "../../System/BitFieldExtSize.h"
#include "../../System/ArgType.h"
#include "../../System/Inode.h"
#include "../../System/GrpId.h"
#include "../../System/UsrId.h"
#include "../../System/DaSi.h"
#include "../../System/PVec.h"
#include "CursorData.h"
#include "Rp.h"

namespace RiotBase {
template<class FuncName> struct Surdef_1;
template<class FuncName> struct Surdef_2;
template<class FuncName> struct Surdef_3;
class CommonSymbolicData; // for DeclMethods
class LazySourceInfo;
class OtMessage;
class OtMerger;
class Category;
class Callback;
class Db;

using PairRpPtr = std::pair<Rp *,Rp *>;

extern int sh_instanciated_types; ///< Type::rs_instanciated_types() = pow( 2, sh_instanciated_types )
extern Rp *indestructible_fake;

#define RO_CALL( FUNCNAME, ... ) RiotBase::RF_##FUNCNAME::call( __VA_ARGS__ )

template<class T> struct AutoIndestructibleFakeIfNotRef { ~AutoIndestructibleFakeIfNotRef() { v.rp = 0; } T &v; };
template<class T> struct AutoIndestructibleFakeIfNotRef<T &> { ~AutoIndestructibleFakeIfNotRef() {} T &v; };
template<class T> typename T::__FaKeImpossibleType as_a_Variable( T &a ) { return a; }

% for m in methods:
struct RF_${m.name} {
    using TF = ${m.ret}( ${m.type_args()} ); enum { nb_args = ${m.nb_args} }; static const char *name() { return "${m.name}"; }
    % if m.nb_args == 1:
    static TF *&get_f( const Rp *v0 ) { return *reinterpret_cast<TF **>( reinterpret_cast<char *>( table ) + v0->type_off ); }
    static TF **table;
    % elif m.nb_args == 2:
    static TF *&get_f( const Rp *v0, const Rp *v1 ) { return *reinterpret_cast<TF **>( reinterpret_cast<char *>( table ) + ( v1->type_off << sh_instanciated_types ) + v0->type_off ); }
    static TF **table;
    % else:
    static TF *&get_f( const Rp *v0, const Rp *v1, const Rp *v2 );
    struct Item { TF *f; PI32 t0, t1, t2; };
    static Vec<Item> *table;
    % endif
    static ${m.ret_os()}call( ${m.decl_args()} ) { return get_f( ${string.join([ "v" + str( i ) for i in range( m.nb_args ) ], ", " )} )( ${m.name_args()} ); }
    static Surdef_${m.nb_args}<RF_${m.name}> *last_surdef;
};
% endfor

% for m in std_ops:
struct RF_${m.name} {
    template<${ string.join( [ "class T" + str( i ) for i in range( m.nb_args ) ], "," ) }>
    static Rp *call( ${ string.join( [ "T" + str( i ) + " &&v" + str( i ) for i in range( m.nb_args ) ], ", " ) } ) {
        ${ string.join( [ "AutoIndestructibleFakeIfNotRef<T" + str( i ) + "> ai_" + str( i ) + "{ v" + str( i ) + " };" for i in range( m.nb_args ) ], "\n        " ) }
        % for n in itertools.product( ["o","b"], repeat = m.nb_args ):
        % if "o" in n:
        if ( ${ string.join( [ "std::is_reference<T" + str( i ) + ">::value == " + str( ( n[ i ] == "b" ) * 1 ) for i in range( len( n ) ) ], " and " ) } ) \
        % else:
        \
        % endif
return RO_CALL( ${m.name}_${string.join( n, "" )}, ${ string.join( [ "v" + str( i ) + ".rp" for i in range( m.nb_args ) ], ", " ) } );
        % endfor
    }
    // template<${ string.join( [ "class T" + str( i ) for i in range( m.nb_args ) ], "," ) }>
    // static Rp *call_gen( ${ string.join( [ "T" + str( i ) + " &&v" + str( i ) for i in range( m.nb_args ) ], ", " ) } ) {
    //     return call_var( ${ string.join( [ "as_a_Variable( std::forward<T" + str( i ) + ">( v" + str( i ) + " ) )" for i in range( m.nb_args ) ], ", " ) } );
    // }
};
% endfor



} // namespace RiotBase
