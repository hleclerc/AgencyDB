#pragma once

#include "Task.h"
#include <set>

namespace RiotBase {

/**
  When deleted, the tasks taken as parameters of << are desactivated (if not already deleted).
*/
class TaskReceiver {
public:
    ~TaskReceiver();
    Task            *operator<<        ( Task *task ) { return add_task( task ); } // to add a task
    Task            *add_task          ( Task *task ); // to add a task
    void             rem_task          ( Task *task ); // to remove a task

    virtual void     on_empty_task_list();

    std::set<Task *> tasks;
};

} // namespace RiotBase
