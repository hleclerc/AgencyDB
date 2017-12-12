#pragma once

#include "../Patch/PatchCreator.h"
#include "../Image.h"

namespace RiotBase {

///
class PatchCreatorImage : public PatchCreator<PatchCreatorImage> {
public:
    enum {
        pmult = 1024 // pixel multiplication -> enable to approximate non integer coordinates as integer
    };

    // operation types
    struct apply_brush_1pix { static const char *name() { return "apply_brush_1pix"; } void apply( PatchCreatorImage *pc, PT x, PT y ) const; static bool valid( PT x, PT y ) { return x != std::numeric_limits<PT>::max(); }  };

    using OperationTypes = std::tuple<apply_brush_1pix>;

    // methods
    PatchCreatorImage( Rp *src );

    static std::string   type_name        ();
    void                 self_special_func( const char *func, Rp **parms, int nb_args );
};

} // namespace RiotBase
