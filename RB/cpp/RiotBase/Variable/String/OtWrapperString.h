#pragma once

#include "../../System/TypeListAppender.h"
#include "../../System/AutoPtr.h"
#include "../Ot/OtWrapper.h"

namespace RiotBase {
#define _TCB template<class Cb>

///
class OtWrapperString : public OtWrapper<OtWrapperString> {
public:
    // operation types
    struct op_insert         { static const char *name() { return "insert"        ; } static bool valid( PT pos, const CbString &str                      ) { return str.size(); } };
    struct op_remove         { static const char *name() { return "remove"        ; } static bool valid( PT pos, PT len                                   ) { return len;        } };
    struct op_remove_und     { static const char *name() { return "remove_und"    ; } static bool valid( PT pos, const CbString &str                      ) { return str.size(); } };
    struct op_new_cursor     { static const char *name() { return "new_cursor"    ; } static bool valid( DevId dev_id, PI64 loc_id, PT pos                ) { return loc_id;     } };
    struct op_set_cursor     { static const char *name() { return "set_cursor"    ; } static bool valid( DevId dev_id, PI64 loc_id, PT pos                ) { return loc_id;     } };
    struct op_set_cursor_und { static const char *name() { return "set_cursor_und"; } static bool valid( DevId dev_id, PI64 loc_id, PT pos, PT old_pos    ) { return loc_id;     } };
    struct op_rem_cursor     { static const char *name() { return "rem_cursor"    ; } static bool valid( DevId dev_id, PI64 loc_id                        ) { return loc_id;     } };
    struct op_rem_cursor_und { static const char *name() { return "rem_cursor_und"; } static bool valid( DevId dev_id, PI64 loc_id, PT old_pos, UsrId usr ) { return loc_id;     } };
    using OperationTypes = typename TypeListAppender<OtWrapperOperationTypes,op_insert,op_remove,op_remove_und,op_new_cursor,op_set_cursor,op_set_cursor_und,op_rem_cursor,op_rem_cursor_und>::T;

    // rights types
    struct can_read          { static const char *name() { return "read"; } };
    struct can_insert        { static const char *name() { return "insert"; } };
    struct can_remove        { static const char *name() { return "remove"; } };
    struct can_append        { static const char *name() { return "append"; } };
    using RightTypes = typename TypeListAppender<OtWrapperRightTypes,can_read,can_insert,can_remove,can_append>::T;

    // cursor data
    struct Cursor {
        PT     pos;
        PI64   loc_id;        ///< id of the cursor (to be used with dev_id)
        DevId  dev_id;        ///< from which dev
        UsrId  usr_id;        ///< from which usr
        void  *int_id;        ///< internal id (can be used internally, not sent to other instances)
    };

    // Rp methods
    OtWrapperString( Rp *str, UsrId usr_id = {} );
    ~OtWrapperString();

    static std::string      type_name           ();
    static const Category  *static_category     ();
    const Rp               *prox_ref            () const;
    void                    write_structure     ( std::ostream &os ) const;

    Rp                     *new_cursor          ( void *int_id );
    void                    rem_cursor          ( void *int_id );
    void                    get_cursors         ( Vec<CursorData> &res ) const;
    void                    sig_change          ();

    // basic helpers
    void                    _substring          ( PT pos, PT len, const std::function<void(DaSi)> &f ) const;
    PT                      _size               () const;

    // helpers that check the rights and register the operations
    _TCB void               _insert             ( PT pos, const Cb &data ); ///< version that check the rights for current user, and register the operation in `operations`
    void                    _remove             ( PT pos, PT len );
                     
    Cursor                 *_new_cursor         ( DevId dev_id, PI64 loc_id, PT pos, void *int_id = 0 );
    void                    _set_cursor         ( Cursor *cur, PT pos );
    void                    _rem_cursor         ( Cursor *cur );

    // helpers that do not check anything
    Cursor                 *_new_cursor_wo_cr   ( DevId dev_id, PI64 loc_id, PT pos, UsrId usr, void *int_id );
    void                    _set_cursor_wo_cr   ( Cursor *c, PT pos );
    void                    _rem_cursor_wo_cr   ( Cursor *cur );

