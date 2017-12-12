#pragma once

#include "CallbackDb.h"
#include "DbSession.h"

#include <boost/intrusive/avl_set.hpp>
#include <mutex>

namespace RiotBase {
class DataFile;
class Rp;

///
class DbItem {
public:
    struct Pwdb { template<class T> T *&operator()( T *obj ) const { return obj->prev_with_dbuff ; } };
    struct Ptbs { template<class T> T *&operator()( T *obj ) const { return obj->prev_to_be_saved; } };
    using AvlMemberHook = boost::intrusive::avl_set_member_hook<>;

    DbItem( const Category *category, Inode inode, Db *db, Rp *rp = 0 );
    ~DbItem();

    Rp                               *&check_rp();
    DbItem                            *inc_ref () { ++cpt_use; return this; }

    static bool                        exec_all();

    // memory
    Rp                                *rp;               ///< may be the null (e.g. if not used), or the value, or a lazyfier (that will be replaced after an associated event)
    DbCallback                         callback;         ///<

    // permanent storage
    DataFile                          *datafile;
    PI64                               datafile_len;
    PI64                               datafile_offset;
    Buffer                            *datafile_buffer;  ///< filled by the DataFile thread

    DbItem                            *prev_with_dbuff;  ///< prev with datafile_buffer
    static CycleEndedList<DbItem,Pwdb> list_with_dbuff;  ///< list of DbItem to be save in the permanent storage
    static std::mutex                  mutex_lwdb_list;  ///< should be enough (to have a global mutex)

    // generic information
    const Category                    *category;         ///<
    Inode                              inode;            ///<
    Db                                *db;               ///<

    // sessions
    Vec<RcPtr<DbSession>>              sessions;         ///< connected sessions (which are interested by the changes on this)

    AvlMemberHook                      table_hook;       ///<

    mutable int                        cpt_use;
};

} // namespace RiotBase
