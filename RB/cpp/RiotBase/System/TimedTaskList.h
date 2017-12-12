/* -*- C++ -*- */
#pragma once

#include "DoublyLinkedCycleEndedList.h"
#include "Task.h"

namespace RiotBase {

/// store tasks by execution date (in chained list, for each second starting from now).
/// max delay = $list_size second
/// Tasks are never deleted by this
class TimedTaskList {
public:
    TimedTaskList( double unit, PT max_delay );

    template<class Operation>
    Task         *add_oper          ( Operation &&tbd, double delay ); ///< tbd will be called before $delay_max seconds. If already in the list, the task will be re-planned at min( cur_time + delay_min, time_first_insertion + old_delay_max )
    Task         *add_task          ( Task *tbd, double delay ); ///< will call tbd->exec in $delay seconds. If already in the list, the task will be re-planned at min( cur_time + delay_min, time_first_insertion + old_delay_max )

    void          exec              (); ///< execute what has to be done. Tasks are then deleted and removed from the list

    bool          empty             () const; ///< true if there is no task to execute

    PI64          cur_time_div      (); ///< current time, expressed in TimedTaskList unit

protected:
    using TaskList = DoublyLinkedCycleEndedList<Task>;

    void          exec_items_in_list( TaskList &l );

    double        unit;
    Vec<TaskList> list;          ///< lists of task, sorted by delay to be executed
    TaskList      imm_list;      ///< task to be executed immediately
    PI64          time_last_exe;
    PT            off_last_exe;
};

template<class Operation>
Task *TimedTaskList::add_oper( Operation &&tbd, double delay ) {
    return add_task( Task::New( std::move( tbd ) ), delay );
}

}