    // OtWrapper surdefs
    using OtWrapper<OtWrapperString>::right_to;
    bool                    right_to            ( op_insert        , UsrId as_usr, PT pos, const CbString &str                      ) { return has_right( can_insert{}, as_usr ) or ( pos == _size() and has_right( can_append{}, as_usr ) ); }
    bool                    right_to            ( op_remove        , UsrId as_usr, PT pos, PT len                                   ) { return has_right( can_remove{}, as_usr ); }
    bool                    right_to            ( op_remove_und    , UsrId as_usr, PT pos, const CbString &str                      ) { return has_right( can_remove{}, as_usr ); }
    bool                    right_to            ( op_new_cursor    , UsrId as_usr, DevId dev_id, PI64 loc_id, PT pos                ) { return has_right( can_read  {}, as_usr ); }
    bool                    right_to            ( op_set_cursor    , UsrId as_usr, DevId dev_id, PI64 loc_id, PT pos                ) { return has_right( can_read  {}, as_usr ); }
    bool                    right_to            ( op_set_cursor_und, UsrId as_usr, DevId dev_id, PI64 loc_id, PT pos, PT old_pos    ) { return has_right( can_read  {}, as_usr ); }
    bool                    right_to            ( op_rem_cursor    , UsrId as_usr, DevId dev_id, PI64 loc_id                        ) { return has_right( can_read  {}, as_usr ); }
    bool                    right_to            ( op_rem_cursor_und, UsrId as_usr, DevId dev_id, PI64 loc_id, PT old_pos, UsrId usr ) { return has_right( can_read  {}, as_usr ); }

    using OtWrapper<OtWrapperString>::apply_op;
    void                    apply_op            ( op_insert        , Aod &aod, PT pos, const CbString &str                      );
    void                    apply_op            ( op_remove        , Aod &aod, PT pos, PT len                                   );
    void                    apply_op            ( op_remove_und    , Aod &aod, PT pos, const CbString &str                      );
    void                    apply_op            ( op_new_cursor    , Aod &aod, DevId dev_id, PI64 loc_id, PT pos                );
    void                    apply_op            ( op_set_cursor    , Aod &aod, DevId dev_id, PI64 loc_id, PT pos                );
    void                    apply_op            ( op_set_cursor_und, Aod &aod, DevId dev_id, PI64 loc_id, PT pos, PT old_pos    );
    void                    apply_op            ( op_rem_cursor    , Aod &aod, DevId dev_id, PI64 loc_id                        );
    void                    apply_op            ( op_rem_cursor_und, Aod &aod, DevId dev_id, PI64 loc_id, PT old_pos, UsrId usr );

    using OtWrapper<OtWrapperString>::transmission;
    void                    transmission        ( op_remove_und    , BBQ br, PT pos, CbString &old );
    void                    transmission        ( op_set_cursor_und, BBQ br, DevId dev_id, PI64 loc_id, PT pos, PT old_pos );
    void                    transmission        ( op_rem_cursor_und, BBQ br, DevId dev_id, PI64 loc_id, PT old_pos, UsrId usr );

    using OtWrapper<OtWrapperString>::register_patch_op;
    void                    register_patch_op   ( op_remove    , BBQ bq, PT pos, PT len );
    void                    register_patch_op   ( op_set_cursor, BBQ bq, DevId dev_id, PI64 loc_id, PT pos );
    void                    register_patch_op   ( op_rem_cursor, BBQ bq, DevId dev_id, PI64 loc_id );

    using OtWrapper<OtWrapperString>::undo_patch;
    void                    undo_patch          ( op_insert        , Aod &aod, PT pos, const CbString &str );
    void                    undo_patch          ( op_remove        , Aod &aod, PT pos, PT len              );
    void                    undo_patch          ( op_remove_und    , Aod &aod, PT pos, const CbString &str );
    void                    undo_patch          ( op_new_cursor    , Aod &aod, DevId dev_id, PI64 loc_id, PT pos );
    void                    undo_patch          ( op_set_cursor    , Aod &aod, DevId dev_id, PI64 loc_id, PT pos );
    void                    undo_patch          ( op_set_cursor_und, Aod &aod, DevId dev_id, PI64 loc_id, PT pos, PT old_pos );
    void                    undo_patch          ( op_rem_cursor    , Aod &aod, DevId dev_id, PI64 loc_id );
    void                    undo_patch          ( op_rem_cursor_und, Aod &aod, DevId dev_id, PI64 loc_id, PT old_pos, UsrId usr );

