#include "Error.h"

namespace RiotBase {

Error::Error( std::string msg ) : msg( msg ) {
}

std::string Error::type_name() {
    return "Error";
}

void Error::write_to_stream( std::ostream &os ) const {
    os << "Error: " << msg;
}

} // namespace RiotBase
