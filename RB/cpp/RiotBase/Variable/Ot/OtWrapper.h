#pragma once

#include "../../System/ApplyVa.h"
#include "../../System/NbArgs.h"
#include "../../System/Unref.h"
#include "../Core/Callback.h"
#include "../Core/RpTempl.h"
#include "OtMerger.h"
#include <map>

namespace RiotBase {

///
template<class T>
class OtWrapper : public RpTempl<T> {
public:
    // common operation types
    struct op_add_grp_right { static const char *name() { return "add_grp_right"; } static bool valid( GrpId, unsigned v ) { return v; } };
    struct op_rem_grp_right { static const char *name() { return "rem_grp_right"; } static bool valid( GrpId, unsigned v ) { return v; } };
    struct op_add_usr_right { static const char *name() { return "add_usr_right"; } static bool valid( UsrId, unsigned v ) { return v; } };
    struct op_rem_usr_right { static const char *name() { return "rem_usr_right"; } static bool valid( UsrId, unsigned v ) { return v; } };
    using OtWrapperOperationTypes = TypeList<op_add_usr_right,op_rem_usr_right,op_add_grp_right,op_rem_grp_right>;

    // common right types
    struct can_add_rights { static const char *name() { return "add_rights"; } };
    struct can_rem_rights { static const char *name() { return "rem_rights"; } };
    using OtWrapperRightTypes = TypeList<can_add_rights,can_rem_rights>;

    // typedefs
    using UsrRightList         = std::map<UsrId,unsigned>;
    using GrpRightList         = std::map<GrpId,unsigned>;

    struct Aod {
        enum { NEW_PATCH, UNK_PATCH, STD_PATCH, UNDO };
        UsrId as_usr;
        int   type;
    };

    // Rp methods
    OtWrapper( UsrId usr_id = {} ) { usr_rights[ usr_id ] = ( 1u << T::RightTypes::size ) - 1; }

    Rp                     *make_ot_wrapper     () { return this; }
    void                    pull_ot_changes     ( BBQ changes ) { static_cast<T *>( this )->freeze_last_changes(); changes.write_some( std::move( last_ops ) ); }
    void                    write_ot_data       ( std::ostream &os, CbStringPtr data ) { int cpt = 0; patch_data_visitor( BinStream<CbStringPtr>( &data ), [&]( auto op, auto&& ...args ) { os << ( cpt++ ? " " : "" ) << op.name() << "("; int cpt_val = 0; apply_va( [ & ]( auto &&val ) { os << ( cpt_val++ ? ",": "" ) << std::forward<decltype(val)>( val ); }, std::forward<decltype(args)>( args )... ); os << ")"; } ); }

    Vec<std::string>        get_possible_rights () const { Vec<std::string> res; T::RightTypes::apply( [&]( auto st ) { res << decltype( st )::T::name(); } ); return res; }
    Vec<GrpId>              get_groups_in_acl   () const { Vec<GrpId> res; for( auto p : grp_rights ) res << p.first; return res; }
    Vec<UsrId>              get_users_in_acl    () const { Vec<UsrId> res; for( auto p : usr_rights ) res << p.first; return res; }

    Vec<std::string>        get_grp_rights      ( GrpId grp ) const { Vec<std::string> res; unsigned r = grp_right( grp ); T::RightTypes::apply( [&]( auto st ) { if ( r & 1 ) res << decltype( st )::T::name(); r >>= 1; } ); return res; }
    Vec<std::string>        get_usr_rights      ( UsrId usr ) const { Vec<std::string> res; unsigned r = usr_right( usr ); T::RightTypes::apply( [&]( auto st ) { if ( r & 1 ) res << decltype( st )::T::name(); r >>= 1; } ); return res; }
    bool                    add_grp_right       ( GrpId grp, const std::string &right ) { PT n = num_right( right ); return n < T::RightTypes::size ? _add_grp_right( grp, 1u << n ) : false; }
    bool                    add_usr_right       ( UsrId usr, const std::string &right ) { PT n = num_right( right ); return n < T::RightTypes::size ? _add_usr_right( usr, 1u << n ) : false; }
    bool                    rem_grp_right       ( GrpId grp, const std::string &right ) { PT n = num_right( right ); return n < T::RightTypes::size ? _rem_grp_right( grp, 1u << n ) : false; }
    bool                    rem_usr_right       ( UsrId usr, const std::string &right ) { PT n = num_right( right ); return n < T::RightTypes::size ? _rem_usr_right( usr, 1u << n ) : false; }

