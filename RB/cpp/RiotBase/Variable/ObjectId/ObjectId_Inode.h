#pragma once

#include "../../System/Inode.h"
#include "../Core/RpTempl.h"

namespace RiotBase {

///
class ObjectId_Inode : public RpTempl<ObjectId_Inode> {
public:
    enum { term_value = true, atomic_value = true };

    ObjectId_Inode( Inode inode ) : inode( inode ) {}

    static std::string  type_name        ();
    void                write_to_stream  ( std::ostream &os ) const;
    const Category     *category         () const;
    bool                to_Inode         ( Inode *res , PT nb_items ) const;
    Rp                 *copy             () const;
    Rp                 *make_ot_wrapper  ();

    Inode               inode;
};

} // namespace RiotBase
