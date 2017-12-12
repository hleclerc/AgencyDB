#pragma once

#include "../../Variable/Symbol/RpTemplSym.h"
#include "DbItemFromFile.h"

namespace RiotBase {
pouetox

///
class DbItemContentFromFile : public RpTemplSym<DbItemContentFromFile> {
public:
    DbItemContentFromFile( DbItemFromFile *diff );

    static std::string     type_name();

    DbItemFromFile        *diff;
};

} // namespace RiotBase
