import BinaryReader from "../RiotBase/System/BinaryReader";
import BinaryWriter from "../RiotBase/System/BinaryWriter";
import Variable     from "../RiotBase/Variable/Core/Variable";
import String       from "../RiotBase/Variable/String";
import Number       from "../RiotBase/Variable/Number";
import Array        from "../RiotBase/Variable/Array";
import check        from "../RiotBase/System/Check";
import tpc          from "./tpc";
declare var Proxy : any;

function test_base_op() {
    // insert, remove, display
    let a = new Array<Number>( [ 1, 2 ] );
    check( a.length, 2 );
    check( a, "[1,2]" );

    a.insert( 2, new Number( 3 ) );
    check( a.length, 3 );
    check( a, "[1,2,3]" );
    a.splice( 1, 1 );
    check( a, "[1,3]" );

    // array of array
    a.insert( 1, [ 4, 5 ] );
    check( a, "[1,[4,5],3]" );

    // selection
    a.at( 0 ).val = 10;
    a.at( 2 ).val = 30;
    check( a, "[10,[4,5],30]" );

    // proxy
    if ( Proxy != null )
        check( a[ 0 ], 10 );

    a.val = [ 5, 6 ];
    check( a, "[5,6]" );
}

function test_rw() {
    let a = new Array<Variable>( [ 1, "A" ] );

    let bw = new BinaryWriter;
    bw.write_Variable( a );
    bw.write_Variable( a );
    bw.write_Variable( a );

    let br = new BinaryReader( bw.to_Uint8Array() );
    check( br.read_Variable(), a );
    br.skip_Variable();
    check( br.read_Variable(), a );
}

function test_parallel_changes() {
    // init
    tpc( Array, function( s ) {
    }, [ "a" ], '[a]' );

    // simple inserts and removals
    tpc( Array, function( s ) {
        s.m[ 0 ].d.push( "b" );
    }, [ "a" ], "[a,b]" );

    tpc( Array, function( s ) {
        s.m[ 0 ].d.splice( 0, 1 );
    }, [ "a", "b" ], "[b]" );

    // change of items
    tpc( Array, function( s ) {
        s.m[ 0 ].d.at( 0 ).append( "A" );
    }, [ "a", "b" ], "[aA,b]" );

    tpc( Array, function( s ) {
        s.m[ 0 ].d.at( 0 ).insert( 0, "A" );
        s.m[ 1 ].d.at( 0 ).insert( 1, "Z" );
    }, [ "a", "b" ], "[AaZ,b]" );
}

function test_change_num() {
    let cpt = 0;
    let a = new Array<Number>( [ 1, 2 ] );
    a.onchange( function() { cpt += a.at( 0 ).toNumber(); } );

    a.val = [ 1, 2 ];

    setTimeout( () => {
        a.val = [ 10, 2 ];
        setTimeout( () => {
            check( cpt, 10 );
        }, 1 )
    }, 1 );
}

function test_change_arr() {
    let cpt = "";
    let a = new Array<any>( [ [ 1, "a" ], 2 ] );
    a.onchange( () => cpt += a.at( 0 ).at( 1 ).toString() );

    a.val = [ [ 1, "a" ], 2 ];

    setTimeout( () => {
        a.val = [ [ 1, "b" ], 2 ];
        setTimeout( () => {
            check( a, "[[1,b],2]" );
            check( cpt, "b" );
        }, 1 );
    }, 1 );
}

// test_base_op();
test_rw();
// test_parallel_changes();
// test_change_num()
// test_change_arr()