    using OtWrapper<OtWrapperString>::unk_new_or_new_unk;
    void                    unk_new_or_new_unk  ( op_insert        , op_insert        , UsrId asu_n, BBQ bq_n, BBQ bq_o, PT &p_n, CbString &d_n, PT &p_o, CbString &d_o );
    void                    unk_new_or_new_unk  ( op_insert        , op_remove        , UsrId asu_n, BBQ bq_n, BBQ bq_o, PT &p_n, CbString &d_n, PT &p_o, PT &l_o );
    void                    unk_new_or_new_unk  ( op_insert        , op_remove_und    , UsrId asu_n, BBQ bq_n, BBQ bq_o, PT &p_n, CbString &d_n, PT &p_o, CbString &d_o );
    void                    unk_new_or_new_unk  ( op_insert        , op_new_cursor    , UsrId asu_n, BBQ bq_n, BBQ bq_o, PT &p_n, CbString &d_n, DevId &dev_id_o, PI64 &loc_id_o, PT &pos_o );
    void                    unk_new_or_new_unk  ( op_insert        , op_set_cursor    , UsrId asu_n, BBQ bq_n, BBQ bq_o, PT &p_n, CbString &d_n, DevId &dev_id_o, PI64 &loc_id_o, PT &pos_o );
    void                    unk_new_or_new_unk  ( op_insert        , op_set_cursor_und, UsrId asu_n, BBQ bq_n, BBQ bq_o, PT &p_n, CbString &d_n, DevId &dev_id_o, PI64 &loc_id_o, PT &pos_o, PT &old_pos_o );
                                                
    void                    unk_new_or_new_unk  ( op_remove        , op_insert        , UsrId asu_n, BBQ bq_n, BBQ bq_o, PT &p_n, PT &l_n, PT &p_o, CbString &d_o );
    void                    unk_new_or_new_unk  ( op_remove        , op_remove        , UsrId asu_n, BBQ bq_n, BBQ bq_o, PT &p_n, PT &l_n, PT &p_o, PT &l_o );
    void                    unk_new_or_new_unk  ( op_remove        , op_remove_und    , UsrId asu_n, BBQ bq_n, BBQ bq_o, PT &p_n, PT &l_n, PT &p_o, CbString &d_o );
    void                    unk_new_or_new_unk  ( op_remove        , op_new_cursor    , UsrId asu_n, BBQ bq_n, BBQ bq_o, PT &p_n, PT &l_n, DevId &dev_id_o, PI64 &loc_id_o, PT &pos_o );
    void                    unk_new_or_new_unk  ( op_remove        , op_set_cursor    , UsrId asu_n, BBQ bq_n, BBQ bq_o, PT &p_n, PT &l_n, DevId &dev_id_o, PI64 &loc_id_o, PT &pos_o );
    void                    unk_new_or_new_unk  ( op_remove        , op_set_cursor_und, UsrId asu_n, BBQ bq_n, BBQ bq_o, PT &p_n, PT &l_n, DevId &dev_id_o, PI64 &loc_id_o, PT &pos_o, PT &old_pos_o );
                                                
    void                    unk_new_or_new_unk  ( op_remove_und    , op_insert        , UsrId asu_n, BBQ bq_n, BBQ bq_o, PT &p_n, CbString &d_n, PT &p_o, CbString &d_o );
    void                    unk_new_or_new_unk  ( op_remove_und    , op_remove        , UsrId asu_n, BBQ bq_n, BBQ bq_o, PT &p_n, CbString &d_n, PT &p_o, PT &l_o );
    void                    unk_new_or_new_unk  ( op_remove_und    , op_remove_und    , UsrId asu_n, BBQ bq_n, BBQ bq_o, PT &p_n, CbString &d_n, PT &p_o, CbString &d_o );
    void                    unk_new_or_new_unk  ( op_remove_und    , op_new_cursor    , UsrId asu_n, BBQ bq_n, BBQ bq_o, PT &p_n, CbString &d_n, DevId &dev_id_o, PI64 &loc_id_o, PT &pos_o );
    void                    unk_new_or_new_unk  ( op_remove_und    , op_set_cursor    , UsrId asu_n, BBQ bq_n, BBQ bq_o, PT &p_n, CbString &d_n, DevId &dev_id_o, PI64 &loc_id_o, PT &pos_o );
    void                    unk_new_or_new_unk  ( op_remove_und    , op_set_cursor_und, UsrId asu_n, BBQ bq_n, BBQ bq_o, PT &p_n, CbString &d_n, DevId &dev_id_o, PI64 &loc_id_o, PT &pos_o, PT &old_pos_o );
                                                
