/// <reference path="../node_modules/@types/mocha/index.d.ts"/>
//// nsmake global define NO_OPERATIONAL_TRANSFORM
import Codegen         from "../src/Symbol/Codegen";
import Graphviz        from "../src/Core/Graphviz";
import LvString        from "../src/LvString";
import { _if, _while } from "../src/symbol";
import { assert } from "chai";

function sequ( a, b, msg?: string ) {
    assert.equal( a.toString(), b.toString(), msg );
}

/**
 */
describe( 'Codegen', () => {
    it( 'mixed self/not self', () => {
    } );
});
 

