import DelayedSource  from "../RiotBase/Variable/Symbol/DelayedSource";
import DeclTypesForRW from "../RiotBase/Variable/Core/DeclTypesForRW";
import BinaryReader   from "../RiotBase/System/BinaryReader";
import Number         from "../RiotBase/Variable/Number";
import check          from "../RiotBase/System/Check";


function test_WebWorker() {
    let a = new Number( new DelayedSource( new Number( 1 ) ) );
    let b = a.add( 10 );
    check( b.is_symbolic(), true );

    b.then( ( val ) => {
        check( val, 11 );
        check( b.is_symbolic(), false );
    } );

    setTimeout( () => {
        check( b, 11 );
    }, 500 );
}

function test_WebWorker_self_op() {
    let a = new Number( new DelayedSource( new Number( 1 ) ) );
    a.self_add( 10 );
    check( a.is_symbolic(), true );

    a.then( ( val ) => {
        check( val, 11 );
        check( a.is_symbolic(), false );
    } );

    setTimeout( () => {
        check( a, 11 );
    }, 500 );
}

function test_load_variable_unknown_type() {
    let d = new Uint8Array( [ DeclTypesForRW.num.GenString, 1, 69 ] );
    let v = new BinaryReader( d ).read_Variable();
    let c = 0;
    v.then( function() { c = 1; } );

    setTimeout( () => {
        check( v, "E" );
        check( c, 1 );
    }, 500 );
}

// test_WebWorker();
// test_WebWorker_self_op();
test_load_variable_unknown_type();