    void                    unk_new_or_new_unk  ( op_new_cursor    , op_insert        , UsrId asu_n, BBQ bq_n, BBQ bq_o, DevId &dev_id_n, PI64 &loc_id_n, PT &pos_n, PT &p_o, CbString &d_o );
    void                    unk_new_or_new_unk  ( op_new_cursor    , op_remove        , UsrId asu_n, BBQ bq_n, BBQ bq_o, DevId &dev_id_n, PI64 &loc_id_n, PT &pos_n, PT &p_o, PT &l_o );
    void                    unk_new_or_new_unk  ( op_new_cursor    , op_remove_und    , UsrId asu_n, BBQ bq_n, BBQ bq_o, DevId &dev_id_n, PI64 &loc_id_n, PT &pos_n, PT &p_o, CbString &d_o );
                                                
    void                    unk_new_or_new_unk  ( op_set_cursor    , op_insert        , UsrId asu_n, BBQ bq_n, BBQ bq_o, DevId &dev_id_n, PI64 &loc_id_n, PT &pos_n, PT &p_o, CbString &d_o );
    void                    unk_new_or_new_unk  ( op_set_cursor    , op_remove        , UsrId asu_n, BBQ bq_n, BBQ bq_o, DevId &dev_id_n, PI64 &loc_id_n, PT &pos_n, PT &p_o, PT &l_o );
    void                    unk_new_or_new_unk  ( op_set_cursor    , op_remove_und    , UsrId asu_n, BBQ bq_n, BBQ bq_o, DevId &dev_id_n, PI64 &loc_id_n, PT &pos_n, PT &p_o, CbString &d_o );
    void                    unk_new_or_new_unk  ( op_set_cursor    , op_new_cursor    , UsrId asu_n, BBQ bq_n, BBQ bq_o, DevId &dev_id_n, PI64 &loc_id_n, PT &pos_n, DevId &dev_id_o, PI64 &loc_id_o, PT &pos_o );
    void                    unk_new_or_new_unk  ( op_set_cursor    , op_set_cursor    , UsrId asu_n, BBQ bq_n, BBQ bq_o, DevId &dev_id_n, PI64 &loc_id_n, PT &pos_n, DevId &dev_id_o, PI64 &loc_id_o, PT &pos_o );
    void                    unk_new_or_new_unk  ( op_set_cursor    , op_set_cursor_und, UsrId asu_n, BBQ bq_n, BBQ bq_o, DevId &dev_id_n, PI64 &loc_id_n, PT &pos_n, DevId &dev_id_o, PI64 &loc_id_o, PT &pos_o, PT &old_pos_o );
    void                    unk_new_or_new_unk  ( op_set_cursor    , op_rem_cursor    , UsrId asu_n, BBQ bq_n, BBQ bq_o, DevId &dev_id_n, PI64 &loc_id_n, PT &pos_n, DevId &dev_id_o, PI64 &loc_id_o, PT &pos_o );
    void                    unk_new_or_new_unk  ( op_set_cursor    , op_rem_cursor_und, UsrId asu_n, BBQ bq_n, BBQ bq_o, DevId &dev_id_n, PI64 &loc_id_n, PT &pos_n, DevId &dev_id_o, PI64 &loc_id_o, PT &old_pos_o, UsrId usr_o );
                                                
