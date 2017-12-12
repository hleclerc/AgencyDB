#pragma once

#include "Methods.h"

namespace RiotBase {
class Category;

///
class Type {
public:
    Type();
    virtual ~Type();

    int                     potentially_updated_type_off();

    static int              rs_instanciated_types       (); ///< reserved size for function tables for this class
    static Type            *get_type_of                 ( const Rp *obj ); ///<

    virtual std::string     name                        () = 0;
    virtual void            init_static_attributes      () = 0;

    template<class T> bool  isa                         () const;                      ///<
    virtual bool            isa                         ( const Type *anc ) const = 0; ///<
    virtual bool            unknown_term_value          () const = 0;                  ///< symbolic (terminal) value. terminal means that it is not a proxy
    virtual bool            known_term_value            () const { return term_value() and not unknown_value(); }
    virtual bool            unknown_value               () const = 0;                  ///< symbolic value, or proxy on a symbolic value
    virtual bool            known_value                 () const { return not unknown_value(); }
    virtual bool            term_value                  () const = 0;
    virtual bool            atomic_value                () const = 0;
    virtual bool            error_value                 () const = 0;
    virtual const Category *static_category             () const = 0;

    #include "Type_has_surdef_for_cpp.h"

    // DynObj attributes
    PI32                    type_off;
    PI32                    type_num;

    // type lists
    Type                   *prev_type_anc;
    static Type            *last_type_anc;
    static int              nb_instanciated_types; ///<
    static Vec<Type *>      by_type_off;           ///< beware: type_off = num_type_off * sizeof( void * )
};

} // namespace RiotBase
