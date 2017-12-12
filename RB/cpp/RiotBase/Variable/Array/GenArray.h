#pragma once

#include "../../System/TypeListAppender.h"
#include "../Core/AutoRpDel.h"
#include "../Core/Variable.h"
#include "../Ot/OtWrapper.h"
#include <set>

namespace RiotBase {
class GenArraySelect;

///
class GenArray : public OtWrapper<GenArray> {
public:
    // enum for RpTempl
    enum { term_value = true };

    // operation types
    struct op_insert     { static const char *name() { return "insert"        ; } static bool valid( PT pos, const Variable &str ) { return pos != std::numeric_limits<PT>::max(); } };
    struct op_remove     { static const char *name() { return "remove"        ; } static bool valid( PT pos, PT len              ) { return len;        } };
    struct op_remove_und { static const char *name() { return "remove_und"    ; } static bool valid( PT pos, const Variable &str ) { return pos != std::numeric_limits<PT>::max(); } };
    using OperationTypes = typename TypeListAppender<OtWrapperOperationTypes,op_insert,op_remove,op_remove_und>::T;

    // rights types
    struct can_read      { static const char *name() { return "read"; } };
    struct can_insert    { static const char *name() { return "insert"; } };
    struct can_remove    { static const char *name() { return "remove"; } };
    struct can_append    { static const char *name() { return "append"; } };
    using RightTypes = typename TypeListAppender<OtWrapperRightTypes,can_read,can_insert,can_remove,can_append>::T;

    using SetGenArraySelect = std::set<GenArraySelect *>;

    GenArray( const Category *category_array, const Category *category_val, UsrId usr_id = {} );

    static std::string   type_name       ();
    void                 write_to_stream ( std::ostream &os ) const;
    const Category      *category        () const;
    // PatchMerger         *new_patch_merger();
    Rp                  *copy            () const;
    Rp                  *get_size        () const;
    Rp                  *front_b         () const;
    Rp                  *front_list_b    () const;
    Rp                  *back_b          () const;
    Rp                  *back_list_b     () const;
    Rp                  *head_b          () const;
    Rp                  *tail_b          () const;
    void                 apply           ( std::function<void(const Rp *)> f ) const;

    Rp                  *select          ( const RiotBase::Rp* key, bool mod_right ) const; ///< mod_right = right to modify
    void                 self_push_back_b( Rp *val );
    void                 self_push_back_o( Rp *val );

    void                 _reserve        ( PT size );

    // OtWrapper surdefs
    using OtWrapper<GenArray>::right_to;
    bool                 right_to            ( op_insert    , UsrId as_usr, PT pos, const Variable &var  ) { return has_right( can_insert{}, as_usr ) or ( pos == items.size() and has_right( can_append{}, as_usr ) ); }
    bool                 right_to            ( op_remove    , UsrId as_usr, PT pos, PT len               ) { return has_right( can_remove{}, as_usr ); }
    bool                 right_to            ( op_remove_und, UsrId as_usr, PT pos, const Variable &var  ) { return has_right( can_remove{}, as_usr ); }

    using OtWrapper<GenArray>::apply_op;
    void                 apply_op            ( op_insert    , Aod &aod, PT pos, const Variable &var );
    void                 apply_op            ( op_remove    , Aod &aod, PT pos, PT len              );
    void                 apply_op            ( op_remove_und, Aod &aod, PT pos, const Variable &var );

    using OtWrapper<GenArray>::transmission;
    void                 transmission        ( op_remove_und, BBQ br, PT pos, Variable &old );

    using OtWrapper<GenArray>::register_patch_op;
    void                 register_patch_op   ( op_remove    , BBQ bq, PT pos, PT len );

    using OtWrapper<GenArray>::undo_patch;
    void                 undo_patch          ( op_insert    , Aod &aod, PT pos, const Variable &var );
    void                 undo_patch          ( op_remove    , Aod &aod, PT pos, PT len              );
    void                 undo_patch          ( op_remove_und, Aod &aod, PT pos, const Variable &var );


    const Category      *category_array;
    const Category      *category_val;
    SetGenArraySelect    set_array_selects;
    Vec<AutoRpDel>       items;
};

} // namespace RiotBase
