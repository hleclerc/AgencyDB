#include "LazySourceExtValue.h"
#include "LazySourceInfo.h"
// #include "../../Db/Db.h"

namespace RiotBase {

LazySourceExtValue::LazySourceExtValue( DbItem *item ) : LazySource<LazySourceExtValue>( nullptr/*item->category*/ ), item( item ), inode( /*item->inode*/ ) {
    TODO;
}

LazySourceExtValue::LazySourceExtValue( Inode inode ) : LazySource<LazySourceExtValue>( nullptr ), item( nullptr ), inode( inode ) {
    TODO;
}

LazySourceExtValue::~LazySourceExtValue() {
    TODO;
    //if ( item )
    //    dec_ref( item );
}

std::string LazySourceExtValue::type_name() {
    return "LazySourceExtValue";
}

void LazySourceExtValue::launch() {
    TODO;
}

void LazySourceExtValue::get_lazy_source_info( LazySourceInfo &info, const LazySourceInfo ** ) const {
    // info.prob_is_on     << std::make_pair( FqId{ item->db, item->inode.dev, item->db->get_usr_id() }, 1.0 );
    TODO;
}

void LazySourceExtValue::write_to_stream( std::ostream &os ) const {
    os << inode;
}

void LazySourceExtValue::serialize( BBQ bq, const PT *num_children ) const {
    bq << num_in_tuple_types() << inode;
}

Rp *LazySourceExtValue::deserialize( BinStream<CbStringPtr> bq, Vec<RpAndUse> &children ) {
    Inode inode = bq.read();
    return LazySourceExtValue::mkl( inode );
}

} // namespace RiotBase
