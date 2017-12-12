#pragma once

#include "PatchCreatorString.h"
#include "../Core/RpTempl.h"

namespace RiotBase {

///
class PatchCreatorStringCursor : public RpTempl<PatchCreatorStringCursor> {
public:
    PatchCreatorStringCursor( PatchCreatorString *pcs, PatchCreatorString::Cursor *cur ); ///< cur may be null
    ~PatchCreatorStringCursor();

    static std::string          type_name();
    void                        write_to_stream( std::ostream &os ) const;
    void                        write_structure( std::ostream &os ) const;
    bool                        to_PT          ( PT   *res, PT nb_items ) const;
    bool                        to_Bool        ( bool *res, PT nb_items ) const;
    Rp                         *copy           () const;

    PatchCreatorString         *pcs;
    PatchCreatorString::Cursor *cur;
};

} // namespace RiotBase
