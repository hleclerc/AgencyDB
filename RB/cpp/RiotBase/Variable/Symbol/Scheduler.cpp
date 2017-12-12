#include "CommonSymbolicData.h"
#include "Scheduler.h"
#include "Lazyfier.h"
#include "Symbol.h"

namespace RiotBase {

namespace {

struct SchedulerData {

};

void get_lazyfiers( Vec<Lazyfier *> &lazyfiers, const Rp *rp ) {
    if ( CommonSymbolicData *csd = RO_CALL( common_symbolic_data, rp ) ) {
        if ( csd->op_id == CommonSymbolicData::cur_op_id )
            return;
        csd->op_id = CommonSymbolicData::cur_op_id;
        csd->op_mp = new SchedulerData;

        for( Lazyfier *p : csd->lazyfier_parents )
            lazyfiers << p;
        for( const Rp *p : csd->parents )
            get_lazyfiers( lazyfiers, p );
        for( PT i = 0, n = RO_CALL( nb_symbolic_children, rp ); i < n; ++i )
            get_lazyfiers( lazyfiers, RO_CALL( symbolic_child, rp, i ) );
    }
}

}

Scheduler global_scheduler;

Scheduler::Adder::Adder( Scheduler *s ) : s( s ) {
    ++CommonSymbolicData::cur_op_id;
}

Scheduler::Adder::~Adder() {
    // goal: replace lazyfiers->cur by symbol and take them for execution in parallel
    s->m.lock();
    for( Lazyfier *l : lazyfiers ) {
        // s->to_be_computed;
        l->cur = Symbol::New( "tmp", RO_CALL( category, l->cur ) );
    }
    s->m.unlock();
}

Scheduler::Adder &Scheduler::Adder::operator<<( Rp *rp ) {
    get_lazyfiers( lazyfiers, rp->type_num == StaticStore<TypeTempl<Lazyfier>>::data.type_num ? static_cast<Lazyfier *>( rp )->cur : rp );
    return *this;
}

Scheduler::Adder Scheduler::raii_adder() {
    return this;
}

} // namespace RiotBase
