import CommonSymbolicData from "./CommonSymbolicData";
import Variable           from "../Core/Variable";
import Method             from "../Core/Method";
import Lazyfier           from "./Lazyfier";
import Executed           from "./Executed";
import Sym                from "./Sym";

var leaves                   = new Array<Sym>();
var timeout_reg_lazy_sources : number;

function _reg_res_exec_item( rp : Sym, res : Variable ) {
    if ( res.is_symbolic() ) {
        // console.log( 'waiting: ' + res.rp_structure() );
        // when we have the value
        res.common_symbolic_data().ww_then = ( val ) => {
            _reg_res_exec_item( rp, val );
            _exec_the_stuff(); // restart with the new leaves (if val if not symbolic)
        };
        // launch the query
        return Method.methods["ww_launch"].call_1( res );
    }
    // -> register the known value
    rp.csd.ww_val = res;

    // lazyfier_parents
    for( let lazyfier of rp.csd.lazyfier_parents ) {
        lazyfier.type_off = Executed.type_off;
        lazyfier.type_num = Executed.type_num;
        lazyfier.sym = res;
        for( let then_func of lazyfier.then_list )
            then_func( res );
    }

    // continue in the graph
    function _not_done( x : Variable ) {
        let csd = x.common_symbolic_data();
        return csd && ! csd.ww_val;
    }
    for( let p of rp.csd.parents )
        if ( ! new Variable( p ).symbolic_children().some( _not_done ) )
            leaves.push( <Sym>p );
}

function _exec_the_stuff() {
    while ( leaves.length ) {
        // select a leaf
        let rp = leaves.pop();

        // exec
        let res = rp.exec();

        // push the parents that are ready to be executed in the leaves
        if ( res )
            _reg_res_exec_item( rp, res );
    }
}

//
export default
class LazySource extends Sym {
    constructor( cl, reg_in_lazy_sources = true ) {
        super( cl );

        if ( reg_in_lazy_sources )
            this._reg_in_lazy_sources();
    }

    _reg_in_lazy_sources() {
        leaves.push( this );
        if ( ! timeout_reg_lazy_sources )
            timeout_reg_lazy_sources = setTimeout( _exec_the_stuff, 0 );
    }
}
Sym.make_templ( LazySource );

// import LazyOut from "./LazyOut";
//
//
//
// import BinaryReader       from "../../System/BinaryReader";
// import Map                from "../../System/Map";
// import Variable           from "../Core/Variable";
// import Rp                 from "../Core/Rp";
// import CommonSymbolicData from "./CommonSymbolicData";
// import Lazyfier           from "./Lazyfier";
// import Executed           from "./Executed";
// import Sym                from "./Sym";
//
// // if not in a WebWorker
// if ( typeof importScripts == 'undefined' ) {
//     var active_lazy_sources      = new Array<LazySource>();
//     var timeout_reg_lazy_sources : number;
//     var worker                   = new Worker( '../RiotBase/Variable/Symbol/LazyOpScheduler.main.js' );
//     var worker_is_ready          = false;
//     var lazyfier_map             = new Map<number,Lazyfier>();
//
//     worker.onmessage = function( ev : MessageEvent ) {
//         // we're going to modify some lazyfiers.sym, so we need first to flush the graphs
//         _send_graphs_to_worker();
//
//         //
//         if ( ev.data == "ready" ) {
//             worker_is_ready = true;
//             _send_graphs_to_worker();
//             return;
//         }
//         let lazyfier = lazyfier_map.get( ev.data.id );
//         let br       = new BinaryReader( ev.data.val );
//         let val      = br.read_Variable();
//         for( let c of lazyfier.then_list ) {
//             lazyfier.type_off = Executed.type_off;
//             lazyfier.type_num = Executed.type_num;
//             lazyfier.sym = val;
//             c( val );
//         }
//     };
//
//     function get_lazyfiers( lazyfiers : Array<Lazyfier>, obj: Variable ) {
//         let csd = obj.common_symbolic_data();
//         if ( csd ) {
//             if ( csd.op_id == CommonSymbolicData.cur_op_id )
//                 return;
//             csd.op_id = CommonSymbolicData.cur_op_id;
//
//             for( let i of csd.lazyfier_parents )
//                 lazyfiers.push( i );
//             for( let i of csd.parents )
//                 get_lazyfiers( lazyfiers, new Variable( i ) );
//         }
//     }
//
//     function get_graph_data( graph_data : Array<any>, obj: Variable ) : number {
//         let csd = obj.common_symbolic_data();
//         if ( csd ) {
//             if ( csd.sched_id )
//                 return csd.sched_id;
//             csd.op_id = CommonSymbolicData.cur_op_id;
//
//             // recursion
//             let children = [];
//             for( let i of obj.symbolic_children() )
//                 children.push( get_graph_data( graph_data, i ) );
//
//             // register
//             csd.sched_id = ++CommonSymbolicData.cur_sched_id;
//             obj.get_graph_data( graph_data, children, csd.sched_id );
//             return csd.sched_id;
//         } else {
//             let sched_id = ++CommonSymbolicData.cur_sched_id;
//             obj.get_graph_data( graph_data, [], sched_id );
//             return sched_id;
//         }
//     }
//
//     var _send_graphs_to_worker = function() {
//         //
//         let lazyfiers = new Array<Lazyfier>();
//         ++CommonSymbolicData.cur_op_id;
//         for( let i of active_lazy_sources )
//             get_lazyfiers( lazyfiers, new Variable( i ) );
//
//         let graph_data = [];
//         // let sym_to_lazy_ans = new Map<Rp,Rp>;
//         for( let i of lazyfiers ) {
//             let id_res = get_graph_data( graph_data, i.sym );
//             //
//             let id_out = ++CommonSymbolicData.cur_sched_id;
//             i.sym.rp = new LazyOut( id_out );
//             graph_data.push( {
//                 id      : id_out,
//                 name    : "LazyOut",
//                 children: [ id_res ],
//                 then    : i.get_then_list_id()
//             } );
//
//             lazyfier_map.set( id_out, i );
//         }
//
//         worker.postMessage( graph_data );
//         active_lazy_sources = [];
//     }
// }
//
// //
// export default
// class LazySource extends Sym {
//     constructor( cl, reg_in_lazy_sources = true ) {
//         super( cl );
//
//         // if not in a WebWorker
//         if ( reg_in_lazy_sources && typeof importScripts == 'undefined' )
//             this._reg_in_lazy_sources();
//     }
//
//     _reg_in_lazy_sources() {
//         active_lazy_sources.push( this );
//         if ( worker_is_ready && ! timeout_reg_lazy_sources )
//             timeout_reg_lazy_sources = setTimeout( _send_graphs_to_worker, 0 );
//     }
// }
//
// import LazyOut from "./LazyOut";
