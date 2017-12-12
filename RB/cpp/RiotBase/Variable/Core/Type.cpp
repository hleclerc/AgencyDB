#include "FunctionTable_Ancestor.h"
#include "Type.h"

namespace RiotBase {

Type       *Type::last_type_anc         = 0;
int         Type::nb_instanciated_types = 1;
Vec<Type *> Type::by_type_off          ( Size(), rs_instanciated_types() );

int sh_instanciated_types = 4; ///< Type::rs_instanciated_types() = pow( 2, sh_instanciated_types )


Type::Type() : type_off( 0 ), type_num( last_type_anc ? last_type_anc->type_num + 1 : 0 ), prev_type_anc( last_type_anc ) {
    last_type_anc = this;
}

Type::~Type() {
}

int Type::potentially_updated_type_off() {
    if ( type_off )
        return type_off;
    type_off = sizeof( void * ) * nb_instanciated_types++;

    if ( nb_instanciated_types > rs_instanciated_types() ) {
        ++sh_instanciated_types;
        for( FunctionTable_Ancestor *p = FunctionTable_Ancestor::last_function_table; p; p = p->prev_function_table )
            p->resize_table();
        by_type_off.resize( rs_instanciated_types() );
    }

    by_type_off[ type_off / sizeof( void * ) ] = this;
    return type_off;
}

int Type::rs_instanciated_types() {
    return 1 << sh_instanciated_types;
}

Type *Type::get_type_of( const Rp *obj ) {
    if ( obj->type_off )
        return by_type_off[ obj->type_off / sizeof( void * ) ];

    if ( Type *res = last_type_anc ) {
        for( int num = last_type_anc->type_num - obj->type_num; num; --num )
            res = res->prev_type_anc;
        return res;
    }

    return 0;
}

// TODO: check registered RiotBase_DEF_FUNC_1
#include "Type_has_surdef_for_h.h"


} // namespace RiotBase
