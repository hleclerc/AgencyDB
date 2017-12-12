#pragma once

#include "../../System/TypeList.h"
#include "Category.h"
#include "Ref.h"

namespace RiotBase {
Rp *new_Symbol( const class String &name, const Category *category );

///
template<class T>
class VariableTempl : public Variable {
public:
    VariableTempl( Rp *v = 0 ) : Variable( v ) {}

    static T               symbol          ( const class String &name ) { return T( new_Symbol( name, category ) ); }

    // then
    using        Variable::then;

    template<class Functor>
    void                   then            ( const Functor &f ) { then( (Callback *)new CallbackFunctor<Functor,T>( f ) ); }

    // category
    static const Category *get_category_key() { WARNING("get_category_key() is not defined..."); return category; }
    static const Category *get_category_val() { WARNING("get_category_val() is not defined..."); return category; }

    static Category       *category;

private:
    struct CategoryLoc : public Category {
        virtual std::string     name        () const { return T::category_name(); }
        virtual Rp             *create      ( UsrId as_usr ) const { Rp *res; new ( &res ) T( Variable::CreatorId{}, as_usr ); return res; }
        virtual const Category *category_key() const { return T::get_category_val(); }
        virtual const Category *category_val() const { return T::get_category_key(); }
    };

    static CategoryLoc category_loc;
};

template<class T> typename VariableTempl<T>::CategoryLoc VariableTempl<T>::category_loc;
template<class T> Category *VariableTempl<T>::category = &VariableTempl<T>::category_loc;

} // namespace RiotBase
