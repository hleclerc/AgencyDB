#include "../../System/Scast.h"
#include "Executed.h"

namespace RiotBase {

std::string Executed::type_name() {
    return "Executed";
}

Rp **Executed::prox_ptr() {
    return &val.rp;
}

const Rp *Executed::prox_ref() const {
    static_assert( sizeof( Executed ) == sizeof( Lazyfier ), "..." );
    return val.rp;
}

void Executed::write_structure( std::ostream &os ) const {
    RO_CALL( write_structure, val.rp, os << "Executed(" ); os << ')';
}

bool Executed::is_symbolic() const {
    return RO_CALL( is_symbolic, val.rp );
}

} // namespace RiotBase

