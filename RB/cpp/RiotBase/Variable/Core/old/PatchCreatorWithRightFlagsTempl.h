#pragma once

#include "../Core/RpTemplProxy.h"
#include <map>

namespace RiotBase {

#define _TCB  template<class Cb>
#define _TCBO template<class Cb,class Op>

///
struct PatchCreatorWithRightFlagsVisitor {
    void add_grp_right ( GrpId grp, unsigned flags ) {}
    void add_usr_right ( UsrId usr, unsigned flags ) {}
    void rem_grp_right ( GrpId grp, unsigned flags ) {}
    void rem_usr_right ( UsrId usr, unsigned flags ) {}
};


///
template<class T,class I=RpTemplProxy<T>>
struct PatchCreatorWithRightFlagsTempl : I {
    // patch types
    static constexpr PI8 PATCH_TYPE_ADD_GRP_RIGHT = 0;
    static constexpr PI8 PATCH_TYPE_REM_GRP_RIGHT = 1;
    static constexpr PI8 PATCH_TYPE_ADD_USR_RIGHT = 2;
    static constexpr PI8 PATCH_TYPE_REM_USR_RIGHT = 3;
    static constexpr PI8 END_PATCH_TYPE_FOR_RIGHT = 4;
    using Visitor = PatchCreatorWithRightFlagsVisitor;

    /// what to do with each data from unk and a new add_grp_right
    struct UnkNewPatchVisitor_add_grp_right : T::Visitor {
        UnkNewPatchVisitor_add_grp_right( GrpId grp_new, unsigned &flags_new ) : grp_new( grp_new ), flags_new( flags_new ) {}
        void add_grp_right( GrpId grp, unsigned &flags ) { if ( grp == grp_new ) flags &= ~ flags_new; }
        void rem_grp_right( GrpId grp, unsigned &flags ) { if ( grp == grp_new ) flags &= ~ flags_new; }
        GrpId     grp_new;
        unsigned &flags_new;
    };

    /// what to do with each data from unk and a new add_usr_right
    struct UnkNewPatchVisitor_add_usr_right : T::Visitor {
        UnkNewPatchVisitor_add_usr_right( UsrId usr_new, unsigned &flags_new ) : usr_new( usr_new ), flags_new( flags_new ) {}
        void add_usr_right( UsrId usr, unsigned flags ) { if ( usr == usr_new ) flags &= ~ flags_new; }
        void rem_usr_right( UsrId usr, unsigned flags ) { if ( usr == usr_new ) flags &= ~ flags_new; }
        UsrId     usr_new;
        unsigned &flags_new;
    };

    /// what to do with each data from unk and a new rem_grp_right
    struct UnkNewPatchVisitor_rem_grp_right : T::Visitor {
        UnkNewPatchVisitor_rem_grp_right( GrpId grp_new, unsigned &flags_new ) : grp_new( grp_new ), flags_new( flags_new ) {}
        void add_grp_right( GrpId grp, unsigned &flags ) { if ( grp == grp_new ) flags &= ~ flags_new; }
        void rem_grp_right( GrpId grp, unsigned &flags ) { if ( grp == grp_new ) flags &= ~ flags_new; }
        GrpId     grp_new;
        unsigned &flags_new;
    };

    /// what to do with each data from unk and a new rem_usr_right
    struct UnkNewPatchVisitor_rem_usr_right : T::Visitor {
        UnkNewPatchVisitor_rem_usr_right( UsrId usr_new, unsigned &flags_new ) : usr_new( usr_new ), flags_new( flags_new ) {}
        void add_usr_right ( UsrId usr, unsigned flags ) { if ( usr == usr_new ) flags &= ~ flags_new; }
        void rem_usr_right ( UsrId usr, unsigned flags ) { if ( usr == usr_new ) flags &= ~ flags_new; }
        UsrId     usr_new;
        unsigned &flags_new;
    };

    /// what to do with each data from unk and a new add_grp_right
    struct NewUnkPatchVisitor_add_grp_right : T::Visitor {
        NewUnkPatchVisitor_add_grp_right( GrpId grp_unk, unsigned &flags_unk ) : grp_unk( grp_unk ), flags_unk( flags_unk ) {}
        void add_grp_right( GrpId grp, unsigned &flags ) { if ( grp == grp_unk ) flags &= ~ flags_unk; }
        void rem_grp_right( GrpId grp, unsigned &flags ) { if ( grp == grp_unk ) flags &= ~ flags_unk; }
        GrpId     grp_unk;
        unsigned &flags_unk;
    };