    void                    unk_new_or_new_unk  ( op_set_cursor_und, op_insert        , UsrId asu_n, BBQ bq_n, BBQ bq_o, DevId &dev_id_n, PI64 &loc_id_n, PT &pos_n, PT &old_pos_n, PT &p_o, CbString &d_o );
    void                    unk_new_or_new_unk  ( op_set_cursor_und, op_remove        , UsrId asu_n, BBQ bq_n, BBQ bq_o, DevId &dev_id_n, PI64 &loc_id_n, PT &pos_n, PT &old_pos_n, PT &p_o, PT &l_o );
    void                    unk_new_or_new_unk  ( op_set_cursor_und, op_remove_und    , UsrId asu_n, BBQ bq_n, BBQ bq_o, DevId &dev_id_n, PI64 &loc_id_n, PT &pos_n, PT &old_pos_n, PT &p_o, CbString &d_o );
    void                    unk_new_or_new_unk  ( op_set_cursor_und, op_new_cursor    , UsrId asu_n, BBQ bq_n, BBQ bq_o, DevId &dev_id_n, PI64 &loc_id_n, PT &pos_n, PT &old_pos_n, DevId &dev_id_o, PI64 &loc_id_o, PT &pos_o );
    void                    unk_new_or_new_unk  ( op_set_cursor_und, op_set_cursor    , UsrId asu_n, BBQ bq_n, BBQ bq_o, DevId &dev_id_n, PI64 &loc_id_n, PT &pos_n, PT &old_pos_n, DevId &dev_id_o, PI64 &loc_id_o, PT &pos_o );
    void                    unk_new_or_new_unk  ( op_set_cursor_und, op_set_cursor_und, UsrId asu_n, BBQ bq_n, BBQ bq_o, DevId &dev_id_n, PI64 &loc_id_n, PT &pos_n, PT &old_pos_n, DevId &dev_id_o, PI64 &loc_id_o, PT &pos_o, PT &old_pos_o );
    void                    unk_new_or_new_unk  ( op_set_cursor_und, op_rem_cursor    , UsrId asu_n, BBQ bq_n, BBQ bq_o, DevId &dev_id_n, PI64 &loc_id_n, PT &pos_n, PT &old_pos_n, DevId &dev_id_o, PI64 &loc_id_o, PT &pos_o );
    void                    unk_new_or_new_unk  ( op_set_cursor_und, op_rem_cursor_und, UsrId asu_n, BBQ bq_n, BBQ bq_o, DevId &dev_id_n, PI64 &loc_id_n, PT &pos_n, PT &old_pos_n, DevId &dev_id_o, PI64 &loc_id_o, PT &old_pos_o, UsrId usr_o );
                                                
    void                    unk_new_or_new_unk  ( op_rem_cursor    , op_new_cursor    , UsrId asu_n, BBQ bq_n, BBQ bq_o, DevId &dev_id_n, PI64 &loc_id_n, DevId &dev_id_o, PI64 &loc_id_o, PT &pos_o );
    void                    unk_new_or_new_unk  ( op_rem_cursor    , op_set_cursor    , UsrId asu_n, BBQ bq_n, BBQ bq_o, DevId &dev_id_n, PI64 &loc_id_n, DevId &dev_id_o, PI64 &loc_id_o, PT &pos_o );
    void                    unk_new_or_new_unk  ( op_rem_cursor    , op_set_cursor_und, UsrId asu_n, BBQ bq_n, BBQ bq_o, DevId &dev_id_n, PI64 &loc_id_n, DevId &dev_id_o, PI64 &loc_id_o, PT &pos_o, PT &old_pos_o );
    void                    unk_new_or_new_unk  ( op_rem_cursor    , op_rem_cursor    , UsrId asu_n, BBQ bq_n, BBQ bq_o, DevId &dev_id_n, PI64 &loc_id_n, DevId &dev_id_o, PI64 &loc_id_o );
    void                    unk_new_or_new_unk  ( op_rem_cursor    , op_rem_cursor_und, UsrId asu_n, BBQ bq_n, BBQ bq_o, DevId &dev_id_n, PI64 &loc_id_n, DevId &dev_id_o, PI64 &loc_id_o, PT &old_pos_o, UsrId usr_o );
                                                
    void                    unk_new_or_new_unk  ( op_rem_cursor_und, op_new_cursor    , UsrId asu_n, BBQ bq_n, BBQ bq_o, DevId &dev_id_n, PI64 &loc_id_n, PT &old_pos_n, UsrId usr_n, DevId &dev_id_o, PI64 &loc_id_o, PT &pos_o );
    void                    unk_new_or_new_unk  ( op_rem_cursor_und, op_set_cursor    , UsrId asu_n, BBQ bq_n, BBQ bq_o, DevId &dev_id_n, PI64 &loc_id_n, PT &old_pos_n, UsrId usr_n, DevId &dev_id_o, PI64 &loc_id_o, PT &pos_o );
    void                    unk_new_or_new_unk  ( op_rem_cursor_und, op_set_cursor_und, UsrId asu_n, BBQ bq_n, BBQ bq_o, DevId &dev_id_n, PI64 &loc_id_n, PT &old_pos_n, UsrId usr_n, DevId &dev_id_o, PI64 &loc_id_o, PT &pos_o, PT &old_pos_o );
    void                    unk_new_or_new_unk  ( op_rem_cursor_und, op_rem_cursor    , UsrId asu_n, BBQ bq_n, BBQ bq_o, DevId &dev_id_n, PI64 &loc_id_n, PT &old_pos_n, UsrId usr_n, DevId &dev_id_o, PI64 &loc_id_o );
    void                    unk_new_or_new_unk  ( op_rem_cursor_und, op_rem_cursor_und, UsrId asu_n, BBQ bq_n, BBQ bq_o, DevId &dev_id_n, PI64 &loc_id_n, PT &old_pos_n, UsrId usr_n, DevId &dev_id_o, PI64 &loc_id_o, PT &old_pos_o, UsrId usr_o );


