#pragma once

#include "../../Variable/Symbol/LazySource.h"

namespace RiotBase {
class Category;

///
class ObjInFile : public LazySource<ObjInFile> {
public:
    ObjInFile( const Category *category ) : LazySource<ObjInFile>( category ) {}

    static std::string type_name();
};

} // namespace RiotBase
