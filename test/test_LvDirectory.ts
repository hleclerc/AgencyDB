/// <reference path="../node_modules/@types/mocha/index.d.ts"/>
import test_ot     from "../src/Test/test_ot"
import Inode       from "../src/System/Inode"
import DevId       from "../src/System/DevId"
import LvDirectory from "../src/LvDirectory"
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
        sequ( s.toString(), "{home:0@-----------~}" );
        s.delete( "home" );
        sequ( s.toString(), "{}" );
    });

    it( 'ot', () => {
        test_ot<LvDirectory>( LvDirectory, 2, ( vls, dbs ) => {
            vls[ 0 ].set( "home", new Inode( new DevId( "ujtameVI3fN~" ), 17 ) );
        }, "{home:17@ujtameVI3fN~}" );
    });
});
 