    //
    using OtWrapper<OtWrapperString>::unk_std_comb;
    void                    unk_std_comb        ( op_insert        , op_insert        , UsrId asu_s, BBQ bq_u, PT &p_s, CbString &d_n, PT &p_u, CbString &d_u );
    void                    unk_std_comb        ( op_insert        , op_remove        , UsrId asu_s, BBQ bq_u, PT &p_s, CbString &d_n, PT &p_u, PT &l_u );
    void                    unk_std_comb        ( op_insert        , op_remove_und    , UsrId asu_s, BBQ bq_u, PT &p_s, CbString &d_n, PT &p_u, CbString &d_u );
    void                    unk_std_comb        ( op_insert        , op_new_cursor    , UsrId asu_s, BBQ bq_u, PT &p_s, CbString &d_n, DevId &dev_id_u, PI64 &loc_id_u, PT &pos_u );
    void                    unk_std_comb        ( op_insert        , op_set_cursor    , UsrId asu_s, BBQ bq_u, PT &p_s, CbString &d_n, DevId &dev_id_u, PI64 &loc_id_u, PT &pos_u );
    void                    unk_std_comb        ( op_insert        , op_set_cursor_und, UsrId asu_s, BBQ bq_u, PT &p_s, CbString &d_n, DevId &dev_id_u, PI64 &loc_id_u, PT &pos_u, PT &old_pos_u );
                  
    void                    unk_std_comb        ( op_remove        , op_insert        , UsrId asu_s, BBQ bq_u, PT &p_s, PT &l_s, PT &p_u, CbString &d_u );
    void                    unk_std_comb        ( op_remove        , op_remove        , UsrId asu_s, BBQ bq_u, PT &p_s, PT &l_s, PT &p_u, PT &l_u );
    void                    unk_std_comb        ( op_remove        , op_remove_und    , UsrId asu_s, BBQ bq_u, PT &p_s, PT &l_s, PT &p_u, CbString &d_u );
    void                    unk_std_comb        ( op_remove        , op_new_cursor    , UsrId asu_s, BBQ bq_u, PT &p_s, PT &l_s, DevId &dev_id_u, PI64 &loc_id_u, PT &pos_u );
    void                    unk_std_comb        ( op_remove        , op_set_cursor    , UsrId asu_s, BBQ bq_u, PT &p_s, PT &l_s, DevId &dev_id_u, PI64 &loc_id_u, PT &pos_u );
    void                    unk_std_comb        ( op_remove        , op_set_cursor_und, UsrId asu_s, BBQ bq_u, PT &p_s, PT &l_s, DevId &dev_id_u, PI64 &loc_id_u, PT &pos_u, PT &old_pos_u );
                  
    void                    unk_std_comb        ( op_remove_und    , op_insert        , UsrId asu_s, BBQ bq_u, PT &p_s, CbString &d_s, PT &p_u, CbString &d_u );
    void                    unk_std_comb        ( op_remove_und    , op_remove        , UsrId asu_s, BBQ bq_u, PT &p_s, CbString &d_s, PT &p_u, PT &l_u );
    void                    unk_std_comb        ( op_remove_und    , op_remove_und    , UsrId asu_s, BBQ bq_u, PT &p_s, CbString &d_s, PT &p_u, CbString &d_u );
    void                    unk_std_comb        ( op_remove_und    , op_new_cursor    , UsrId asu_s, BBQ bq_u, PT &p_s, CbString &d_s, DevId &dev_id_u, PI64 &loc_id_u, PT &pos_u );
    void                    unk_std_comb        ( op_remove_und    , op_set_cursor    , UsrId asu_s, BBQ bq_u, PT &p_s, CbString &d_s, DevId &dev_id_u, PI64 &loc_id_u, PT &pos_u );
    void                    unk_std_comb        ( op_remove_und    , op_set_cursor_und, UsrId asu_s, BBQ bq_u, PT &p_s, CbString &d_s, DevId &dev_id_u, PI64 &loc_id_u, PT &pos_u, PT &old_pos_u );
                  
