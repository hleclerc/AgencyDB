#include "../Core/Deserialize.h"
#include "../../System/Ccast.h"
#include "../../System/Scast.h"
#include "../../EvLoop/Gev.h"
#include "../../Db/Db.h"

#include "LocExecGraphOutputLazyfier.h"
#include "RecvFromExtExec.h"
#include "SendToExtExec.h"
#include "Graphviz.h"
#include "Executed.h"
#include "ExecList.h"

namespace RiotBase {


namespace {

void _fill_exec_info_rec( Vec<RecvFromExtExec *> &receivers, Vec<Rp *> &leaves, Rp *rp ) {
    CommonSymbolicData *csd = RO_CALL( common_symbolic_data, rp );
    if ( not csd or csd->op_id == CommonSymbolicData::cur_op_id )
        return;
    csd->op_id = CommonSymbolicData::cur_op_id;

    //    ExecInfo *exec_info = new ExecInfo;
    reinterpret_cast<PT &>( csd->op_mp ) = ExecList::OBJ_STATE_WAITING;

    if ( int n = RO_CALL( nb_symbolic_children, rp ) ) {
        for( int i = 0; i < n; ++i )
            _fill_exec_info_rec( receivers, leaves, ccast( RO_CALL( symbolic_child, rp, i ) ) );
    } else if ( rp->type_num == StaticStore<TypeTempl<RecvFromExtExec>>::data.type_num ) {
        receivers << scast( rp );
    } else {
        leaves << rp;
    }
}

bool all_children_are_done( const Rp *rp ) {
    //        CommonSymbolicData *csd = RO_CALL( common_symbolic_data, RO_CALL( symbolic_child, rp, i ) );
    //        if ( csd and not reinterpret_cast<ExecInfo *>( csd->op_mp )->res )
    //            return false;
    for( int i = 0, n = RO_CALL( nb_symbolic_children, rp ); i < n; ++i )
        if ( RO_CALL( common_symbolic_data, RO_CALL( symbolic_child, rp, i ) ) )
            return false;
    return true;
}

}

ExecList::Result_Watcher::Result_Watcher( ExecList *exec_list ) : exec_list( exec_list ) {
}

bool ExecList::Result_Watcher::event( PI64 n ) {
    // lists of stuff to be used
    exec_list->results_mutex.lock();
    Vec<LocExecGraphOutputLazyfier *> legoll = std::move( exec_list->legoll );
    Vec<DataToSend> senders = std::move( exec_list->senders );
    exec_list->results_mutex.unlock();

    // lazyfier gateways
    for( LocExecGraphOutputLazyfier *o : legoll ) {
        Rp *val = o->children[ 0 ];
        o->children[ 0 ] = 0;

        PT cpt = 0;
        for( Lazyfier *l : o->csd.lazyfier_parents ) {
            if ( stop_gev_at.erase( l ) )
                gev->stop();
            if ( l->sym_selects.size() )
                TODO;
            Executed::assign_type_to( l )->val.rp = cpt++ ? RO_CALL( copy, val ) : val;
        }
        while( cpt-- )
            RO_CALL( del, o );
    }

    // senders
    for( const DataToSend &s : senders )
        db->send_graph_ans( s.loc, s.num, s.dat );
    return true;
}


ExecList::ExecList() : cont( true ), result_watcher( this ) {
    *gev << &result_watcher;

    for( unsigned num_cpus = 1 /*std::thread::hardware_concurrency()*/; num_cpus--; ) {
        threads << std::thread( [ & ]() {
            std::unique_lock<std::mutex> lock( mutex );
            while ( true ) {
                // get a new leaf
                while ( cont and leaves.empty() )
                    sig_ent.wait( lock );
                if ( cont == false and leaves.empty() )
                    return;
                Rp *rp = leaves.back();
                leaves.pop_back();

                // say to the rest of the world that we are going to execute
                CommonSymbolicData *csd = RO_CALL( common_symbolic_data, rp );
                reinterpret_cast<PT &>( csd->op_mp ) = ExecList::OBJ_STATE_EXECUTING;

                //
                lock.unlock();

                // execution
                Rp *res = RO_CALL( exec_mp, rp );

                // need the lock again
                lock.lock();

                // if canceled (by another thread, during execution), it means that the parents have been removed
                if ( reinterpret_cast<PT &>( csd->op_mp ) == ExecList::OBJ_STATE_CANCELED ) {
                    if ( res )
                        RO_CALL( del, res );
                    RO_CALL( del, rp );
                } else if ( res ) {
                    // we need a copy of parents because obj may be deleted by the "replace rp in parents" procedure
                    Vec<const Rp *> parents = csd->parents;

                    // replace rp in parents
                    if ( res )
                        for( const Rp *p : parents )
                            for( int i = 0, n = RO_CALL( nb_symbolic_children, p ); i < n; ++i )
                                if ( RO_CALL( symbolic_child, p, i ) == rp )
                                    RO_CALL( set_child, ccast( p ), i, res, true );

                    // add executable parents in leaves
                    for( const Rp *p : parents ) {
                        if ( all_children_are_done( p ) ) {
                            leaves << ccast( p );
                            sig_ent.notify_all();
                        }
                    }
                } else {
                    RO_CALL( del, rp ); // if object is not used anywhere, we can delete it
                }
            }
        } );
    }
}

ExecList::~ExecList() {
    mutex.lock();
    cont = false;
    sig_ent.notify_all();
    mutex.unlock();

    for( std::thread &t : threads )
        t.join();
}


void ExecList::operator<<( const Vec<Rp *> &outputs ) {
    //    Graphviz gr;
    //    for( Rp *out : outputs )
    //        gr << out;
    //    gr.display();

    // make tmp lists in the main thread
    Vec<RecvFromExtExec *> tmp_receivers;
    Vec<Rp *> tmp_leaves;
    ++CommonSymbolicData::cur_op_id;
    for( Rp *out : outputs )
        _fill_exec_info_rec( tmp_receivers, tmp_leaves, out );

    // send them in the execution one
    mutex.lock();
    receivers.append( std::move( tmp_receivers ) );
    leaves.append( std::move( tmp_leaves ) );
    sig_ent.notify_all();
    mutex.unlock();
}

void ExecList::graph_ans( DevId loc, PI64 num, CbStringPtr msg ) {
    for( PT i = 0; i < receivers.size(); ++i ) {
        RecvFromExtExec *receiver = receivers[ i ];
        if ( receiver->loc == loc and receiver->num == num ) {
            receivers.remove_unordered( i );

            Vec<RpAndUse> objects;
            receiver->rp = deserialize( &msg, objects );

            mutex.lock();
            leaves << receiver;
            sig_ent.notify_all();
            mutex.unlock();

            return;
        }
    }
}

LazyNew<ExecList> exec_list;

} // namespace RiotBase
