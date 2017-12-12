#include "OtWrapperInode.h"
#include "ObjectId_Inode.h"
#include "../ObjectId.h"

namespace RiotBase {

OtWrapperInode::OtWrapperInode( const Inode &val, UsrId usr_id ) : OtWrapperAtomic<OtWrapperInode,Inode>( val, usr_id ) {}

std::string OtWrapperInode::type_name() {
    return "OtWrapperInode";
}

const Category *OtWrapperInode::category() const {
    return ObjectId::category;
}

bool OtWrapperInode::to_Inode( Inode *res, PT nb_items ) const {
    if ( nb_items != 1 )
        return false;
    *res = val;
    return true;
}

Rp *OtWrapperInode::copy() const {
    return ObjectId_Inode::New( val );
}

namespace {

RiotBase_DEF_FUNC_2( self_set_b, 1, tv0->isa<OtWrapperInode>() and tv1->has_surdef_for( RF_to_Inode() ) ) {
    Inode res;
    if ( RO_CALL( to_Inode, v1, &res, 1 ) )
        static_cast<OtWrapperInode *>( v0 )->set_val( res );
}

}

} // namespace RiotBase
