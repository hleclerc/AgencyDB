#pragma once

#include "../../System/CbString.h"
#include "../../System/UsrId.h"
#include <string>

namespace RiotBase {
class FqId;
class Rp;

///
class Category {
public:
    Category();

    virtual std::string     name        () const = 0;
    virtual Rp             *create      ( UsrId as_usr = {} ) const = 0;
    Rp                     *create      ( UsrId as_usr, CbString &patch_data ) const;
    virtual const Category *category_key() const = 0;
    virtual const Category *category_val() const = 0;

    static Category        *find( std::string n );

    Category        *prev;
    static Category *last;
};

} // namespace RiotBase
