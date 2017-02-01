/// <reference path="../../node_modules/@types/mocha/index.d.ts"/>
//// nsmake global define NO_OPERATIONAL_TRANSFORM
import GenOperations, { _if, Op,
       OtWrapperWithRightFlags } from "../Core/GenOperations"
import LvNumber                  from "../LvNumber"
import LvString                  from "../LvString"

// symbolic representation
class OtWrapperString extends OtWrapperWithRightFlags { val = new LvString; }

//
let go = new GenOperations<OtWrapperString>( OtWrapperString, { val: "val.data" } );
go.define_rights_by_flags( [ "read", "insert", "remove", "append" ] );

// operation types
class Insert { pos = new LvNumber; str = new LvString; }
class Remove { pos = new LvNumber; len = new LvNumber; }
class RemUnd { pos = new LvNumber; str = new LvString; }
type StrOp = Insert | Remove | RemUnd;

// declaration of operation types for AgencyDB
go.apply( Insert, ( d: OtWrapperString, o: Insert ) => d.val.insert( o.pos, o.str        ) );
go.apply( Remove, ( d: OtWrapperString, o: Remove ) => d.val.remove( o.pos, o.len        ) );
go.apply( RemUnd, ( d: OtWrapperString, o: RemUnd ) => d.val.remove( o.pos, o.str.length ) );

go.undo ( Insert, ( d: OtWrapperString, o: Insert ) => d.val.remove( o.pos, o.str.length ) );
go.undo ( Remove, null                                                                     );
go.undo ( RemUnd, ( d: OtWrapperString, o: RemUnd ) => d.val.insert( o.pos, o.str        ) );

go.store( Remove, ( d: OtWrapperString, o: Remove ) => [ { type: RemUnd, data: { pos: o.pos, str: d.val.substr( o.pos, o.len ) } as RemUnd } ] );

//
go.right( Insert, ( d: OtWrapperString, o: Insert, f: LvNumber, r: LvNumber ) => r.set( f.and_bin( go.flag( "insert" ) ).or_log( f.and_bin( go.flag( "append" ) ).and_log( o.pos.is_equ( d.val.length ) ) ) ) );
go.right( Remove, ( d: OtWrapperString, o: Insert, f: LvNumber, r: LvNumber ) => r.set( f.and_bin( go.flag( "remove" ) ) ) );
go.right( RemUnd, ( d: OtWrapperString, o: Insert, f: LvNumber, r: LvNumber ) => r.set( f.and_bin( go.flag( "remove" ) ) ) );

// combinations
go.fwd_trans( Insert, Insert, ( o: Insert, n: Insert ) => {
    _if( o.pos.is_sup( n.pos ), () => {
        // i: 01234      
        // o: 0123unk4    INS(4,unk)
        // n: 0new1234    INS(1,new)
        // r: 0new123unk4 n:(o->r) = INS(1,new); o:(n->r) = INS(7,unk)
        o.pos.self_add( n.str.length );
    }, () => {
        // i: 01234
        // o: 0123unk4    INS(4,unk)
        // n: 0new1234    INS(1,new)
        // r: 0new123unk4 n:(o->r) = INS(1,new); o:(n->r) = INS(7,unk)
        n.pos.self_add( o.str.length );
    } );
} );

go.fwd_trans( Remove, Insert, ( o: Remove, n: Insert, l ) => {
    _if( n.pos.is_supeq( o.pos.add( o.len ) ), () => {
        // i: 012345
        // o: 0345      REM(1,2)
        // n: 01234new5 INS(5,new)
        // r: 034new5   n:(o->r) = INS(3,new); o:(n->r) = REM(1,2)
        n.pos.self_sub( o.len ); // 3
    }, n.pos.is_infeq( o.pos ), () => {
        // i: 012345
        // o: 0125      REM(3,2)
        // n: 0new12345 INS(1,new)
        // r: 0new125   n:(o->r) = INS(1,new); o:(n->r) = REM(6,2)
        o.pos.self_add( n.str.length );
    }, () => {
        // i: 012345
        // o: 05        REM(1,4)
        // n: 012new345 INS(3,new)
        // r: 0new5     n:(o->r) = INS(1,new); o:(n->r) = REM(1,2) + REM(4,2)
        //l.push( { type: Remove, data: { pos: o.pos, len: n.pos.sub( o.pos ) } } ); // 1, 2
        l.push( Remove, { pos: o.pos, len: n.pos.sub( o.pos ) } );
        // Prop 1: on fait un bq symbolique. Prop 2: on fait une liste JSON-like symbolique avec succession de données (typées UsrId, PT, ...)
        // genre les données à ajouter au bq
        o.len.self_sub( n.pos.sub( o.pos ) ); // 2
        n.pos.set( o.pos );                   // 1
        o.pos.self_add( n.str.length );       // 4
    } );
} );

process.stdout.write( `import OtWrapperString from "./OtWrapperString"\n` );
go.write( "ts" );


