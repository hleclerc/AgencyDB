#pragma once

#include "RpTemplSym.h"
#include "../String.h"

namespace RiotBase {

///
class Symbol : public RpTemplSym<Symbol> {
public:
    Symbol( String name, const Category *category );

    static std::string     type_name      ();
    void                   write_to_stream( std::ostream &os ) const;

    String                 name;
    Lazyfier              *lazyfier;
};

} // namespace RiotBase
