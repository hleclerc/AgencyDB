#pragma once

#include "../../Variable/Core/Callback.h"
#include "../../Variable/Core/RpTempl.h"
#include "../../System/RcPtr.h"

namespace RiotBase {
class DbItemContentFromFile;
class DataFile;
class DbItem;

///
class DbItemFromFile : public RpTempl<DbItemFromFile> {
public:
    DbItemFromFile( const Category *cat, DataFile *datafile, PI64 offset, PI64 len );
    ~DbItemFromFile();

    static std::string    type_name      ();
    void                  self_on_change ( Callback *c );
    const Category       *category       () const;
    Rp                  **prox_ptr       ();
    const Rp             *prox_ref       () const;

    DbItem               *db_item;
    DataFile             *datafile;
    PI64                  offset;
    PI64                  len;
    const Category       *cat;
    Vec<RcPtr<Callback>>  callbacks;
    mutable Rp           *content;
    bool                  scheduled;
};

} // namespace RiotBase
