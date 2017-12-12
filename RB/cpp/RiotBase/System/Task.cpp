#include "TaskReceiver.h"

namespace RiotBase {

Task::Task( int flags ) : timed_task_list_ptr( 0 ), prev( 0 ), next( 0 ), flags( flags ) {
}

Task::~Task() {
    // generic receivers
    for( TaskReceiver *r : receivers )
        r->rem_task( this );
    receivers.clear();

    //
    desactivate();
}

void Task::desactivate() {
    // if it was registered
    if ( timed_task_list_ptr ) {
        timed_task_list_ptr->remove( this );
        timed_task_list_ptr = 0;
    }
}

void Task::do_not_delete_after_execution() {
    flags |= DO_NOT_DELETE_AFTER_EXECUTION;
}

} // namespace RiotBase