    void                    unk_std_comb        ( op_set_cursor    , op_new_cursor    , UsrId asu_s, BBQ bq_u, DevId &dev_id_s, PI64 &loc_id_s, PT &pos_s, DevId &dev_id_u, PI64 &loc_id_u, PT &pos_u );
    void                    unk_std_comb        ( op_set_cursor    , op_set_cursor    , UsrId asu_s, BBQ bq_u, DevId &dev_id_s, PI64 &loc_id_s, PT &pos_s, DevId &dev_id_u, PI64 &loc_id_u, PT &pos_u );
    void                    unk_std_comb        ( op_set_cursor    , op_set_cursor_und, UsrId asu_s, BBQ bq_u, DevId &dev_id_s, PI64 &loc_id_s, PT &pos_s, DevId &dev_id_u, PI64 &loc_id_u, PT &pos_u, PT &old_pos_u );
                  
    void                    unk_std_comb        ( op_set_cursor_und, op_new_cursor    , UsrId asu_s, BBQ bq_u, DevId &dev_id_s, PI64 &loc_id_s, PT &pos_s, PT &old_pos_s, DevId &dev_id_u, PI64 &loc_id_u, PT &pos_u );
    void                    unk_std_comb        ( op_set_cursor_und, op_set_cursor    , UsrId asu_s, BBQ bq_u, DevId &dev_id_s, PI64 &loc_id_s, PT &pos_s, PT &old_pos_s, DevId &dev_id_u, PI64 &loc_id_u, PT &pos_u );
    void                    unk_std_comb        ( op_set_cursor_und, op_set_cursor_und, UsrId asu_s, BBQ bq_u, DevId &dev_id_s, PI64 &loc_id_s, PT &pos_s, PT &old_pos_s, DevId &dev_id_u, PI64 &loc_id_u, PT &pos_u, PT &old_pos_u );
                  
    void                    unk_std_comb        ( op_rem_cursor    , op_new_cursor    , UsrId asu_s, BBQ bq_u, DevId &dev_id_s, PI64 &loc_id_s, DevId &dev_id_u, PI64 &loc_id_u, PT &pos_u );
    void                    unk_std_comb        ( op_rem_cursor    , op_set_cursor    , UsrId asu_s, BBQ bq_u, DevId &dev_id_s, PI64 &loc_id_s, DevId &dev_id_u, PI64 &loc_id_u, PT &pos_u );
    void                    unk_std_comb        ( op_rem_cursor    , op_set_cursor_und, UsrId asu_s, BBQ bq_u, DevId &dev_id_s, PI64 &loc_id_s, DevId &dev_id_u, PI64 &loc_id_u, PT &pos_u, PT &old_pos_u );
                  
    void                    unk_std_comb        ( op_rem_cursor_und, op_new_cursor    , UsrId asu_s, BBQ bq_u, DevId &dev_id_s, PI64 &loc_id_s, PT &old_pos_s, UsrId usr_s, DevId &dev_id_u, PI64 &loc_id_u, PT &pos_u );
    void                    unk_std_comb        ( op_rem_cursor_und, op_set_cursor    , UsrId asu_s, BBQ bq_u, DevId &dev_id_s, PI64 &loc_id_s, PT &old_pos_s, UsrId usr_s, DevId &dev_id_u, PI64 &loc_id_u, PT &pos_u );
    void                    unk_std_comb        ( op_rem_cursor_und, op_set_cursor_und, UsrId asu_s, BBQ bq_u, DevId &dev_id_s, PI64 &loc_id_s, PT &old_pos_s, UsrId usr_s, DevId &dev_id_u, PI64 &loc_id_u, PT &pos_u, PT &old_pos_u );


    Rp                     *str;
    Vec<AutoPtr<Cursor>>    cursors;        ///<
    PI64                    cur_cursor_id;
};

} // namespace RiotBase
