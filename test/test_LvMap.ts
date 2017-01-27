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
        //s.set( "a", "A" );
        s.get( "a" ).set( "A" );
        sequ( s, "{a:A}" );
        console.log( s.get( "a" ).toString() );
        // sequ( s.get( "a" ), "A" );
        
        //sequ( s           , "test" );
    });
});
 

