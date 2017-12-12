#pragma once

#include "../../System/Vec.h"
#include "../String.h"

namespace RiotBase {

///
class Graphviz {
public:
    Graphviz &operator<<( const Rp *v );
    int       display   ( const std::string &base_filename = ".res" );
    void      write     ( std::ostream &os );

protected:
    Vec<const Rp *> lst_var;
};

} // namespace RiotBase