// void OtWrapperString::unk_new_or_new_unk( op_insert, op_remove_und, UsrId asu_n, BBQ bq_n, BBQ bq_o, PT &p_n, CbString &d_n, PT &p_o, CbString &d_o ) {
//     if ( p_n >= p_o + d_o.size() ) {
//         // orig 012345
//         // real 0345      REM(p_o=1,l_o=2)
//         // imag 01234new5 INS(p_n=5,d_n=new)
//         // obj  034new5   (real -> obj = new: INS 3,new; imag -> obj = unk: REM 1,2)
//         p_n -= d_o.size(); // 3
//     } else if ( p_n <= p_o ) {
//         // orig 012345
//         // real 0125      REM(p_o=3,l_o=2)
//         // imag 0new12345 INS(p_n=1,d_n=new)
//         // obj  0new125   (real -> obj = new: INS 1,new; imag -> obj = unk: REM 6,2)
//         p_o += d_n.size();
//     } else {
//         // orig 012345
//         // real 05        REM(p_o=1,l_o=4)
//         // imag 012new345 INS(p_n=3,d_n=new)
//         // obj  0new5     (real -> obj = new: INS 1,new; imag -> obj = unk: REM 1,'01' + REM 4,'34')
//         reg_op_in( bq_o, op_remove_und{}, p_o, d_o.beg_upto( p_n - p_o ) ); // 1, '12'
//         d_o.skip_some( p_n - p_o );  // '34'
//         p_n = p_o;         // 1
//         p_o += d_n.size(); // 4
//     }
// }

// void OtWrapperString::unk_new_or_new_unk( op_insert, op_set_cursor, UsrId asu_n, BBQ bq_n, BBQ bq_o, PT &p_n, CbString &d_n, DevId &dev_id_o, PI64 &loc_id_o, PT &pos_o ) {
//     if ( pos_o >= p_n )
//         pos_o += d_n.size();
// }

// void OtWrapperString::unk_new_or_new_unk( op_insert, op_new_cursor, UsrId asu_n, BBQ bq_n, BBQ bq_o, PT &p_n, CbString &d_n, DevId &dev_id_o, PI64 &loc_id_o, PT &pos_o ) {
//     if ( pos_o >= p_n )
//         pos_o += d_n.size();
// }

// void OtWrapperString::unk_new_or_new_unk( op_insert, op_set_cursor_und, UsrId asu_n, BBQ bq_n, BBQ bq_o, PT &p_n, CbString &d_n, DevId &dev_id_o, PI64 &loc_id_o, PT &pos_o, PT &old_pos_o ) {
//     if ( pos_o >= p_n )
//         pos_o += d_n.size();
// }

// void OtWrapperString::unk_new_or_new_unk( op_remove, op_insert, UsrId asu_n, BBQ bq_n, BBQ bq_o, PT &p_n, PT &l_n, PT &p_o, CbString &d_o ) {
//     if ( p_o <= p_n ) {
//         // orig 01234567
//         // real 01ab234567 INS(p_o=2,d_o=ab)
//         // imag 012347     REM(p_n=5,l_n=2)
//         // obj  01ab2347   (real -> obj = new: REM 7,2; imag -> obj = unk: INS 2,ab)
//         p_n += d_o.size();
//     } else if ( p_o >= p_n + l_n ) {
//         // orig 01234567
//         // real 01234ab567 INS(p_o=5,d_o=ab)
//         // imag 014567     REM(p_n=2,l_n=2)
//         // obj  014ab567   (real -> obj = new: REM 2,2; imag -> obj = unk: INS 3,ab)
//         p_o -= l_n;
//     } else {
//         // orig 01234567
//         // real 0123ab4567 INS(p_o=4,d_o=ab)
//         // imag 0167       REM(p_n=2,l_n=4)
//         // obj  01ab67     (real -> obj = new: REM 2,2 + REM 4,2; imag -> obj = unk: INS 2,ab)
//         _substring( p_n, p_o - p_n, [&]( DaSi d ) { reg_op_in( bq_n, op_remove_und{}, p_n, d ); } ); // 2, 2
//         OtWrapperString::Aod aod_n; aod_n.as_usr = asu_n;
//         apply_op( op_remove{}, aod_n, p_n, p_o - p_n );
//         l_n -= p_o - p_n;  // 2
//         p_o = p_n;         // 2
//         p_n += d_o.size(); // 4
//     }
// }

