/// <reference path="../node_modules/@types/mocha/index.d.ts"/>
import Codegen    from "../src/Symbol/Codegen" 
import LvString   from "../src/LvString"
import LvNumber   from "../src/LvNumber"
import LvArray    from "../src/LvArray"
import { assert } from "chai"

function sequ( a, b, msg?: string ) {
    assert.equal( a.toString(), b.toString(), msg );
}

/**
 */
describe( 'LvArray', () => {
    it( 'basic operations', () => {
        const T = LvArray( LvString );
        let l = new T;

        // set/get
        l.push( "a" );
        sequ( l, "[a]" );
    });

    it( 'symbolic', () => {
        const T = LvArray( LvString );
        let l = T.symbol( "l" ), m = new T;

        l.push( "a" );
        sequ( l, "push_s(l,a)" );
        console.log( Codegen.make_code( [ l ] ) );
        

        m.push( LvString.symbol( "a" ) );
        sequ( m, "push_s([],a)"  ); 
        console.log( Codegen.make_code( [ m ] ) ); 
    });
});

