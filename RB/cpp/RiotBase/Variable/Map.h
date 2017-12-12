#pragma once

#include "Core/VariableTempl.h"
#include "Number.h"

namespace RiotBase {

Rp *new_OtWrapperMap( const Category *category_map, const Category *category_key, const Category *category_val, UsrId usr_id = {} );

///
template<class K,class V>
class Map : public VariableTempl<Map<K,V>> {
public:
    using TP = VariableTempl<Map<K,V>>;

    Map( Variable::CreatorId, UsrId usr_id ) : TP( new_OtWrapperMap( TP::category, K::category, V::category, usr_id ) ) {}
    Map() : TP( new_OtWrapperMap( TP::category, K::category, V::category ) ) {}
    Map( const Map &map ) : TP( RO_CALL( copy, map.rp ) ) {}
    Map( Rp *rp ) : TP( rp ) {}

    const Ref<V>           operator[]      ( const K &key ) const { return RO_CALL( select_bb, this->rp, key.rp ); }
    Ref<V>                 operator[]      ( const K &key ) { return RO_CALL( mod_select, this->rp, key.rp ); }
    Map                   &operator=       ( const Map &m ) { RO_CALL( self_set_b, this->rp, m.rp ); return *this; }

    Map                   &del             ( const K &key ) { RO_CALL( self_rem_key_b, this->rp, key.rp ); return *this; }

    Number                 size            () const { return RO_CALL( get_size, this->rp ); }

    static std::string     category_name   () { return "Map<" + K::category_name() + "," + V::category_name() + ">"; }
    static const Category *get_category_key() { return K::category; }
    static const Category *get_category_val() { return V::category; }
};

} // namespace RiotBase
