#include "../../System/SetOfPatchId.h"
#include "../../System/BinStream.h"
#include "../../System/PatchId.h"
#include "../../System/AutoPtr.h"
#include "../../System/UsrId.h"
#include "../../System/GrpId.h"
#include "../../System/Date.h"
#include "../../System/PVec.h"
#include "../Core/Callback.h"


namespace RiotBase {
struct PatchMerger;
class OtMessage;
class FqId;

///
class CommonPatchCreatorData {
public:
    ///
    struct Patch {
        Patch() : prev( 0 ), next( 0 ) {}

        PatchId        patch_id; ///< patch id used in the machine (in `loc_id`) which has made the modification (defined by inst_id)
        UsrId          user_id;  ///< who has made the changes
        Date           date;     ///< date of this change (absolute local date)
        CbString       data;     ///< content (operations)

        AutoPtr<Patch> prev;
        Patch         *next;
        int            num;      ///< num in the list (used to sort the patches in functions like get_changes, ...)
    };

    ///
    struct ExtState {
        SetOfPatchId   known;
        SetOfPatchId   sent;
    };

    //
    struct ApplyOpData  { enum { NEW_PATCH, UNK_PATCH, STD_PATCH, UNDO }; const FqId *id; UsrId as_usr; int type; };

    /// for the visitors
    struct Transmission {};
    struct Identity     {};
    struct NewPatch     { const FqId &id; UsrId as_usr; PT nb_rem_unk; PT nb_rem_std; BBQ bq_new; };
    struct UnkPatch     { const FqId &id; UsrId as_usr; PT nb_rem_new; PT nb_rem_std; BBQ bq_unk; };
    struct StdPatch     { const FqId &id; UsrId as_usr; PT nb_rem_unk; PT nb_rem_new; };
    struct Print        { const FqId &id; };
    struct Undo         { ApplyOpData *aod; };
    struct UnkNewPatch  { NewPatch *np; BBQ bq_unk; };
    struct NewUnkPatch  { UnkPatch *up; BBQ bq_new; };
    struct StdPatch_New { const FqId &id; UsrId as_usr; PT nb_rem_unk; PT nb_rem_new; BBQ bq_std; };
    struct StdPatch_Unk { const FqId &id; UsrId as_usr; };
    struct NewStdPatch  { StdPatch_New *sp; BBQ bq_new; };
    struct UnkStdPatch  { StdPatch_Unk *sp; BBQ bq_unk; };


    // methods
    CommonPatchCreatorData( UsrId usr_id, unsigned flags );

    OtMessage           *get_changes_for ( PVec<FqId> dst, const FqId &id, std::function<void( BBQ bq, const CbString &data )> trans );
    void                 app_changes     ( BBQ ans, CbStringPtr changes, const FqId &id, const FqId &src, PVec<FqId> dst, PatchMerger *merger );
    void                 freeze_last_mod ();
    PI64                 new_patch_id_num() const;

    Patch               *first           ();
    const Patch         *first           () const;
    Patch               *find_patch      ( PI64 num, DevId dev );

    void                 write_patch_data( std::ostream &os, int nb_sp, const FqId &id, std::function<void( std::ostream &os, CbStringPtr data )> func ) const;

    unsigned             grp_right       ( GrpId grp ) const { auto iter = grp_rights.find( grp ); return iter == grp_rights.end() ? 0 : iter->second; }
    unsigned             usr_right       ( UsrId usr ) const { auto iter = usr_rights.find( usr ); return iter == usr_rights.end() ? 0 : iter->second; }

    Vec<GrpId>           get_groups_in_acl() const;
    Vec<UsrId>           get_users_in_acl() const;

    // attributes
    using UsrRightList = std::map<UsrId,unsigned>;
    using GrpRightList = std::map<GrpId,unsigned>;
    using MapExtStates = std::map<FqId,ExtState>;

    Vec<RcPtr<Callback>> on_change_list; ///< callbacks
    CbQueue              operations;     ///< unregistered patch data
    UsrRightList         usr_rights;
    GrpRightList         grp_rights;
    AutoPtr<Patch>       last_patch;
    mutable MapExtStates ext_states;     ///< data sent to other machine, known by other machines
    SetOfPatchId         patch_ids;      ///<
    BBQ                  bo;             ///< BinStream on operations
};

}