// void OtWrapperString::unk_new_or_new_unk( op_remove, op_remove, UsrId asu_n, BBQ bq_n, BBQ bq_o, PT &p_n, PT &l_n, PT &p_o, PT &l_o ) {
//     if ( p_o <= p_n ) {
//         if ( p_o + l_o <= p_n ) {
//             // orig 0123456789
//             // real 0456789    REM(p_o=1,l_o=3)
//             // imag 01234589   REM(p_n=6,l_n=2)
//             // obj  04589      (real -> obj = new: REM 3,2; imag -> obj = unk: REM 1,3)
//             p_n -= l_o; // 3
//         } else if ( p_o + l_o <= p_n + l_n ) {
//             // orig 0123456789
//             // real 06789      REM(p_o=1,l_o=5)
//             // imag 012389     REM(p_n=4,l_n=4)
//             // obj  089        (real -> obj = new: REM 1,2; imag -> obj = unk: REM 1,3)
//             PT d = p_o + l_o - p_n; // 2
//             l_n -= d;  // 2
//             l_o -= d;  // 3
//             p_n = p_o; // 1
//         } else {
//             // orig 0123456789
//             // real 09         REM(p_o=1,l_o=8)
//             // imag 01236789   REM(p_n=4,l_n=2)
//             // obj  09         (real -> obj = new: VOID; imag -> obj = unk: REM 1,6)
//             l_o -= l_n;
//             l_n = 0;
//         }
//     } else {
//         if ( p_n + l_n <= p_o ) {
//             // orig 0123456789
//             // real 01234589   REM(p_o=6,l_o=2)
//             // imag 0456789    REM(p_n=1,l_n=3)
//             // obj  04589      (real -> obj = new: REM 1,3; imag -> obj = unk: REM 3,2)
//             p_o -= l_n; // 3
//         } else if ( p_n + l_n <= p_o + l_o ) {
//             // orig 0123456789
//             // real 012389     REM(p_o=4,l_o=4)
//             // imag 06789      REM(p_n=1,l_n=5)
//             // obj  089        (real -> obj = new: REM 1,3; imag -> obj = unk: REM 1,2)
//             PT d = p_n + l_n - p_o; // 2
//             l_o -= d;  // 2
//             l_n -= d;  // 3
//             p_o = p_n; // 1
//         } else {
//             // orig 0123456789
//             // real 01236789   REM(p_o=4,l_o=2)
//             // imag 09         REM(p_n=1,l_n=8)
//             // obj  09         (real -> obj = new: REM 1,6; imag -> obj = unk: VOID)
//             l_n -= l_o;
//             l_o = 0;
//         }
//     }
// }

// void OtWrapperString::unk_new_or_new_unk( op_remove, op_remove_und, UsrId asu_n, BBQ bq_n, BBQ bq_o, PT &p_n, PT &l_n, PT &p_o, CbString &d_o ) {
//     if ( p_o <= p_n ) {
//         if ( p_o + d_o.size() <= p_n ) {
//             // orig 0123456789
//             // real 0456789    REM(p_o=1,d_o='123')
//             // imag 01234589   REM(p_n=6,l_n=2)
//             // obj  04589      (real -> obj = new: REM 3,2; imag -> obj = unk: REM_UND 1,'123')
//             p_n -= d_o.size(); // 3
//         } else if ( p_o + d_o.size() <= p_n + l_n ) {
//             // orig 0123456789
//             // real 06789      REM_UND(p_o=1,d_o='12345')
//             // imag 012389     REM(p_n=4,l_n=4)
//             // obj  089        (real -> obj = new: REM 1,2; imag -> obj = unk: REM_UND 1,'123')
//             PT d = p_o + d_o.size() - p_n; // 2
//             d_o = d_o.beg_upto( d_o.size() - d ); // '123'
//             l_n -= d;  // 2
//             p_n = p_o; // 1
//         } else {
//             // orig 0123456789
//             // real 09         REM_UND(p_o=1,d_o='12345678')
//             // imag 01236789   REM(p_n=4,l_n=2)
//             // obj  09         (real -> obj = new: VOID; imag -> obj = unk: REM_UND 1,'123678')
//             CbQueue t;
//             t.write_some( d_o.beg_upto( p_n - p_o ) );
//             t.write_some( d_o.end_from( p_n - p_o + l_n ) );
//             d_o = std::move( t );
//             l_n = 0;
//         }
//     } else {
//         if ( p_n + l_n <= p_o ) {
//             // orig 0123456789
//             // real 01234589   REM_UND(p_o=6,d_o='67')
//             // imag 0456789    REM(p_n=1,l_n=3)
//             // obj  04589      (real -> obj = new: REM 1,3; imag -> obj = unk: REM_UND 3,'67')
//             p_o -= l_n; // 3
//         } else if ( p_n + l_n <= p_o + d_o.size() ) {
//             // orig 0123456789
//             // real 012389     REM_UND(p_o=4,d_o='4567')
//             // imag 06789      REM(p_n=1,l_n=5)
//             // obj  089        (real -> obj = new: REM 1,3; imag -> obj = unk: REM_UND 1,'67')
//             PT d = p_n + l_n - p_o; // 2
//             d_o.skip_some( d ); // '67'
//             l_n -= d;  // 3
//             p_o = p_n; // 1
//         } else {
//             // orig 0123456789
//             // real 01236789   REM(p_o=4,d_o.size()=2)
//             // imag 09         REM(p_n=1,l_n=8)
//             // obj  09         (real -> obj = new: REM 1,6; imag -> obj = unk: VOID)
//             l_n -= d_o.size();
//             d_o.free();
//         }
//     }
// }

