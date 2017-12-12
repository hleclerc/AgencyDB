#pragma once

#include "CommonPatchCreatorData.h"
#include "../../System/RefOrConv.h"
#include "../../System/ApplyVa.h"
#include "../../System/NbArgs.h"
#include "../../System/Unref.h"
#include "../../System/Scast.h"
#include "../../System/FqId.h"
#include "../Core/RpTempl.h"
#include "PatchMerger.h"
#include <map>

namespace RiotBase {

// operation types
struct op_add_grp_right { static const char *name() { return "add_grp_right"; } static bool valid( GrpId, unsigned v ) { return v; } };
struct op_rem_grp_right { static const char *name() { return "rem_grp_right"; } static bool valid( GrpId, unsigned v ) { return v; } };
struct op_add_usr_right { static const char *name() { return "add_usr_right"; } static bool valid( UsrId, unsigned v ) { return v; } };
struct op_rem_usr_right { static const char *name() { return "rem_usr_right"; } static bool valid( UsrId, unsigned v ) { return v; } };

// right types
struct can_add_rights { static const char *name() { return "add_rights"; } };
struct can_rem_rights { static const char *name() { return "rem_rights"; } };

///
template<class T>
class PatchCreator : public RpTempl<T> {
public:
    using RightsOperationTypes = std::tuple<op_add_usr_right,op_rem_usr_right,op_add_grp_right,op_rem_grp_right>;
    using RightsRightTypes    = std::tuple<can_add_rights,can_rem_rights>;
    using Transmission         = CommonPatchCreatorData::Transmission;
    using Aod                  = CommonPatchCreatorData::ApplyOpData;
    using Undo                 = CommonPatchCreatorData::Undo;

    // methods
    PatchCreator( UsrId usr_id = {} ) : pcd( usr_id, ( 1u << nb_rights() ) - 1 ) {}

    OtMessage             *get_changes_for         ( PVec<FqId> dst, const FqId &id );
    void                   app_changes             ( BBQ ans, const CbString &msg, const FqId &id, FqId src, PVec<FqId> dst );
    void                   write_patch_data        ( std::ostream &os, int nb_sp, const FqId &id ) const;
    Vec<std::string>       get_possible_rights     () const;
    Vec<GrpId>             get_groups_in_acl       () const { return pcd.get_groups_in_acl(); }
    Vec<UsrId>             get_users_in_acl        () const { return pcd.get_users_in_acl(); }
                                                   
    Vec<std::string>       get_grp_rights          ( GrpId grp ) const;
    Vec<std::string>       get_usr_rights          ( UsrId usr ) const;
    bool                   add_grp_right           ( GrpId grp, const std::string &right );
    bool                   add_usr_right           ( UsrId usr, const std::string &right );
    bool                   rem_grp_right           ( GrpId grp, const std::string &right );
    bool                   rem_usr_right           ( UsrId usr, const std::string &right );
                                                   
    void                   sig_change              ();
                                                   
    void                   self_on_change          ( Callback *c ) { pcd.on_change_list << c; c->reg_wait(); }
                                                   
    void                   get_last_changes        ( CbQueue &msg );
    PatchMerger           *new_patch_merger        ();
                                                   
    // operations                                  
    template<class Op>                             
    static PI8             num_op                  ();
                                                   
    template<class Op,class... Args>               
    static void            reg_op                  ( BBQ bq, const Op &op, Args&& ...args );
                                                   
    template<class... Args>                        
    static void            visitor                 ( CbStringPtr cata, Args&& ...args );
                                                   
    template<class... Args>                        
    static void            reverse_visitor         ( CbStringPtr cata, Args&& ...args );
                                                   
    //                                             
    void                   apply_op                ( op_add_grp_right, Aod &aod, GrpId, unsigned );
    void                   apply_op                ( op_rem_grp_right, Aod &aod, GrpId, unsigned );
    void                   apply_op                ( op_add_usr_right, Aod &aod, UsrId, unsigned );
    void                   apply_op                ( op_rem_usr_right, Aod &aod, UsrId, unsigned );
                                                   
    void                   undo_patch              ( op_add_grp_right, Undo u, GrpId grp, unsigned flags ) { apply_op( op_rem_grp_right{}, *u.aod, grp, flags ); }
    void                   undo_patch              ( op_rem_grp_right, Undo u, GrpId grp, unsigned flags ) { apply_op( op_add_grp_right{}, *u.aod, grp, flags ); }
    void                   undo_patch              ( op_add_usr_right, Undo u, UsrId usr, unsigned flags ) { apply_op( op_rem_usr_right{}, *u.aod, usr, flags ); }
    void                   undo_patch              ( op_rem_usr_right, Undo u, UsrId usr, unsigned flags ) { apply_op( op_add_usr_right{}, *u.aod, usr, flags ); }
                                                   
