/// <reference path="../../node_modules/@types/mocha/index.d.ts"/>
//// nsmake global define NO_OPERATIONAL_TRANSFORM
import GenOperations, { _if } from "../Core/GenOperations"
import LvNumber               from "../LvNumber"
import LvString               from "../LvString"

let go = new GenOperations( 'OtWrapperString' );

// operation types
go.add_op( 'insert', [ "pos:PT", "sup:String" ], {
    ts: ( name, suffix ) => `${ name }.val.data = ${ name }.val.data.slice( 0, pos${ suffix } ) + sup${ suffix } + ${ name }.val.data.slice( pos${ suffix } );`
}, {
    ts: ( name, suffix ) => `${ name }.val.data = ${ name }.val.data.slice( 0, pos${ suffix } ) + ${ name }.val.data.slice( pos${ suffix } + sup${ suffix }.length );`
} );

go.add_op( 'remove', [ "pos:PT", "len:PT" ], {
    ts: ( name, suffix ) => `${ name }.val.data = ${ name }.val.data.slice( 0, pos${ suffix } ) + ${ name }.val.data.slice( pos${ suffix } + len${ suffix } );`
}, {
    ts: ( name, suffix ) => `${ name }.val.data = "proute";`
} );

interface Insert { pos: LvNumber; sup: LvString };

// combinations
go.add_ftr( 'insert', 'insert', ( o: Insert, n: Insert ) => {
    _if( o.pos.is_sup( n.pos ), () => {
        // orig 01234
        // real 0123unk4    INS(args_o[ 0 ]=4,args_o[ 1 ]=unk)
        // imag 0new1234    INS(args_n[ 0 ]=1,args_n[ 1 ]=new)
        // obj  0new123unk4 (real -> obj = new: INS 1,new; imag -> obj = unk: INS 7,unk)
        o.pos.self_add( n.sup.length );
    }, () => {
        // orig 01234
        // real 0123unk4    INS(args_o[ 0 ]=4,args_o[ 1 ]=unk)
        // imag 0new1234    INS(args_n[ 0 ]=1,args_n[ 1 ]=new)
        // obj  0new123unk4 (real -> obj = new: INS 1,new; imag -> obj = unk: INS 7,unk)
        n.pos.self_add( o.sup.length );
    } );
} );

process.stdout.write( `import OtWrapperString from "./OtWrapperString"\n` );
go.write( "ts" );