// void OtWrapperString::unk_new_or_new_unk( op_remove, op_new_cursor, UsrId asu_n, BBQ bq_n, BBQ bq_o, PT &p_n, PT &l_n, DevId &dev_id_o, PI64 &loc_id_o, PT &pos_o ) {
//     if ( pos_o >= p_n + l_n )
//         pos_o -= l_n;
//     else if ( pos_o > p_n )
//         pos_o = p_n;
// }

// void OtWrapperString::unk_new_or_new_unk( op_remove, op_set_cursor, UsrId asu_n, BBQ bq_n, BBQ bq_o, PT &p_n, PT &l_n, DevId &dev_id_o, PI64 &loc_id_o, PT &pos_o ) {
//     if ( pos_o >= p_n + l_n )
//         pos_o -= l_n;
//     else if ( pos_o > p_n )
//         pos_o = p_n;
// }

// void OtWrapperString::unk_new_or_new_unk( op_remove, op_set_cursor_und, UsrId asu_n, BBQ bq_n, BBQ bq_o, PT &p_n, PT &l_n, DevId &dev_id_o, PI64 &loc_id_o, PT &pos_o , PT &old_pos_o ) {
//     if ( pos_o >= p_n + l_n )
//         pos_o -= l_n;
//     else if ( pos_o > p_n )
//         pos_o = p_n;
// }

// void OtWrapperString::unk_new_or_new_unk( op_remove_und, op_insert, UsrId asu_n, BBQ bq_n, BBQ bq_o, PT &p_n, CbString &d_n, PT &p_o, CbString &d_o ) {
//     if ( p_o <= p_n ) {
//         // orig 01234567
//         // real 01ab234567 INS(p_o=2,d_o=ab)
//         // imag 012347     REM(p_n=5,l_n=2)
//         // obj  01ab2347   (real -> obj = new: REM 7,2; imag -> obj = unk: INS 2,ab)
//         p_n += d_o.size();
//     } else if ( p_o >= p_n + d_n.size() ) {
//         // orig 01234567
//         // real 01234ab567 INS(p_o=5,d_o=ab)
//         // imag 014567     REM(p_n=2,l_n=2)
//         // obj  014ab567   (real -> obj = new: REM 2,2; imag -> obj = unk: INS 3,ab)
//         p_o -= d_n.size();
//     } else {
//         // orig 01234567
//         // real 0123ab4567 INS(p_1=4,d_1=ab)
//         // imag 0167       REM_UND(p_0=2,d_0=2345)
//         // obj  01ab67     (real -> obj = unk: REM_UND 2,23 + REM_UND 4,45; imag -> obj = new: INS 2,ab)
//         reg_op_in( bq_n, op_remove_und{}, p_n, CbString( d_n, 0, p_o - p_n ) ); // 2, 23
//         OtWrapperString::Aod aod_n; aod_n.as_usr = asu_n;
//         apply_op( op_remove(), aod_n, p_n, p_o - p_n );
//         d_n.skip_some( p_o - p_n );  // 45
//         p_o = p_n;                   // 2
//         p_n += d_o.size();           // 4
//     }
// }

// void OtWrapperString::unk_new_or_new_unk( op_remove_und, op_remove, UsrId asu_n, BBQ bq_n, BBQ bq_o, PT &p_n, CbString &d_n, PT &p_o, PT &l_o ) {
//     if ( p_o <= p_n ) {
//         if ( p_o + l_o <= p_n ) {
//             // orig 0123456789
//             // real 0456789    REM(p_o=1,l_o=3)
//             // imag 01234589   REM_UND(p_n=6,l_n='67')
//             // obj  04589      (real -> obj = new: REM 3,2; imag -> obj = _n: REM 1,3)
//             p_n -= l_o; // 3
//         } else if ( p_o + l_o <= p_n + d_n.size() ) {
//             // orig 0123456789
//             // real 06789      REM(p_o=1,l_o=5)
//             // imag 012389     REM_UND(p_n=4,d_n='4567')
//             // obj  089        (real -> obj = _n: REM_UND 1,'67'; imag -> obj = _o: REM_UND 1,'123')
//             PT d = p_o + l_o - p_n; // 2
//             l_o -= d; // '123'
//             d_n.skip_some( d );  // '67'
//             p_n = p_o; // 1
//         } else {
//             // orig 0123456789
//             // real 09         REM(p_o=1,l_o=8)
//             // imag 01236789   REM_UND(p_n=4,d_n='45')
//             // obj  09         (real -> obj = _n: VOID; imag -> obj = _n: REM 1,'123678')
//             l_o -= d_n.size();
//             d_n.free();
//         }
//     } else {
//         if ( p_n + d_n.size() <= p_o ) {
//             // orig 0123456789
//             // real 01234589   REM(p_o=6,l_o=2)
//             // imag 0456789    REM_UND(p_n=1,d_n='123')
//             // obj  04589      (real -> obj = _n: REM 1,'123'; imag -> obj = _n: REM 3,2)
//             p_o -= d_n.size(); // 3
//         } else if ( p_n + d_n.size() <= p_o + l_o ) {
//             // orig 0123456789
//             // real 012389     REM(p_o=4,l_o=4)
//             // imag 06789      REM_UND(p_n=1,d_n='12345')
//             // obj  089        (real -> obj = _n: REM 1,'123'; imag -> obj = _n: REM 1,2)
//             PT d = p_n + d_n.size() - p_o; // 2
//             l_o -= d;  // 2
//             d_n = d_n.beg_upto( d_n.size() - d );  // '123'
//             p_o = p_n; // 1
//         } else {
//             // orig 0123456789
//             // real 01236789   REM(p_o=4,l_o=2)
//             // imag 09         REM_UND(p_n=1,d_n='12345678')
//             // obj  09         (real -> obj = _n: REM 1,'123678'; imag -> obj = _n: VOID)
//             CbQueue t;
//             t.write_some( d_n.beg_upto( p_o - p_n ) );
//             t.write_some( d_n.end_from( p_o - p_n + l_o ) );
//             d_n = std::move( t );
//             l_o = 0;
//         }
//     }
// }