    bool                   right_to                ( op_add_grp_right, UsrId as_usr, GrpId, unsigned ) { return has_right( can_add_rights{}, as_usr ); }
    bool                   right_to                ( op_rem_grp_right, UsrId as_usr, GrpId, unsigned ) { return has_right( can_rem_rights{}, as_usr ); }
    bool                   right_to                ( op_add_usr_right, UsrId as_usr, UsrId, unsigned ) { return has_right( can_add_rights{}, as_usr ); }
    bool                   right_to                ( op_rem_usr_right, UsrId as_usr, UsrId, unsigned ) { return has_right( can_rem_rights{}, as_usr ); }
                                                   
    template<class Op,class... Args>               
    void                   transmission            ( Op op, BBQ br, const FqId &id, Args&& ...args );
                                                   
    bool                   _add_grp_right          ( GrpId grp, unsigned flags );
    bool                   _add_usr_right          ( UsrId usr, unsigned flags );
    bool                   _rem_grp_right          ( GrpId grp, unsigned flags );
    bool                   _rem_usr_right          ( UsrId usr, unsigned flags );

    template<class Op_0,class Op_1,class... Args>
    void                   unk_new_or_new_unk ( Op_0, Op_1, UsrId asu_n, BBQ bq_0, BBQ bq_1, Args& ...args ) {}
                  
    void                   unk_new_or_new_unk ( op_add_grp_right, op_add_grp_right, UsrId asu_n, BBQ bq_n, BBQ bq_o, GrpId grp_n, unsigned &flags_n, GrpId grp_o, unsigned &flags_o ) { if ( grp_o == grp_n ) flags_o &= ~ flags_n; }
    void                   unk_new_or_new_unk ( op_add_grp_right, op_rem_grp_right, UsrId asu_n, BBQ bq_n, BBQ bq_o, GrpId grp_n, unsigned &flags_n, GrpId grp_o, unsigned &flags_o ) { if ( grp_o == grp_n ) flags_o &= ~ flags_n; }
    void                   unk_new_or_new_unk ( op_rem_grp_right, op_add_grp_right, UsrId asu_n, BBQ bq_n, BBQ bq_o, GrpId grp_n, unsigned &flags_n, GrpId grp_o, unsigned &flags_o ) { if ( grp_o == grp_n ) flags_o &= ~ flags_n; }
    void                   unk_new_or_new_unk ( op_rem_grp_right, op_rem_grp_right, UsrId asu_n, BBQ bq_n, BBQ bq_o, GrpId grp_n, unsigned &flags_n, GrpId grp_o, unsigned &flags_o ) { if ( grp_o == grp_n ) flags_o &= ~ flags_n; }
    void                   unk_new_or_new_unk ( op_add_usr_right, op_add_usr_right, UsrId asu_n, BBQ bq_n, BBQ bq_o, UsrId usr_n, unsigned &flags_n, UsrId usr_o, unsigned &flags_o ) { if ( usr_o == usr_n ) flags_o &= ~ flags_n; }
    void                   unk_new_or_new_unk ( op_add_usr_right, op_rem_usr_right, UsrId asu_n, BBQ bq_n, BBQ bq_o, UsrId usr_n, unsigned &flags_n, UsrId usr_o, unsigned &flags_o ) { if ( usr_o == usr_n ) flags_o &= ~ flags_n; }
    void                   unk_new_or_new_unk ( op_rem_usr_right, op_add_usr_right, UsrId asu_n, BBQ bq_n, BBQ bq_o, UsrId usr_n, unsigned &flags_n, UsrId usr_o, unsigned &flags_o ) { if ( usr_o == usr_n ) flags_o &= ~ flags_n; }
    void                   unk_new_or_new_unk ( op_rem_usr_right, op_rem_usr_right, UsrId asu_n, BBQ bq_n, BBQ bq_o, UsrId usr_n, unsigned &flags_n, UsrId usr_o, unsigned &flags_o ) { if ( usr_o == usr_n ) flags_o &= ~ flags_n; }

    template<class Op_s,class Op_u,class... Args>
    void                   unk_std_comb            ( Op_s, Op_u, UsrId asu_s, BBQ bq_u, Args& ...args ) {}
                                                   
    void                   unk_std_comb            ( op_add_grp_right, op_add_grp_right, UsrId asu_s, BBQ bq_u, GrpId grp_s, unsigned &flags_s, GrpId grp_u, unsigned &flags_u ) { if ( grp_s == grp_u ) flags_u &= ~ flags_s; }
    void                   unk_std_comb            ( op_add_grp_right, op_rem_grp_right, UsrId asu_s, BBQ bq_u, GrpId grp_s, unsigned &flags_s, GrpId grp_u, unsigned &flags_u ) { if ( grp_s == grp_u ) flags_u &= ~ flags_s; }
    void                   unk_std_comb            ( op_rem_grp_right, op_add_grp_right, UsrId asu_s, BBQ bq_u, GrpId grp_s, unsigned &flags_s, GrpId grp_u, unsigned &flags_u ) { if ( grp_s == grp_u ) flags_u &= ~ flags_s; }
    void                   unk_std_comb            ( op_rem_grp_right, op_rem_grp_right, UsrId asu_s, BBQ bq_u, GrpId grp_s, unsigned &flags_s, GrpId grp_u, unsigned &flags_u ) { if ( grp_s == grp_u ) flags_u &= ~ flags_s; }
    void                   unk_std_comb            ( op_add_usr_right, op_add_usr_right, UsrId asu_s, BBQ bq_u, UsrId usr_s, unsigned &flags_s, UsrId usr_u, unsigned &flags_u ) { if ( usr_s == usr_u ) flags_u &= ~ flags_s; }
    void                   unk_std_comb            ( op_add_usr_right, op_rem_usr_right, UsrId asu_s, BBQ bq_u, UsrId usr_s, unsigned &flags_s, UsrId usr_u, unsigned &flags_u ) { if ( usr_s == usr_u ) flags_u &= ~ flags_s; }
    void                   unk_std_comb            ( op_rem_usr_right, op_add_usr_right, UsrId asu_s, BBQ bq_u, UsrId usr_s, unsigned &flags_s, UsrId usr_u, unsigned &flags_u ) { if ( usr_s == usr_u ) flags_u &= ~ flags_s; }
    void                   unk_std_comb            ( op_rem_usr_right, op_rem_usr_right, UsrId asu_s, BBQ bq_u, UsrId usr_s, unsigned &flags_s, UsrId usr_u, unsigned &flags_u ) { if ( usr_s == usr_u ) flags_u &= ~ flags_s; }

