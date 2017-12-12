#include "../../System/BinStream.h"
#include "../../System/AutoPtr.h"
#include "../Ot/OtMerger.h"
#include "Category.h"
#include "Methods.h"

namespace RiotBase {

Category *Category::last = 0;

Category::Category() {
    prev = last;
    last = this;
}

Rp *Category::create( UsrId as_usr, CbString &patch_data ) const {
    Rp *res = create( as_usr );

    CbQueue new_patch_data;
    AutoPtr<OtMerger> p = RO_CALL( new_ot_merger, res );
    p->new_patch( &new_patch_data, patch_data, as_usr, 0, 0 );
    p->finalize();

    patch_data = std::move( new_patch_data );
    return res;
}

Category *Category::find( std::string n ) {
    for( Category *c = last; c; c = c->prev )
        if ( c->name() == n )
            return c;
    return 0;
}


} // namespace RiotBase
