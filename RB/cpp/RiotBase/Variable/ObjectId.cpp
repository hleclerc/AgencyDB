#include "ObjectId/ObjectId_Inode.h"
#include "ObjectId/OtWrapperInode.h"
#include "ObjectId.h"

namespace RiotBase {

ObjectId::ObjectId( Variable::CreatorId, UsrId usr_id ) {
    if ( usr_id )
        rp = OtWrapperInode::New( Inode{}, usr_id );
    else
        rp = ObjectId_Inode::New( Inode{} );
}

ObjectId::ObjectId( const ObjectId &id ) : ObjectId( RO_CALL( copy, id.rp ) ) {
}

ObjectId::ObjectId( Inode inode ) : ObjectId( ObjectId_Inode::New( inode ) ) {
}

bool ObjectId::to_Inode( Inode &res ) const {
    return RO_CALL( to_Inode, rp, &res, 1 );
}

std::string ObjectId::category_name() {
    return "ObjectId";
}

} // namespace RiotBase
