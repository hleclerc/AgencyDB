#pragma once

namespace RiotBase {
class CheckSent;
class EvLoop;

/**
*/
class Event {
public:
    struct VtableOnly {};

    Event( int fd = -1 );
    Event( VtableOnly ); ///< a constructor that does not assign any attribute (else than the vtable). Permits to do a new( ptr ) T to change _only_ the vtable (underlying type)
    virtual ~Event(); ///< destructor of child classes may read information from the state attribute

    // public attributes
    int          fd;      ///< file descriptor
    EvLoop      *ev_loop; ///< the event_loop where $this is registered

    void         reg_for_deletion         (); ///< this will be deleted after the event round

    void         wait_for_more_out        ();
    void         wait_for_more_inp        () { waiting |= WF_more_inp; }

    void         generic_error            () { errors |= EF_generic_error; reg_for_deletion(); } ///<
    void         rd_hup_error             () { errors |= EF_rd_hup_error;  reg_for_deletion(); } ///< gracefully ended connection (but we try to play with it again)
    void         hup_error                () { errors |= EF_hup_error;     reg_for_deletion(); } ///< not so gracefully ended connection (but we try to play with it again)
    void         sys_error                () { errors |= EF_sys_error;     reg_for_deletion(); } ///< I/O error

protected:
    enum { ///< Waiting Flags
        WF_more_inp = 1,
        WF_more_out = 2,
        WF_pendingo = 4,
    };

    enum { ///< Error Flags
        EF_generic_error = 1, ///< generic error. TODO: more accurate information
        EF_rd_hup_error  = 2,  ///< gracefully ended connection
        EF_hup_error     = 4,     ///< not so gracefully ended connection
        EF_sys_error     = 8,     ///< I/O error

        _last_Event_EF   = 8
    };

    friend class EvLoop;
    friend class CheckSent;

    virtual void inp                      (); ///< if input data.
    virtual void out                      (); ///< if ready for output.
    virtual void rdy                      (); ///< called after installation of this in the event loop

    virtual bool has_out                  (); ///<

    virtual bool can_be_deleted_by_ev_loop() const; ///<

    // attributes
    Event       *next_ev_to_del; ///<
    int          waiting;        ///< combinations of WF_...
    int          errors;         ///< combinations of EF_...
};

} // namespace RiotBase
