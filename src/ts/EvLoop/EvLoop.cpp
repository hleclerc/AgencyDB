#include "../System/Assert.h"
#include "../System/Log.h"

#include "Util/CheckSent.h"
#include "PostFunc.h"
// #include "Watcher.h"

#include <linux/sockios.h>
#include <sys/ioctl.h>
#include <sys/epoll.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>

namespace RiotBase {

EvLoop::EvLoop() {
    // epoll init
    event_fd = epoll_create1( 0 );
    if ( event_fd < 0 )
        perror( "epoll_create1" );

    // default values
    last_ev_to_del = 0;
    last_post_func = 0;
    nb_out_waiting = 0;
    timer          = 0;
    ret            = 0;
    
    //
    // watcher        = new Watcher;
}

EvLoop::~EvLoop() {
    if ( event_fd >= 0 ) close( event_fd );
    // delete watcher;
    delete timer;
}

int EvLoop::run() {
    cnt = true;

    // loop
    const int max_events = 64;
    epoll_event events[ max_events ];
    do {
        // post funcs
        while ( exec_post_funcs() );
        
        //
        int nfds = epoll_wait( event_fd, events, max_events, -1 );
        if ( nfds == -1 ) {
            if ( errno != EINTR ) {
                perror( "epoll_wait" );
                return -1;
            }
            continue;
        }

        // for each event
        for( int n = 0; n < nfds; ++n ) {
            Event *rq = reinterpret_cast<Event *>( events[ n ].data.ptr );

            if ( ( events[ n ].events & EPOLLOUT ) and not rq->next_ev_to_del ) { // ready for output (after a EAGAIN or not)
                if ( rq->waiting & Event::WF_more_out ) {
                    rq->waiting &= ~Event::WF_more_out;
                    --nb_out_waiting;
                }
                rq->out();
            }

            if ( ( events[ n ].events & EPOLLIN ) and not rq->next_ev_to_del ) { // there are some input data
                rq->waiting &= ~Event::WF_more_inp;
                rq->inp();
            }

            if ( events[ n ].events & EPOLLRDHUP ) // end of the connection (gracefully closed by peer)
                rq->rd_hup_error();

            if ( events[ n ].events & EPOLLHUP ) // end of the connection
                rq->hup_error();

            if ( events[ n ].events & EPOLLERR ) // generic error
                rq->generic_error();

            if ( rq->waiting == 0 ) // if nothing more to do, add it to the deletion list
                rq->reg_for_deletion();
        }

        // deletions (due to closed connections, errors, ...)
        if ( Event *rq = last_ev_to_del ) {
            last_ev_to_del = 0;

            while ( true ) {
                Event *oq = rq;
                rq = rq->next_ev_to_del;

                //
                if ( oq->waiting & Event::WF_pendingo )
                    --nb_out_waiting;

                // if the Event can output data, it is possible that it still has to send data, meaning that we have to wait (again)
                if ( oq->has_out() and not oq->errors ) {
                    unsigned long pending = 0;
                    if ( ioctl( oq->fd, SIOCOUTQ, &pending ) )
                        perror( "ioctl SIOCOUTQ (in the main event loop)" );
                    // yes, we have pending data...
                    if ( pending ) {
                        // create a timer if not already done
                        if ( not timer )
                            *this << ( timer = new CheckSent( 0.2 ) ); // register the timer in the event loop

                        // add the event in the list of the events to be checked
                        oq->next_ev_to_del = last_ev_to_del ? last_ev_to_del : oq;
                        last_ev_to_del = oq;

                        oq->waiting |= Event::WF_pendingo;
                        ++nb_out_waiting;
                    } else if ( oq->can_be_deleted_by_ev_loop() )
                        delete oq;
                } else if ( oq->can_be_deleted_by_ev_loop() )
                    delete oq;

                if ( rq == oq )
                    break;
            }
        }
    } while ( cnt );

    return ret;
}

void EvLoop::stop( int ret_val ) {
    ret = ret_val;
    cnt = false;
}

void EvLoop::wait_for_data_to_be_sent() {
    while ( nb_out_waiting )
        run();
}

EvLoop &EvLoop::operator<<( Event *ev_obj ) {
    ev_obj->ev_loop = this;

    epoll_event ev;
    ev.events = EPOLLIN | EPOLLOUT | EPOLLRDHUP | EPOLLET; // ET -> trigger
    ev.data.u64 = 0; // for valgrind on 32 bits machines
    ev.data.ptr = ev_obj;
    if ( epoll_ctl( event_fd, EPOLL_CTL_ADD, ev_obj->fd, &ev ) == -1 )
        DISP_ERROR( "epoll_ctl add: {}", strerror( errno ) );

    ev_obj->rdy();
    return *this;
}

EvLoop &EvLoop::operator>>( Event *ev_obj ) {
    if ( epoll_ctl( event_fd, EPOLL_CTL_DEL, ev_obj->fd, 0 ) == -1 )
        DISP_ERROR( "epoll_ctl del: {}", strerror( errno ) );
    return *this;
}

EvLoop &EvLoop::operator<<( PostFunc *pf_obj ) {
    if ( not pf_obj->ev_loop ) {
        pf_obj->ev_loop = this;

        pf_obj->prev_post_func = last_post_func;
        last_post_func = pf_obj;
    }
    return *this;
}

EvLoop &EvLoop::operator>>( PostFunc *pf_obj ) {
    pf_obj->ev_loop = 0;
    if ( last_post_func == pf_obj ) {
        last_post_func = pf_obj->prev_post_func;
    } else {
        for( PostFunc *pf = last_post_func; pf->prev_post_func; pf = pf->prev_post_func ) {
            if ( pf->prev_post_func == pf_obj ) {
                pf->prev_post_func = pf_obj->prev_post_func;
                break;
            }
        }
    }
    return *this;
}

//void EvLoop::sig() {
//    watcher->sig();
//}

bool EvLoop::exec_post_funcs() {
    bool res = false;
    for( PostFunc *pf = last_post_func, *of; pf; pf = of ) {
        of = pf->prev_post_func;
        res |= pf->exec();
    }
    return res;
}

} // namespace RiotBase
