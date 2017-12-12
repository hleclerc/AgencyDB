#pragma once

#include "../../System/TypeList.h"
#include "../../System/Log.h"
#include <tuple>

namespace RiotBase {
template<class T,bool self_for_arg=false> class SymOp;
template<class T> class Bn;
class  LazySourceKnownValueExt;
class  LazySourceExtValue;
class  RecvFromExtExec;
class  SendToExtExec;
struct Op_add_key;
struct Op_split;
struct Op_join;
class  GenString;


///
using TupleTypes = TypeList<
    #define DECL_TUPLE_TYPE( TYPE ) TYPE,
    #include "TupleTypesDecl.h"
    #undef DECL_TUPLE_TYPE
>;

template<class T>
PT num_in_tuple_types() {
    if ( TupleTypes::index_of<T>() == TupleTypes::size )
        DISP_ERROR( "Type {} not registered in TupleTypes", T::type_name() );
    return TupleTypes::index_of<T>();
}


} // namespace RiotBase
