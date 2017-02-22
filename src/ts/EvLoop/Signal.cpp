#include "Util/FileDescriptor.h"
#include "EvLoop.h"
#include "Signal.h"

#include <sys/signalfd.h>
#include <signal.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>

namespace RiotBase {

static int make_signal_fd( const int *sigs ) {
    // set up a mask
    sigset_t mask;
    sigemptyset( &mask );
    for( int i = 0; sigs[ i ] >= 0; ++i )
        sigaddset( &mask, sigs[ i ] );

    // block signals
    if ( sigprocmask( SIG_BLOCK, &mask, 0 ) == -1 ) {
        perror( "sigprocmask" );
        return -1;
    }

    // get a (non blocking) file descriptor
    int fd = signalfd( -1, &mask, 0 );
    if ( fd < 0 )
        perror( "signalfd" );
    if ( set_non_blocking( fd ) < 0 ) {
        perror( "non blocking signalfd" );
        close( fd );
        return -1;
    }

    return fd;
}


Signal::Signal( const int *sigs ) : Event( make_signal_fd( sigs ) ) {
}

void Signal::inp() {
    signalfd_siginfo sig_info;
    while ( true ) {
        ssize_t s = read( fd, &sig_info, sizeof( sig_info ) );

        // end of the connection
        if ( s == 0 )
            return;

        // error
        if ( s < 0 ) {
            #warning EINTR ?
            if ( errno == EAGAIN or errno == EWOULDBLOCK )
                return wait_for_more_inp();
            return perror( "Pb reading signals" );
        }

        if ( s < (ssize_t)sizeof( sig_info ) ) {
            fprintf( stderr, "TODO: partial read with signals\n" );
            return;
        }

        if ( not signal( sig_info.ssi_signo ) )
            return;
    }
}

bool Signal::has_out() {
    return false;
}

} // namespace RiotBase