    /// what to do with each data from unk and a new add_usr_right
    struct NewUnkPatchVisitor_add_usr_right : T::Visitor {
        NewUnkPatchVisitor_add_usr_right( UsrId usr_unk, unsigned &flags_unk ) : usr_unk( usr_unk ), flags_unk( flags_unk ) {}
        void add_usr_right ( UsrId usr, unsigned flags ) { if ( usr == usr_unk ) flags &= ~ flags_unk; }
        void rem_usr_right ( UsrId usr, unsigned flags ) { if ( usr == usr_unk ) flags &= ~ flags_unk; }
        UsrId     usr_unk;
        unsigned &flags_unk;
    };

    /// what to do with each data from unk and a new rem_grp_right
    struct NewUnkPatchVisitor_rem_grp_right : T::Visitor {
        NewUnkPatchVisitor_rem_grp_right( GrpId grp_unk, unsigned &flags_unk ) : grp_unk( grp_unk ), flags_unk( flags_unk ) {}
        void add_grp_right( GrpId grp, unsigned &flags ) { if ( grp == grp_unk ) flags &= ~ flags_unk; }
        void rem_grp_right( GrpId grp, unsigned &flags ) { if ( grp == grp_unk ) flags &= ~ flags_unk; }
        GrpId     grp_unk;
        unsigned &flags_unk;
    };

    /// what to do with each data from unk and a new rem_usr_right
    struct NewUnkPatchVisitor_rem_usr_right : T::Visitor {
        NewUnkPatchVisitor_rem_usr_right( UsrId usr_unk, unsigned &flags_unk ) : usr_unk( usr_unk ), flags_unk( flags_unk ) {}
        void add_usr_right ( UsrId usr, unsigned flags ) { if ( usr == usr_unk ) flags &= ~ flags_unk; }
        void rem_usr_right ( UsrId usr, unsigned flags ) { if ( usr == usr_unk ) flags &= ~ flags_unk; }
        UsrId     usr_unk;
        unsigned &flags_unk;
    };

    struct NewPatchVisitor : T::Visitor {
        NewPatchVisitor( UsrId cur_usr, DevId cur_dev, UsrId as_usr, typename T::Merger *pms, T *pcs, BinStream<CbQueue> tmp_bq_new ) : cur_usr( cur_usr ), cur_dev( cur_dev ), as_usr( as_usr ), pms( pms ), pcs( pcs ), tmp_bq_new( tmp_bq_new ) {
        }
        void add_grp_right( GrpId grp, unsigned flags ) {
            // updates for local storage
            if ( grp.dev == cur_dev )
                grp.dev = 0;

            // check that XOR on flags would work
            flags &= ~ pcs->_grp_right( grp );
            if ( not flags )
                return;

            // modifications -> unk should not modify flags from `flags`
            CbQueue cp_cq_unk = std::move( pms->cq_unk );
            typename T::UnkNewPatchVisitor_add_grp_right upv{ grp, flags };
            T::operations_visitor( cp_cq_unk, upv, &pms->cq_unk );

            // check the rights
            if ( not pcs->_right_to_add_rights( as_usr ) ) {
                pms->cq_unk = std::move( cp_cq_unk );
                return;
            }

            // register the new operation
            if ( flags ) {
                tmp_bq_new << T::PATCH_TYPE_ADD_GRP_RIGHT << grp << flags;
                pcs->_add_grp_right_wo_cr( grp, flags );
            }
        }
        void rem_grp_right( GrpId grp, unsigned flags ) {
            // updates for local storage
            if ( grp.dev == cur_dev )
                grp.dev = 0;

            // check that XOR on flags would work
            flags &= pcs->_grp_right( grp );
            if ( not flags )
                return;

            // modifications -> unk should not modify flags from `flags`
            CbQueue cp_cq_unk = std::move( pms->cq_unk );
            typename T::UnkNewPatchVisitor_rem_grp_right upv{ grp, flags };
            T::operations_visitor( cp_cq_unk, upv, &pms->cq_unk );

            // check the rights
            if ( not pcs->_right_to_rem_rights( as_usr ) ) {
                pms->cq_unk = std::move( cp_cq_unk );
                return;
            }

            // register the new operation
            if ( flags ) {
                tmp_bq_new << T::PATCH_TYPE_REM_GRP_RIGHT << grp << flags;
                pcs->_rem_grp_right_wo_cr( grp, flags );
            }
        }
        void add_usr_right( UsrId usr, unsigned flags ) {
            // updates for local storage
            if ( usr.dev == cur_dev )
                usr.dev = 0;
            if ( usr == cur_usr )
                usr = UsrId{};

            // check that XOR on flags would work
            flags &= ~ pcs->_usr_right( usr );
            if ( not flags )
                return;

            // modifications
            CbQueue cp_cq_unk = std::move( pms->cq_unk );
            typename T::UnkNewPatchVisitor_add_usr_right upv{ usr, flags };
            T::operations_visitor( cp_cq_unk, upv, &pms->cq_unk );

            // check the rights
            if ( not pcs->_right_to_add_rights( as_usr ) ) {
                pms->cq_unk = std::move( cp_cq_unk );
                return;
            }

            // register the new operation
            if ( flags ) {
                tmp_bq_new << T::PATCH_TYPE_ADD_USR_RIGHT << usr << flags;
                pcs->_add_usr_right_wo_cr( usr, flags );
            }
        }
        void rem_usr_right( UsrId usr, unsigned flags ) {
            // updates for local storage
            if ( usr.dev == cur_dev )
                usr.dev = 0;
            if ( usr == cur_usr )
                usr = UsrId{};

            // check that XOR on flags would work
            flags &= pcs->_usr_right( usr );
            if ( not flags )
                return;

            // register the new operation
            CbQueue cp_cq_unk = std::move( pms->cq_unk );
            typename T::UnkNewPatchVisitor_rem_usr_right upv{ usr, flags };
            T::operations_visitor( cp_cq_unk, upv, &pms->cq_unk );

            // check the rights
            if ( not pcs->_right_to_rem_rights( as_usr ) ) {
                pms->cq_unk = std::move( cp_cq_unk );
                return;
            }

            // register the new operation
            if ( flags ) {
                tmp_bq_new << T::PATCH_TYPE_REM_USR_RIGHT << usr << flags;
                pcs->_rem_usr_right_wo_cr( usr, flags );
            }
        }
        UsrId               cur_usr;
        DevId               cur_dev;
        UsrId               as_usr;
        typename T::Merger *pms;
        T                  *pcs;
        BinStream<CbQueue>  tmp_bq_new;
    };


