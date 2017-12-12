#pragma once

#include "../../Variable/Core/Callback.h"

namespace RiotBase {
class DbItem;
class Db;

///
class DbCallback : public Callback {
public:
    DbCallback( DbItem *db_item );

    virtual void        exec( Rp **rp = 0 );
    virtual std::string name() const;

    DbItem             *db_item;
};

} // namespace RiotBase
