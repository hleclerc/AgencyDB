#pragma once

#include "../Core/PatchMerger.h"

namespace RiotBase {
class PatchCreatorAtomic;

///
class PatchMergerAtomic : public PatchMerger {
public:
    PatchMergerAtomic( PatchCreatorAtomic *pcs );

    virtual bool        is_valid    ( const CbString &msg ) const;
    virtual void        set_creator ( UsrId usr_id );

    virtual void        new_patch   ( CbString &data, const FqId &id, UsrId as_usr, PT nb_rem_unk, PT nb_rem_std );
    virtual void        unk_patch   ( CbString &data, const FqId &id, UsrId as_usr, PT nb_rem_new, PT nb_rem_std );
    virtual void        std_patch   ( CbString &data, const FqId &id, UsrId as_usr, PT nb_rem_unk, PT nb_rem_new );

    virtual void        undo        ( const CbString &data, const FqId &id );

    virtual void        transmission( BinStream<CbQueue> bq, const CbString &patch_data, const FqId &id, UsrId as_usr );

    PatchCreatorAtomic *pcs;
    CbQueue             cq_unk; ///< effect of unknown patches
    CbQueue             cq_new;
};

} // namespace RiotBase
