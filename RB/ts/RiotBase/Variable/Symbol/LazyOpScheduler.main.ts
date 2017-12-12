import BinaryReader from '../../System/BinaryReader';
import BinaryWriter from '../../System/BinaryWriter';
import Map          from '../../System/Map';
import Variable     from '../Core/Variable';
import Method       from '../Core/Method';

// declare function require( s: string );

interface Item {
    name    : string;
    data    : any;
    id      : number;
    parents : Array<Item>;
    children: Array<Item>;
    then    : number;
    val     : Variable;
}

declare function postMessage( obj: any );

//
var graph_data = new Map<Number,Item>();
var leaves = [];

function exec_item( item : Item ) {
    switch ( item.name ) {
        case 'variable': {
            let br = new BinaryReader( item.data );
            return br.read_Variable();
        }
        case 'add': {
            return Method.methods["add_bb"].call_2( item.children[ 0 ].val, item.children[ 1 ].val );
        }
        case 'LazyOut': {
            let bw = new BinaryWriter();
            bw.write_Variable( item.children[ 0 ].val );
            postMessage( {
                id  : item.id,
                val : bw.to_Uint8Array(),
                then: item.then
            } );
            return;
        }
        default:
            console.error( "TODO: item name " + item.name );
    }
    return 10;
}

function reg_res_exec_item( item, res ) {
    if ( res.is_symbolic() ) {
        // when we have the value
        res.common_symbolic_data().ww_then = ( val ) => {
            reg_res_exec_item( item, val );
            exec(); // restart with the new leaves (if val if not symbolic)
        };
        // launch the query
        return Method.methods["ww_launch"].call_1( res );
    }
    // -> register the know value
    item.val = res;

    // continue in the graph
    for( let p of item.parents )
        if ( ! p.children.some( ( x ) => ! x.val || x.val.is_symbolic() ) )
            leaves.push( p );
}

function exec() {
    while ( leaves.length ) {
        // select a leaf
        let item = leaves.pop();

        // exec and go to the parents if possible
        let res = exec_item( item );
        if ( res )
            reg_res_exec_item( item, res );
    }
}

postMessage( "ready" );

self.addEventListener( 'message', function( e ) {
    // add items in the map
    for( let item of e.data ) {
        graph_data.set( item.id, item );
        item.parents = [];
    }
    // add parents and transform children as a list of items
    for( let item of e.data ) {
        if ( item.children && item.children.length ) {
            let nch = [];
            for( let ch of item.children ) {
                let c = graph_data.get( ch )
                c.parents.push( item );
                nch.push( c );
            }
            item.children = nch;
        } else {
            leaves.push( item );
        }
    }
    // move .then in LazyOut parents if possible
    for( let item of e.data ) {
        if ( item.then ) {
            for( let p of item.parents ) {
                if ( p.name == "LazyOut" ) {
                    p.then = item.then;
                    delete item.then;
                    break;
                }
            }
        }
    }
    //
    exec();
}, false );
