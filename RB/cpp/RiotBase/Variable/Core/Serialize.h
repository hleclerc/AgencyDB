#pragma once

#include "../../System/Vec.h"
#include "Methods.h"

namespace RiotBase {

///
class Serializer {
public:
    Serializer( bool known_nb_outputs );

    void             write_to( BBQ bq ) const;

    Vec<const Rp *>  to_serialize;
    bool             known_nb_outputs;
};

/// make something that can be sent to a BBQ (only one output version)
Serializer serializer( const Rp *rp );

} // namespace RiotBase
