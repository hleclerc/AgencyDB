#include "TimedTaskList.h"
#include "Assert.h"

#include <algorithm>
#include <time.h>
#include <cmath>

namespace RiotBase {

TimedTaskList::TimedTaskList( double unit, PT max_delay ) : unit( unit ), list( Size(), PT( std::ceil( max_delay / unit ) ) ) {
    time_last_exe = cur_time_div();
    off_last_exe  = 0;
}

Task *TimedTaskList::add_task( Task *tbd, double delay ) {
    PI64 ct = cur_time_div(), delay_div = std::round( delay / unit ), time_of_exec = ct + delay_div;

    // task has already been registered ?
    if ( tbd->timed_task_list_ptr ) {
        // already scheduled to be executed before or at the same time ?
        if ( tbd->time_of_exec < time_of_exec )
            return tbd;

        // else, we have to remove it from the time slot
        tbd->timed_task_list_ptr->remove( tbd );
    }

    // in which sub-list tbd has to be registered ?
    TaskList *rl = &imm_list;
    if ( delay_div ) {
        PT diff = time_of_exec - time_last_exe;
        rl = list.ptr() + ( off_last_exe + std::min( diff, list.size() - 1 ) ) % list.size();
    }

    // push in the coresponding chained list
    tbd->timed_task_list_ptr = rl;
    tbd->time_of_exec = time_of_exec;
    rl->push_back( tbd );
    return tbd;
}

void TimedTaskList::exec() {
    // non-delayed list
    exec_items_in_list( imm_list );

    // the delayed lists to check
    PT ct = cur_time_div(), end_off = off_last_exe + std::min( ct - time_last_exe, list.size() - 1 );
    time_last_exe = ct;

    for( PT off = off_last_exe; off <= end_off; ++off )
        exec_items_in_list( list[ off % list.size() ] );

    off_last_exe = end_off % list.size();
}

bool TimedTaskList::empty() const {
    for( PT i = 0; i < list.size(); ++i )
        if ( ! list[ i ].empty() )
            return false;
    return true;
}

PI64 TimedTaskList::cur_time_div() {
    struct timespec tp;
    if ( clock_gettime( CLOCK_MONOTONIC_COARSE, &tp ) ) {
         WARNING( "Pb with the monotonic clock" );
         static time_t cpt = 0;
         return cpt++;
    }
    return tp.tv_sec * unit + tp.tv_nsec / ( 1000000000 / unit );
}

void TimedTaskList::exec_items_in_list( TaskList &l ) {
    l.clear( []( Task *t ) {
        t->run();
        if ( t->flags & Task::DO_NOT_DELETE_AFTER_EXECUTION )
            t->desactivate();
        else
            delete t;
    } );
}

}
