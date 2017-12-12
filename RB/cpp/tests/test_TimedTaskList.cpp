/**
  test of Task, TimedTaskList and TaskReceiver

  (mainly for delayed tasks that can be canceled if a dependency is deleted...)
*/

#include <RiotBase/System/TimedTaskList.h>
#include <RiotBase/System/TaskReceiver.h>
#include <RiotBase/System/Print.h>
#include <RiotBase/System/Check.h>
#include <unistd.h>
#include <sstream>
using namespace RiotBase;

struct A : Task {
    A( std::ostringstream *os, std::string msg, int delay = 0 ) : os( os ), msg( msg ), delay( delay ) {}
    ~A() { *os << "delete(" << msg << ") "; }
    virtual void run() { *os << "msg(" << msg << ") "; }
    std::ostringstream *os;
    std::string msg;
    int delay; ///< doc
};

void test_delay() {
    std::ostringstream os;

    A *tasks[] = {
        new A( &os, "0" , 0 ),
        new A( &os, "0b", 0 ),
        new A( &os, "1" , 1 ),
        new A( &os, "1r", 1 ),
        new A( &os, "2r", 2 ),
        new A( &os, "2d", 2 ),
        new A( &os, "2" , 2 ),
    };

    {
        // start on a changed nb sec
        TimedTaskList tl( 1, 64 );
        for( PI64 t = tl.cur_time_div(); t == tl.cur_time_div(); );

        // fill the task list
        for( A *t : tasks )
            tl.add_task( t, t->delay );
        tasks[ 3 ]->desactivate();
        delete tasks[ 4 ];

        {
            TaskReceiver task_receiver;
            task_receiver << tasks[ 5 ]; // this task will be desactivated because of the task_receiver deletion
        }

        // check that the tasks are executed
        std::cout << "Starting a 4 seconds test" << std::endl;
        for( int i = 0, div = 5; i < 4 * div; ++i ) {
            if ( i % div == div / 2 )
                os << "T" << ( i + div - 1 ) / div << " ";
            tl.exec();
            usleep( 1000000 / 5 );
        }
        CHECK( tl.empty(), true );
        delete tasks[ 3 ];
    }
    CHECK( os.str(), "delete(2r) delete(2d) msg(0) delete(0) msg(0b) delete(0b) T1 msg(1) delete(1) T2 msg(2) delete(2) T3 T4 delete(1r) " );
                  //  delete(2r) msg(0) delete(0) msg(0b) delete(0b) T1 msg(1) delete(1) T2 msg(2) delete(2) T3 T4 delete(1r)
}

int main() {
    test_delay();
}
