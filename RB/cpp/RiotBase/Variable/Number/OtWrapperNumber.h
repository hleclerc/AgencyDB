#pragma once

#include "../../System/TypeListAppender.h"
#include "../Ot/OtWrapper.h"
#include "../Number.h"

namespace RiotBase {

///
class OtWrapperNumber : public OtWrapper<OtWrapperNumber> {
public:
    // using Aod = typename OtWrapper<OtWrapperNumber>::Aod;

    // operation types
    struct op_set     { static const char *name() { return "set";     } static bool valid( const Number &val ) { return true; } };
    struct op_set_und { static const char *name() { return "set_und"; } static bool valid( const Number &val, const Number &old ) { return true; } };
    struct op_add     { static const char *name() { return "add";     } static bool valid( const Number &val ) { return true; } };
    using OperationTypes = typename TypeListAppender<OtWrapperOperationTypes,op_set,op_set_und,op_add>::T;

    // rights types
    struct can_read     { static const char *name() { return "read" ; } };
    struct can_decrease { static const char *name() { return "decrease"; } };
    struct can_increase { static const char *name() { return "increase"; } };
    using RightTypes = typename TypeListAppender<OtWrapperRightTypes,can_read,can_decrease,can_increase>::T;

    // methods
    OtWrapperNumber( Rp *val, UsrId usr_id = {} );

    static std::string  type_name               ();
    const Rp           *prox_ref                () const;
    void                write_structure         ( std::ostream &os ) const;

    // helpers
    void                self_add                ( const Number &n );

    // operations
    using OtWrapper<OtWrapperNumber>::apply_op;
    void                apply_op                ( op_set    , Aod &aod, const Number &str                    );
    void                apply_op                ( op_set_und, Aod &aod, const Number &str, const Number &old );
    void                apply_op                ( op_add    , Aod &aod, const Number &str                    );

    // rights
    using OtWrapper<OtWrapperNumber>::right_to;
    bool                right_to                ( op_set    , UsrId as_usr, const Number &str                    );
    bool                right_to                ( op_set_und, UsrId as_usr, const Number &str, const Number &old );
    bool                right_to                ( op_add    , UsrId as_usr, const Number &str                    );

    // undo for OtWrapperAtomic operations
    using OtWrapper<OtWrapperNumber>::undo_patch;
    void                undo_patch              ( op_set    , Aod &aod, Number &str              );
    void                undo_patch              ( op_set_und, Aod &aod, Number &str, Number &old );
    void                undo_patch              ( op_add    , Aod &aod, Number &str              );

    //
    using OtWrapper<OtWrapperNumber>::transmission;
    void                transmission            ( op_set_und, BBQ br, Number &str, Number &old );

    //
    using OtWrapper<OtWrapperNumber>::register_patch_op;
    void                register_patch_op       ( op_set    , BBQ bq, Number &str );

    //
    Number              val;
};

}