// void OtWrapperString::unk_new_or_new_unk( op_remove_und, op_remove_und, UsrId asu_n, BBQ bq_n, BBQ bq_o, PT &p_n, CbString &d_n, PT &p_o, CbString &d_o ) {
//     if ( p_o <= p_n ) {
//         if ( p_o + d_o.size() <= p_n ) {
//             // orig 0123456789
//             // real 0456789    REM(p_o=1,d_o='123')
//             // imag 01234589   REM(p_n=6,d_n='67')
//             // obj  04589      (real -> obj = _n: REM_UND 3,2; imag -> obj = _o: REM_UND 1,'123')
//             p_n -= d_o.size(); // 3
//         } else if ( p_o + d_o.size() <= p_n + d_n.size() ) {
//             // orig 0123456789
//             // real 06789      REM_UND(p_o=1,d_o='12345')
//             // imag 012389     REM(p_n=4,d_n='4567')
//             // obj  089        (real -> obj = _n: REM_UND 1,'67'; imag -> obj = _o: REM_UND 1,'123')
//             PT d = p_o + d_o.size() - p_n; // 2
//             d_o = d_o.beg_upto( d_o.size() - d ); // '123'
//             d_n.skip_some( d );  // '67'
//             p_n = p_o; // 1
//         } else {
//             // orig 0123456789
//             // real 09         REM_UND(p_o=1,d_o='12345678')
//             // imag 01236789   REM(p_n=4,d_n.size()=2)
//             // obj  09         (real -> obj = _n: VOID; imag -> obj = _o: REM_UND 1,'123678')
//             CbQueue t;
//             t.write_some( d_o.beg_upto( p_n - p_o ) );
//             t.write_some( d_o.end_from( p_n - p_o + d_n.size() ) );
//             d_o = std::move( t );
//             d_n.free();
//         }
//     } else {
//         if ( p_n + d_n.size() <= p_o ) {
//             // orig 0123456789
//             // real 01234589   REM_UND(p_o=6,d_o='67')
//             // imag 0456789    REM(p_n=1,d_n.size()='123')
//             // obj  04589      (real -> obj = _n: REM_UND 1,'123'; imag -> obj = _o: REM_UND 3,'67')
//             p_o -= d_n.size(); // 3
//         } else if ( p_n + d_n.size() <= p_o + d_o.size() ) {
//             // orig 0123456789
//             // real 012389     REM_UND(p_o=4,d_o='4567')
//             // imag 06789      REM(p_n=1,d_n='12345')
//             // obj  089        (real -> obj = _n: REM_UND 1,'123'; imag -> obj = _o: REM_UND 1,'67')
//             PT d = p_n + d_n.size() - p_o; // 2
//             d_o.skip_some( d ); // '67'
//             d_n = d_n.beg_upto( d_n.size() - d ); // '123'
//             p_o = p_n; // 1
//         } else {
//             // orig 0123456789
//             // real 01236789   REM(p_o=4,d_o='45')
//             // imag 09         REM(p_n=1,d_n='12345678')
//             // obj  09         (real -> obj = _n: REM_UND 1,'123678'; imag -> obj = _o: VOID)
//             CbQueue t;
//             t.write_some( d_n.beg_upto( p_o - p_n ) );
//             t.write_some( d_n.end_from( p_o - p_n + d_o.size() ) );
//             d_n = std::move( t );
//             d_o.free();
//         }
//     }
// }

