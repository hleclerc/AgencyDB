#pragma once

#include "Core/VariableTempl.h"

namespace RiotBase {

///
class ObjectId : public VariableTempl<ObjectId> {
public:
    ObjectId( Variable::CreatorId, UsrId usr_id );
    ObjectId( const ObjectId &id );
    ObjectId( Inode inode = Inode() );
    ObjectId( Rp *rp ) : VariableTempl<ObjectId>( rp ) {}

    ObjectId          &operator=  ( const ObjectId &o ) { RO_CALL( self_set_b, rp, o.rp ); return *this; }
    bool               to_Inode   ( Inode &res ) const;

    static std::string category_name();
};

} // namespace RiotBase
