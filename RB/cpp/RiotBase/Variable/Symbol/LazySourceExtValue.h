#pragma once

// #include "../../Db/Core/DbItem.h"
#include "../Core/TupleTypes.h"
#include "../Core/RpAndUse.h"
#include "LazySource.h"

namespace RiotBase {
class DbItem;

///
class LazySourceExtValue : public LazySource<LazySourceExtValue> {
public:
    LazySourceExtValue( DbItem *item );
    LazySourceExtValue( Inode inode );
    ~LazySourceExtValue();

    static std::string type_name           ();
    void               launch              ();
    void               get_lazy_source_info( LazySourceInfo &info, const LazySourceInfo ** ) const;
    void               write_to_stream     ( std::ostream & ) const;
    void               serialize           ( BBQ bq, const PT *num_children ) const;
    static Rp *        deserialize         ( BinStream<CbStringPtr> bq, Vec<RpAndUse> &children );

    DbItem            *item;
    Inode              inode; ///< should be the same in item, expected if loc_to_glo of glo_to_loc procedure
};

} // namespace RiotBase
