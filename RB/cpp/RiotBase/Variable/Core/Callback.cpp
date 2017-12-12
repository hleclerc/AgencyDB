#include "Callback.h"

namespace RiotBase {

CycleEndedList<Callback> Callback::waiting;

Callback::~Callback() {
    if ( prev )
        waiting.remove( this );
}

std::string Callback::name() const {
    return "?";
}

bool Callback::exec_all() {
    bool ret = not waiting.empty();

    waiting.clear( []( Callback *c ) {
        c->exec();
    } );

    return ret;
}

} // namespace RiotBase
