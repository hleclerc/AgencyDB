import BinaryWriter from "../RiotBase/System/BinaryWriter";
import String       from "../RiotBase/Variable/String";
import Map          from "../RiotBase/Variable/Map";
import check        from "../RiotBase/System/Check";
import tpc          from "./tpc";

function test_base_op() {
    let obj = new Map<String,String>( { a : "A" } );
    check( obj, "{a:A}" );
    obj.set( "b", "B" );
    check( obj, "{a:A,b:B}" );
    obj.get( "a" ).set( "Z" );
    check( obj.get( "a" ), "Z" );
}

function test_parallel_changes() {
    // init
    tpc( Map, function( s ) {
    }, {a:'A'}, '{a:A}' );

    // simple inserts and removals
    tpc( Map, function( s ) {
        s.m[ 0 ].d.set( "b", "B" );
    }, {}, "{b:B}" );

    tpc( Map, function( s ) {
        s.m[ 0 ].d.delete( "a" );
        s.m[ 0 ].d.delete( "b" );
    }, {a:'A'}, '{}' );

    tpc( Map, function( s ) {
        s.m[ 0 ].d.set( "a", "A" );
        s.m[ 0 ].d.delete( "a" );
    }, {}, '{}' );

    // change of items
    tpc( Map, function( s ) {
        s.m[ 0 ].d.get( "a" ).append( "Z" );
    }, {a:'A'}, '{a:AZ}' );

    tpc( Map, function( s ) {
        s.m[ 0 ].d.get( "a" ).insert( 0, "0" );
        s.m[ 1 ].d.get( "a" ).insert( 1, "1" );
    }, {a:'A'}, '{a:0A1}' );
}

function test_onchange() {
    let cpt = "";
    let obj = new Map<String,String>();
    obj.set( "a", "A" );
    obj.onchange( function() { cpt += obj.get( "a" ).toString(); } );
    obj.set( "a", "A" );
    setTimeout( function() {
        obj.set( "a", "Z" );
        setTimeout( function() {
            check( cpt, "Z" );
        }, 1 );
    }, 1 );
}


test_base_op()
test_parallel_changes();
// test_onchange()
