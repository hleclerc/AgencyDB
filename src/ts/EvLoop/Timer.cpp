#include "../System/TypeConfig.h"
#include "Util/FileDescriptor.h"
#include "Timer.h"

#include <sys/timerfd.h>
#include <stdint.h>
#include <unistd.h>
#include <stdio.h>
#include <time.h>
#include <math.h>

#ifdef METIL_COMP_DIRECTIVE
#pragma lib_name rt
#endif // METIL_COMP_DIRECTIVE

namespace RiotBase {

int Timer::make_timer_fd( double delay, double freq ) {
    timespec now;
    if ( clock_gettime( CLOCK_REALTIME, &now ) == -1 ) {
        perror( "clock_gettime" );
        return -1;
    }

    //
    int fd = timerfd_create( CLOCK_REALTIME, 0 );
    if ( fd == -1 ) {
        perror( "timerfd_create" );
        return fd;
    }

    if ( set_non_blocking( fd ) == -1 ) {
        perror( "non blocking timer fd" );
        close( fd );
        return -1;
    }


    PI64 di = 1e9 * delay + now.tv_nsec; // PI64 would be to small for tv_sec, but for a delay, it's ok
    PI64 fi = 1e9 * freq; // PI64 would be to small for tv_sec, but for a freq, it's ok

    itimerspec new_value;
    new_value.it_value.tv_sec = now.tv_sec + di / 1000000000;
    new_value.it_value.tv_nsec = di % 1000000000;
    new_value.it_interval.tv_sec = fi / 1000000000;
    new_value.it_interval.tv_nsec = fi % 1000000000;
    if ( timerfd_settime( fd, TFD_TIMER_ABSTIME, &new_value, NULL ) == -1 ) {
        perror( "timerfd_settime" );
        close( fd );
        return -1;
    }

    return fd;
}


Timer::Timer( double delay, double freq ) : Event( make_timer_fd( delay, freq ? freq : delay ) ) {
}

void Timer::inp() {
    uint64_t nb_expirations;
    ssize_t s = read( fd, &nb_expirations, sizeof( uint64_t ) );
    if ( timeout( s >= (ssize_t)sizeof( uint64_t ) ? nb_expirations : 1 ) )
        wait_for_more_inp();
}

bool Timer::has_out() {
    return false;
}

} // namespace RiotBase
