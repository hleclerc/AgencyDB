/// <reference path="../node_modules/@types/mocha/index.d.ts"/>
import DevId      from "../src/System/DevId";
import { VarAnc } from "../src/Core/Variable";
import Method     from "../src/Core/Method";
import test_ot    from "../src/Test/test_ot";
import LvString   from "../src/LvString";
import Db         from "../src/Db";
import { assert } from "chai";

function sequ( a, b, msg?: string ) {
    assert.equal( a.toString(), b.toString(), msg );
}

/**
 * OnChange; 
 *   changement permanent => remonte à chaque opération _sxx
 *   lorsque ce n'est pas symbolique => prop: on fait un proxy, mais celui-ci transfère les fonctions lorsque ça devient symbolique
 * 
 * Autre approche: 
 */
describe( 'String', () => {
    it( 'basic operations', () => {
        let s = new LvString( "test" );
        sequ( s                   , "test"        );
        sequ( s.length            , 4             );
        sequ( s.append( "ouille" ), "testouille"  );
        sequ( s.insert( 9, "s" )  , "testouillse" );
        sequ( s.remove( 2, 8 )    , "tee"         );
    });

    it( 'on change', ( done ) => {
        let s = new LvString( "test" );

        let cpt = 0;
        s.onChange( () => ++cpt );
        s.append( "_" );
        s.append( "_" );
        setTimeout( () => {
            sequ( s, "test__" );
            sequ( cpt, 1 );
            done();
        }, 20 );
    });

    it( 'select', () => {
        let a = new LvString( "test" );
        sequ( a.charAt( 2 ), "s" );

        let intercept = [];
        Method.int_call_s = ( a ) => intercept.push( a );   

        let s = a.select( 1 );
        sequ( a, "test", "basic test of RefProxy" );
        sequ( s, "e", "read sub value" );
        s.set( '3' );
        sequ( a, "t3st", "string with modified sub value" );

        assert.equal( intercept.length, 2, "nb interceptions ('s' is considered as modified even if it's not really the case)" ); 
        assert.equal( intercept[ 0 ], s, "interception of the right variable" );
        assert.equal( intercept[ 1 ], a, "interception of the right variable" );
    });

    it( 'intercept + creation date', () => {
        let a = new LvString( "a" );

        let intercept = [];
        const date = ++VarAnc.date;
        Method.int_call_s = ( a ) => { if ( a.date < date ) intercept.push( a ); };   

        let b = new LvString( "b" );
        a.append( '+' );
        b.append( '+' );
        sequ( a, "a+" );
        sequ( b, "b+" );

        assert.equal( intercept.length, 1, "modification of b should be counted: it is created after" ); 
        assert.equal( intercept[ 0 ], a, "interception of the right variable" );
    });

    it( 'operationnal tranform', () => {
        test_ot<LvString>( LvString, 2, ( vls, dbs ) => {
            vls[ 0 ].append( "a" );
        }, "a", "send from 0" );

        test_ot<LvString>( LvString, 2, ( vls, dbs ) => {
            vls[ 1 ].append( "a" );
        }, "a", "send from 1" );

        test_ot<LvString>( LvString, 2, ( vls, dbs ) => {
            vls[ 0 ].append( "a" );
            dbs[ 0 ].send_changes();
            vls[ 1 ].append( "b" );
        }, "ab", "send succession" );

        test_ot<LvString>( LvString, 2, ( vls, dbs ) => {
            console.log( "1 --------------" );
            vls[ 0 ].insert( 0, "abcd" );
            dbs.forEach( x => x.send_changes() );
            console.log( "2 --------------" );
            vls[ 0 ].insert( 1, "X" );
            vls[ 1 ].insert( 3, "Y" );
        }, "aXbcYd", "concurrent insertions" );
    });
});
 

