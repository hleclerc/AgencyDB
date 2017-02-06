/// <reference path="../node_modules/@types/mocha/index.d.ts"/>
import Inode       from "../src/System/Inode";
import LvDirectory from "../src/LvDirectory";
import { assert } from "chai";

function sequ( a, b, msg?: string ) {
    assert.equal( a.toString(), b.toString(), msg );
}

/**
 */
describe( 'LvDirectory', () => {
    it( 'basic operations', () => {
        let s = new LvDirectory();
        s.set( "home", new Inode() );
        console.log( s.toString() );
    });
});
 