// void OtWrapperString::unk_new_or_new_unk( op_remove_und, op_new_cursor, UsrId asu_n, BBQ bq_n, BBQ bq_o, PT &p_n, CbString &d_n, DevId &dev_id_o, PI64 &loc_id_o, PT &pos_o ) {
//     if ( pos_o >= p_n + d_n.size() )
//         pos_o -= d_n.size();
//     else if ( pos_o > p_n )
//         pos_o = p_n;
// }

// void OtWrapperString::unk_new_or_new_unk( op_remove_und, op_set_cursor, UsrId asu_n, BBQ bq_n, BBQ bq_o, PT &p_n, CbString &d_n, DevId &dev_id_o, PI64 &loc_id_o, PT &pos_o ) {
//     if ( pos_o >= p_n + d_n.size() )
//         pos_o -= d_n.size();
//     else if ( pos_o > p_n )
//         pos_o = p_n;
// }

// void OtWrapperString::unk_new_or_new_unk( op_remove_und, op_set_cursor_und, UsrId asu_n, BBQ bq_n, BBQ bq_o, PT &p_n, CbString &d_n, DevId &dev_id_o, PI64 &loc_id_o, PT &pos_o, PT &old_pos_o ) {
//     if ( pos_o >= p_n + d_n.size() )
//         pos_o -= d_n.size();
//     else if ( pos_o > p_n )
//         pos_o = p_n;
// }

// void OtWrapperString::unk_new_or_new_unk( op_new_cursor, op_insert, UsrId asu_n, BBQ bq_n, BBQ bq_o, DevId &dev_id_n, PI64 &loc_id_n, PT &pos_n, PT &p_o, CbString &d_o ) {
//     if ( pos_n >= p_o )
//         pos_n += d_o.size();
// }

// void OtWrapperString::unk_new_or_new_unk( op_new_cursor, op_remove, UsrId asu_n, BBQ bq_n, BBQ bq_o, DevId &dev_id_n, PI64 &loc_id_n, PT &pos_n, PT &p_o, PT &l_o ) {
//     if ( pos_n >= p_o + l_o )
//         pos_n -= l_o;
//     else if ( pos_n > p_o )
//         pos_n = p_o;
// }

// void OtWrapperString::unk_new_or_new_unk( op_new_cursor, op_remove_und, UsrId asu_n, BBQ bq_n, BBQ bq_o, DevId &dev_id_n, PI64 &loc_id_n, PT &pos_n, PT &p_o, CbString &d_o ) {
//     if ( pos_n >= p_o + d_o.size() )
//         pos_n -= d_o.size();
//     else if ( pos_n > p_o )
//         pos_n = p_o;
// }

// void OtWrapperString::unk_new_or_new_unk( op_set_cursor, op_insert, UsrId asu_n, BBQ bq_n, BBQ bq_o, DevId &dev_id_n, PI64 &loc_id_n, PT &pos_n, PT &p_o, CbString &d_o ) {
//     if ( pos_n >= p_o )
//         pos_n += d_o.size();
// }

// void OtWrapperString::unk_new_or_new_unk( op_set_cursor, op_remove, UsrId asu_n, BBQ bq_n, BBQ bq_o, DevId &dev_id_n, PI64 &loc_id_n, PT &pos_n, PT &p_o, PT &l_o ) {
//     if ( pos_n >= p_o + l_o )
//         pos_n -= l_o;
//     else if ( pos_n > p_o )
//         pos_n = p_o;
// }

// void OtWrapperString::unk_new_or_new_unk( op_set_cursor, op_remove_und, UsrId asu_n, BBQ bq_n, BBQ bq_o, DevId &dev_id_n, PI64 &loc_id_n, PT &pos_n, PT &p_o, CbString &d_o ) {
//     if ( pos_n >= p_o + d_o.size() )
//         pos_n -= d_o.size();
//     else if ( pos_n > p_o )
//         pos_n = p_o;
// }

// void OtWrapperString::unk_new_or_new_unk( op_set_cursor, op_new_cursor, UsrId asu_n, BBQ bq_n, BBQ bq_o, DevId &dev_id_n, PI64 &loc_id_n, PT &pos_n, DevId &dev_id_o, PI64 &loc_id_o, PT &pos_o ) {
//     // it is impossible to have the same ids
// }

// void OtWrapperString::unk_new_or_new_unk( op_set_cursor, op_set_cursor, UsrId asu_n, BBQ bq_n, BBQ bq_o, DevId &dev_id_n, PI64 &loc_id_n, PT &pos_n, DevId &dev_id_o, PI64 &loc_id_o, PT &pos_o ) {
//     if ( dev_id_o == dev_id_n and loc_id_o == loc_id_n )
//         loc_id_o = 0;
// }

// void OtWrapperString::unk_new_or_new_unk( op_set_cursor, op_set_cursor_und, UsrId asu_n, BBQ bq_n, BBQ bq_o, DevId &dev_id_n, PI64 &loc_id_n, PT &pos_n, DevId &dev_id_o, PI64 &loc_id_o, PT &pos_o, PT &old_pos_o ) {
//     if ( dev_id_o == dev_id_n and loc_id_o == loc_id_n )
//         loc_id_o = 0;
// }