    // to create _und version when there is a non und one
    template<class Op,class... Args>
    void                   register_patch_op( const Op &op, BBQ bq, Args&& ...args ) { reg_op( bq, op, std::forward<Args>( args )... ); }

    // rights              
    template<class Op>     
    static PI8             num_right               ( const Op &op );
                           
    static PI8             num_right_str           ( const std::string &right );
                           
    static constexpr PI8   nb_rights               ();
                           
    template<class Rt>     
    bool                   has_right               ( const Rt &rt, UsrId as_usr = {} ) const { return pcd.usr_right( as_usr ) & ( 1u << num_right( rt ) ); }
                           
    // OT                  
    void                   freeze_last_mod         () {} ///< may be surdefined to add data in pcd.operations
    void                   patch_merger_finalize   ( const FqId &id ) {}

    CommonPatchCreatorData pcd;

protected:
    //
    struct PcMerger : PatchMerger {
        PcMerger( T *pc ) : pc( pc ) {}

        virtual ~PcMerger() {}

        virtual void set_creator( UsrId usr_id ) {}

        virtual void new_patch( CbString &data, const FqId &id, UsrId as_usr, PT nb_rem_unk, PT nb_rem_std ) {
            // we will update data + cq_unk and append data to cq_new if necessary
            CbQueue data_new;
            CommonPatchCreatorData::NewPatch op{ id, as_usr, nb_rem_unk, nb_rem_std, &data_new };
            visitor( data, op, this );
            if ( nb_rem_unk or nb_rem_std ) // if there are unk or std patches, we need a representation of the preceding new stuff to modify the corresponding data
                cq_new.write_some( data_new );
            data = std::move( data_new );
        }
        virtual void unk_patch( CbString &data, const FqId &id, UsrId as_usr, PT nb_rem_new, PT nb_rem_std ) {
            // we will update data + cq_unk and append data to cq_new if necessary
            CbQueue data_unk;
            CommonPatchCreatorData::UnkPatch op{ id, as_usr, nb_rem_new, nb_rem_std, &data_unk };
            visitor( data, op, this );
            if ( nb_rem_new or nb_rem_std ) // if there are unk or std patches, we need a representation of the preceding new stuff to modify the corresponding data
                cq_unk.write_some( data_unk );
            data = std::move( data_unk );
        }
        virtual void std_patch( CbString &data, const FqId &id, UsrId as_usr, PT nb_rem_unk, PT nb_rem_new ) {
            // goal:
            // - update std data according to new stuff
            // - make new' (new after std)
            // - make the operations
            CbQueue data_new;
            CommonPatchCreatorData::StdPatch_New npc{ id, as_usr, nb_rem_unk, nb_rem_new, &data_new };
            visitor( data, npc, this );
            data = std::move( data_new );

            // goal:
            // - make unk' (unk after std)
            CommonPatchCreatorData::StdPatch_Unk v_u{ id, as_usr };
            visitor( data, v_u, this );
        }

        virtual void undo( CbStringPtr data, const FqId &id, UsrId as_usr ) {
            Aod aod{ &id, as_usr, Aod::UNDO };
            reverse_visitor( data, Undo{ &aod }, pc );
        }

        virtual void finalize( const FqId &id ) {
            pc->patch_merger_finalize( id );
        }

        T            *pc;
        CbQueue       cq_unk; ///< effect of unknown patches
        CbQueue       cq_new; ///< effect of new patches
    };

    // for a given instruction code. Goal: find the corresponding instruction
    template<class T0,class... Trgs,class... Args>
    static void            _visitor          ( BinStream<CbStringPtr> data, PI8 code, std::tuple<T0,Trgs...>, Args&& ...args );
    template<class... Args>
    static void            _visitor          ( BinStream<CbStringPtr> data, PI8 code, std::tuple<>, Args&& ...args );

