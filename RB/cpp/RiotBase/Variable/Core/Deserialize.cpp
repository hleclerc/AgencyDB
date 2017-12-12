#include "../../System/TypeList.h"
#include "Deserialize.h"
#include "TupleTypes.h"
#include "RpAndUse.h"

// type to be read
#include "../Symbol/LazySourceKnownValueExt.h"
#include "../Symbol/LazySourceExtValue.h"
#include "../Symbol/RecvFromExtExec.h"
#include "../Symbol/SendToExtExec.h"
#include "../String/GenString.h"
#include "../Symbol/SymOp.h"
#include "../Symbol/Op.h"
#include "../Number/Bn.h"

namespace RiotBase {

namespace {

template<class Cq>
Rp *_read( BinStream<Cq> bq, Vec<RpAndUse> &objects, PT num, TypeList<> ) {
    spdlog::get("console")->error( "Unknow graph type (num={})", num );
    return 0;
}

template<class Cq,class Head,class... Tail>
Rp *_read( BinStream<Cq> bq, Vec<RpAndUse> &objects, PT num, TypeList<Head,Tail...> ) {
    return num ? _read( bq, objects, num - 1, TypeList<Tail...>{} ) : Head::deserialize( bq, objects );
}

}

Rp *deserialize( BinStream<CbStringPtr> bq, Vec<RpAndUse> &objects ) {
    return _read( bq, objects, bq.read(), TupleTypes{} );
}


} // namespace RiotBase
