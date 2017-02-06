/// <reference path="../../node_modules/@types/mocha/index.d.ts"/>
//// nsmake global define NO_OPERATIONAL_TRANSFORM
import GenOperations, { _if, Op,
       OtWrapperWithRightFlags } from "../Core/GenOperations"
import LvNumber                  from "../LvNumber"
import LvString                  from "../LvString"

// symbolic representation
class RpMap extends OtWrapperWithRightFlags { val = new LvString; }

//
let go = new GenOperations<RpMap>( RpMap, { val: "val.data" } );
go.define_rights_by_flags( [ "set", "add", "delete" ] );

// operation types
class Insert { pos = new LvNumber; str = new LvString; }

// declaration of operation types for AgencyDB
// go.apply( Insert, ( d: RpMap, o: Insert ) => d.val.insert( o.pos, o.str        ) );
// go.apply( Remove, ( d: RpMap, o: Remove ) => d.val.remove( o.pos, o.len        ) );
// go.apply( RemUnd, ( d: RpMap, o: RemUnd ) => d.val.remove( o.pos, o.str.length ) );

// go.undo ( Insert, ( d: RpMap, o: Insert ) => d.val.remove( o.pos, o.str.length ) );
// go.undo ( Remove, null                                                                     );
// go.undo ( RemUnd, ( d: RpMap, o: RemUnd ) => d.val.insert( o.pos, o.str        ) );

// go.store( Remove, ( d: RpMap, o: Remove, b ) => b.push( RemUnd, { pos: o.pos, str: d.val.substr( o.pos, o.len ) } as RemUnd ) );

// // store conditions
// go.stcnd( Insert, ( o: Insert, r ) => r.set( o.str.length ) );
// go.stcnd( Remove, ( o: Remove, r ) => r.set( o.len ) );
// go.stcnd( RemUnd, ( o: RemUnd, r ) => r.set( o.str.length ) );

// //
// go.right( Insert, ( d: RpMap, o: Insert, f: LvNumber, r: LvNumber ) => r.set( f.andBin( go.flag( "insert" ) ).orLog( f.andBin( go.flag( "append" ) ).andLog( o.pos.isEqu( d.val.length ) ) ) ) );
// go.right( Remove, ( d: RpMap, o: Insert, f: LvNumber, r: LvNumber ) => r.set( f.andBin( go.flag( "remove" ) ) ) );
// go.right( RemUnd, ( d: RpMap, o: Insert, f: LvNumber, r: LvNumber ) => r.set( f.andBin( go.flag( "remove" ) ) ) );

// combinations
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


process.stdout.write( `import RpMap from "./RpMap"\n` );
go.write( "ts" );
