#pragma once

#include "../../System/TypeListAppender.h"
#include "../../System/CycleEndedList.h"
#include "../../System/AutoPtr.h"
#include "../../System/PatchId.h"
#include "../Ot/OtWrapper.h"

#include <boost/intrusive/avl_set.hpp>

namespace RiotBase {

///
class OtWrapperMap : public OtWrapper<OtWrapperMap> {
public:
    enum { term_value = true };

    // operation types
    struct op_new_key     { static const char *name() { return "new_key";     } static bool valid( PatchId key_id, const CbString &key_data, const CbString &val_data ) { return key_id; } };
    struct op_rem_key     { static const char *name() { return "rem_key";     } static bool valid( PatchId key_id                                                     ) { return key_id; } };
    struct op_rem_key_und { static const char *name() { return "rem_key_und"; } static bool valid( PatchId key_id, const CbString &key_data, const CbString &val_data ) { return key_id; } };
    struct op_mod_key     { static const char *name() { return "mod_key";     } static bool valid( PatchId key_id, const CbString &key_data                           ) { return key_id; } };
    struct op_mod_val     { static const char *name() { return "mod_val";     } static bool valid( PatchId key_id, const CbString &val_data                           ) { return key_id; } };
    using OperationTypes = typename TypeListAppender<typename OtWrapper<OtWrapperMap>::OtWrapperOperationTypes,op_new_key,op_rem_key,op_rem_key_und,op_mod_key,op_mod_val>::T;

    // rights types
    struct can_add_key { static const char *name() { return "add_key" ; } };
    struct can_rem_key { static const char *name() { return "rem_key" ; } };
    using RightTypes = typename TypeListAppender<typename OtWrapper<OtWrapperMap>::OtWrapperRightTypes,can_add_key,can_rem_key>::T;

    // items of the map are stored with absolute ids
    struct Node {
        struct ICmp {
            bool operator()( const Node    &a, const Node    &b ) const { return a.id < b.id; }
            bool operator()( const Node    &a, const PatchId &b ) const { return a.id < b   ; }
            bool operator()( const PatchId &a, const Node    &b ) const { return a    < b.id; }
        };
        struct KCmp {
            bool operator()( const Node &a, const Node &b ) const { return RO_CALL( inf_immediate, a.key, b.key ); }
            bool operator()( const Node &a, const Rp   *b ) const { return RO_CALL( inf_immediate, a.key, b     ); }
            bool operator()( const Rp   *a, const Node &b ) const { return RO_CALL( inf_immediate, a    , b.key ); }
        };
        using AvlMemberHook = boost::intrusive::avl_set_member_hook<>;

        Node() : prev( 0 ) {
        }

        ~Node() {
            RO_CALL( del, key );
            RO_CALL( del, val );
        }

        AvlMemberHook member_hook_key;
        AvlMemberHook member_hook_id;
        Node         *prev;            ///< in modified items
        Rp           *key;
        Rp           *val;
        PatchId       id;
    };

    using KMap = boost::intrusive::avl_multiset<Node,
        boost::intrusive::compare<Node::KCmp>,
        boost::intrusive::member_hook<
            Node,
            boost::intrusive::avl_set_member_hook<>,
            &Node::member_hook_key
        >
    >;

    using IMap = boost::intrusive::avl_multiset<Node,
        boost::intrusive::compare<Node::ICmp>,
        boost::intrusive::member_hook<
            Node,
            boost::intrusive::avl_set_member_hook<>,
            &Node::member_hook_id
        >
    >;

    using MapMerger = std::map<PatchId,std::pair<AutoPtr<OtMerger>,Node *>>;

    // methods
    OtWrapperMap( const Category *category_map, const Category *category_key, const Category *category_val, UsrId usr_id );
    ~OtWrapperMap();

    static std::string   type_name               ();
    void                 write_to_stream         ( std::ostream &os ) const;
    const Category      *category                () const;
    Rp                  *get_size                () const;
    Rp                  *copy                    () const;
                                                 
