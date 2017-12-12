#include "../../System/CbStringPtr.h"
#include "../../System/Scast.h"

#include "../Core/AutoRpDel.h"
#include "../ObjectId.h"

#include "OtWrapperInode.h"
#include "ObjectId_Inode.h"

namespace RiotBase {

std::string ObjectId_Inode::type_name() {
    return "ObjectId_Inode";
}

void ObjectId_Inode::write_to_stream( std::ostream &os ) const {
    os << inode;
}

const Category *ObjectId_Inode::category() const {
    return ObjectId::category;
}

bool ObjectId_Inode::to_Inode( Inode *res, PT nb_items ) const {
    if ( nb_items != 1 )
        return false;
    *res = inode;
    return true;
}

Rp *ObjectId_Inode::copy() const {
    return ObjectId_Inode::New( inode );
}

Rp *ObjectId_Inode::make_ot_wrapper() {
    Inode i = inode;
    RO_CALL( del, this );
    return OtWrapperInode::New( i );
}

namespace {

RiotBase_DEF_FUNC_2( self_set_b, 1, tv0->isa<ObjectId_Inode>() and tv1->has_surdef_for( RF_to_Inode() ) ) {
    Inode res;
    RO_CALL( to_Inode, v1, &res, 1 );
    static_cast<ObjectId_Inode *>( v0 )->inode = res;
}

}

} // namespace RiotBase