    void                    sig_change          () { for( RcPtr<Callback> &c : on_change_list ) c->reg_wait(); }
    void                    self_on_change      ( Callback *c ) { on_change_list << c; c->reg_wait(); }

    OtMerger               *new_ot_merger       ();

    // methods that may be surdefined in T
    void                    freeze_last_changes () {}

    // ot operations
    template<class Op>
    static PI8              num_op              () { constexpr PT res = T::OperationTypes::template index_of<Op>(); static_assert( res < 256, "too much operations" ); static_assert( res < T::OperationTypes::size, "Operation not registered" ); return res; }

    template<class Op,class... Args>
    void                    reg_op              ( const Op &op, Args&& ...args ) { reg_op_in( &last_ops, op, std::forward<Args>( args )... ); }

    template<class Op,class... Args>
    static void             reg_op_in           ( BBQ bq, const Op &op, Args&& ...args );

    template<class Cq,class Op>
    static bool             patch_data_visitor  ( BinStream<Cq> bq, const Op &op ); ///< return true if reading was ok

    template<class Op>
    static bool             reverse_patch_data_visitor  ( CbStringPtr bq, const Op &op ); ///< return true if reading was ok

    template<class Rt>
    static PI8              num_right           () { constexpr PT res = T::RightTypes::template index_of<Rt>(); static_assert( res < 8 * sizeof( unsigned ), "too much operations" ); static_assert( res < T::RightTypes::size, "Operation not registered" ); return res; }

    static PT               num_right           ( const std::string &name ) { return T::RightTypes::find( [&]( auto st ) { return decltype( st )::T::name() == name; } ); }

    template<class Rt>
    bool                    has_right           ( Rt, UsrId as_usr = {} ) const { return usr_right( as_usr ) & ( 1u << num_right<Rt>() ); }

    unsigned                grp_right           ( GrpId grp ) const { auto iter = grp_rights.find( grp ); return iter == grp_rights.end() ? 0 : iter->second; }
    unsigned                usr_right           ( UsrId usr ) const { auto iter = usr_rights.find( usr ); return iter == usr_rights.end() ? 0 : iter->second; }
    bool                    _add_grp_right      ( GrpId grp, unsigned flags );
    bool                    _add_usr_right      ( UsrId usr, unsigned flags );
    bool                    _rem_grp_right      ( GrpId grp, unsigned flags );
    bool                    _rem_usr_right      ( UsrId usr, unsigned flags );

    // update_patch_data_l0: what to do when new (or unk) data comes
    template<class Op,class... Args>
    void                    update_patch_data_l0( Op, Args& ...args ) {}

    void                    update_patch_data_l0( op_add_grp_right, GrpId &grp, unsigned &flags ) { flags &= ~ grp_right( grp ); }
    void                    update_patch_data_l0( op_rem_grp_right, GrpId &grp, unsigned &flags ) { flags &= grp_right( grp ); }
    void                    update_patch_data_l0( op_add_usr_right, UsrId &usr, unsigned &flags ) { flags &= ~ usr_right( usr ); }
    void                    update_patch_data_l0( op_rem_usr_right, UsrId &usr, unsigned &flags ) { flags &= usr_right( usr ); }

    // right_to
    bool                    right_to             ( op_add_grp_right, UsrId as_usr, GrpId, unsigned ) { return has_right( can_add_rights{}, as_usr ); }
    bool                    right_to             ( op_rem_grp_right, UsrId as_usr, GrpId, unsigned ) { return has_right( can_rem_rights{}, as_usr ); }
    bool                    right_to             ( op_add_usr_right, UsrId as_usr, UsrId, unsigned ) { return has_right( can_add_rights{}, as_usr ); }
    bool                    right_to             ( op_rem_usr_right, UsrId as_usr, UsrId, unsigned ) { return has_right( can_rem_rights{}, as_usr ); }

    // apply_op
    void                    apply_op             ( op_add_grp_right, Aod &aod, GrpId, unsigned );
    void                    apply_op             ( op_rem_grp_right, Aod &aod, GrpId, unsigned );
    void                    apply_op             ( op_add_usr_right, Aod &aod, UsrId, unsigned );
    void                    apply_op             ( op_rem_usr_right, Aod &aod, UsrId, unsigned );

