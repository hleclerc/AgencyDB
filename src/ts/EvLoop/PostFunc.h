#pragma once

#include "EvLoop.h"

namespace RiotBase {

class PostFunc {
public:
    PostFunc() : ev_loop( 0 ), prev_post_func( 0 ) {}
    virtual ~PostFunc() {}

    virtual bool exec() = 0; ///< return true is something has been done that may necessitate to loop over the other postfuncs

    EvLoop   *ev_loop;
    PostFunc *prev_post_func;
};

} // namespace RiotBase
