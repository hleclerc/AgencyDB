#include "PatchCreatorAtomic.h"
#include "PatchMergerAtomic.h"
#include "../Core/Methods.h"
#include "../Core/Surdef.h"

namespace RiotBase {

namespace {

/// what to do with each data from unk and a new add_grp_right
struct UnkNewPatchVisitor_add_grp_right : PatchCreatorAtomic::Visitor {
    UnkNewPatchVisitor_add_grp_right( GrpId grp_new, unsigned &flags_new ) : grp_new( grp_new ), flags_new( flags_new ) {}
    void add_grp_right( GrpId grp, unsigned &flags ) { if ( grp == grp_new ) flags &= ~ flags_new; }
    void rem_grp_right( GrpId grp, unsigned &flags ) { if ( grp == grp_new ) flags &= ~ flags_new; }
    Inode     grp_new;
    unsigned &flags_new;
};

/// what to do with each data from unk and a new add_usr_right
struct UnkNewPatchVisitor_add_usr_right : PatchCreatorAtomic::Visitor {
    UnkNewPatchVisitor_add_usr_right( UsrId usr_new, unsigned &flags_new ) : usr_new( usr_new ), flags_new( flags_new ) {}
    void add_usr_right( UsrId usr, unsigned flags ) { if ( usr == usr_new ) flags &= ~ flags_new; }
    void rem_usr_right( UsrId usr, unsigned flags ) { if ( usr == usr_new ) flags &= ~ flags_new; }
    Inode     usr_new;
    unsigned &flags_new;
};

/// what to do with each data from unk and a new rem_grp_right
struct UnkNewPatchVisitor_rem_grp_right : PatchCreatorAtomic::Visitor {
    UnkNewPatchVisitor_rem_grp_right( GrpId grp_new, unsigned &flags_new ) : grp_new( grp_new ), flags_new( flags_new ) {}
    void add_grp_right( GrpId grp, unsigned &flags ) { if ( grp == grp_new ) flags &= ~ flags_new; }
    void rem_grp_right( GrpId grp, unsigned &flags ) { if ( grp == grp_new ) flags &= ~ flags_new; }
    Inode     grp_new;
    unsigned &flags_new;
};

/// what to do with each data from unk and a new rem_usr_right
struct UnkNewPatchVisitor_rem_usr_right : PatchCreatorAtomic::Visitor {
    UnkNewPatchVisitor_rem_usr_right( UsrId usr_new, unsigned &flags_new ) : usr_new( usr_new ), flags_new( flags_new ) {}
    void add_usr_right ( UsrId usr, unsigned flags ) { if ( usr == usr_new ) flags &= ~ flags_new; }
    void rem_usr_right ( UsrId usr, unsigned flags ) { if ( usr == usr_new ) flags &= ~ flags_new; }
    Inode     usr_new;
    unsigned &flags_new;
};

/// for each item in new patches...
/// data contain fully specified dev_id and usr_id (operations_visitor will transform some dev_id and usr_id to 0 if equal to cur_dev or cur_usr)
struct NewPatchVisitor {
    void set_value( const CbString &d_0 ) {
        if ( pcs->_right_to_write( as_usr ) ) {
            tmp_bq_new << PatchCreatorAtomic::PATCH_TYPE_SET_VALUE_UND << d_0;
            pcs->_get_raw_data( tmp_bq_new );
            pcs->_set_raw_data_wo_cr( d_0, cur_dev );
        }
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
        UnkNewPatchVisitor_add_grp_right upv{ grp, flags };
        PatchCreatorAtomic::operations_visitor( cp_cq_unk, upv, &pms->cq_unk );

        // check the rights
        if ( not pcs->_right_to_add_rights( as_usr ) ) {
            pms->cq_unk = std::move( cp_cq_unk );
            return;
        }

        // register the new operation
        if ( flags ) {
            tmp_bq_new << PatchCreatorAtomic::PATCH_TYPE_ADD_GRP_RIGHT << grp << flags;
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
        UnkNewPatchVisitor_rem_grp_right upv{ grp, flags };
        PatchCreatorAtomic::operations_visitor( cp_cq_unk, upv, &pms->cq_unk );

        // check the rights
        if ( not pcs->_right_to_rem_rights( as_usr ) ) {
            pms->cq_unk = std::move( cp_cq_unk );
            return;
        }

        // register the new operation
        if ( flags ) {
            tmp_bq_new << PatchCreatorAtomic::PATCH_TYPE_REM_GRP_RIGHT << grp << flags;
            pcs->_rem_grp_right_wo_cr( grp, flags );
        }
    }
    void add_usr_right( UsrId usr, unsigned flags ) {
        // updates for local storage
        if ( usr.dev == cur_dev )
            usr.dev = 0;
        if ( usr == cur_usr )
            usr = Inode{};

        // check that XOR on flags would work
        flags &= ~ pcs->_usr_right( usr );
        if ( not flags )
            return;

        // modifications
        CbQueue cp_cq_unk = std::move( pms->cq_unk );
        UnkNewPatchVisitor_add_usr_right upv{ usr, flags };
        PatchCreatorAtomic::operations_visitor( cp_cq_unk, upv, &pms->cq_unk );

        // check the rights
        if ( not pcs->_right_to_add_rights( as_usr ) ) {
            pms->cq_unk = std::move( cp_cq_unk );
            return;
        }

        // register the new operation
        if ( flags ) {
            tmp_bq_new << PatchCreatorAtomic::PATCH_TYPE_ADD_USR_RIGHT << usr << flags;
            pcs->_add_usr_right_wo_cr( usr, flags );
        }
    }
    void rem_usr_right( UsrId usr, unsigned flags ) {
        // updates for local storage
        if ( usr.dev == cur_dev )
            usr.dev = 0;
        if ( usr == cur_usr )
            usr = Inode{};

        // check that XOR on flags would work
        flags &= pcs->_usr_right( usr );
        if ( not flags )
            return;

        // register the new operation
        CbQueue cp_cq_unk = std::move( pms->cq_unk );
        UnkNewPatchVisitor_rem_usr_right upv{ usr, flags };
        PatchCreatorAtomic::operations_visitor( cp_cq_unk, upv, &pms->cq_unk );

        // check the rights
        if ( not pcs->_right_to_rem_rights( as_usr ) ) {
            pms->cq_unk = std::move( cp_cq_unk );
            return;
        }

        // register the new operation
        if ( flags ) {
            tmp_bq_new << PatchCreatorAtomic::PATCH_TYPE_REM_USR_RIGHT << usr << flags;
            pcs->_rem_usr_right_wo_cr( usr, flags );
        }
    }
    void set_value_und( const CbString &d, const CbString &o ) {
        WARNING( "Patches sent through the network should not contain _und patch type versions" );
        set_value( d );
    }
    DevId               cur_dev;
    Inode               cur_usr;
    Inode               as_usr;
    PatchMergerAtomic  *pms;
    PatchCreatorAtomic *pcs;
    BinStream<CbQueue>  tmp_bq_new;
};

}

PatchMergerAtomic::PatchMergerAtomic( PatchCreatorAtomic *pcs ) : pcs( pcs ) {
}

bool PatchMergerAtomic::is_valid( const CbString &msg ) const {
    return true;
}

void PatchMergerAtomic::set_creator( UsrId usr_id ) {
    pcs->_set_creator( usr_id );
}

void PatchMergerAtomic::new_patch( CbString &data, const FqId &id, UsrId as_usr, PT nb_rem_unk, PT nb_rem_std ) {
    // goal:
    // - make a copy of the patch data, with local identifiers (if dev_id == cur_dev -> dev_id = 0, ...)
    // - make the changes
    // - update cq_new describing changes that std_ and unk_ patches do not know
    CbQueue tmp_cq_new;
    NewPatchVisitor npc{ db, as_usr, this, pcs, &tmp_cq_new };
    PatchCreatorAtomic::operations_visitor( data, npc );
    if ( nb_rem_unk or nb_rem_std )
        cq_new.write_some( tmp_cq_new ); // if there are unk or std patches, we need a representation of the preceding new stuff to modify the corresponding data
    data = std::move( tmp_cq_new );
}

void PatchMergerAtomic::unk_patch( CbString &data, const FqId &id, UsrId as_usr, PT nb_rem_new, PT nb_rem_std ) {
    // pcs->_set_raw_data( data, cur_dev, false );
    TODO;
}

void PatchMergerAtomic::std_patch( CbString &data, const FqId &id, UsrId as_usr, PT nb_rem_unk, PT nb_rem_new ) {
    // pcs->_set_raw_data( data, cur_dev, false );
    TODO;
}

void PatchMergerAtomic::undo( const CbString &data, const FqId &id ) {
    TODO;
}

void PatchMergerAtomic::transmission( BinStream<CbQueue> bq, const CbString &patch_data, const FqId &id, UsrId as_usr ) {
    struct GetChangesVisitor {
        void set_value    ( const CbString &d         ) { bn << PatchCreatorAtomic::PATCH_TYPE_SET_VALUE << d; }
        void add_grp_right( GrpId grp, unsigned flags ) { bn << PatchCreatorAtomic::PATCH_TYPE_ADD_GRP_RIGHT << grp.glo_dev( cur_dev ) << flags; }
        void rem_grp_right( GrpId grp, unsigned flags ) { bn << PatchCreatorAtomic::PATCH_TYPE_REM_GRP_RIGHT << grp.glo_dev( cur_dev ) << flags; }
        void add_usr_right( UsrId usr, unsigned flags ) { bn << PatchCreatorAtomic::PATCH_TYPE_ADD_USR_RIGHT << ( usr ? usr : cur_usr ).glo_dev( cur_dev ) << flags; }
        void rem_usr_right( UsrId usr, unsigned flags ) { bn << PatchCreatorAtomic::PATCH_TYPE_REM_USR_RIGHT << ( usr ? usr : cur_usr ).glo_dev( cur_dev ) << flags; }
        void set_value_und( const CbString &d, const CbString &o ) { set_value( d ); }
        BinStream<CbQueue> bn;
        DevId              cur_dev;
        Inode              cur_usr;
    };

    CbQueue cn;
    GetChangesVisitor gc{ &cn, db };
    PatchCreatorAtomic::operations_visitor( patch_data, gc );
    bq << std::move( cn );
}

} // namespace RiotBase
