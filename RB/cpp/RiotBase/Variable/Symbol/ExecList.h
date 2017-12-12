#pragma once

#include "../../EvLoop/Watcher.h"
#include "../../System/LazyNew.h"
#include "../../System/AutoPtr.h"
#include "../Core/AutoRpDel.h"
#include "ExecInfo.h"

#include <condition_variable>
#include <thread>
#include <mutex>
#include <set>

namespace RiotBase {
class LocExecGraphOutputLazyfier;
class RecvFromExtExec;
class SendToExtExec;
class Lazyfier;

/**
  List of things to be executed.

  TODO: optimize (lockless stuff, ...)
*/
class ExecList {
public:
    struct Result_Watcher : Watcher {
        Result_Watcher( ExecList *exec_list );

        virtual bool               event( PI64 n );

        ExecList                  *exec_list;
        std::set<const Lazyfier *> stop_gev_at;
    };
    struct DataToSend {
        DevId   loc;
        PI64    num;
        CbQueue dat;
    };
    enum { // reinterpret_cast<PT &>( op_mp )
        OBJ_STATE_WAITING   = 0,
        OBJ_STATE_EXECUTING = 1,
        OBJ_STATE_CANCELED  = 2, // if a parent is deleted from the main thread, the main thread can cancel execution
    };

    ExecList();
    ~ExecList();

    /// add corresponding leaves to this->leaves. Each Rp with common_symbolic_data also have op_mp = new ExecInfo
    void                              operator<<( const Vec<Rp *> &outputs );
    void                              graph_ans ( DevId loc, PI64 num, CbStringPtr msg );
    void                              join      ();

    // leaves                         
    Vec<RecvFromExtExec *>            receivers;
    Vec<Rp *>                         leaves;  ///< instruction to be executed when available threads
    Vec<std::thread>                  threads;
    std::condition_variable           sig_ent; ///< signal for new entries
    std::mutex                        mutex;
    bool                              cont;

    // results
    Result_Watcher                    result_watcher;
    std::mutex                        results_mutex;
    Vec<DataToSend>                   senders;
    Vec<LocExecGraphOutputLazyfier *> legoll;
};

extern LazyNew<ExecList> exec_list;

} // namespace RiotBase