// void OtWrapperString::unk_new_or_new_unk( op_set_cursor, op_rem_cursor, UsrId asu_n, BBQ bq_n, BBQ bq_o, DevId &dev_id_n, PI64 &loc_id_n, PT &pos_n, DevId &dev_id_o, PI64 &loc_id_o, PT &pos_o ) {
//     if ( dev_id_o == dev_id_n and loc_id_o == loc_id_n ) // cursor has been deleted in an unk patch -> do not keep it
//         loc_id_n = 0;
// }

// void OtWrapperString::unk_new_or_new_unk( op_set_cursor, op_rem_cursor_und, UsrId asu_n, BBQ bq_n, BBQ bq_o, DevId &dev_id_n, PI64 &loc_id_n, PT &pos_n, DevId &dev_id_o, PI64 &loc_id_o, PT &old_pos_o, UsrId usr_o ) {
//     if ( dev_id_o == dev_id_n and loc_id_o == loc_id_n ) // cursor has been deleted in an unk patch -> do not keep it
//         loc_id_n = 0;
// }

// void OtWrapperString::unk_new_or_new_unk( op_set_cursor_und, op_insert, UsrId asu_n, BBQ bq_n, BBQ bq_o, DevId &dev_id_n, PI64 &loc_id_n, PT &pos_n, PT &old_pos_n, PT &p_o, CbString &d_o ) {
//     if ( pos_n >= p_o )
//         pos_n += d_o.size();
// }

// void OtWrapperString::unk_new_or_new_unk( op_set_cursor_und, op_remove, UsrId asu_n, BBQ bq_n, BBQ bq_o, DevId &dev_id_n, PI64 &loc_id_n, PT &pos_n, PT &old_pos_n, PT &p_o, PT &l_o ) {
//     if ( pos_n >= p_o + l_o )
//         pos_n -= l_o;
//     else if ( pos_n > p_o )
//         pos_n = p_o;
// }

// void OtWrapperString::unk_new_or_new_unk( op_set_cursor_und, op_remove_und, UsrId asu_n, BBQ bq_n, BBQ bq_o, DevId &dev_id_n, PI64 &loc_id_n, PT &pos_n, PT &old_pos_n, PT &p_o, CbString &d_o ) {
//     if ( pos_n >= p_o + d_o.size() )
//         pos_n -= d_o.size();
//     else if ( pos_n > p_o )
//         pos_n = p_o;
// }

// void OtWrapperString::unk_new_or_new_unk( op_set_cursor_und, op_new_cursor, UsrId asu_n, BBQ bq_n, BBQ bq_o, DevId &dev_id_n, PI64 &loc_id_n, PT &pos_n, PT &old_pos_n, DevId &dev_id_o, PI64 &loc_id_o, PT &pos_o ) {
//     // it is impossible to have the same ids
// }

// void OtWrapperString::unk_new_or_new_unk( op_set_cursor_und, op_set_cursor, UsrId asu_n, BBQ bq_n, BBQ bq_o, DevId &dev_id_n, PI64 &loc_id_n, PT &pos_n, PT &old_pos_n, DevId &dev_id_o, PI64 &loc_id_o, PT &pos_o ) {
//     if ( dev_id_o == dev_id_n and loc_id_o == loc_id_n )
//         loc_id_o = 0;
// }

// void OtWrapperString::unk_new_or_new_unk( op_set_cursor_und, op_set_cursor_und, UsrId asu_n, BBQ bq_n, BBQ bq_o, DevId &dev_id_n, PI64 &loc_id_n, PT &pos_n, PT &old_pos_n, DevId &dev_id_o, PI64 &loc_id_o, PT &pos_o, PT &old_pos_o ) {
//     if ( dev_id_o == dev_id_n and loc_id_o == loc_id_n )
//         loc_id_o = 0;
// }

// void OtWrapperString::unk_new_or_new_unk( op_set_cursor_und, op_rem_cursor, UsrId asu_n, BBQ bq_n, BBQ bq_o, DevId &dev_id_n, PI64 &loc_id_n, PT &pos_n, PT &old_pos_n, DevId &dev_id_o, PI64 &loc_id_o, PT &pos_o ) {
//     if ( dev_id_o == dev_id_n and loc_id_o == loc_id_n ) // cursor has been deleted in an unk patch -> do not keep it
//         loc_id_n = 0;
// }

// void OtWrapperString::unk_new_or_new_unk( op_set_cursor_und, op_rem_cursor_und, UsrId asu_n, BBQ bq_n, BBQ bq_o, DevId &dev_id_n, PI64 &loc_id_n, PT &pos_n, PT &old_pos_n, DevId &dev_id_o, PI64 &loc_id_o, PT &old_pos_o, UsrId usr_o ) {
//     if ( dev_id_o == dev_id_n and loc_id_o == loc_id_n ) // cursor has been deleted in an unk patch -> do not keep it
//         loc_id_n = 0;
// }

