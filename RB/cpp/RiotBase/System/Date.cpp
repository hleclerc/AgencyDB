#include "Print.h"
#include "Date.h"
#include <stdio.h>
#include <time.h>
#include <thread>

namespace RiotBase {

Date::Date( PI64 msec ) : msec( msec ) {
}

Date::Date() : msec( 0 ) {

}

void Date::write_to_stream( std::ostream &os ) const {
    //    static double beg = *this;
    //    os << double( *this ) - beg;
    os << double( *this );
}

RiotBase::Date::operator bool() const {
    return msec;
}

Date::operator double() const {
    return msec / 1000.0; // sec + double( nsec ) / 1e9;
}

Date Date::operator-( const Date &d ) const {
    return { msec - d.msec };
    //    if ( nsec >= d.nsec )
    //        return { sec - d.sec, nsec - d.nsec };
    //    return { sec - d.sec - 1, nsec + 1000000000 - d.nsec };
}

Date Date::operator+( const Date &d ) const {
    return { msec + d.msec };
    //    if ( nsec + d.nsec >= 1000000000 )
    //        return { sec + d.sec + 1, nsec + d.nsec - 1000000000 };
    //    return { sec + d.sec, nsec + d.nsec };
}

bool Date::operator>=( const Date &d ) const {
    return msec >= d.msec;
    //    return sec == d.sec ? nsec >= d.nsec : sec >= d.sec;
}

bool Date::operator<( const Date &d ) const {
    return msec < d.msec;
    //    return sec == d.sec ? nsec < d.nsec : sec < d.sec;
}

bool Date::operator!=( const Date &d ) const {
    return msec != d.msec;
    //    return sec != d.sec or nsec != d.nsec;
}

bool Date::operator==( const Date &d ) const {
    return msec == d.msec;
    //     return sec == d.sec and nsec == d.nsec;
}

void Date::operator+=( const Date &d ) {
    msec += d.msec;
    // *this = *this + d;
}

void Date::operator-=( const Date &d ) {
    msec -= d.msec;
    // *this = *this - d;
}

Date Date::current() {
    timespec now;
    if ( clock_gettime( CLOCK_REALTIME, &now ) == -1 ) {
        perror( "clock_gettime" );
        return 0;
    }
    return now.tv_sec * 1000 + now.tv_nsec;
}

void Date::sleep( double sec ) {
    std::this_thread::sleep_for( std::chrono::milliseconds( int64_t( sec * 1000 ) ) );
    //    timespec rqtp, rmtp;
    //    rqtp.tv_sec = sec;
    //    rqtp.tv_nsec = ( sec - rqtp.tv_sec ) * 1e9;
    //    while ( nanosleep( &rqtp, &rmtp ) and errno == EINTR )
    //        rqtp = rmtp;
}

} // namespace RiotBase
