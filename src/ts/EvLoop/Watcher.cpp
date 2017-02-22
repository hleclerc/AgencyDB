#include "../System/TypeConfig.h"
#include "../System/Print.h"
#include "Util/FileDescriptor.h"
#include "Watcher.h"
#include "EvLoop.h"

#include <sys/eventfd.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>

namespace RiotBase {

static int make_watcher_fd() {
    // get a (non blocking) file descriptor
    int fd = eventfd( 0, EFD_NONBLOCK | EFD_SEMAPHORE );
    if ( fd < 0 )
        perror( "eventfd" );
    return fd;
}


Watcher::Watcher() : Event( make_watcher_fd() ) {
}

void Watcher::sig() {
    PI64 val = 1;
    write( fd, &val, sizeof( val ) );
}

void Watcher::inp() {
    while ( true ) {
        PI64 val;
        ssize_t s = read( fd, &val, sizeof( val ) );

        // end of the connection
        if ( s == 0 )
            return wait_for_more_inp();

        // error
        if ( s < 0 ) {
            #warning EINTR ?
            if ( errno == EAGAIN or errno == EWOULDBLOCK )
                return wait_for_more_inp();
            return perror( "Pb reading signals" );
        }

        if ( not event( val ) )
            return wait_for_more_inp();
    }
}

bool Watcher::has_out() {
    return false;
}

bool Watcher::event( PI64 n ) {
    return true;
}

} // namespace RiotBase