// void OtWrapperString::unk_new_or_new_unk( op_rem_cursor, op_new_cursor, UsrId asu_n, BBQ bq_n, BBQ bq_o, DevId &dev_id_n, PI64 &loc_id_n, DevId &dev_id_o, PI64 &loc_id_o, PT &pos_o ) {
//     if ( dev_id_o == dev_id_n and loc_id_o == loc_id_n )
//         loc_id_o = 0;
// }

// void OtWrapperString::unk_new_or_new_unk( op_rem_cursor, op_set_cursor, UsrId asu_n, BBQ bq_n, BBQ bq_o, DevId &dev_id_n, PI64 &loc_id_n, DevId &dev_id_o, PI64 &loc_id_o, PT &pos_o ) {
//     if ( dev_id_o == dev_id_n and loc_id_o == loc_id_n )
//         loc_id_o = 0;
// }

// void OtWrapperString::unk_new_or_new_unk( op_rem_cursor, op_set_cursor_und, UsrId asu_n, BBQ bq_n, BBQ bq_o, DevId &dev_id_n, PI64 &loc_id_n, DevId &dev_id_o, PI64 &loc_id_o, PT &pos_o, PT &old_pos_o ) {
//     if ( dev_id_o == dev_id_n and loc_id_o == loc_id_n )
//         loc_id_o = 0;
// }

// void OtWrapperString::unk_new_or_new_unk( op_rem_cursor, op_rem_cursor, UsrId asu_n, BBQ bq_n, BBQ bq_o, DevId &dev_id_n, PI64 &loc_id_n, DevId &dev_id_o, PI64 &loc_id_o ) {
//     if ( dev_id_o == dev_id_n and loc_id_o == loc_id_n )
//         loc_id_o = 0;
// }

// void OtWrapperString::unk_new_or_new_unk( op_rem_cursor, op_rem_cursor_und, UsrId asu_n, BBQ bq_n, BBQ bq_o, DevId &dev_id_n, PI64 &loc_id_n, DevId &dev_id_o, PI64 &loc_id_o, PT &old_pos_o, UsrId usr_o ) {
//     if ( dev_id_o == dev_id_n and loc_id_o == loc_id_n )
//         loc_id_o = 0;
// }

// void OtWrapperString::unk_new_or_new_unk( op_rem_cursor_und, op_new_cursor, UsrId asu_n, BBQ bq_n, BBQ bq_o, DevId &dev_id_n, PI64 &loc_id_n, PT &old_pos_n, UsrId usr_n, DevId &dev_id_o, PI64 &loc_id_o, PT &pos_o ) {
//     if ( dev_id_o == dev_id_n and loc_id_o == loc_id_n )
//         loc_id_o = 0;
// }

// void OtWrapperString::unk_new_or_new_unk( op_rem_cursor_und, op_set_cursor, UsrId asu_n, BBQ bq_n, BBQ bq_o, DevId &dev_id_n, PI64 &loc_id_n, PT &old_pos_n, UsrId usr_n, DevId &dev_id_o, PI64 &loc_id_o, PT &pos_o ) {
//     if ( dev_id_o == dev_id_n and loc_id_o == loc_id_n )
//         loc_id_o = 0;
// }

// void OtWrapperString::unk_new_or_new_unk( op_rem_cursor_und, op_set_cursor_und, UsrId asu_n, BBQ bq_n, BBQ bq_o, DevId &dev_id_n, PI64 &loc_id_n, PT &old_pos_n, UsrId usr_n, DevId &dev_id_o, PI64 &loc_id_o, PT &pos_o, PT &old_pos_o ) {
//     if ( dev_id_o == dev_id_n and loc_id_o == loc_id_n )
//         loc_id_o = 0;
// }

// void OtWrapperString::unk_new_or_new_unk( op_rem_cursor_und, op_rem_cursor, UsrId asu_n, BBQ bq_n, BBQ bq_o, DevId &dev_id_n, PI64 &loc_id_n, PT &old_pos_n, UsrId usr_n, DevId &dev_id_o, PI64 &loc_id_o ) {
//     if ( dev_id_o == dev_id_n and loc_id_o == loc_id_n )
//         loc_id_o = 0;
// }

// void OtWrapperString::unk_new_or_new_unk( op_rem_cursor_und, op_rem_cursor_und, UsrId asu_n, BBQ bq_n, BBQ bq_o, DevId &dev_id_n, PI64 &loc_id_n, PT &old_pos_n, UsrId usr_n, DevId &dev_id_o, PI64 &loc_id_o, PT &old_pos_o, UsrId usr_o ) {
//     if ( dev_id_o == dev_id_n and loc_id_o == loc_id_n )
//         loc_id_o = 0;
// }
