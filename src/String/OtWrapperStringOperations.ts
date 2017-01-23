//// nsmake global define NO_OPERATIONAL_TRANSFORM
import GenOperations, { _if } from "../Core/GenOperations"
import LvNumber               from "../LvNumber"
import LvString               from "../LvString"

let go = new GenOperations( 'OtWrapperString' );

go.add_op( 'insert', [ "pos:PT", "sup:String" ], {
    ts: name => `${ name }.val.data = ${ name }.val.data.slice( 0, pos ) + sup + ${ name }.val.data.slice( pos );`
} );

go.add_op( 'remove', [ "pos:PT", "len:PT" ], {
    ts: name => `${ name }.val.data = ${ name }.val.data.slice( 0, pos ) + ${ name }.val.data.slice( pos + len );`
} );

interface Insert { pos: LvNumber; sup: LvString };

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
//     if ( args_o[ 0 ] > args_n[ 0 ] ) {
//     } else {
//     }

process.stdout.write( `import OtWrapperString from "./OtWrapperString"\n` );
go.write( "ts" );