    // unk_new_or_new_unk
    template<class Op_0,class Op_1,class... Args>
    void                    unk_new_or_new_unk   ( Op_0, Op_1, UsrId asu_n, BBQ bq_0, BBQ bq_1, Args& ...args ) {}

    void                    unk_new_or_new_unk   ( op_add_grp_right, op_add_grp_right, UsrId asu_n, BBQ bq_n, BBQ bq_o, GrpId grp_n, unsigned &flags_n, GrpId grp_o, unsigned &flags_o ) { if ( grp_o == grp_n ) flags_o &= ~ flags_n; }
    void                    unk_new_or_new_unk   ( op_add_grp_right, op_rem_grp_right, UsrId asu_n, BBQ bq_n, BBQ bq_o, GrpId grp_n, unsigned &flags_n, GrpId grp_o, unsigned &flags_o ) { if ( grp_o == grp_n ) flags_o &= ~ flags_n; }
    void                    unk_new_or_new_unk   ( op_rem_grp_right, op_add_grp_right, UsrId asu_n, BBQ bq_n, BBQ bq_o, GrpId grp_n, unsigned &flags_n, GrpId grp_o, unsigned &flags_o ) { if ( grp_o == grp_n ) flags_o &= ~ flags_n; }
    void                    unk_new_or_new_unk   ( op_rem_grp_right, op_rem_grp_right, UsrId asu_n, BBQ bq_n, BBQ bq_o, GrpId grp_n, unsigned &flags_n, GrpId grp_o, unsigned &flags_o ) { if ( grp_o == grp_n ) flags_o &= ~ flags_n; }
    void                    unk_new_or_new_unk   ( op_add_usr_right, op_add_usr_right, UsrId asu_n, BBQ bq_n, BBQ bq_o, UsrId usr_n, unsigned &flags_n, UsrId usr_o, unsigned &flags_o ) { if ( usr_o == usr_n ) flags_o &= ~ flags_n; }
    void                    unk_new_or_new_unk   ( op_add_usr_right, op_rem_usr_right, UsrId asu_n, BBQ bq_n, BBQ bq_o, UsrId usr_n, unsigned &flags_n, UsrId usr_o, unsigned &flags_o ) { if ( usr_o == usr_n ) flags_o &= ~ flags_n; }
    void                    unk_new_or_new_unk   ( op_rem_usr_right, op_add_usr_right, UsrId asu_n, BBQ bq_n, BBQ bq_o, UsrId usr_n, unsigned &flags_n, UsrId usr_o, unsigned &flags_o ) { if ( usr_o == usr_n ) flags_o &= ~ flags_n; }
    void                    unk_new_or_new_unk   ( op_rem_usr_right, op_rem_usr_right, UsrId asu_n, BBQ bq_n, BBQ bq_o, UsrId usr_n, unsigned &flags_n, UsrId usr_o, unsigned &flags_o ) { if ( usr_o == usr_n ) flags_o &= ~ flags_n; }

    // unk_std_comb
    template<class Op_s,class Op_u,class... Args>
    void                    unk_std_comb         ( Op_s, Op_u, UsrId asu_s, BBQ bq_u, Args& ...args ) {}

    void                    unk_std_comb         ( op_add_grp_right, op_add_grp_right, UsrId asu_s, BBQ bq_u, GrpId grp_s, unsigned &flags_s, GrpId grp_u, unsigned &flags_u ) { if ( grp_s == grp_u ) flags_u &= ~ flags_s; }
    void                    unk_std_comb         ( op_add_grp_right, op_rem_grp_right, UsrId asu_s, BBQ bq_u, GrpId grp_s, unsigned &flags_s, GrpId grp_u, unsigned &flags_u ) { if ( grp_s == grp_u ) flags_u &= ~ flags_s; }
    void                    unk_std_comb         ( op_rem_grp_right, op_add_grp_right, UsrId asu_s, BBQ bq_u, GrpId grp_s, unsigned &flags_s, GrpId grp_u, unsigned &flags_u ) { if ( grp_s == grp_u ) flags_u &= ~ flags_s; }
    void                    unk_std_comb         ( op_rem_grp_right, op_rem_grp_right, UsrId asu_s, BBQ bq_u, GrpId grp_s, unsigned &flags_s, GrpId grp_u, unsigned &flags_u ) { if ( grp_s == grp_u ) flags_u &= ~ flags_s; }
    void                    unk_std_comb         ( op_add_usr_right, op_add_usr_right, UsrId asu_s, BBQ bq_u, UsrId usr_s, unsigned &flags_s, UsrId usr_u, unsigned &flags_u ) { if ( usr_s == usr_u ) flags_u &= ~ flags_s; }
    void                    unk_std_comb         ( op_add_usr_right, op_rem_usr_right, UsrId asu_s, BBQ bq_u, UsrId usr_s, unsigned &flags_s, UsrId usr_u, unsigned &flags_u ) { if ( usr_s == usr_u ) flags_u &= ~ flags_s; }
    void                    unk_std_comb         ( op_rem_usr_right, op_add_usr_right, UsrId asu_s, BBQ bq_u, UsrId usr_s, unsigned &flags_s, UsrId usr_u, unsigned &flags_u ) { if ( usr_s == usr_u ) flags_u &= ~ flags_s; }
    void                    unk_std_comb         ( op_rem_usr_right, op_rem_usr_right, UsrId asu_s, BBQ bq_u, UsrId usr_s, unsigned &flags_s, UsrId usr_u, unsigned &flags_u ) { if ( usr_s == usr_u ) flags_u &= ~ flags_s; }

