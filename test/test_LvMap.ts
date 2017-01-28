/// <reference path="../node_modules/@types/mocha/index.d.ts"/>
import LvString             from "../src/LvString"; 
import LvMap, { new_LvMap } from "../src/LvMap";
import { assert }           from "chai";

function sequ( a, b, msg?: string ) {
    assert.equal( a.toString(), b.toString(), msg );
}

/**
 */
describe( 'LvMap', () => {
    it( 'basic operations', () => {
        let s = new_LvMap( LvString, LvString );

        // set/get
        s.set( "b", "B" );
        s.get( "a" ).set( "A" );
        sequ( s, "{a:A,b:B}" );
        sequ( s.get( "a" ), "A" );
    });

    it( 'symbolic', () => {
        let s = LvMap.symbol( LvString, LvString, "m" );

        // set/get
        //console.log( `${ s.get( "k" ) }` );
        
        // s.set( "b", "B" );
        // s.get( "a" ).set( "A" );
        sequ( s.get( "k" ), "select(m,k)" );
        // sequ( s.get( "a" ), "A" );
    });
});
 

