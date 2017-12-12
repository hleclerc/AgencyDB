#pragma once

#include "../../Variable/Ot/PatchManager.h"
// #include "DbSession.h"

#include <boost/intrusive/avl_set.hpp>
#include <mutex>

namespace RiotBase {
class Category;
class DataFile;
class Rp;

/**

*/
class DbItem {
public:
    //    struct Pwdb { template<class T> T *&operator()( T *obj ) const { return obj->prev_with_dbuff ; } };
    struct Ptbs { template<class T> T *&operator()( T *obj ) const { return obj->prev_in_db_cycled_list; } };
    using AvlMemberHook = boost::intrusive::avl_set_member_hook<>;

    DbItem( Db *db, Inode inode, Rp *rp );
    ~DbItem();

    DbItem                            *inc_ref () { ++cpt_use; return this; }

    // generic information
    Inode                              inode;                  ///<
    PatchManager                       patch_manager;          ///< may be the null (e.g. if not used), or the value, or a lazyfier (that will be replaced after an associated event)

    //
    DbItem                            *prev_in_db_cycled_list; ///< != 0 means that it can be in the Db::items_to_be_signaled_to_tracker list, or in the Db::items_with_updates_to_be_sent list

    //    // permanent storage
    //    DataFile                          *datafile;
    //    PI64                               datafile_len;
    //    PI64                               datafile_offset;
    //    Buffer                            *datafile_buffer;  ///< filled by the DataFile thread

    //    DbItem                            *prev_with_dbuff;  ///< prev with datafile_buffer
    //    static CycleEndedList<DbItem,Pwdb> list_with_dbuff;  ///< list of DbItem to be save in the permanent storage
    //    static std::mutex                  mutex_lwdb_list;  ///< should be enough (to have a global mutex)

    // sessions
    // Vec<RcPtr<DbSession>>              sessions;         ///< connected sessions (which are interested by the changes on this)

    // table data
    AvlMemberHook                      table_hook;       ///<
    mutable int                        cpt_use;
};

} // namespace RiotBase