    struct UnkPatchVisitor {
        UnkPatchVisitor( UsrId cur_usr, DevId cur_dev, UsrId as_usr, typename T::Merger *pms, T *pcs, BinStream<CbQueue> tmp_bq_unk ) : cur_usr( cur_usr ), cur_dev( cur_dev ), as_usr( as_usr ), pms( pms ), pcs( pcs ), tmp_bq_unk( tmp_bq_unk ) {
        }
        void add_grp_right( GrpId &grp, unsigned &flags ) {
            // check that XOR on flags would work
            flags &= ~ pcs->_grp_right( grp );
            if ( not flags )
                return;

            // modifications -> new should not modify flags from `flags`
            CbQueue cp_cq_new = std::move( pms->cq_new );
            typename T::NewUnkPatchVisitor_add_grp_right upv{ grp, flags };
            T::operations_visitor( cp_cq_new, upv, &pms->cq_new );
            if ( not flags )
                return;

            // check the rights
            if ( not pcs->_right_to_add_rights( as_usr ) ) {
                pms->cq_new = std::move( cp_cq_new );
                return;
            }

            // execute the corresponding operation
            pcs->_add_grp_right_wo_cr( grp, flags );
        }
        void rem_grp_right( GrpId &grp, unsigned &flags ) {
            // check that XOR on flags would work
            flags &= pcs->_grp_right( grp );
            if ( not flags )
                return;

            // modifications -> unk should not modify flags from `flags`
            CbQueue cp_cq_new = std::move( pms->cq_new );
            typename T::NewUnkPatchVisitor_rem_grp_right upv{ grp, flags };
            T::operations_visitor( cp_cq_new, upv, &pms->cq_new );
            if ( not flags )
                return;

            // check the rights
            if ( not pcs->_right_to_rem_rights( as_usr ) ) {
                pms->cq_new = std::move( cp_cq_new );
                return;
            }

            // execute the corresponding operation
            pcs->_rem_grp_right_wo_cr( grp, flags );
        }
        void add_usr_right ( UsrId &usr, unsigned &flags ) {
            // check that XOR on flags would work
            flags &= ~ pcs->_usr_right( usr );
            if ( not flags )
                return;

            // modifications
            CbQueue cp_cq_new = std::move( pms->cq_new );
            typename T::NewUnkPatchVisitor_add_usr_right upv{ usr, flags };
            T::operations_visitor( cp_cq_new, upv, &pms->cq_new );
            if ( not flags )
                return;

            // check the rights
            if ( not pcs->_right_to_add_rights( as_usr ) ) {
                pms->cq_new = std::move( cp_cq_new );
                return;
            }

            // execute the corresponding operation
            pcs->_add_usr_right_wo_cr( usr, flags );
        }
        void rem_usr_right( UsrId &usr, unsigned &flags ) {
            // check that XOR on flags would work
            flags &= pcs->_usr_right( usr );
            if ( not flags )
                return;

            // register the new operation
            CbQueue cp_cq_new = std::move( pms->cq_new );
            typename T::NewUnkPatchVisitor_rem_usr_right upv{ usr, flags };
            T::operations_visitor( cp_cq_new, upv, &pms->cq_new );
            if ( not flags )
                return;

            // check the rights
            if ( not pcs->_right_to_rem_rights( as_usr ) ) {
                pms->cq_new = std::move( cp_cq_new );
                return;
            }

            // execute the corresponding operation
            pcs->_rem_usr_right_wo_cr( usr, flags );
        }
        UsrId               cur_usr;
        DevId               cur_dev;
        UsrId               as_usr;
        typename T::Merger *pms;
        T                  *pcs;
        BinStream<CbQueue>  tmp_bq_unk;
    };


