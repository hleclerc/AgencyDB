#pragma once

#include "../Patch/PatchCreator.h"
#include "../Number.h"
#include "Bn.h"

namespace RiotBase {

///
class PatchCreatorNumber : public PatchCreator<PatchCreatorNumber> {
public:
    using Aod = typename PatchCreator<PatchCreatorNumber>::Aod;

    // operation types
    struct op_set     { static const char *name() { return "set";     } static bool valid( const Number &val ) { return true; } };
    struct op_set_und { static const char *name() { return "set_und"; } static bool valid( const Number &val, const Number &old ) { return true; } };
    struct op_add     { static const char *name() { return "add";     } static bool valid( const Number &val ) { return true; } };

    using OperationTypes = std::tuple<op_set,op_set_und,op_add>;

    // rights types
    struct can_read     { static const char *name() { return "read" ; } };
    struct can_decrease { static const char *name() { return "decrease"; } };
    struct can_increase { static const char *name() { return "increase"; } };

    using RightTypes = std::tuple<can_read,can_decrease,can_increase>;

    // methods
    PatchCreatorNumber( Rp *val, UsrId usr_id = {} );

    static std::string   type_name              ();
    const Rp            *prox_ref               () const;
    void                 write_structure        ( std::ostream &os ) const;
                                                
    // helpers                                  
    void                 self_add               ( const Number &n );

    // operations
    using PatchCreator<PatchCreatorNumber>::apply_op;
    void                apply_op                ( op_set    , Aod &aod, const Number &str                    );
    void                apply_op                ( op_set_und, Aod &aod, const Number &str, const Number &old );
    void                apply_op                ( op_add    , Aod &aod, const Number &str                    );

    // rights
    using PatchCreator<PatchCreatorNumber>::right_to;
    bool                right_to                ( op_set    , UsrId as_usr, const Number &str                    );
    bool                right_to                ( op_set_und, UsrId as_usr, const Number &str, const Number &old );
    bool                right_to                ( op_add    , UsrId as_usr, const Number &str                    );

    // undo for PatchCreatorAtomic operations
    using PatchCreator<PatchCreatorNumber>::undo_patch;
    void                undo_patch              ( op_set    , CommonPatchCreatorData::Undo u, const Number &str                    );
    void                undo_patch              ( op_set_und, CommonPatchCreatorData::Undo u, const Number &str, const Number &old );
    void                undo_patch              ( op_add    , CommonPatchCreatorData::Undo u, const Number &str                    );

    // to create _und version when there is a non und one
    using PatchCreator<PatchCreatorNumber>::register_patch_op;
    void                register_patch_op( op_set, BBQ bq, Number &n );

    Number              val;
};



} // namespace RiotBase
