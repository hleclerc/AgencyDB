#pragma once

#include "../../System/Print.h"
#include "../../System/Vec.h"
#include <condition_variable>
#include <thread>
#include <string>
#include <mutex>

namespace RiotBase {
class CbQueue;
class DbItem;

///
class DataFile {
public:
    enum { use_dio = false };

    DataFile( std::string path, int fd, int id );
    ~DataFile();

    void        write_to_stream( std::ostream &os ) const;
    bool        has_work_to_do () const;
    void        sched_write    ( CbQueue &&cq );
    void        sched_read     ( DbItem *item );

    // carac
    std::string             path;
    int                     fd;
    int                     id;

    // scheduling
    std::thread             sched_thread;
    std::mutex              mutex;
    std::condition_variable waitc;
    Vec<DbItem *>           to_read;
    Vec<CbQueue>            to_write;
    bool                    stop_th;
};

} // namespace RiotBase
