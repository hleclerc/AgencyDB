/// <reference path="../node_modules/@types/mocha/index.d.ts"/>
import UsrId   from "../src/System/UsrId";
import DevId   from "../src/System/DevId";
import LvUsrId from "../src/LvUsrId";
import { assert } from "chai";

function sequ( a, b, msg?: string ) {
    assert.equal( a.toString(), b.toString(), msg );
}

/**
 */
describe( 'LvUsrId', () => {
    it( 'basic operations', () => {
        let s = new LvUsrId( new UsrId( new DevId( "gMWDnGjZfxZ~" ), 1 ) );
        let t = new LvUsrId( s ), u  = t.copy();
        sequ( s, "1@gMWDnGjZfxZ~" );
        sequ( t, "1@gMWDnGjZfxZ~" );
        sequ( u, "1@gMWDnGjZfxZ~" );
        s.set( new UsrId( new DevId( "O5HeHcHU1Io~" ), 2 ) );
        sequ( s, "2@O5HeHcHU1Io~" );
        sequ( t, "1@gMWDnGjZfxZ~" );
    });
});
 

