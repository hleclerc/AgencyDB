#include "../Symbol/CommonSymbolicData.h"
#include "Serializer.h"

namespace RiotBase {

namespace {

PT _write_to_rec( BBQ bq, const Rp *rp, PT &nb_items_in_data ) {
    CommonSymbolicData *csd = RO_CALL( common_symbolic_data, rp );
    if ( csd ) {
        if ( csd->op_id == CommonSymbolicData::cur_op_id )
            return reinterpret_cast<const PT &>( csd->op_mp );
        csd->op_id = CommonSymbolicData::cur_op_id;
        reinterpret_cast<PT &>( csd->op_mp ) = nb_items_in_data;
    }
    PT res = nb_items_in_data++;

    Vec<PT> ch_nums;
    for( int c = 0, n = RO_CALL( nb_symbolic_children, rp ); c < n; ++c )
        ch_nums << _write_to_rec( bq, RO_CALL( symbolic_child, rp, c ), nb_items_in_data );

    RO_CALL( graph_serialize, rp, bq, ch_nums.ptr() );

    return res;
}

}


Serializer::Serializer( bool known_nb_outputs ) : known_nb_outputs( known_nb_outputs ) {
}

void Serializer::write_to( BBQ bq ) const {
    if ( not known_nb_outputs )
        bq.write_unsigned( to_serialize.size() );

    CbQueue dat;
    PT nb_items_in_data = 0;
    ++CommonSymbolicData::cur_op_id;
    for( const Rp *rp : to_serialize )
        bq << _write_to_rec( &dat, rp, nb_items_in_data );
    bq.write_some( std::move( dat ) ); //
}


Serializer serializer( const Rp *rp ) {
    Serializer res( true );
    res.to_serialize << rp;
    return res;
}


} // namespace RiotBase
