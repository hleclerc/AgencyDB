#include "DeserializeLazyGraph.h"
#include "LazySourceExtValue.h"
#include "LazyOpScheduler.h"
#include "SendValue.h"
#include "SymOp.h"
#include "Op.h"

#include "../Core/TupleTypes.h"
#include "../Core/RpAndUse.h"

#include "../String/GenString.h"
#include "../Number/Bn.h"


namespace RiotBase {

namespace {

//void _read_lazy_graph( Vec<RpAndUse> &children, BinStream<CbStringPtr> bq, PT num, S<std::tuple<>>, const FqId &id, Db *db ) {
//    spdlog::get("console")->error( "Unknow graph type (num={})", num );
//}

//template<class Head,class... Tail>
//void _read_lazy_graph( Vec<RpAndUse> &children, BinStream<CbStringPtr> bq, PT num, S<std::tuple<Head,Tail...>>, const FqId &id, Db *db ) {
//    if ( num )
//        return _read_lazy_graph( children, bq, num - 1, S<std::tuple<Tail...>>{}, db, db );
//    if ( Rp *res = Head::graph_deserialize( bq, children ) )
//        children << res;
//    else
//        spdlog::get( "console" )->error( "bad graph data" );
//}


}

void deserialize_lazy_graph( DbSession *session, PI64 dst_id, const Vec<PT> &out_nums, CbStringPtr graph, const FqId &id ) {
    TODO;
//    Vec<RpAndUse> children;
//    BinStream<CbStringPtr> bq( &graph );
//    while( not bq.empty() ) {
//        PT num = bq.read_unsigned();
//        if ( bq.error() )
//            break;
//        _read_lazy_graph( children, bq, num, S<TupleTypes>{}, db, session->get_db() );
//    }
//    if ( bq.error() ) {
//        for( RpAndUse &c : children )
//            if ( c.use )
//                RO_CALL( del, c.rp );
//        return;
//    }
//    // add symbolic parents to transmit the values when done
//    Vec<Rp *> parents;
//    for( PT i = 0; i < out_nums.size(); ++i ) {
//        Rp *rp = children[ out_nums[ i ] ].rp;
//        if ( RO_CALL( is_symbolic, rp ) ) {
//            // add a parent such that when the value is known, send it
//            ++children[ out_nums[ i ] ].use;
//            parents << SendValue::New( session, rp );
//        } else {
//            CbQueue msg;
//            RO_CALL( get_raw_data, rp, &msg );
//            session->send_res_graph( dst_id, i, msg );
//        }
//    }
//    // -> execute the stuff
//    // dans le cas présent, on a un DbItemPtr, graph_deserialize pourrait produire directement la valeure finale...
//    // sauf si on veut pouvoir "scheduler" tout ça...
//    // Rq: scheduler pourrait se faire lors de la construction du graph:
//    for( Rp *p : parents )
//        lazy_op_scheduler << p;
}


} // namespace RiotBase
