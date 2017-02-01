/// <reference path="../node_modules/@types/mocha/index.d.ts"/>
import Codegen    from "../src/Symbol/Codegen" 
import LvString   from "../src/LvString"
import LvNumber   from "../src/LvNumber"
import LvMap      from "../src/LvMap"
import { assert } from "chai"

function sequ( a, b, msg?: string ) {
    assert.equal( a.toString(), b.toString(), msg );
}

/**
 */
describe( 'LvMap', () => {
    it( 'basic operations', () => {
        const T = LvMap( LvString, LvString );
        let s = new T;

        // set/get
        s.set( "b", "B" );
        s.get( "a" ).set( "A" );
        sequ( s, "{a:A,b:B}" );
        sequ( s.get( "a" ), "A" );
    });

    it( 'basic operations on a map of map', () => {
        const T = LvMap( LvString, LvMap( LvString, LvMap( LvString, LvString ) ) );
        let s = new T;

        // set/get
        s.get( "a" ).get( "b" ).get( "c" ).set( "ABC" );
        
        sequ( s, "{a:{b:{c:ABC}}}" );
        sequ( s.get( "a" ), "{b:{c:ABC}}" );
        sequ( s.get( "a" ).get( "b" ), "{c:ABC}" );
        sequ( s.get( "a" ).get( "b" ).get( "c" ), "ABC" );
    });

    it( 'symbolic', () => {
        const S = LvMap( LvString, LvString );
        const T = LvMap( LvString, LvMap( LvString, LvString ) );
        let s = S.symbol( "s" );
        let t = T.symbol( "t" );
        
        s.set( "b", "B" );
        sequ( Codegen.make_code( [ s ] ), 's.set("b","B");' );
        
        t.get( "k" ).get( "l" ).set( "KL" );
        sequ( Codegen.make_code( [ t ] ), 't.get("k").set("l","KL");' );

        s.set( LvString.symbol( "c" ), "C" );
        s.set( LvString.symbol( "d" ), "D" );
        sequ( Codegen.make_code( [ s ] ), 's.set("b","B");s.set(c,"C");s.set(d,"D");' );
    });

    it( 'symbolic from non symbolic', () => {
        const S = LvMap( LvString, LvString );
        let s = new S, t = new S;
        
        s.set( LvString.symbol( "b" ), "B" );
        sequ( Codegen.make_code( [ s ] ), 'var T0={};T0.set(b,"B");' );

        t.set( "b", LvString.symbol( "B" ) );
        sequ( Codegen.make_code( [ t ] ), 'var T0={};T0.set("b",B);' );
    });

    it( 'symbolic self ops', () => {
        const S = LvMap( LvString, LvString );
        // const T = LvMap( LvString, LvMap( LvString, LvString ) );
        let s = S.symbol( "s" );
        // let t = T.symbol( "t" );
        
        s.get( "b" ).append( "B" );
        sequ( Codegen.make_code( [ s ] ), 's.set("b",s.get("b")+"B");' );

        // t.get( "b" ).get( "c" ).append( "BC" );
        // console.log( t.toString() );
        // console.log( Codegen.make_code( [ t ] ) );
    });
});

