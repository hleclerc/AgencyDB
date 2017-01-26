/// <reference path="../node_modules/@types/mocha/index.d.ts"/>
//// nsmake global define NO_OPERATIONAL_TRANSFORM
import Codegen    from "../src/Symbol/Codegen";
import LvString   from "../src/LvString";
import { assert } from "chai";

function sequ( a, b, msg?: string ) {
    assert.equal( a.toString(), b.toString(), msg );
}

/**
 */
describe( 'Codegen', () => {
    it( 'basic operations', () => {
        //let s = new LvString( "test" );
        let s = LvString.symbol( "s" );
        let t = LvString.symbol( "t" );
        s.remove( 1, 2 );
        t.insert( 1, "u" );
        
        sequ( Codegen.make_code( [ s ] ), "s=s.substr(0,1)+s.substr(1+2);" );
        sequ( Codegen.make_code( [ t ] ), 't=t.substr(0,1)+"u"+t.substr(1);' );
    });

});
 

