#pragma once

#include "RpTemplProxy.h"
#include "Callback.h"

namespace RiotBase {

///
class Callbackifier : public RpTemplProxy<Callbackifier> {
public:
    Callbackifier( Rp *src, Callback *c );

    static std::string   type_name      ();
    Rp                 **prox_ptr       ();
    void                 on_change      ( Callback *c );
    void                 write_structure( std::ostream &os ) const;
    void                 sig_change     ();

    Vec<RcPtr<Callback>> on_change_list;
};

} // namespace RiotBase
