/**
  test of Task, TimedTaskList and TaskReceiver

  (mainly for delayed tasks that can be canceled if a dependency is deleted...)
*/
//// nsmake global inc_path ..

#include <RiotBase/System/DoublyLinkedCycleEndedList.h>
#include <RiotBase/System/Print.h>
#include <RiotBase/System/Check.h>
#include <RiotBase/System/Vec.h>
using namespace RiotBase;

struct A {
    A( int val ) : prev( 0 ), next( 0 ), val( val ) {}
    void write_to_stream( std::ostream &os ) const { os << val; }

    A  *prev;
    A  *next;
    int val;
};

void test_base_ops() {
    Vec<A> r;
    for( int var = 0; var < 5; ++var )
        r << var;

    DoublyLinkedCycleEndedList<A> l;
    for( A &a : r )
        l << &a;
    CHECK( l.size(), r.size() );
    CHECK( l, "0 1 2 3 4" );
    CHECK( l.reversed(), "4 3 2 1 0" );

    CHECK( l.remove( &r[ 1 ] ).remove( &r[ 3 ] ), "0 2 4" );
    CHECK( l.reversed(), "4 2 0" );
    CHECK( l.remove( &r[ 0 ] ), "2 4" );
    CHECK( l.reversed(), "4 2" );
    CHECK( l.remove( &r[ 4 ] ), "2" );
    CHECK( l.reversed(), "2" );
    CHECK( l.remove( &r[ 2 ] ), "" );
    CHECK( l.reversed(), "" );
}

int main() {
    test_base_ops();
}


