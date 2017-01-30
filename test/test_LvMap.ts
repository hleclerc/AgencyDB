/// <reference path="../node_modules/@types/mocha/index.d.ts"/>
import Codegen    from "../src/Symbol/Codegen" 
import LvString   from "../src/LvString"
import LvMap      from "../src/LvMap"
import { assert } from "chai"

function sequ( a, b, msg?: string ) {
    assert.equal( a.toString(), b.toString(), msg );
}

/**
 */
describe( 'LvMap', () => {
    // it( 'basic operations', () => {
    //     const T = LvMap( LvString, LvString );
    //     let s = new T;

    //     // set/get
    //     s.set( "b", "B" );
    //     s.get( "a" ).set( "A" );
    //     sequ( s, "{a:A,b:B}" );
    //     sequ( s.get( "a" ), "A" );
    // });

    // it( 'basic operations on a map of map', () => {
    //     const T = LvMap( LvString, LvMap( LvString, LvMap( LvString, LvString ) ) );
    //     let s = new T;

    //     // set/get
    //     s.get( "a" ).get( "b" ).get( "c" ).set( "ABC" );
        
    //     sequ( s, "{a:{b:{c:ABC}}}" );
    //     sequ( s.get( "a" ), "{b:{c:ABC}}" );
    //     sequ( s.get( "a" ).get( "b" ), "{c:ABC}" );
    //     sequ( s.get( "a" ).get( "b" ).get( "c" ), "ABC" );
    // });

    it( 'symbolic', () => {
        const S = LvMap( LvString, LvString );
        const T = LvMap( LvString, LvMap( LvString, LvString ) );
        let s = S.symbol( "s" );
        let t = T.symbol( "t" );

        // set/get
        //console.log( `${ s.get( "k" ) }` );
        
        s.set( "b", "B" );
        //sequ( s.get( "k" ), "select(s,[k])" );
        // s.set( "b", "B" );
        // s.set( "c", "C" );
        console.log( s.toString() );
        // console.log( Codegen.make_code( [ s ] ) );
        // s=[b];s=[c]; => s.set( "b", "B" ); s.set( "c", "C" );
        
        // console.log( "get:", t.get( "k" ).toString() );
        // console.log( "getget:", t.get( "k" ).get( "l" ).toString() );
        // t.get( "k" ).get( "l" ).set( "KL" );
        // console.log( t.toString() );
        // console.log( Codegen.make_code( [ t ] ) );
        // t=[k,l]; => s.get( "k" ).set( "l", "KL" );
        
        // sequ( s.get( "a" ), "A" );
    });

    // it( 'symbolic rec', () => {
    //     let s = LvMap.symbol( LvString, LvString, "m" );

    //     // set/get
    //     //console.log( `${ s.get( "k" ) }` );
        
    //     // s.set( "b", "B" );
    //     // s.get( "a" ).set( "A" );
    //     sequ( s.get( "k" ), "select(m,k)" );
    //     s.set( "b", "B" );
    //     s.set( "c", "C" );
    //     console.log( s.toString() );
    //     console.log( Codegen.make_code( [ s ] ) );
        
    //     // sequ( s.get( "a" ), "A" );
    // });
});
 

