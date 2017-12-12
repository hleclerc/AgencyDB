#pragma once

#include "../Atomic/OtWrapperAtomic.h"

namespace RiotBase {

///
class OtWrapperInode : public OtWrapperAtomic<OtWrapperInode,Inode> {
public:
    enum { term_value = true };

    OtWrapperInode( const Inode &val, UsrId usr_id = {} );

    static std::string type_name    ();
    const Category    *category     () const;
    bool               to_Inode     ( Inode *res, PT nb_items ) const;
    Rp                *copy         () const;
};

} // namespace RiotBase
