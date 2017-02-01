/// <reference path="../node_modules/@types/mocha/index.d.ts"/>
//// nsmake global define NO_OPERATIONAL_TRANSFORM
import Codegen    from "../src/Symbol/Codegen";
import Graphviz   from "../src/Core/Graphviz";
import LvString   from "../src/LvString";
import { assert } from "chai";

function sequ( a, b, msg?: string ) {
    assert.equal( a.toString(), b.toString(), msg );
}

/**
 */
describe( 'Codegen', () => {
    it( 'basic operations', () => {
        let s = LvString.symbol( "s" );
        s.applyMethod( "pouet", new LvString( "17" ) );

        console.log( s.toString() );
        Graphviz.display( [ s.rp ] );
        
        // sequ( Codegen.make_code( [ s ] ), 't=t.substr(0,1)+"u"+t.substr(1);' );
    });

});
 

