/// <reference path="../../node_modules/@types/mocha/index.d.ts"/>
//// nsmake global define NO_OPERATIONAL_TRANSFORM
import GenOperations, { _if, Op,
       OtWrapperWithRightFlags } from "../Core/GenOperations"
import LvNumber                  from "../LvNumber"
import LvString                  from "../LvString"
import LvInode                   from "../LvInode"

// symbolic representation
class RpDirectory extends OtWrapperWithRightFlags { data = new LvString; }

//
let go = new GenOperations<RpDirectory>( RpDirectory, { data: "" } );
go.define_rights_by_flags( [ "set", "add", "delete" ] );

// operation types
class SetVal { name = new LvString; inode = new LvInode; }
class Delete { name = new LvString; }
class DelUnd { name = new LvString; inode = new LvInode; }

// declaration of operation types for AgencyDB
go.apply( SetVal, ( d: RpDirectory, o: SetVal ) => d.data.applyMethod( "set", o.name, o.inode ) );
go.apply( Delete, ( d: RpDirectory, o: Delete ) => d.data.applyMethod( "delete", o.name       ) );
go.apply( DelUnd, ( d: RpDirectory, o: DelUnd ) => d.data.applyMethod( "delete", o.name       ) );

// go.undo ( Insert, ( d: RpDirectory, o: Insert ) => d.val.remove( o.pos, o.str.length ) );
// go.undo ( Remove, null                                                                     );
// go.undo ( RemUnd, ( d: RpDirectory, o: RemUnd ) => d.val.insert( o.pos, o.str        ) );

// go.store( Remove, ( d: RpDirectory, o: Remove, b ) => b.push( RemUnd, { pos: o.pos, str: d.val.substr( o.pos, o.len ) } as RemUnd ) );

// // store conditions
// go.stcnd( Insert, ( o: Insert, r ) => r.set( o.str.length ) );
// go.stcnd( Remove, ( o: Remove, r ) => r.set( o.len ) );
// go.stcnd( RemUnd, ( o: RemUnd, r ) => r.set( o.str.length ) );

// //
// go.right( Insert, ( d: RpDirectory, o: Insert, f: LvNumber, r: LvNumber ) => r.set( f.andBin( go.flag( "insert" ) ).orLog( f.andBin( go.flag( "append" ) ).andLog( o.pos.isEqu( d.val.length ) ) ) ) );
// go.right( Remove, ( d: RpDirectory, o: Insert, f: LvNumber, r: LvNumber ) => r.set( f.andBin( go.flag( "remove" ) ) ) );
// go.right( RemUnd, ( d: RpDirectory, o: Insert, f: LvNumber, r: LvNumber ) => r.set( f.andBin( go.flag( "remove" ) ) ) );
go.right( SetVal, ( d: RpDirectory, o: SetVal, f: LvNumber, r: LvNumber ) => r.set( f.andBin( go.flag( "set"    ) ) ) );
go.right( Delete, ( d: RpDirectory, o: Delete, f: LvNumber, r: LvNumber ) => r.set( f.andBin( go.flag( "delete" ) ) ) );
go.right( DelUnd, ( d: RpDirectory, o: DelUnd, f: LvNumber, r: LvNumber ) => r.set( f.andBin( go.flag( "delete" ) ) ) );

// // combinations
// go.fwd_trans( Insert, Insert, ( o: Insert, n: Insert ) => {
//     _if( o.pos.isSup( n.pos ), () => {
//         // i: 01234      
//         // o: 0123unk4    INS(4,unk)
//         // n: 0new1234    INS(1,new)
//         // r: 0new123unk4 n:(o->r) = INS(1,new); o:(n->r) = INS(7,unk)
//         o.pos.selfAdd( n.str.length );
//     }, () => {
//         // i: 01234
//         // o: 0123unk4    INS(4,unk)
//         // n: 0new1234    INS(1,new)
//         // r: 0new123unk4 n:(o->r) = INS(1,new); o:(n->r) = INS(7,unk)
//         n.pos.selfAdd( o.str.length );
//     } );
// } );

process.stdout.write( `import RpDirectory  from "./RpDirectory"\n` );
process.stdout.write( `import Inode        from "../System/Inode"\n` );
go.write( "ts" );