    // transmission
    template<class Op,class... Args>
    void                    transmission         ( Op op, BBQ glo, Args&& ...args ) { /*PRINT( __PRETTY_FUNCTION__ ); */reg_op_in( glo, op, std::forward<decltype(args)>( args )... ); }

    //
    template<class Op,class... Args>
    void                    register_patch_op    ( Op op, BBQ bq, Args&& ...args ) { reg_op_in( bq, op, std::forward<Args>( args )... ); }

    // undo_patch
    void                    undo_patch           ( op_add_grp_right, Aod &aod, GrpId grp, unsigned flags ) { apply_op( op_rem_grp_right{}, aod, grp, flags ); }
    void                    undo_patch           ( op_rem_grp_right, Aod &aod, GrpId grp, unsigned flags ) { apply_op( op_add_grp_right{}, aod, grp, flags ); }
    void                    undo_patch           ( op_add_usr_right, Aod &aod, UsrId usr, unsigned flags ) { apply_op( op_rem_usr_right{}, aod, usr, flags ); }
    void                    undo_patch           ( op_rem_usr_right, Aod &aod, UsrId usr, unsigned flags ) { apply_op( op_add_usr_right{}, aod, usr, flags ); }

    //
    void                    ot_merger_finalize   () {}

    // attributes
    Vec<RcPtr<Callback>>    on_change_list; ///< callbacks
    UsrRightList            usr_rights;
    GrpRightList            grp_rights;
    CbQueue                 last_ops;       ///< unregistered patch data
};


// -------------------------------------------------------------------------------------
// implementation
// -------------------------------------------------------------------------------------

template<class T>
template<class Op,class... Args>
void OtWrapper<T>::reg_op_in( BBQ bq, const Op &op, Args&& ...args ) {
    constexpr PT nargs = NbArgs<decltype(Op::valid)>::value;
    // operation id
    bq << num_op<Op>();

    // arguments
    auto n = apply_van( [ & ]( auto &&arg, auto n ) {
        static_assert( n.val < nargs, "too much arguments" );
        write_as( bq, TypeList<typename Unref<typename ArgType<decltype(Op::valid),n.val>::T>::T>(), std::forward<decltype(arg)>( arg ) );
    }, std::forward<Args>( args )... );

    // check nb args was correct
    static_assert( n.val == nargs, "missing arguments" );
}

template<class T>
template<class Cq,class Op>
bool OtWrapper<T>::patch_data_visitor( BinStream<Cq> bq, const Op &op ) {
    while ( not bq.empty() ) {
        PI8 code = bq.read_byte();
        if ( bq.error() )
            return false;

        // find type corresponding to `code`
        bool res = T::OperationTypes::apply( [&]( auto ot ){
            using Ot = typename decltype( ot )::T;
            ArgsAsTypeList<decltype(Ot::valid)>::T::map( [&]( auto st ) {
                typename decltype( st )::T res = bq.read();
                return res;
            }, [&]( auto&& ...args ) {
                op( Ot{}, std::forward<decltype(args)>( args )... );
            } );
        }, code );
        if ( not res )
            return false;
    }
    return true;
}

template<class T>
template<class Op>
bool OtWrapper<T>::reverse_patch_data_visitor( CbStringPtr cq_data, const Op &op ) {
    Vec<CbStringPtr> positions; positions.reserve( cq_data.size() / 10 );
    BinStream<CbStringPtr> data( &cq_data );

    // forward
    while ( not data.empty() ) {
        positions << *data.buf;

        // type num
        PI8 code = data.read_byte();
        if ( data.error() )
            return false;

        // skip data
        bool res = T::OperationTypes::apply( [&]( auto ot ){
            using Ot = typename decltype( ot )::T;
            ArgsAsTypeList<decltype(Ot::valid)>::T::apply( [&]( auto st ) {
                data.skip( st );
            } );
        }, code );
        if ( not res )
            return false;
    }

    // backward
    for( PT np = positions.size(); np--; ) {
        CbStringPtr cb = positions[ np ];
        BinStream<CbStringPtr> data( &cb );
        PI8 code = data.read_byte();

        // find type corresponding to `code`
        T::OperationTypes::apply( [&]( auto ot ){
            using Ot = typename decltype( ot )::T;
            ArgsAsTypeList<decltype(Ot::valid)>::T::map( [&]( auto st ) {
                typename decltype( st )::T res = data.read();
                return res;
            }, [&]( auto&& ...args ) {
                op( Ot{}, std::forward<decltype(args)>( args )... );
            } );
        }, code );
    }

    return true;
}

template<class T>
bool OtWrapper<T>::_add_grp_right( GrpId grp, unsigned flags ) {
    if ( not has_right( can_add_rights{} ) or not flags )
        return false;

    auto iter = grp_rights.find( grp );
    if ( iter == grp_rights.end() )
        iter = grp_rights.insert( iter, std::pair<GrpId,unsigned>( grp, 0u ) );

    flags &= ~ iter->second;
    if ( flags ) {
        reg_op( op_add_grp_right{}, grp, flags );
        iter->second |= flags;
        static_cast<T *>( this )->sig_change();
    }
    return true;
}

template<class T>
bool OtWrapper<T>::_add_usr_right( UsrId usr, unsigned flags ) {
    if ( not has_right( can_add_rights{} ) or not flags )
        return false;

    auto iter = usr_rights.find( usr );
    if ( iter == usr_rights.end() )
        iter = usr_rights.insert( iter, std::pair<UsrId,unsigned>( usr, 0u ) );

    flags &= ~ iter->second;
    if ( flags ) {
        reg_op( op_add_usr_right{}, usr, flags );
        iter->second |= flags;
        static_cast<T *>( this )->sig_change();
    }
    return true;
}

template<class T>
bool OtWrapper<T>::_rem_grp_right( GrpId grp, unsigned flags ) {
    if ( not has_right( can_rem_rights{} ) or not flags )
        return false;

    auto iter = grp_rights.find( grp );
    if ( iter == grp_rights.end() )
        return true;

    flags &= iter->second;
    if ( flags ) {
        reg_op( op_rem_grp_right{}, grp, flags );
        iter->second &= ~flags;
        static_cast<T *>( this )->sig_change();
    }
    return true;
}

template<class T>
bool OtWrapper<T>::_rem_usr_right( UsrId usr, unsigned flags ) {
    if ( not has_right( can_rem_rights{} ) or not flags )
        return false;

    auto iter = usr_rights.find( usr );
    if ( iter == usr_rights.end() )
        return true;

    flags &= iter->second;
    if ( flags ) {
        reg_op( op_rem_usr_right{}, usr, flags );
        iter->second &= ~flags;
        static_cast<T *>( this )->sig_change();
    }
    return true;
}

template<class T>
OtMerger *OtWrapper<T>::new_ot_merger() {
    struct MyMerger : OtMerger {
        MyMerger( T *pc ) : pc( pc ) {}
        virtual ~MyMerger() {}

        virtual bool glo_to_loc( BBQ loc, CbStringPtr data, DevId cur_dev, UsrId cur_usr ) const {
            return pc->patch_data_visitor( BinStream<CbStringPtr>( &data ), [&]( auto op, auto&& ...args ) {
                apply_va( [&]( auto &arg ) { RiotBase::glo_to_loc( arg, cur_dev, cur_usr ); }, args... );
                pc->reg_op_in( loc, op, std::forward<decltype(args)>( args )... );
            } );
        }

        virtual bool loc_to_glo( BBQ glo, CbStringPtr data, DevId cur_dev, UsrId cur_usr ) const {
            return pc->patch_data_visitor( BinStream<CbStringPtr>( &data ), [&]( auto op, auto&& ...args ) {
                apply_va( [&]( auto &arg ) { RiotBase::loc_to_glo( arg, cur_dev, cur_usr ); }, args... );
                pc->transmission( op, glo, args... );
            } );
        }

        virtual void new_patch( BBQ res, CbStringPtr msg, UsrId as_usr, PT nb_rem_unk, PT nb_rem_std ) {
            CbQueue tmp_res; // res may contain data...
            pc->patch_data_visitor( BinStream<CbStringPtr>( &msg ), [&]( auto op_new, auto&& ...args_new ) {
                pc->update_patch_data_l0( op_new, args_new... );

                // 1) update data according to cq_unk; 2) make cq_unk'
                CbQueue updated_cq_unk;
                pc->patch_data_visitor( BinStream<CbQueue>( &cq_unk ), [&]( auto op_unk, auto&& ...args_unk ){
                    pc->unk_new_or_new_unk( op_new, op_unk, as_usr, &tmp_res, &updated_cq_unk, args_new..., args_unk... );
                    if ( op_unk.valid( args_unk... ) )
                        pc->reg_op_in( BinStream<CbQueue>( &updated_cq_unk ), op_unk, std::forward<decltype( args_unk )>( args_unk )... );
                } );

                // check validity and rights to register and apply the operation
                if ( op_new.valid( args_new... ) and pc->right_to( op_new, as_usr, args_new... ) ) {
                    // register operations
                    pc->register_patch_op( op_new, BBQ( &tmp_res ), args_new... );
                    cq_unk = std::move( updated_cq_unk );

                    // apply operation
                    Aod aod{ as_usr, Aod::NEW_PATCH };
                    pc->apply_op( op_new, aod, args_new... );
                }
            } );

            if ( nb_rem_unk or nb_rem_std ) // if there are unk or std patches, we need a representation of the preceding new stuff to modify the corresponding data
                cq_new.write_some( tmp_res );
            res.write_some( tmp_res );
        }
        virtual void unk_patch( BBQ res, CbStringPtr msg, UsrId as_usr, PT nb_rem_new, PT nb_rem_std ) {
            CbQueue tmp_res; // res may contain data...
            pc->patch_data_visitor( BinStream<CbStringPtr>( &msg ), [&]( auto op_unk, auto&& ...args_unk ) {
                pc->update_patch_data_l0( op_unk, args_unk... );

                // 1) update data according to cq_new; 2) make cq_new'
                CbQueue updated_cq_new;
                pc->patch_data_visitor( BinStream<CbQueue>( &cq_new ), [&]( auto op_new, auto&& ...args_new ){
                    pc->unk_new_or_new_unk( op_unk, op_new, as_usr, &tmp_res, &updated_cq_new, args_unk..., args_new... );
                    if ( op_new.valid( args_new... ) )
                        pc->reg_op_in( BinStream<CbQueue>( &updated_cq_new ), op_new, std::forward<decltype( args_new )>( args_new )... );
                } );

                // check validity and rights to register and apply the operation
                if ( op_unk.valid( args_unk... ) and pc->right_to( op_unk, as_usr, args_unk... ) ) {
                    // register operations
                    pc->reg_op_in( BBQ( &tmp_res ), op_unk, args_unk... );
                    cq_new = std::move( updated_cq_new );

                    // apply operation
                    Aod aod{ as_usr, Aod::UNK_PATCH };
                    pc->apply_op( op_unk, aod, args_unk... );
                }
            } );

            if ( nb_rem_new or nb_rem_std ) // if there are unk or std patches, we need a representation of the preceding new stuff to modify the corresponding data
                cq_unk.write_some( tmp_res );
            res.write_some( tmp_res );
        }
        virtual void std_patch( BBQ res, CbStringPtr msg, UsrId as_usr, PT nb_rem_unk, PT nb_rem_new ) {
            // goal:
            // - update std data according to new stuff
            // - make new' (new after std)
            // - make the operations
            CbQueue tmp_res; // res may contain data...
            pc->patch_data_visitor( BinStream<CbStringPtr>( &msg ), [&]( auto op_std, auto&& ...args_std ) {
                pc->update_patch_data_l0( op_std, args_std... );

                // modifications
                CbQueue updated_cq_new;
                pc->patch_data_visitor( BinStream<CbQueue>( &cq_new ), [&]( auto op_new, auto&& ...args_new ) {
                    pc->unk_new_or_new_unk( op_std, op_new, as_usr, &tmp_res, &updated_cq_new, args_std..., args_new... );
                    if ( op_new.valid( args_new... ) )
                        pc->reg_op_in( BinStream<CbQueue>( &updated_cq_new ), op_new, std::forward<decltype( args_new )>( args_new )... );
                } );

                // check validity + rights
                if ( op_std.valid( args_std... ) and pc->right_to( op_std, as_usr, args_std... ) ) {
                    // register operations (in merger->cq_unk, ...)
                    pc->reg_op_in( BBQ( &tmp_res ), op_std, args_std... );
                    cq_new = std::move( updated_cq_new );

                    // apply operation
                    Aod aod{ as_usr, Aod::STD_PATCH };
                    pc->apply_op( op_std, aod, args_std... );
                }
            } );

            // goal:
            // - make unk' (unk after std)
            CbStringPtr data = tmp_res;
            pc->patch_data_visitor( BinStream<CbStringPtr>( &data ), [&]( auto op_std, auto&& ...args_std ) {
                CbQueue updated_cq_unk;
                pc->reverse_patch_data_visitor( cq_unk, [&]( auto op_unk, auto&& ...args_unk ) {
                    pc->unk_std_comb( op_std, op_unk, as_usr, &updated_cq_unk, args_std..., args_unk... );
                    pc->reg_op_in( BinStream<CbQueue>( &updated_cq_unk ), op_unk, std::forward<decltype( args_unk )>( args_unk )... );
                } );

                cq_unk.clear();
                pc->reverse_patch_data_visitor( updated_cq_unk, [&]( auto op_unk, auto&& ...args_unk ) {
                    pc->reg_op_in( BBQ( &cq_unk ), op_unk, std::forward<decltype( args_unk )>( args_unk )... );
                } );
            } );

            res.write_some( std::move( tmp_res ) );
        }

        virtual void undo( CbStringPtr data, UsrId as_usr ) {
            Aod aod{ as_usr, Aod::UNDO };
            pc->reverse_patch_data_visitor( data, [&]( auto op, auto&& ...args ){
                pc->undo_patch( op, aod, args... );
            } );
        }

        virtual void finalize() {
            pc->ot_merger_finalize();
        }

        T           *pc;
        CbQueue      cq_unk; ///< cumulated effect of unknown patches
        CbQueue      cq_new; ///< cumulated effect of new patches
    };

    return new MyMerger( static_cast<T *>( this ) );
}

template<class T>
void OtWrapper<T>::apply_op( op_add_grp_right, Aod &aod, GrpId grp, unsigned flags ) {
    auto iter = grp_rights.find( grp );
    if ( iter == grp_rights.end() )
        iter = grp_rights.insert( iter, std::pair<GrpId,unsigned>( grp, 0u ) );

    if ( flags &= ~ iter->second ) {
        iter->second |= flags;
        static_cast<T *>( this )->sig_change();
    }
}

template<class T>
void OtWrapper<T>::apply_op( op_rem_grp_right, Aod &aod, GrpId grp, unsigned flags ) {
    auto iter = grp_rights.find( grp );
    if ( iter == grp_rights.end() )
        return;

    if ( flags &= iter->second ) {
        iter->second &= ~flags;
        static_cast<T *>( this )->sig_change();
    }
}

template<class T>
void OtWrapper<T>::apply_op( op_add_usr_right, Aod &aod, UsrId usr, unsigned flags ) {
    auto iter = usr_rights.find( usr );
    if ( iter == usr_rights.end() )
        iter = usr_rights.insert( iter, std::pair<UsrId,unsigned>( usr, 0u ) );

    if ( flags &= ~ iter->second ) {
        iter->second |= flags;
        static_cast<T *>( this )->sig_change();
    }
}

template<class T>
void OtWrapper<T>::apply_op( op_rem_usr_right, Aod &aod, UsrId usr, unsigned flags ) {
    auto iter = usr_rights.find( usr );
    if ( iter == usr_rights.end() )
        return;

    if ( flags &= iter->second ) {
        iter->second &= ~flags;
        static_cast<T *>( this )->sig_change();
    }
}

} // namespace RiotBase