    Rp                  *mod_select              ( const Rp *key );
    Rp                  *select_bb               ( const Rp *key ) const;
    void                 add_key                 ( const Rp *key );
    void                 rem_key                 ( const Rp *key );

    // helpers
    void                 sig_change_val          ( Node *n );
    void                 sig_change_key          ( Node *n );
    Node                *add_item_wocr           ( PatchId id, Rp *key, Rp *val );
    void                 rem_item_wocr           ( PatchId id );
    void                 rem_item_wocr           ( Node *n );
                                                 
    // OT surdefs                                
    void                 freeze_last_changes     ();
    void                 ot_merger_finalize      ();

    // operations
    using OtWrapper<OtWrapperMap>::apply_op;
    void                 apply_op                ( op_new_key    , Aod &aod, PatchId key_id, CbString &key_data, CbString &val_data );
    void                 apply_op                ( op_rem_key    , Aod &aod, PatchId key_id                                         );
    void                 apply_op                ( op_rem_key_und, Aod &aod, PatchId key_id, CbString &key_data, CbString &val_data );
    void                 apply_op                ( op_mod_key    , Aod &aod, PatchId key_id, CbString &key_data                     );
    void                 apply_op                ( op_mod_val    , Aod &aod, PatchId key_id, CbString &val_data                     );

    // rights
    using OtWrapper<OtWrapperMap>::right_to;
    bool                 right_to                ( op_new_key    , UsrId as_usr, PatchId key_id, CbString &key_data, CbString &val_data ) { return this->has_right( can_add_key{}, as_usr ); }
    bool                 right_to                ( op_rem_key    , UsrId as_usr, PatchId key_id                                         ) { return this->has_right( can_rem_key{}, as_usr ); }
    bool                 right_to                ( op_rem_key_und, UsrId as_usr, PatchId key_id, CbString &key_data, CbString &val_data ) { return right_to( op_rem_key{}, as_usr, key_id ); }
    bool                 right_to                ( op_mod_key    , UsrId as_usr, PatchId key_id, CbString &key_data                     ) { return true; }
    bool                 right_to                ( op_mod_val    , UsrId as_usr, PatchId key_id, CbString &val_data                     ) { return true; }

    // undo
    using OtWrapper<OtWrapperMap>::undo_patch;
    void                 undo_patch              ( op_new_key    , Aod &aod, PatchId key_id, CbString &key_data, CbString &val_data );
    void                 undo_patch              ( op_rem_key    , Aod &aod, PatchId key_id                                         );
    void                 undo_patch              ( op_rem_key_und, Aod &aod, PatchId key_id, CbString &key_data, CbString &val_data );
    void                 undo_patch              ( op_mod_key    , Aod &aod, PatchId key_id, CbString &key_data                     );
    void                 undo_patch              ( op_mod_val    , Aod &aod, PatchId key_id, CbString &key_data                     );

    // transmission
    using OtWrapper<OtWrapperMap>::transmission;
    void                 transmission            ( op_rem_key_und, BBQ br, PatchId key_id, CbString &key_data, CbString &val_data );

    // to create _und version when there is a non und one
    using OtWrapper<OtWrapperMap>::register_patch_op;
    void                 register_patch_op       ( op_rem_key, BBQ bq, PatchId key_id );

    OtMerger            *get_key_merger          ( PatchId key_id );
    OtMerger            *get_val_merger          ( PatchId key_id );

    const Category      *category_map;
    const Category      *category_key;
    const Category      *category_val;
    PI64                 loc_id_num;
    KMap                 kmap;
    IMap                 imap;

    CycleEndedList<Node> new_nodes;
    CycleEndedList<Node> mod_nodes;

    MapMerger            key_mergers; // tmp room for PatchMerger * for modified keys (during merges, we need to keep the corresponding OtMergers alive)
    MapMerger            val_mergers; // tmp room for PatchMerger * for modified vals (during merges, we need to keep the corresponding OtMergers alive)
};


} // namespace RiotBase
