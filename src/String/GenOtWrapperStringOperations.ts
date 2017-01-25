/// <reference path="../../node_modules/@types/mocha/index.d.ts"/>
//// nsmake global define NO_OPERATIONAL_TRANSFORM
import GenOperations, { _if, 
         OpWriter, Op } from "../Core/GenOperations"
import LvNumber         from "../LvNumber"
import LvString         from "../LvString"


// operation types
class Insert { pos = new LvNumber(); str = new LvString(); };
class Remove { pos = new LvNumber(); len = new LvNumber(); };
class RemUnd { pos = new LvNumber(); str = new LvString(); };
// type Op = Insert | Remove | RemUnd;

// declaration of operation types for AgencyDB
let go = new GenOperations<LvString>( 'OtWrapperString', LvString );
go.apply( Insert, ( d: LvString, o: Insert ) => d.insert( o.pos, o.str ) );
go.apply( Remove, ( d: LvString, o: Remove ) => d.remove( o.pos, o.len ) );

go.undo ( Insert, ( d: LvString, o: Insert ) => d.remove( o.pos, o.str.length ) );
go.undo ( Remove, ( d: LvString, o: Remove ) => d.insert( o.pos, "proute" ) );


// go.add_op( 'insert', [ "pos:PT", "sup:String" ], {
//     apply: {
//         ts: ( name, suffix ) => `${ name }.val.data = ${ name }.val.data.slice( 0, pos${ suffix } ) + sup${ suffix } + ${ name }.val.data.slice( pos${ suffix } );`
//     }, 
//     undo: {
//         ts: ( name, suffix ) => `${ name }.val.data = ${ name }.val.data.slice( 0, pos${ suffix } ) + ${ name }.val.data.slice( pos${ suffix } + sup${ suffix }.length );`
//     } 
// } );


// go.add_op( 'insert', [ "pos:PT", "sup:String" ], {
//     apply: {
//         ts: ( name, suffix ) => `${ name }.val.data = ${ name }.val.data.slice( 0, pos${ suffix } ) + sup${ suffix } + ${ name }.val.data.slice( pos${ suffix } );`
//     }, 
//     undo: {
//         ts: ( name, suffix ) => `${ name }.val.data = ${ name }.val.data.slice( 0, pos${ suffix } ) + ${ name }.val.data.slice( pos${ suffix } + sup${ suffix }.length );`
//     } 
// } );

// go.add_op( 'remove', [ "pos:PT", "len:PT" ], {
//     apply: {
//         ts: ( name, suffix ) => `${ name }.val.data = ${ name }.val.data.slice( 0, pos${ suffix } ) + ${ name }.val.data.slice( pos${ suffix } + len${ suffix } );`
//     }, 
//     undo: {
//         ts: null
//     },
//     store: ( b: OpWriter, o: Remove ) => { b( { type: "remund", pos: o.pos, str:  } as RemUnd ); }
// } );

// go.add_op( 'remund', [ "pos:PT", "len:PT" ], {
//     apply: {
//         ts: ( name, suffix ) => `${ name }.val.data = ${ name }.val.data.slice( 0, pos${ suffix } ) + ${ name }.val.data.slice( pos${ suffix } + len${ suffix } );`
//     }, 
//     undo: {
//         ts: ( name, suffix ) => `${ name }.val.data = "proute";`
//     },
// } );

// // combinations
// go.add_ftr( 'insert', 'insert', ( o: Insert, n: Insert ): { o: Array<Op>, n: Array<Op> } => {
//     _if( o.pos.is_sup( n.pos ), () => {
//         // orig 01234
//         // real 0123unk4    INS(args_o[ 0 ]=4,args_o[ 1 ]=unk)
//         // imag 0new1234    INS(args_n[ 0 ]=1,args_n[ 1 ]=new)
//         // obj  0new123unk4 (real -> obj = new: INS 1,new; imag -> obj = unk: INS 7,unk)
//         o.pos.self_add( n.str.length );
//     }, () => {
//         // orig 01234
//         // real 0123unk4    INS(args_o[ 0 ]=4,args_o[ 1 ]=unk)
//         // imag 0new1234    INS(args_n[ 0 ]=1,args_n[ 1 ]=new)
//         // obj  0new123unk4 (real -> obj = new: INS 1,new; imag -> obj = unk: INS 7,unk)
//         n.pos.self_add( o.str.length );
//     } );
//     return { o: [ o ], n: [ n ] };
// } );

process.stdout.write( `import OtWrapperString from "./OtWrapperString"\n` );
go.write( "ts" );
