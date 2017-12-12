#pragma once

#include "OtWrapperString.h"

namespace RiotBase {

///
class OtWrapperStringCursor : public RpTempl<OtWrapperStringCursor> {
public:
    OtWrapperStringCursor( OtWrapperString *pcs, OtWrapperString::Cursor *cur ); ///< cur may be null
    ~OtWrapperStringCursor();

    static std::string       type_name();
    void                     write_to_stream( std::ostream &os ) const;
    void                     write_structure( std::ostream &os ) const;
    bool                     to_PT          ( PT   *res, PT nb_items ) const;
    bool                     to_Bool        ( bool *res, PT nb_items ) const;
    Rp                      *copy           () const;

    OtWrapperString         *pcs;
    OtWrapperString::Cursor *cur;
};

} // namespace RiotBase
