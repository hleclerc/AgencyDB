/// <reference path="../node_modules/@types/mocha/index.d.ts"/>
import Codegen         from "../src/Symbol/Codegen";
import Graphviz        from "../src/Core/Graphviz";
import Method          from "../src/Core/Method";
import LvBuffer        from "../src/LvBuffer";
import LvString        from "../src/LvString";
import LvNumber        from "../src/LvNumber";
import { _if, _while } from "../src/symbol";
import { assert }      from "chai";

function sequ( a, b, msg?: string ) {
    assert.equal( a.toString(), b.toString(), msg );
}

/**
 * 
 * 
 */
describe( 'Generation', () => {
    it( 'mixed self/not self', () => {
        const a = LvString.symbol( "A" );
        const b = LvString.symbol( "B" );
        const t = b.concat( a ); // ba
        const o = b.copy(); // b
        b.selfConcat( "c" ); // => bc
        b.selfConcat( t ); // bcba
        b.selfConcat( o ); // bcbab

        // Graphviz.display( [ b.rp ] );
        const code = Codegen.make_code( [ b ] );
        sequ( code, 'let T0=B+A,T1=B;B+="c";B+=T0;B+=T1;' );
        {
            let B = "b", A = "a";
            eval( code );
            sequ( B, "bcbab" );
        }
    } );

    it( 'basic string operations', () => {
        let s = LvString.symbol( "s" );
        let t = LvString.symbol( "t" );
        s.remove( 1, 2 );
        t.insert( 1, "u" );
        
        sequ( Codegen.make_code( [ s ] ), "s=s.substr(0,1)+s.substr(3);" );
        sequ( Codegen.make_code( [ t ] ), 't=t.substr(0,1)+"u"+t.substr(1);' );
    });

    it( 'modify a new value', () => {
        const a = LvString.symbol( "a" );
        const b = LvString.symbol( "b" );
        let c = a.concat( b );
        c.selfConcat( "c" );

        const code = Codegen.make_code( [ c ] );
        //Graphviz.display( [ c.rp ] );
        sequ( code, 'let T0=a+b;T0+="c";' );
    } );

    it( 'if basic', () => {
        const str = LvString.symbol( "str" ), cond = LvString.symbol( "cond" );
        _if( cond, () => { str.selfConcat( 'c' ); } );
        str.selfConcat( 'd' );

        // Graphviz.display( [ str.rp ] );
        sequ( Codegen.make_code( [ str ] ), 'if(cond){str+="c";}str+="d";' );
    } );

    it( 'if else', () => {
        const str = LvString.symbol( "str" ), c0 = LvString.symbol( "c0" ), c1 = LvString.symbol( "c1" );
        _if( c0, () => str.selfConcat( 's0' ), c1, () => str.selfConcat( 's1' ), () => str.selfConcat( 's2' ) );

        // Graphviz.display( [ str.rp ] );
        sequ( Codegen.make_code( [ str ] ), 'if(c0){str+="s0";}else{if(c1){str+="s1";}else{str+="s2";}}' );
    } );

    it( 'if with interleaved outputs', () => {
        const a = LvString.symbol( "A" );
        const b = LvString.symbol( "B" );
        const c = LvString.symbol( "C" );
        _if( c, () => {
            const t = a.copy();
            a.set( b );
            b.set( t );
        } );

        const code = Codegen.make_code( [ a, b ] )
        // Graphviz.display( [ a.rp, b.rp ] );
        sequ( code, 'if(C){let T0=A;A=B;B=T0;}' );
        {
            let A = "a", B = "b", C = null;
            eval( code );
            sequ( A, "a" );
            sequ( B, "b" );
            C = '.';
            eval( code );
            sequ( B, "a" );
            sequ( A, "b" );
        }
    } );

    it( 'variable selfed twice, via a copy', () => {
        const a = LvString.symbol( "a" );
        const b = a.copy();
        a.selfConcat('.');
        b.selfConcat('.');

        const code = Codegen.make_code( [ a, b ] )
        // Graphviz.display( [ a.rp, b.rp ] );
        sequ( code, 'let T0=a;a+=".";T0+=".";' );
    } );

    it( 'while with symbols', () => {
        // const n = symbol<Number>( Number, "n" ), c = new Number( 5 );
        // _while( () => n.isInf( 10 ),
        //         () => { c.selfAdd( n ); n.selfAdd( 1 ); } );
        const n = LvNumber.symbol( "n" );
        _while( () => n.isInf( 10 ),
                () => n.selfAdd( 1 ) );

        // Graphviz.display( [ n.rp ] );
        sequ( Codegen.make_code( [ n ] ), 'while(n<10){n+=1;}' );
    } );

    it( 'while with variable created during the loop', () => {
        const n = LvNumber.symbol( "n" );
        _while( () => n.isInf( 10 ),
                () => { let i = new LvNumber( 2 ); i.set( 1 ); n.selfAdd( i ); } );

        // Graphviz.display( [ n.rp ] );
        sequ( Codegen.make_code( [ n ] ), 'while(n<10){n+=1;}' );
    } );

    it( 'operator precedence', () => {
        const r = LvNumber.symbol( "r" );
        const n = LvNumber.symbol( "n" );
        r.set( n.add( 5 ).mul( 10 ) );

        // Graphviz.display( [ r.rp ] );
        sequ( Codegen.make_code( [ r ] ), 'r=(n+5)*10;' );
    } );
    
    it( 'applyMethod', () => {
        let s = LvString.symbol( "s" );
        s.applyMethod( "murf", new LvString( "17" ) );
        sequ( Codegen.make_code( [ s ] ), 's.murf("17");' );
    });

    it( 'if if', () => {
        let v = LvNumber.symbol( "v" );
        _if( LvNumber.symbol( "c" ), () => {
            _if( LvNumber.symbol( "d" ), () => {
                v.set( 10 );
            } );
        } );

        const code = Codegen.make_code( [ v ] );
        //Graphviz.display( [ v.rp ] );
        sequ( code, 'if(c){if(d){v=10;}}' );
    } );

    // it( 'while if', () => {
    //     let v = LvNumber.symbol( "v" );
    //     _while( () => {
    //         let cont = LvNumber.symbol( "cont" );
    //         _if( LvNumber.symbol( "d" ), () => {
    //             cont.set( 10 );
    //             v.set( 50 );
    //         } );
    //         return cont;
    //     } );

    //     const code = Codegen.make_code( [ v ] );
    //     Graphviz.display( [ v.rp ] );
    //     sequ( code, 'if(c){if(d){v=10;}}' );
    // } );

    it( 'read a Vq number from a Buffer', () => {
        // read number
        function read_val( res: LvNumber, data: LvBuffer, cursor: LvNumber ) {
            let shift = new LvNumber( 0 );
            res.val = 0; 
            _while( () => {
                let cont = cursor.isInf( data.byteLength );
                _if( cont, () => {
                    let val = data.at( cursor );
                    cursor.selfAdd( 1 );
                    _if( val.isSupEq( 128 ), () => {
                        res.selfAdd( val.sub( 128 ).signedShiftLeft( shift ) );
                    }, () => {
                        res.selfAdd( val.signedShiftLeft( shift ) );
                        cont.set( 0 )
                    } );
                } );
                return cont;
            }, () => {
                shift.selfAdd( 7 );
            } );
            return res;
        }

        const res = LvNumber.symbol( "output" );
        read_val( res, LvBuffer.symbol( "data" ), LvNumber.symbol( "cursor" ) );
        // Graphviz.display( [ res.rp ] );
        
        const code = Codegen.make_code( [ res ] );
        // console.log( "code:", code );
        {
            var cursor = 0, data = Buffer.from( [ 130, 3 ] ), output;
            eval( code );
            sequ( cursor, 2 );
            sequ( output, 130 - 128 + ( 3 << 7 ) );
        }
    } );

    // it( 'select', () => {
    //     const str = symbol<String>( String, "str" );
    //     const res = symbol<Number>( Number, "res" );
    //     res.set( str.charAt( 10 ) );

    //     // Graphviz.display( [ r.rp ] );
    //     sequ( Codegen.make_code( [ res ] ), 'r=(n+5)*10;' );
    // } );

    // it( 'while with known values', () => {
    //     let k = 0;
    //     _while( () => k < 10, () => k += 1 );
    //     sequ( k, 10 );
    // } );

    // it( 'pouet', () => {
        // let ofg = new OtFuncsGeneration();
        // ofg.reg_operation( "insert", [ { name: 'pos', type: "PT" }, { name: 'str', type: "String" } ] );
        // ofg.reg_right( "insert" );

        // ofg.reg_nuf( "insert", "insert", function( asu_n, bq_n, bq_o, n, o ) {
        //     _if( o.pos.sup( n.pos ), () => {
        //         // orig 01234
        //         // real 0123unk4    INS(args_o[ 0 ]=4,args_o[ 1 ]=unk)
        //         // imag 0new1234    INS(args_n[ 0 ]=1,args_n[ 1 ]=new)
        //         // obj  0new123unk4 (real -> obj = new: INS 1,new; imag -> obj = unk: INS 7,unk)
        //         o.pos += n.str.length;
        //     }, () => {
        //         // orig 01234
        //         // real 0123unk4    INS(args_o[ 0 ]=4,args_o[ 1 ]=unk)
        //         // imag 0new1234    INS(args_n[ 0 ]=1,args_n[ 1 ]=new)
        //         // obj  0new123unk4 (real -> obj = new: INS 1,new; imag -> obj = unk: INS 7,unk)
        //         n.pos += o.str.length;
        //     } );
        // } );

        // ofg.
        // let a = new String( "test" ), b = symbol<String>( String, 'b' );
        // right: function( rp: OtWrapperString, flags : number, pos : number, sup: string ) : boolean {
        //     return OtWrapperString.can_insert.ok( flags ) || ( pos == rp.val.length.val && OtWrapperString.can_append.ok( flags ) );
        // },
        // valid: function( rp: OtWrapperString, pos : number, sup: string ) : boolean {
        //     return sup.length != 0;
        // },
        // apply_op( rp: OtWrapperString, aod, pos : number, sup: string ) : void {
        //     for( let c of rp.cursors ) {
        //         for( let caret of c.carets ) {
        //             if ( caret.beg >= pos )
        //                 caret.beg += sup.length;
        //             if ( caret.end >= pos )
        //                 caret.end += sup.length;
        //         }
        //     }

        //     methods["self_insert_bb"].call_3( rp.val, new Number( pos ), new String( sup ) );
        //     rp.sig_change()
        // },
        // undo( rp: OtWrapperString, aod, pos : number, sup: string ) : void {
        //     OtWrapperString.op_remove.apply_op( rp, aod, pos, sup.length );
        // },
        // update_patch_data_l0( rp: OtWrapperString, args ) : void {
        // }
        /**
         * 
         */
    // });
});
 

