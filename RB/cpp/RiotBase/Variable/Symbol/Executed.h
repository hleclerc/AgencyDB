#pragma once

#include "../Core/AutoRpDel.h"
#include "Lazyfier.h"

namespace RiotBase {
class GenSymSelect;

///
class Executed : public RpTempl<Executed> {
public:
    static std::string  type_name      ();
    Rp                **prox_ptr       ();
    const Rp           *prox_ref       () const;
    void                write_structure( std::ostream &os ) const;
    bool                is_symbolic    () const;

    AutoRpDel           val;
    Vec<GenSymSelect *> fake_sym_selects; ///< because we want sizeof( Executed ) == sizeof( Lazyfier )
};

} // namespace RiotBase
