#include "../System/Log.h"
#include "EvLoop.h"
#include "Event.h"

#include <unistd.h>

namespace RiotBase {

Event::Event( int fd ) : fd( fd ), ev_loop( nullptr ), next_ev_to_del( 0 ), waiting( WF_more_inp ), errors( 0 ) {
}

Event::Event( VtableOnly vo ) {
}

Event::~Event() {
    if ( fd > 2 )
        close( fd );
}

void Event::inp() {
}

void Event::out() {
}

void Event::rdy() {
}

bool Event::has_out() {
    return true;
}

bool Event::can_be_deleted_by_ev_loop() const {
    return true;
}

void Event::wait_for_more_out() {
    if ( not ( waiting & Event::WF_more_out ) ) {
        ++ev_loop->nb_out_waiting;
        waiting |= WF_more_out;
    }
}

void Event::reg_for_deletion() {
    if ( next_ev_to_del )
        return;
    if ( ev_loop ) {
        next_ev_to_del = ev_loop->last_ev_to_del ? ev_loop->last_ev_to_del : this;
        ev_loop->last_ev_to_del = this;
    } else
        DISP_ERROR( "reg_for_deletion() on an Event that has not been registered yet" );
}

} // namespace RiotBase
