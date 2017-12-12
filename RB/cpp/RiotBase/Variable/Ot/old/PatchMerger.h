#pragma once

#include "../../System/CbQueue.h"
#include "../../System/UsrId.h"

namespace RiotBase {
class FqId;

///
struct PatchMerger {
    virtual ~PatchMerger() {}

    virtual bool is_valid    ( CbStringPtr msg ) const { return true; }
    virtual void set_creator ( UsrId usr_id ) = 0;

    virtual void new_patch   ( CbString &data, const FqId &id, UsrId as_usr, PT nb_rem_unk, PT nb_rem_std ) = 0;
    virtual void unk_patch   ( CbString &data, const FqId &id, UsrId as_usr, PT nb_rem_new, PT nb_rem_std ) = 0;
    virtual void std_patch   ( CbString &data, const FqId &id, UsrId as_usr, PT nb_rem_unk, PT nb_rem_new ) = 0;

    virtual void undo        ( CbStringPtr data, const FqId &id, UsrId as_usr ) = 0;
    virtual void finalize    ( const FqId &id ) {}
};

}
