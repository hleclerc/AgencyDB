#pragma once

#include <boost/intrusive/avl_set.hpp>
#include "../../System/CycleEndedList.h"
#include "../Core/Callback.h"
#include "../Core/RpTempl.h"
#include <map>

namespace RiotBase {

///
class GenMap : public RpTempl<GenMap> {
public:
    enum { term_value = true };

    static constexpr PI8 NEW_NODE = 0;
    static constexpr PI8 MOD_KEY  = 1;
    static constexpr PI8 MOD_VAL  = 2;

    struct Node {
        struct ICmp {
            bool operator()( const Node  &a, const Node  &b ) const { return a.id < b.id; }
            bool operator()( const Node  &a, const Inode &b ) const { return a.id < b   ; }
            bool operator()( const Inode &a, const Node  &b ) const { return a    < b.id; }
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

        AvlMemberHook        member_hook_key;
        AvlMemberHook        member_hook_id;
        Vec<RcPtr<Callback>> on_change_keys;
        Vec<RcPtr<Callback>> on_change_vals;
        Node                *prev;
        Rp                  *key;
        Rp                  *val;
        Inode                id;
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

    // rights
    enum {
        #define DECL_RIGHT( R ) num_RIGHT_TO_##R,
        #include "rightsdecl.h"
        #undef DECL_RIGHT
        INVALID_RIGHT = 666
    };
    #define DECL_RIGHT( R ) static constexpr PI8 RIGHT_TO_##R = 1 << unsigned( num_RIGHT_TO_##R );
    #include "rightsdecl.h"
    #undef DECL_RIGHT

    static constexpr unsigned base_rights =
            0
            #define DECL_RIGHT( R ) + RIGHT_TO_##R
            #include "rightsdecl.h"
            #undef DECL_RIGHT
            ;

    using RightList = std::map<Inode,unsigned>;

    
    GenMap( const Category *category_map, const Category *category_key, const Category *category_val, Inode crea_usr = {} );
    ~GenMap();

    static std::string   type_name          ();
    void                 write_to_stream    ( std::ostream &os ) const;
    const Category      *category           () const;
    void                 self_on_change     ( Callback *c );
    PatchMerger         *new_patch_merger   ();
    Rp                  *get_size           () const;
    Rp                  *copy               () const;
                                            
    Rp                  *mod_select         ( const Rp *key, UsrId as_usr );
    Rp                  *select_bb          ( const Rp *key ) const;
    void                 add_key            ( const Rp *key );
    void                 get_changes        ( CbQueue &cq, const FqId &id );
    void                 sig_change_val     ( Node *n );
    void                 sig_change_key     ( Node *n );
    void                 sig_change         ();
    Node                *add_item           ( Inode id, Rp *key, Rp *val );

    Vec<std::string>     get_possible_rights() const;
    Vec<GrpId>           get_groups_in_acl  () const;
    Vec<UsrId>           get_users_in_acl   () const;
    Vec<std::string>     get_grp_rights   ( GrpId grp ) const;
    Vec<std::string>     get_usr_rights    ( UsrId usr ) const;
    bool                 add_grp_right    ( GrpId grp, const std::string &right );
    bool                 add_usr_right     ( UsrId usr, const std::string &right );
    bool                 rem_grp_right    ( GrpId grp, const std::string &right );
    bool                 rem_usr_right     ( UsrId usr, const std::string &right );

    
    const Category      *category_map;
    const Category      *category_key;
    const Category      *category_val;
    PI64                 loc_id_num;
    KMap                 kmap;
    IMap                 imap;

    CycleEndedList<Node> new_nodes;
    CycleEndedList<Node> mod_vals;
    CycleEndedList<Node> mod_keys;

    Vec<RcPtr<Callback>> on_change_list;
    
    RightList            usr_rights;
    RightList            grp_rights;
};

} // namespace RiotBase
