#pragma once

#include "../../System/VectorClock.h"
#include "../../System/AutoPtr.h"
#include "../../System/Date.h"
#include "../Core/Methods.h"
#include "OtMessage.h"
#include <map>

namespace RiotBase {

///
class PatchManager {
public:
    ///
    struct Patch {
        Patch() : prev( 0 ), next( 0 ) {}

        PatchId        patch_id; ///< patch id used in the machine (in `loc_id`) which has made the modification (defined by inst_id)
        UsrId          usr_id;   ///< who has made the changes
        Date           date;     ///< date of this change (absolute local date)
        CbQueue        data;     ///< content (operations)

        AutoPtr<Patch> prev;
        Patch         *next;
        int            num;      ///< num in the list (used to sort the patches in functions like get_changes, ...)
    };

    ///
    struct ExtState {
        SetOfPatchId   known;
        SetOfPatchId   sent;
    };

    using MapExtStates = std::map<DevId,ExtState>;

    PatchManager( Rp *rp = 0 );
    ~PatchManager();

    OtMessage      *get_changes     ( PVec<DevId> dsts, DevId cur_dev, UsrId cur_usr );
    void            app_changes     ( BBQ ans, CbStringPtr changes, const DevId &src, PVec<DevId> dsts, DevId cur_dev, UsrId cur_usr );

    void            freeze_last_mod ();
    PI64            new_patch_id_num();
    Patch          *first           ();
    Patch          *find_patch      ( PI64 num, DevId dev );

    AutoPtr<Patch>  last_patch;
    MapExtStates    ext_states;     ///< data sent to other machine, known by other machines
    VectorClock patch_ids;      ///< summary of patches stored locally
    Rp             *rp;
};

} // namespace RiotBase