    PatchCreatorWithRightFlagsTempl( UsrId crea_usr ) {
        usr_rights[ crea_usr ] = T::base_rights;
    }

    //
    using RightListUsr = std::map<UsrId,unsigned>;
    using RightListGrp = std::map<GrpId,unsigned>;

    Vec<GrpId> get_groups_in_acl() const {
        Vec<GrpId> res;
        for( std::pair<GrpId,int> p : grp_rights )
            res << p.first;
        return res;
    }

    Vec<UsrId> get_users_in_acl() const {
        Vec<UsrId> res;
        for( std::pair<UsrId,int> p : usr_rights )
            res << p.first;
        return res;
    }

    Vec<std::string> get_grp_rights( GrpId grp ) const {
        auto iter = grp_rights.find( grp );
        if ( iter == grp_rights.end() )
            return {};
        Vec<std::string> res, pr = static_cast<const T *>( this )->get_possible_rights();
        for( unsigned i = 0; i < pr.size(); ++i )
            if ( iter->second & ( 1u << i ) )
                res << pr[ i ];
        return res;
    }

    Vec<std::string> get_usr_rights( UsrId usr ) const {
        auto iter = usr_rights.find( usr );
        if ( iter == usr_rights.end() )
            return {};
        Vec<std::string> res, pr = static_cast<const T *>( this )->get_possible_rights();
        for( unsigned i = 0; i < pr.size(); ++i )
            if ( iter->second & ( 1u << i ) )
                res << pr[ i ];
        return res;
    }

    bool add_grp_right( GrpId grp, const std::string &right ) {
        unsigned n = _get_right_num( right );
        return n != T::NB_RIGHT_TYPES ? _add_grp_right( grp, 1u << n ) : false;
    }

    bool add_usr_right( UsrId usr, const std::string &right ) {
        unsigned n = _get_right_num( right );
        return n != T::NB_RIGHT_TYPES ? _add_usr_right( usr, 1u << n ) : false;
    }

    bool rem_grp_right( GrpId grp, const std::string &right ) {
        unsigned n = _get_right_num( right );
        return n != T::NB_RIGHT_TYPES ? _rem_grp_right( grp, 1u << n ) : false;
    }

    bool rem_usr_right( UsrId usr, const std::string &right ) {
        unsigned n = _get_right_num( right );
        return n != T::NB_RIGHT_TYPES ? _rem_usr_right( usr, 1u << n ) : false;
    }

    unsigned _get_right_num( const std::string &right ) {
        Vec<std::string> pr = static_cast<const T *>( this )->get_possible_rights();
        for( unsigned i = 0; i < pr.size(); ++i )
            if ( pr[ i ] == right )
                return i;
        return T::NB_RIGHT_TYPES;
    }

    bool _usr_right( UsrId usr, unsigned right_flag ) const {
        return _usr_right( usr ) & right_flag;
    }

