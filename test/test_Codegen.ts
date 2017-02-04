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
        const a = LvString.symbol( "A" );

        _if( LvString.symbol( "d0" ), () => {
            _if( LvString.symbol( "d1" ), () => {}, () => {
                        a.selfConcat( "c" );
                _if( LvString.symbol( "d2" ), () => {}, () => {
                        a.selfConcat( "c" );
                    _if( LvString.symbol( "d3" ), () => {}, () => {
                        a.selfConcat( "c" );
                    } )
                } )
            } )
        } )

        // _if( LvString.symbol( "c0" ), () => {

        //     _if( LvString.symbol( "d0" ), () => {
        //         a.selfConcat( "c0,d0" );
        //     }, LvString.symbol( "d1" ), () => {
        //         a.selfConcat( "c0,!d0,d1" );
        //     }, () => {
        //         a.selfConcat( "c0,!d0,!d1" );
        //     } );

        // }, LvString.symbol( "c1" ), () => {

        //     _if( LvString.symbol( "d0" ), () => {
        //         a.selfConcat( "!c0,c1,d0" );
        //     }, LvString.symbol( "d1" ), () => {
        //         a.selfConcat( "!c0,c1,!d0,d1" );
        //     }, () => {
        //         a.selfConcat( "!c0,c1,!d0,!d1" );
        //     } );

        // }, () => {

        //     _if( LvString.symbol( "d0" ), () => {
        //         a.selfConcat( "!c0,!c1,d0" );
        //     }, LvString.symbol( "d1" ), () => {
        //         a.selfConcat( "!c0,!c1,!d0,d1" );
        //     }, () => {
        //         a.selfConcat( "!c0,!c1,!d0,!d1" );
        //     } );

        // } );

        const code = Codegen.make_code( [ a ] );
        console.log( code );
        Graphviz.display( [ a.rp ] );

        // if(c0){
        //     if(d0){
        //         A+="c0,d0";
        //     }else{
        //         if(d1){
        //             A+="c0,!d0,d1";
        //         }else{
        //             A+="c0,!d0,!d1";
        //         }
        //     }
        // }else{
        //     if(c1){
        //         if(d0){
        //             A+="!c0,c1,d0";
        //         }else{
        //             if(d1){
        //                 A+="!c0,c1,!d0,d1";
        //             }else{
        //                 A+="!c0,c1,!d0,!d1";
        //             }
        //         }
        //     }else{
        //         if(d0){
        //             A+="!c0,!c1,d0";
        //         }else{
        //             if(d1){
        //                 A+="!c0,!c1,!d0,d1";
        //             }else{
        //                 A+="!c0,!c1,!d0,!d1";
        //             }
        //         }
        //     }
        // }
    } );
});
 

