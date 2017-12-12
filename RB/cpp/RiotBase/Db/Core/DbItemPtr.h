#pragma once

#include "../../Variable/Core/RpTempl.h"

namespace RiotBase {
class DbItem;

///
class DbItemPtr : public RpTempl<DbItemPtr> {
public:
    DbItemPtr( RcPtr<DbItem> db_item );

    static std::string   type_name      ();
    Rp                 **prox_ptr       ();
    const Rp            *prox_ref       () const;
    void                 write_structure( std::ostream &os ) const;
    void                 self_then      ( Callback *c ) const;

    RcPtr<DbItem>        db_item;
};

} // namespace RiotBase