    unsigned _grp_right( GrpId grp ) const {
        auto iter = grp_rights.find( grp );
        return iter == grp_rights.end() ? 0 : iter->second;
    }

    unsigned _usr_right( UsrId usr ) const {
        auto iter = usr_rights.find( usr );
        return iter == usr_rights.end() ? 0 : iter->second;
    }

    bool _add_grp_right( GrpId grp, unsigned flags ) {
        if ( not static_cast<T *>( this )->_right_to_add_rights() or not flags )
            return false;

        auto iter = grp_rights.find( grp );
        if ( iter == grp_rights.end() )
            iter = grp_rights.insert( iter, std::pair<GrpId,unsigned>( grp, 0u ) );

        flags &= ~ iter->second;
        if ( flags ) {
            BinStream<CbQueue> bq( &operations );
            bq << T::PATCH_TYPE_ADD_GRP_RIGHT << grp << flags;
            iter->second |= flags;
            static_cast<T *>( this )->sig_change();
        }
        return true;
    }

    bool _add_grp_right_wo_cr( GrpId grp, unsigned flags ) {
        if ( flags ) {
            auto iter = grp_rights.find( grp );
            if ( iter == grp_rights.end() )
                grp_rights.insert( iter, std::pair<GrpId,unsigned>( grp, flags ) );
            else
                iter->second |= flags;
            static_cast<T *>( this )->sig_change();
        }
        return true;
    }

    bool _add_usr_right( UsrId usr, unsigned flags ) {
        if ( not _right_to_add_rights() or not flags )
            return false;

        auto iter = usr_rights.find( usr );
        if ( iter == usr_rights.end() )
            iter = usr_rights.insert( iter, std::pair<UsrId,unsigned>( usr, 0u ) );

        flags &= ~ iter->second;
        if ( flags ) {
            BinStream<CbQueue> bq( &operations );
            bq << T::PATCH_TYPE_ADD_USR_RIGHT << usr << flags;
            iter->second |= flags;
            static_cast<T *>( this )->sig_change();
        }
        return true;
    }

    bool _add_usr_right_wo_cr( UsrId usr, unsigned flags ) {
        if ( flags ) {
            auto iter = usr_rights.find( usr );
            if ( iter == usr_rights.end() )
                usr_rights.insert( iter, std::pair<UsrId,unsigned>( usr, flags ) );
            else
                iter->second |= flags;
            static_cast<T *>( this )->sig_change();
        }
        return true;
    }

    bool _rem_grp_right( GrpId grp, unsigned flags ) {
        if ( not _right_to_rem_rights() or not flags )
            return false;

        auto iter = grp_rights.find( grp );
        if ( iter == grp_rights.end() )
            return true;

        flags &= iter->second;
        if ( flags ) {
            BinStream<CbQueue> bq( &operations );
            bq << T::PATCH_TYPE_REM_GRP_RIGHT << grp << flags;

            iter->second &= ~flags;
        }
        return true;
    }

    bool _rem_grp_right_wo_cr( GrpId grp, unsigned flags ) {
        auto iter = grp_rights.find( grp );
        if ( iter != grp_rights.end() )
            iter->second &= ~flags;
        return true;
    }

    bool _rem_usr_right( UsrId usr, unsigned flags ) {
        if ( not _right_to_rem_rights() or not flags )
            return false;

        auto iter = usr_rights.find( usr );
        if ( iter == usr_rights.end() )
            return true;

        flags &= iter->second;
        if ( flags ) {
            BinStream<CbQueue> bq( &operations );
            bq << T::PATCH_TYPE_REM_USR_RIGHT << usr << flags;

            iter->second &= ~flags;
        }
        return true;
    }

    bool _rem_usr_right_wo_cr( UsrId usr, unsigned flags ) {
        auto iter = usr_rights.find( usr );
        if ( iter != usr_rights.end() )
            iter->second &= ~flags;
        return true;
    }

    void _set_creator( UsrId usr_id ) {
        usr_rights.clear();
        usr_rights[ usr_id ] = T::base_rights;
    }

    bool _right_to_add_rights( UsrId usr_id = {} ) {
        return _usr_right( usr_id, T::RIGHT_TO_add_rights );
    }

    bool _right_to_rem_rights( UsrId usr_id = {} ) {
        return _usr_right( usr_id, T::RIGHT_TO_rem_rights );
    }


    CbQueue      operations; ///< patch data
    RightListUsr usr_rights;
    RightListGrp grp_rights;
};

} // namespace RiotBase
