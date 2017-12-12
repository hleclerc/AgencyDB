#include "PatchCreatorAtomic.h"
#include "PatchMergerAtomic.h"

namespace RiotBase {

PatchCreatorAtomic::PatchCreatorAtomic( Rp *src, Inode crea_usr ) : PatchCreatorWithRightFlagsTempl( crea_usr ), changed( false ) {
    this->src.rp = src;
}

std::string PatchCreatorAtomic::type_name() {
    return "PatchCreatorAtomic";
}

void PatchCreatorAtomic::get_changes( CbQueue &cq, const FqId &id ) {
    if ( changed ) {
        if ( old.rp == nullptr or not RO_CALL( equ_immediate, src.rp, old.rp ) ) {
            RO_CALL( get_raw_data, src.rp, cq, cur_dev, false );
            if ( old.rp ) RO_CALL( del, old.rp );
            old.rp = RO_CALL( copy, src.rp );
        }
        changed = false;
    }
}

Rp **PatchCreatorAtomic::prox_ptr() {
    sig_change();
    changed = true;
    return &src.rp;
}

PatchMerger *PatchCreatorAtomic::new_patch_merger() {
    return new PatchMergerAtomic( this );
}

void PatchCreatorAtomic::self_on_change( Callback *c ) {
    on_change_list << c;
    c->reg_wait();
}

void PatchCreatorAtomic::sig_change() {
    for( RcPtr<Callback> &c : on_change_list )
        c->reg_wait();
    RO_CALL( sig_change, src.rp );
}

Vec<std::string> PatchCreatorAtomic::get_possible_rights() const {
    Vec<std::string> res;
    #define DECL_RIGHT( R ) res << #R;
    #include "rightsdecl.h"
    #undef DECL_RIGHT
    return res;
}

void PatchCreatorAtomic::_set_raw_data( const CbString &data, DevId cur_dev ) {
    if ( _right_to_write() ) {
        changed = true;
        _set_raw_data_wo_cr( data, cur_dev );
    }
}

void PatchCreatorAtomic::_set_raw_data_wo_cr( const CbString &data, DevId cur_dev ) {
    RO_CALL( set_raw_data, src.rp, data, cur_dev );
    sig_change();
}

void PatchCreatorAtomic::_get_raw_data( BinStream<CbQueue> bq ) const {
    TODO;
}

namespace {

RiotBase_DEF_FUNC_1( get_changes, 1, tv0->atomic_value(), CbQueue &cq, DevId cur_dev, UsrId usr_id ) {
    RO_CALL( get_raw_data, v0, cq, cur_dev, true );
    v0 = PatchCreatorAtomic::New( v0 );
}

RiotBase_DEF_FUNC_1( self_on_change, 1, tv0->atomic_value(), Callback *c ) {
    PatchCreatorAtomic *pc = PatchCreatorAtomic::New( v0 ); v0 = pc;
    pc->self_on_change( c );
}

RiotBase_DEF_FUNC_1( new_patch_merger, 1, tv0->atomic_value() ) {
    PatchCreatorAtomic *pc = PatchCreatorAtomic::New( v0 ); v0 = pc;
    return pc->new_patch_merger();
}

} // namespace

} // namespace RiotBase