    // for a given instruction. Goal: read the data and call the function
    template<class Op,PT n,PT m,class... Args>
    static void            __visitor         ( BinStream<CbStringPtr> data, Op op, N<n>, N<m>, Args&& ...args );
    template<class Op,PT n,class... Args>
    static void            __visitor         ( BinStream<CbStringPtr> data, Op op, N<n>, N<n>, Args&& ...args );

    // for a given instruction code.
    template<class T0,class... Trgs>
    static void            _skipper          ( BinStream<CbStringPtr> data, PI8 code, std::tuple<T0,Trgs...> );
    static void            _skipper          ( BinStream<CbStringPtr> data, PI8 code, std::tuple<> );

    // for a given instruction.
    template<class Op,PT n,PT m>
    static void            __skipper         ( BinStream<CbStringPtr> data, Op op, N<n>, N<m> );
    template<class Op,PT n>
    static void            __skipper         ( BinStream<CbStringPtr> data, Op op, N<n>, N<n> );

};

template<class T>
template<class Op,class... Args>
void PatchCreator<T>::reg_op( BBQ bq, const Op &op, Args&& ...args ) {
    constexpr PT nargs = NbArgs<decltype(Op::valid)>::value;
    // operation id
    bq << num_op<Op>();

    // arguments
    auto n = apply_van( [ & ]( auto &&arg, auto n ) {
        static_assert( n.val < nargs, "too much arguments" );
        bq << ref_of_conv<typename Unref<typename ArgType<decltype(Op::valid),n.val>::T>::T>( arg );
    }, std::forward<Args>( args )... );

    // check nb args was correct
    static_assert( n.val == nargs, "missing arguments" );
}

template<class T>
template<class Op>
PI8 PatchCreator<T>::num_op() {
    TODO;
    return 0;
//    constexpr int rr = TupleFind<Op,RightsOperationTypes>::value, sr = std::tuple_size<RightsOperationTypes>::value;
//    if ( rr < sr )
//        return rr;
//    constexpr int ro = TupleFind<Op,typename T::OperationTypes>::value, so = std::tuple_size<typename T::OperationTypes>::value;
//    static_assert( rr < sr or ro < so, "Operation not registered" );
//    return sr + ro;
}

template<class T>
template<class Rt>
PI8 PatchCreator<T>::num_right( const Rt &rt ) {
    TODO;
    return 0;
//    constexpr int rr = TupleFind<Rt,RightsRightTypes>::value, sr = std::tuple_size<RightsRightTypes>::value;
//    if ( rr < sr )
//        return rr;
//    constexpr int ro = TupleFind<Rt,typename T::RightTypes>::value, so = std::tuple_size<typename T::RightTypes>::value;
//    static_assert( rr < sr or ro < so, "Right type not registered" );
//    return sr + ro;
}

inline PI8 _num_right_str( const std::string &right, std::tuple<> ) {
    return 0;
}
template<class Head,class ...Tail>
PI8 _num_right_str( const std::string &right, std::tuple<Head,Tail...> ) {
    return Head::name() == right ? 0 : 1 + _num_right_str( right, std::tuple<Tail...>{} );
}

template<class T>
PI8 PatchCreator<T>::num_right_str( const std::string &right ) {
    return _num_right_str( right, std::tuple_cat( RightsRightTypes(), typename T::RightTypes() ) );
}

template<class T>
constexpr PI8 PatchCreator<T>::nb_rights() {
    return std::tuple_size<RightsRightTypes>::value + std::tuple_size<typename T::RightTypes>::value;
}


template<class T>
OtMessage *PatchCreator<T>::get_changes_for( PVec<FqId> dst, const FqId &id ) {
    static_cast<T *>( this )->freeze_last_mod();
    return pcd.get_changes_for( dst, id, [ & ]( BBQ bq, const CbString &data ) {
        visitor( data, Transmission{}, static_cast<T *>( this ), bq, id );
    } );
}

template<class T>
void PatchCreator<T>::app_changes( BBQ ans, const CbString &msg, const FqId &id, const FqId src, PVec<FqId> dst ) {
    static_cast<T *>( this )->freeze_last_mod();
    PcMerger merger( static_cast<T *>( this ) );
    pcd.app_changes( ans, msg, id, src, dst, &merger );
}

template<class T>
void PatchCreator<T>::write_patch_data( std::ostream &os, int nb_sp, const FqId &id ) const {
    CommonPatchCreatorData::Print printer{ id };
    pcd.write_patch_data( os, nb_sp, id, [ & ]( std::ostream &os, CbStringPtr data ) { visitor( data, printer, os ); } );
}

template<class Head,class... Tail>
void _get_possible_rights( Vec<std::string> &res, std::tuple<Head,Tail...> ) {
    res << Head::name();
    _get_possible_rights( res, std::tuple<Tail...>{} );
}
inline void _get_possible_rights( Vec<std::string> &res, std::tuple<> ) {
}

template<class T>
Vec<std::string> PatchCreator<T>::get_possible_rights() const {
    Vec<std::string> res;
    _get_possible_rights( res, std::tuple_cat( RightsRightTypes(), typename T::RightTypes() ) );
    return res;
}

template<class Head,class... Tail>
void _get_right_list_for( Vec<std::string> &res, unsigned flags, std::tuple<Head,Tail...> ) {
    if ( flags & 1 )
        res << Head::name();
    _get_right_list_for( res, flags >> 1, std::tuple<Tail...>{} );
}
inline void _get_right_list_for( Vec<std::string> &res, unsigned flags, std::tuple<> ) {
}

template<class T>
Vec<std::string> PatchCreator<T>::get_grp_rights( GrpId grp ) const {
    Vec<std::string> res;
    _get_right_list_for( res, pcd.grp_right( grp ), std::tuple_cat( RightsRightTypes(), typename T::RightTypes() ) );
    return res;
}

template<class T>
Vec<std::string> PatchCreator<T>::get_usr_rights( UsrId usr ) const {
    Vec<std::string> res;
    _get_right_list_for( res, pcd.usr_right( usr ), std::tuple_cat( RightsRightTypes(), typename T::RightTypes() ) );
    return res;
}

template<class T>
bool PatchCreator<T>::add_grp_right( GrpId grp, const std::string &right ) {
    PI8 n =  num_right_str( right );
    return n != nb_rights() ? _add_grp_right( grp, 1u << n ) : false;
}

template<class T>
bool PatchCreator<T>::add_usr_right( UsrId usr, const std::string &right ) {
    PI8 n = num_right_str( right );
    return n != nb_rights() ? _add_usr_right( usr, 1u << n ) : false;
}

template<class T>
bool PatchCreator<T>::rem_grp_right( GrpId grp, const std::string &right ) {
    PI8 n = num_right_str( right );
    return n != nb_rights() ? _rem_grp_right( grp, 1u << n ) : false;
}

template<class T>
bool PatchCreator<T>::rem_usr_right( UsrId usr, const std::string &right ) {
    PI8 n = num_right_str( right );
    return n != nb_rights() ? _rem_usr_right( usr, 1u << n ) : false;
}

template<class T>
void PatchCreator<T>::sig_change() {
    for( RcPtr<Callback> &c : pcd.on_change_list )
        c->reg_wait();
}

template<class T>
void PatchCreator<T>::get_last_changes( CbQueue &msg ) {
    static_cast<T *>( this )->freeze_last_mod();
    msg.write_some( std::move( pcd.operations ) );
    // visitor(, Transmission(), static_cast<T *>( this ), &msg, db );
}

template<class T>
PatchMerger *PatchCreator<T>::new_patch_merger() {
    return new PcMerger( static_cast<T *>( this ) );
}


template<class T>
template<class... Args>
void PatchCreator<T>::visitor( CbStringPtr cata, Args&& ...args ) {
    auto ops = std::tuple_cat( RightsOperationTypes(), typename T::OperationTypes() );
    BinStream<CbStringPtr> data( &cata );

    while ( not data.empty() ) {
        PI8 code = data.read_byte();
        if ( data.error() )
            break;
        _visitor( data, code, ops, args... );
    }
}

template<class T>
template<class... Args>
void PatchCreator<T>::reverse_visitor( CbStringPtr cata, Args&& ...args ) {
    auto ops = std::tuple_cat( RightsOperationTypes(), typename T::OperationTypes() );
    Vec<CbStringPtr> positions; positions.reserve( cata.size() / 10 );
    BinStream<CbStringPtr> data( &cata );

    // forward
    while ( not data.empty() ) {
        positions << cata;

        PI8 code = data.read_byte();
        if ( data.error() )
            return;
        _skipper( data, code, ops );
    }

    // backward
    for( PT np = positions.size(); np--; ) {
        CbStringPtr cb = positions[ np ];
        BinStream<CbStringPtr> data( &cb );

        PI8 code = data.read_byte();
        _visitor( data, code, ops, args... );
    }
}

// _visitor
template<class T>
template<class T0,class... Trgs,class... Args>
void PatchCreator<T>::_visitor( BinStream<CbStringPtr> data, PI8 code, std::tuple<T0,Trgs...>, Args&& ...args ) {
    if ( code )
        return _visitor( data, code - 1, std::tuple<Trgs...>(), std::forward<Args>( args )... );
    __visitor( data, T0(), N<0>(), N<NbArgs<decltype(T0::valid)>::value>(), std::forward<Args>( args )... );
}
template<class T>
template<class... Args>
void PatchCreator<T>::_visitor( BinStream<CbStringPtr> data, PI8 code, std::tuple<>, Args&& ...args ) {
    ERROR( "Unknown patch type !!" );
}

template<class T>
template<class Op,PT n,PT m,class... Args>
void PatchCreator<T>::__visitor( BinStream<CbStringPtr> data, Op op, N<n>, N<m>, Args&& ...args ) {
    typename Unref<typename ArgType<decltype(Op::valid),n>::T>::T val = data.read();
    __visitor( data, op, N<n+1>(), N<m>(), std::forward<Args>( args )..., val );
}
template<class T>
template<class Op,PT n,class... Args>
void PatchCreator<T>::__visitor( BinStream<CbStringPtr> data, Op op, N<n>, N<n>, Args&& ...args ) {
    pc_visitor( op, args... );
}

//
template<class T>
template<class T0,class... Trgs>
void PatchCreator<T>::_skipper( BinStream<CbStringPtr> data, PI8 code, std::tuple<T0,Trgs...> ) {
    if ( code )
        return _skipper( data, code - 1, std::tuple<Trgs...>() );
    __skipper( data, T0(), N<0>(), N<NbArgs<decltype(T0::valid)>::value>() );
}
template<class T>
void PatchCreator<T>::_skipper( BinStream<CbStringPtr> data, PI8 code, std::tuple<> ) {
    ERROR( "Unknown patch type !!" );
}

template<class T>
template<class Op,PT n,PT m>
void PatchCreator<T>::__skipper( BinStream<CbStringPtr> data, Op op, N<n>, N<m> ) {
     data.read().operator typename Unref<typename ArgType<decltype(Op::valid),n>::T>::T();
    __skipper( data, op, N<n+1>(), N<m>() );
}
template<class T>
template<class Op,PT n>
void PatchCreator<T>::__skipper( BinStream<CbStringPtr> data, Op op, N<n>, N<n> ) {
}

// helpers for rights

template<class T>
void PatchCreator<T>::apply_op( op_add_grp_right, Aod &aod, GrpId grp, unsigned flags ) {
    auto iter = pcd.grp_rights.find( grp );
    if ( iter == pcd.grp_rights.end() )
        iter = pcd.grp_rights.insert( iter, std::pair<GrpId,unsigned>( grp, 0u ) );

    flags &= ~ iter->second;
    if ( flags ) {
        iter->second |= flags;
        static_cast<T *>( this )->sig_change();
    }
}

template<class T>
void PatchCreator<T>::apply_op( op_rem_grp_right, Aod &aod, GrpId grp, unsigned flags ) {
    auto iter = pcd.grp_rights.find( grp );
    if ( iter == pcd.grp_rights.end() )
        return;

    flags &= iter->second;
    if ( flags ) {
        iter->second &= ~flags;
        static_cast<T *>( this )->sig_change();
    }
}

template<class T>
void PatchCreator<T>::apply_op( op_add_usr_right, Aod &aod, UsrId usr, unsigned flags ) {
    auto iter = pcd.usr_rights.find( usr );
    if ( iter == pcd.usr_rights.end() )
        iter = pcd.usr_rights.insert( iter, std::pair<UsrId,unsigned>( usr, 0u ) );

    flags &= ~ iter->second;
    if ( flags ) {
        iter->second |= flags;
        static_cast<T *>( this )->sig_change();
    }
}

template<class T>
void PatchCreator<T>::apply_op( op_rem_usr_right, Aod &aod, UsrId usr, unsigned flags ) {
    auto iter = pcd.usr_rights.find( usr );
    if ( iter == pcd.usr_rights.end() )
        return;

    flags &= iter->second;
    if ( flags ) {
        iter->second &= ~flags;
        static_cast<T *>( this )->sig_change();
    }
}

template<class T>
template<class Op,class... Args>
void PatchCreator<T>::transmission( Op op, BBQ br, const FqId &id, Args&& ...args ) {
    br << num_op<Op>();
    apply_va( [ & ]( auto &val ) {
        loc_to_glo( val, id );
        br << val;
    }, args... );
}

template<class T>
bool PatchCreator<T>::_add_grp_right( GrpId grp, unsigned flags ) {
    if ( not has_right( can_add_rights{} ) or not flags )
        return false;

    auto iter = pcd.grp_rights.find( grp );
    if ( iter == pcd.grp_rights.end() )
        iter = pcd.grp_rights.insert( iter, std::pair<GrpId,unsigned>( grp, 0u ) );

    flags &= ~ iter->second;
    if ( flags ) {
        reg_op( pcd.bo, op_add_grp_right{}, grp, flags );
        iter->second |= flags;
        static_cast<T *>( this )->sig_change();
    }
    return true;
}

template<class T>
bool PatchCreator<T>::_add_usr_right( UsrId usr, unsigned flags ) {
    if ( not has_right( can_add_rights{} ) or not flags )
        return false;

    auto iter = pcd.usr_rights.find( usr );
    if ( iter == pcd.usr_rights.end() )
        iter = pcd.usr_rights.insert( iter, std::pair<UsrId,unsigned>( usr, 0u ) );

    flags &= ~ iter->second;
    if ( flags ) {
        reg_op( pcd.bo, op_add_usr_right{}, usr, flags );
        iter->second |= flags;
        static_cast<T *>( this )->sig_change();
    }
    return true;
}

template<class T>
bool PatchCreator<T>::_rem_grp_right( GrpId grp, unsigned flags ) {
    if ( not has_right( can_rem_rights{} ) or not flags )
        return false;

    auto iter = pcd.grp_rights.find( grp );
    if ( iter == pcd.grp_rights.end() )
        return true;

    flags &= iter->second;
    if ( flags ) {
        reg_op( pcd.bo, op_rem_grp_right{}, grp, flags );
        iter->second &= ~flags;
        static_cast<T *>( this )->sig_change();
    }
    return true;
}

template<class T>
bool PatchCreator<T>::_rem_usr_right( UsrId usr, unsigned flags ) {
    if ( not has_right( can_rem_rights{} ) or not flags )
        return false;

    auto iter = pcd.usr_rights.find( usr );
    if ( iter == pcd.usr_rights.end() )
        return true;

    flags &= iter->second;
    if ( flags ) {
        reg_op( pcd.bo, op_rem_usr_right{}, usr, flags );
        iter->second &= ~flags;
        static_cast<T *>( this )->sig_change();
    }
    return true;
}


// specializations for Transmission
template<class Op,class Pc,class... Args>
void pc_visitor( Op op, CommonPatchCreatorData::Transmission, Pc *pc, BBQ br, const FqId &id, Args&& ...args ) {
    pc->transmission( op, br, id, std::forward<Args>( args )... );
}

// specializations for Identity
template<class Op,class Pc,class... Args>
void pc_visitor( Op, CommonPatchCreatorData::Identity, Pc *pc, BBQ br, Args&& ...args ) {
    br << pc->template num_op<Op>();
    apply_va( [ & ]( const auto &val ) {
        br << val;
    }, args... );
}

// specializations for Undo
template<class Op,class Pc,class... Args> void pc_visitor( Op op, CommonPatchCreatorData::Undo u, Pc *pc, Args&& ...args) { pc->undo_patch( op, u, std::forward<Args>( args )... ); }

// specializations for Print
template<class Op,class... Args>
void pc_visitor( Op, const CommonPatchCreatorData::Print &print, std::ostream &os, Args&& ...args ) {
    int cpt = 0;
    os << " " << Op::name() << "(";
    apply_va( [ & ]( auto &val ) {
        loc_to_glo( val, print.id );
        os << ( cpt++ ? ",": "" ) << val;
    }, args... );
    os << ")";
}

//
template<class Op,class Pc,class... Args>
void update_patch_data_level_0( Op, Pc *pc, Args& ...args ) {
}

template<class Pc,class... Args>
void update_patch_data_level_0( op_add_grp_right, Pc *pc, GrpId &grp, unsigned &flags ) {
    flags &= ~ pc->pcd.grp_right( grp );
}
template<class Pc,class... Args>
void update_patch_data_level_0( op_rem_grp_right, Pc *pc, GrpId &grp, unsigned &flags ) {
    flags &= pc->pcd.grp_right( grp );
}
template<class Pc,class... Args>
void update_patch_data_level_0( op_add_usr_right, Pc *pc, UsrId &usr, unsigned &flags ) {
    flags &= ~ pc->pcd.usr_right( usr );
}
template<class Pc,class... Args>
void update_patch_data_level_0( op_rem_usr_right, Pc *pc, UsrId &usr, unsigned &flags ) {
    flags &= pc->pcd.usr_right( usr );
}

// specializations for NewPatch
template<class Op,class Merger,class... Args>
void pc_visitor( const Op &op, CommonPatchCreatorData::NewPatch &np, Merger *merger, Args&& ...args ) {
    // global -> local data (for inodes, dev_id, ...)
    apply_va( [ & ]( auto &val ) { glo_to_loc( val, np.id ); }, args... );

    //
    update_patch_data_level_0( op, merger->pc, args... );

    // update data according to cq_unk + make cq_unk'
    CbQueue cq_unk;
    CommonPatchCreatorData::UnkNewPatch unp{ &np, &cq_unk };
    merger->pc->visitor( merger->cq_unk, unp, merger, op, args... );

    // check validity + rights
    // PRINT( op.name(), np.as_usr, merger->pc->pcd.usr_right( np.as_usr ), np.cur_dev, merger->pc->right_to( op, np.as_usr, args... ) );
    if ( Op::valid( args... ) and merger->pc->right_to( op, np.as_usr, args... ) ) {
        // register operations
        merger->pc->register_patch_op( op, np.bq_new, args... );
        merger->cq_unk = std::move( cq_unk );

        // apply operation
        CommonPatchCreatorData::ApplyOpData aod{ &np.id, np.as_usr, CommonPatchCreatorData::ApplyOpData::NEW_PATCH };
        merger->pc->apply_op( op, aod, std::forward<Args>( args )... );
    }
}

// specializations for UnkPatch
template<class Op,class Merger,class... Args>
void pc_visitor( const Op &op, CommonPatchCreatorData::UnkPatch &up, Merger *merger, Args&& ...args ) {
    update_patch_data_level_0( op, merger->pc, args... );

    // update data according to cq_unk + make cq_unk'
    CbQueue cq_new;
    CommonPatchCreatorData::NewUnkPatch nup{ &up, &cq_new };
    merger->pc->visitor( merger->cq_new, nup, merger, op, args... );

    // check validity + rights
    if ( Op::valid( args... ) and merger->pc->right_to( op, up.as_usr, args... ) ) {
        // register operations (in merger->cq_unk, ...)
        merger->pc->register_patch_op( op, up.bq_unk, args... );
        merger->cq_new = std::move( cq_new );

        // apply operation
        CommonPatchCreatorData::ApplyOpData aod{ &up.id, up.as_usr, CommonPatchCreatorData::ApplyOpData::UNK_PATCH };
        merger->pc->apply_op( op, aod, std::forward<Args>( args )... );
    }
}

// specializations for StdPatch_...
template<class Op,class Merger,class... Args>
void pc_visitor( const Op &op, CommonPatchCreatorData::StdPatch_New &sp, Merger *merger, Args&& ...args ) {
    update_patch_data_level_0( op, merger->pc, args... );

    // modifications
    CbQueue cq_new;
    CommonPatchCreatorData::NewStdPatch npv{ &sp, &cq_new };
    merger->pc->visitor( merger->cq_new, npv, merger, op, args... );

    // check validity + rights
    if ( Op::valid( args... ) and merger->pc->right_to( op, sp.as_usr, args... ) ) {
        // register operations (in merger->cq_unk, ...)
        merger->pc->register_patch_op( op, sp.bq_std, args... );
        merger->cq_new = std::move( cq_new );

        // apply operation
        CommonPatchCreatorData::ApplyOpData aod{ &sp.id, sp.as_usr, CommonPatchCreatorData::ApplyOpData::STD_PATCH };
        merger->pc->apply_op( op, aod, std::forward<Args>( args )... );
    }
}

template<class Op,class Merger,class... Args>
void pc_visitor( const Op &op, CommonPatchCreatorData::StdPatch_Unk &sp, Merger *merger, Args&& ...args ) {
    CbQueue cq_unk;
    CommonPatchCreatorData::UnkStdPatch usv{ &sp, &cq_unk };
    merger->pc->reverse_visitor( merger->cq_unk, usv, merger, op, args... );

    merger->cq_unk.free();
    merger->pc->reverse_visitor( cq_unk, CommonPatchCreatorData::Identity{}, merger->pc, &merger->cq_unk );
}

// helpers for NewPatch and UnkPatch
template<PT skip>
struct SaveDataWithSkip {
    template<class Op,class Pc,class Head,class... Tail>
    static void save( Op op, Pc *pc, BBQ bq, Head&& head, Tail&& ...tail ) {
        SaveDataWithSkip<skip-1>::save( op, pc, bq, std::forward<Tail>( tail )... );
    }
    template<class Op,class Pc>
    static void save( Op op, Pc *pc, BBQ bq ) {
    }
};
template<>
struct SaveDataWithSkip<0u> {
    template<class Op,class Pc,class... Args>
    static void save( const Op &op, Pc *pc, BBQ bq, Args&& ...args ) {
        // register operation in cq_xxx
        if ( op.valid( args... ) )
            pc->reg_op( bq, op, std::forward<Args>( args )... );
    }

};

// specializations for UnkNewPatch. First args come from New
template<class Op_unk,class Merger,class Op_new, class... Args>
void pc_visitor( const Op_unk &op_unk, CommonPatchCreatorData::UnkNewPatch &unp, Merger *merger, const Op_new &op_new, Args&& ...args ) {
    merger->pc->unk_new_or_new_unk( op_new, op_unk, unp.np->as_usr, unp.np->bq_new, unp.bq_unk, args... );
    SaveDataWithSkip<NbArgs<decltype(Op_new::valid)>::value>::save( op_unk, merger->pc, unp.bq_unk, std::forward<Args>( args )... );
}

// specializations for NewUnkPatch. First args come from Unk
template<class Op_new,class Merger,class Op_unk, class... Args>
void pc_visitor( const Op_new &op_new, CommonPatchCreatorData::NewUnkPatch &nup, Merger *merger, const Op_unk &op_unk, Args&& ...args ) {
    merger->pc->unk_new_or_new_unk( op_unk, op_new, nup.up->as_usr, nup.up->bq_unk, nup.bq_new, args... );
    SaveDataWithSkip<NbArgs<decltype(Op_unk::valid)>::value>::save( op_new, merger->pc, nup.bq_new, std::forward<Args>( args )... );
}

// specializations for NewStdPatch. First args come from Std
template<class Op_new,class Merger,class Op_std, class... Args>
void pc_visitor( const Op_new &op_new, CommonPatchCreatorData::NewStdPatch &nsp, Merger *merger, const Op_std &op_std, Args&& ...args ) {
    merger->pc->unk_new_or_new_unk( op_std, op_new, nsp.sp->as_usr, nsp.sp->bq_std, nsp.bq_new, args... );
    SaveDataWithSkip<NbArgs<decltype(Op_std::valid)>::value>::save( op_new, merger->pc, nsp.bq_new, std::forward<Args>( args )... );
}

// specializations for UnkStdPatch. First args come from Std
template<class Op_unk,class Merger,class Op_std, class... Args>
void pc_visitor( const Op_unk &op_unk, CommonPatchCreatorData::UnkStdPatch &usp, Merger *merger, const Op_std &op_std, Args&& ...args ) {
    merger->pc->unk_std_comb( op_std, op_unk, usp.sp->as_usr, usp.bq_unk, args... );
    SaveDataWithSkip<NbArgs<decltype(Op_std::valid)>::value>::save( op_unk, merger->pc, usp.bq_unk, std::forward<Args>( args )... );
}

} // namespace RiotBase
