/// <reference path="../../node_modules/@types/mocha/index.d.ts"/>
//// nsmake global define NO_OPERATIONAL_TRANSFORM
import GenOperations, { _if, 
         OpWriter, Op } from "../Core/GenOperations"
import LvNumber         from "../LvNumber"
import LvString         from "../LvString"

//
let go = new GenOperations<LvString>( 'OtWrapperString', LvString );

// operation types
class Insert { pos = new LvNumber(); str = new LvString(); };
class Remove { pos = new LvNumber(); len = new LvNumber(); };
class RemUnd { pos = new LvNumber(); str = new LvString(); };
type StrOp = Insert | Remove | RemUnd;

// declaration of operation types for AgencyDB
go.apply( Insert, ( d: LvString, o: Insert ) => d.insert( o.pos, o.str )        );
go.apply( Remove, ( d: LvString, o: Remove ) => d.remove( o.pos, o.len )        );
go.apply( RemUnd, ( d: LvString, o: RemUnd ) => d.remove( o.pos, o.str.length ) );

go.undo ( Insert, ( d: LvString, o: Insert ) => d.remove( o.pos, o.str.length ) );
go.undo ( Remove, null                                                          );
go.undo ( RemUnd, ( d: LvString, o: RemUnd ) => d.insert( o.pos, o.str )        );

go.store( Remove, ( d: LvString, o: Remove ) => [ { type: RemUnd, data: { pos: o.pos, str: d.substr( o.pos, o.len ) } as RemUnd } ] );

// combinations
go.fwd_trans( Insert, Insert, ( o: Insert, n: Insert ) => {
    _if( o.pos.is_sup( n.pos ), () => {
        // orig 01234
        // real 0123unk4    INS(args_o[ 0 ]=4,args_o[ 1 ]=unk)
        // imag 0new1234    INS(args_n[ 0 ]=1,args_n[ 1 ]=new)
        // obj  0new123unk4 (real -> obj = new: INS 1,new; imag -> obj = unk: INS 7,unk)
        o.pos.self_add( n.str.length );
    }, () => {
        // orig 01234
        // real 0123unk4    INS(args_o[ 0 ]=4,args_o[ 1 ]=unk)
        // imag 0new1234    INS(args_n[ 0 ]=1,args_n[ 1 ]=new)
        // obj  0new123unk4 (real -> obj = new: INS 1,new; imag -> obj = unk: INS 7,unk)
        n.pos.self_add( o.str.length );
    } );
} );

process.stdout.write( `import OtWrapperString from "./OtWrapperString"\n` );
go.write( "ts" );
