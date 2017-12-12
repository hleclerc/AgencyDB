#include "Lazyfier.h"
#include "Symbol.h"

namespace RiotBase {

Rp *new_Symbol( const String &name, const Category *category ) {
    return Lazyfier::New( Symbol::New( name, category ) );
}

Symbol::Symbol( String name, const Category *category ) : RpTemplSym<Symbol>( category ), name( name ), lazyfier( 0 ) {
}

std::string Symbol::type_name() {
    return "Symbol";
}

void Symbol::write_to_stream( std::ostream &os ) const {
    os << name;
}

} // namespace RiotBase
