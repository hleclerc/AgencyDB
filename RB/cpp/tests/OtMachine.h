#pragma once

#include <RiotBase/Variable/Ot/PatchManager.h>
#include <RiotBase/System/Log.h>

namespace RiotBase {

/// for test purpose
template<class T>
struct OtMachine {
    struct Link {
        Link( OtMachine *m ) : wrk( true ), m( m ) {}

        bool       wrk; ///< true if link is working
        OtMachine *m;
    };

    OtMachine( PI64 dev = 0 ) : wrk( true ), dev( dev ), usr( KnownUsrId::root_usr ) {
        new ( &pm.rp ) T;
    }

    void prep_msg() {
        if ( not wrk )
            return;
        messages = pm.get_changes( connected_devices(), dev, usr );
    }

    void send_msg() {
        if ( not wrk )
            return;
        for( OtMessage *m = messages.ptr(); m; m = m->prev.ptr() ) {
            for( DevId dst : m->dsts ) {
                CbQueue ans;
                OtMachine *dm = get_machine( dst );
                // DISP_INFO( "App change from {} to {} -----------------", dev, dm->dev );
                dm->pm.app_changes( &ans, m->msg, dev, m->dsts, dm->dev, dm->usr );
            }
        }
    }

    void ackn_msg() {
    }

    void disp_msg() {
    }

    OtMachine *get_machine( DevId dst ) {
        for( Link &l : links )
            if ( l.m->dev == dst )
                return l.m;
        return 0;
    }

    Vec<DevId> connected_devices() const {
        Vec<DevId> res;
        for( const Link &l : links )
            if ( l.m->wrk )
                res << l.m->dev;
        return res;
    }

    T &data() { ///< data (String, ...) we're working on
        return reinterpret_cast<T &>( pm.rp );
    }

    AutoPtr<OtMessage> messages;
    Vec<Link>          links;
    bool               wrk;        ///< working connection
    DevId              dev;
    UsrId              usr;
    PatchManager       pm;
};

template<class T>
void sync( Vec<OtMachine<T>> &machines, Vec<OtMachine<T> *> excluded = {} ) {
    for( OtMachine<T> *m : excluded )
        m->wrk = false;

    // two step procedure (because we want asynchronous tests)
    for( OtMachine<T> &m : machines ) m.prep_msg();
    for( OtMachine<T> &m : machines ) m.send_msg();
    for( OtMachine<T> &m : machines ) m.ackn_msg();
    for( OtMachine<T> &m : machines ) m.disp_msg();

    for( OtMachine<T> *m : excluded )
        m->wrk = true;
}

} // namespace RiotBase
