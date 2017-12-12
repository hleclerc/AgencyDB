#pragma once

#include "../../System/SetOfPatchId.h"
#include "../../System/UsrId.h"
#include "../../System/Date.h"
#include "OtMessage.h"
#include "Rp.h"

namespace RiotBase {
class Category;

/// store the patches for a given variable
class PatchData {
public:
    struct Patch {
        Patch() : prev( 0 ), next( 0 ) {}

        PatchId        patch_id; ///< patch id used in the machine (in `loc_id`) which has made the modification (defined by inst_id)
        UsrId          user_id;  ///< who has made the change
        Date           date;     ///< date of this change (absolute local date)
        CbString       data;     ///< content (operations)

        AutoPtr<Patch> prev;
        Patch         *next;
        int            num;      ///< num in the list (used to sort the patches in functions like get_changes, ...)
    };

    struct ExtState {
        SetOfPatchId   known;
        SetOfPatchId   sent;
    };

    PatchData( Rp **rp = 0 );

    OtMessage                       *get_changes_for                ( const DevId *dev_lst, const FqId &id );
    void                             app_changes                    ( CbQueue &ans, CbString &changes, DevId cur_dev, DevId src_dev, const DevId *dst_devs, UsrId cur_usr );
    void                             freeze_last_modifs_if_necessary( const FqId &id );
    PI64                             new_patch_id_num               () const;
    Patch                           *first                          ();
    const Patch                     *first                          () const;
    Patch                           *find_patch                     ( PI64 num, DevId dev );
    void                             write_to_stream                ( std::ostream &os, DevId cur_dev = 0, UsrId cur_usr = {} ) const;


    Rp                             **rp;
    AutoPtr<Patch>                   last_patch;
    SetOfPatchId                     patch_ids;                              ///<
    mutable std::map<DevId,ExtState> ext_states;                             ///< data sent to other machine, known by other machines
};

} // namespace RiotBase
