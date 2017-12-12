#include "TaskReceiver.h"

namespace RiotBase {

TaskReceiver::~TaskReceiver() {
    for( Task *t : tasks ) { // the task receiver is deleted, meaning that the tasks are not going to be executed
        t->receivers.remove_first_unordered( this );
        delete t;
    }
}

Task *TaskReceiver::add_task( Task *task ) {
    std::set<Task *>::iterator iter = tasks.find( task );
    if ( iter == tasks.end() ) {
        task->receivers.push_back( this ); // needed because rem_task() is called in a receiver when a task is deleted
        tasks.insert( iter, task );
    }
    return task;
}

void TaskReceiver::rem_task( Task *task ) {
    std::set<Task *>::iterator iter = tasks.find( task );
    if ( iter != tasks.end() ) {
        task->receivers.remove_first_unordered( this );
        tasks.erase( iter );
        if ( tasks.empty() )
            on_empty_task_list();
    }
}

void TaskReceiver::on_empty_task_list() {
}

} // namespace RiotBase
