#pragma once

#include "../../System/StaticStore.h"
#include "../../System/TypeList.h"
#include "TypeTempl_DeclDynStat.h"

namespace RiotBase {

///
template<class T>
class TypeTempl : public Type {
public:
    virtual                ~TypeTempl             () {}
    virtual void            init_static_attributes() { T::init_static_attributes(); }
    virtual std::string     name                  () { return T::type_name(); }
    virtual bool            isa                   ( const Type *anc ) const { return this == anc or _isa( TypeList<typename T::ParentClass>(), anc ); }

    virtual bool            unknown_term_value    () const { return T::unknown_value and T::term_value; }
    virtual bool            unknown_value         () const { return T::unknown_value; }
    virtual bool            term_value            () const { return T::term_value; }
    virtual bool            error_value           () const { return T::error_value; }
    virtual bool            atomic_value          () const { return T::atomic_value; }
    virtual const Category *static_category       () const { return T::static_category(); }

    #include "TypeTempl_DefDynStat.h"

protected:
    template<class P> bool _isa( TypeList<P>, const Type *anc ) const { return StaticStore<TypeTempl<P>>::data.isa( anc ); }
    bool _isa( TypeList<void>, const Type *anc ) const { return false; }

};

template<class T>
bool Type::isa() const { return isa( &StaticStore<TypeTempl<T>>::data ); }

template<class T>
PI32 type_num_of() { return StaticStore<TypeTempl<T>>::data.type_num; }

} // namespace RiotBase
