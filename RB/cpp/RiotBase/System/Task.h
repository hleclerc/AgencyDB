#pragma once

#include "DoublyLinkedCycleEndedList.h"
#include "Vec.h"
#include <functional>

namespace RiotBase {
class TimedTaskList;
class TaskReceiver;

/**
  A Task is owned by the TimedTaskList in which it is registered.
*/
class Task {
public:
    enum {
        DO_NOT_DELETE_AFTER_EXECUTION = 1
    };

    Task( int flags = 0 );
    virtual ~Task();

    void                            desactivate(); ///< if we don't want to execute it => THIS WON'T BE OWNED BY TimedTaskList anymore (it it was the registered)
    template<class Op> static Task *New( Op &&func, int flags = 0 );
    void                            do_not_delete_after_execution();

protected:
    using TaskList = DoublyLinkedCycleEndedList<Task>;

    friend struct DefaultGetPrev;
    friend struct DefaultGetNext;
    friend class TimedTaskList;
    friend class TaskReceiver;

    virtual void                    run() = 0;

    // stuff for TimedTaskList (which can be seen as a particular kind of task receiver)
    TaskList                       *timed_task_list_ptr; ///< where `this` belongs. 0 means that the task must not be executed
    PI64                            time_of_exec;        ///< in second * TimedTaskList::div_sec since epoch
    Task                           *prev;                ///< in the time slot list
    Task                           *next;                ///< in the time slot list

    //
    Vec<TaskReceiver *,-1,1>        receivers; ///< if a task has been executed, deletion of a receiver do not have to remove or delete this task
    int                             flags;
};

template<class Op>
Task *Task::New( Op &&func, int flags ) {
    struct TaskFunc : Task {
        TaskFunc( Op &&func, int flags ) : Task( flags ), func( std::move( func ) ) {}
        virtual void run() { func(); }
        Op func;
    };
    return new TaskFunc( std::move( func ), flags );
}

} // namespace RiotBase
