/// <reference path="../node_modules/@types/mocha/index.d.ts"/>
//// nsmake global define NO_OPERATIONAL_TRANSFORM
import LvString   from "../src/LvString"
import LvObj      from "../src/LvObj"
import { assert } from "chai"

function sequ( a, b, msg?: string ) {
    assert.equal( a.toString(), b.toString(), msg );
}

/**
 */
describe( 'LvObj', () => {
    it( 'basic operations', () => {
        let o = new LvObj();
        console.log( o.toString() );
        
    });

});
 

