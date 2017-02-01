/// <reference path="../node_modules/@types/mocha/index.d.ts"/>
import LvString   from "../src/LvString"
import LvAny      from "../src/LvAny"
import { assert } from "chai"

function sequ( a, b, msg?: string ) {
    assert.equal( a.toString(), b.toString(), msg );
}

/**
 */
describe( 'LvAny', () => {
    it( 'basic operations', () => {
        let t = new LvAny( new LvString( "a" ) );
        console.log( t.toString() );
        
    });
});

