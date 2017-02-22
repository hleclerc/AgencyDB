#pragma once

namespace RiotBase {

class CheckSent;
class PostFunc;
class Watcher;
class Event;

/**
*/
class EvLoop {
public:
    EvLoop();
    ~EvLoop();

    int         run                     (); ///< Infinite loop, until stop() is called. return err code < 0 if pb. return ret_val (the value that is sent by this->stop()) if ok.
    void        stop                    ( int ret_val = 0 ); ///< Procedure to be called to gracefully exit from run(). May be called e.g. during a callback
    void        wait_for_data_to_be_sent(); ///< before destruction, the user must wait for out() until all data to be sent is gone

    EvLoop     &operator<<              ( Event *ev_obj ); ///< add a event object that has to be watched. ev_obj may be deleted inside the loop if it says that everything is done with it. thread safe.
    EvLoop     &operator>>              ( Event *ev_obj ); ///< suppression of an event object NOT thread safe

    EvLoop     &operator<<              ( PostFunc *pf_obj ); /// add a functor that will be executed after each event set
    EvLoop     &operator>>              ( PostFunc *pf_obj ); ///< suppression of a post_func object NOT thread safe

    /// to be called from another thread. Add a (void) event to wake up the loop
    // void    sig();
    
protected:
    template<bool v> struct N {};
    friend class CheckSent;
    friend class Event;

    bool       exec_post_funcs          ();

    Event     *last_ev_to_del;  ///< list of events to delete
    PostFunc  *last_post_func;  ///< functions launched after an event sequence
    int        nb_out_waiting;  ///< used in wait_for_data_to_be_sent(): increment each time we have to wait for an output
    int        event_fd;        ///< for epoll
    CheckSent *timer;           ///< it we are waiting for data to be (really) sent before closing some sockets/files/...
    int        ret;             ///< value run() will have to return
    bool       cnt;             ///< continue ?

    // Watcher   *watcher;
};

} // namespace RiotBase
