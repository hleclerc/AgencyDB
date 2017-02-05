/// <reference path="../node_modules/@types/mocha/index.d.ts"/>
import LvBuffer   from "../src/LvBuffer";
import LvString   from "../src/LvString";
import { assert } from "chai";

function sequ( a, b, msg?: string ) {
    assert.equal( a.toString(), b.toString(), msg );
}

/**
 */
describe( 'String', () => {
    it( 'basic operations', () => {
        let s = new LvBuffer( "test" ), b = new LvBuffer( [ 1, 2 ] );
        sequ( s           , "test" );
        sequ( s.byteLength, 4      );
        sequ( b           , "\u0001\u0002" );
        sequ( b.byteLength, 2      );
    });

    it( 'select', () => {
        let b = new LvBuffer( [ 1, 2 ] );
        sequ( b.at( 1 ), "2" );
        
        sequ( b.select( 1 ), "2" );
        b.select( 1 ).set( 3 );
        sequ( b.select( 1 ), "3" );
        sequ( b, "\u0001\u0003" );
    });
});
 

