// import DeclTypesForRW from "../Core/DeclTypesForRW";
// import PatchTypes     from "../Ot/PatchTypes";
//// nsmake global alias ./OtWrapperStringOperations.ts ./(GenOtWrapperStringOperations.ts).ts
import BinaryWriter              from "../System/BinaryWriter";
import BinaryReader              from "../System/BinaryReader";
import UsrId                     from "../System/UsrId";
import DevId                     from "../System/DevId";
import PatchTypes                from "../Core/PatchTypes";
import OtWrapperWithRightFlags   from "../Core/OtWrapperWithRightFlags";
import methods                   from "../Core/methods";
import Rp                        from "../Core/Rp";
import OtWrapperStringOperations from "./OtWrapperStringOperations";
import GenString                 from "./GenString";
import Caret                     from "./Caret";

//
export default
class OtWrapperString extends OtWrapperWithRightFlags {
    constructor( val: GenString, usr = new UsrId ) {
        super( OtWrapperString, usr );
        this.val = val;

        // first patch
        // let str = this.val.toString();
        // if ( str.length )
        //     OtWrapperString.reg_op( this.last_ops, OtWrapperString.op_insert, 0, str );
    }

    prox_ref__b() : Rp {
        return this.val;
    }

    get_patch_type__b( bw: BinaryWriter ) {
        bw.write_PT( PatchTypes.num.OtWrapperString );
    }

    cmp_patch_type__b( br : BinaryReader ) : boolean {
        return br.read_PT() == PatchTypes.num.OtWrapperString;
    }

    _self_insert( pos: number, sup: string, usr_id = new UsrId() ): Rp {
        if ( sup.length && OtWrapperStringOperations.right_to.Insert( this, usr_id, pos, sup ) ) {
            this.val.data = this.val.data.slice( 0, pos ) + sup + this.val.data.slice( pos );
            this.sig_change( bw => OtWrapperStringOperations.bin_repr.Insert( bw, pos, sup ) );
        }
        return this;
    }

    _self_remove( pos: number, len: number, usr_id = new UsrId() ): Rp {
        if ( len && OtWrapperStringOperations.right_to.Remove( this, usr_id, pos, len ) ) {
            const str = this.val.data.slice( pos, pos + len );
            this.val.data = this.val.data.slice( 0, pos ) + this.val.data.slice( pos + len );
            this.sig_change( bw => OtWrapperStringOperations.bin_repr.RemUnd( bw, pos, str ) );
        }
        return this;
    }

    new_patch( res: BinaryWriter, msg: BinaryReader, as_usr: UsrId, cq_unk: BinaryWriter, src_dev?: DevId, src_usr?: UsrId, cur_dev?: DevId, cur_usr?: UsrId ) {
        OtWrapperStringOperations.new_patch( this, res, msg, as_usr, cq_unk, null, src_dev, src_usr, cur_dev, cur_usr );
    }

    undo_patch( data: BinaryReader, as_usr: UsrId ) {
        OtWrapperStringOperations.undo_patch( this, data, as_usr );
    }

    get_possible_rights__b(): Array<string> {
        return OtWrapperStringOperations.get_possible_rights__b();
    }


    // _self_remove( pos : number, len : number, usr_id = new UsrId() ) : boolean {
    //     let str = this.val.val.substr( pos, len );
    //     if ( len && OtWrapperString.op_remove_und.right( this, this.usr_right( usr_id ), pos, str ) ) {
    //         OtWrapperString.reg_op( this.last_ops, OtWrapperString.op_remove_und, pos, str );
    //         OtWrapperString.op_remove_und.apply_op( this, { usr: usr_id, type: 0 }, pos, str );
    //         return true;
    //     }
    //     return false;
    // }

    // _substring( pos : number, len : number ) : string {
    //     return this.val.toString().substr( pos, len );
    // }

    val         : GenString;
    // cursors    = new Array<Cursor>();
    // new_cursors= new Array<Cursor>();
    // mod_cursors= new Array<Cursor>();
    // rem_cursors= new Array<Cursor>();
}
OtWrapperWithRightFlags.make_templ( OtWrapperString );


methods[ "add__sb" ].add_surdef( 2, [ OtWrapperString, "to_String__b" ], function( str: OtWrapperString, sup: Rp ): Rp {
    return str._self_insert( str.val.data.length, methods[ "to_String__b" ].call_1( sup ) );
} );

methods[ "insert__sbb" ].add_surdef( 2, [ OtWrapperString, "to_Number__b", "to_String__b" ], function( str: OtWrapperString, pos: Rp, sup: Rp ): Rp {
    const npos = methods[ "to_Number__b" ].call_1( pos );
    const nsup = methods[ "to_String__b" ].call_1( sup );
    return str._self_insert( npos, nsup );
} );

methods[ "remove__sbb" ].add_surdef( 2, [ OtWrapperString, "to_Number__b", "to_Number__b" ], function( str: OtWrapperString, pos: Rp, len: Rp ): Rp {
    const npos = methods[ "to_Number__b" ].call_1( pos );
    const nlen = methods[ "to_Number__b" ].call_1( len );
    return str._self_remove( npos, nlen );
} );

// // patch types
// static op_insert = {
//     name: "insert",
//     attrs: [ "PT", "String" ],
//     right: function( rp: OtWrapperString, flags : number, pos : number, sup: string ) : boolean {
//         return OtWrapperString.can_insert.ok( flags ) || ( pos == rp.val.length.val && OtWrapperString.can_append.ok( flags ) );
//     },
//     valid: function( rp: OtWrapperString, pos : number, sup: string ) : boolean {
//         return sup.length != 0;
//     },
//     apply_op( rp: OtWrapperString, aod, pos : number, sup: string ) : void {
//         for( let c of rp.cursors ) {
//             for( let caret of c.carets ) {
//                 if ( caret.beg >= pos )
//                     caret.beg += sup.length;
//                 if ( caret.end >= pos )
//                     caret.end += sup.length;
//             }
//         }

//         methods["self_insert_bb"].call_3( rp.val, new Number( pos ), new String( sup ) );
//         rp.sig_change()
//     },
//     undo( rp: OtWrapperString, aod, pos : number, sup: string ) : void {
//         OtWrapperString.op_remove.apply_op( rp, aod, pos, sup.length );
//     },
//     update_patch_data_l0( rp: OtWrapperString, args ) : void {
//     }
// };
// static op_remove = {
//     name: "remove",
//     attrs: [ "PT", "PT" ],
//     right: function( rp: OtWrapperString, flags : number, pos : number, len: number ) : boolean {
//         return OtWrapperString.can_remove.ok( flags );
//     },
//     valid: function( rp: OtWrapperString, pos : number, len: number ) : boolean {
//         return len != 0;
//     },
//     apply_op( rp: OtWrapperString, aod, pos : number, len: number ) : void {
//         for( let c of rp.cursors ) {
//             for( let caret of c.carets ) {
//                 if ( caret.beg >= pos + len )
//                     caret.beg -= len;
//                 else if ( caret.beg > pos )
//                     caret.beg = pos;

//                 if ( caret.end >= pos + len )
//                     caret.end -= len;
//                 else if ( caret.end > pos )
//                     caret.end = pos;
//             }
//         }
//         methods["self_remove_bb"].call_3( rp.val, new Number( pos ), new Number( len ) );
//         rp.sig_change()
//     },
//     undo( rp: OtWrapperString, aod, pos : number, len: number ) : void {
//         throw new Error( "TODO" );
//     },
//     update_patch_data_l0( rp: OtWrapperString, args ) : void {
//     }
// };
// static op_remove_und = {
//     name: "remove_und",
//     attrs: [ "PT", "String" ],
//     right( rp: OtWrapperString, flags : number, pos : number, str: string ) : boolean {
//         return OtWrapperString.can_remove.ok( flags );
//     },
//     valid( rp: OtWrapperString, pos : number, str: string ) : boolean {
//         return str.length != 0;
//     },
//     apply_op( rp: OtWrapperString, aod, pos : number, str: string ) : void {
//         OtWrapperString.op_remove.apply_op( rp, aod, pos, str.length );
//     },
//     undo( rp: OtWrapperString, aod, pos : number, str: string ) : void {
//         OtWrapperString.op_insert.apply_op( rp, aod, pos, str );
//     },
//     update_patch_data_l0( rp: OtWrapperString, args ) : void {
//     }
// };
// static op_new_cursor = {
//     name: "new_cursor",
//     attrs: [ "DevId", "PT", "Array<PT>" ],
//     right: function( rp: OtWrapperString, flags : number, dev_id : DevId, loc_id : number, pos : Array<number> ) : boolean {
//         return OtWrapperString.can_read.ok( flags ); // TODO: something like "only an user can rem its cursors"
//     },
//     valid: function( rp: OtWrapperString, dev_id : DevId, loc_id : number, pos : Array<number> ) : boolean {
//         return loc_id != 0;
//     },
//     apply_op( rp: OtWrapperString, aod, dev_id : DevId, loc_id : number, pos : Array<number> ) : void {
//         let cursor    = new Cursor;
//         cursor.carets = condensed_to_caret_list( pos );
//         cursor.usr_id = aod.as_usr;
//         cursor.dev_id = dev_id;
//         cursor.loc_id = loc_id;
//         rp.cursors.push( cursor );
//         rp.sig_change();
//     },
//     undo( rp: OtWrapperString, aod, dev_id : DevId, loc_id : number, pos : Array<number> ) : void {
//         OtWrapperString.op_rem_cursor.apply_op( rp, aod, dev_id, loc_id );
//     },
//     update_patch_data_l0( rp: OtWrapperString, args ) : void {
//     }
// };
// static op_set_cursor = {
//     name: "set_cursor",
//     attrs: [ "DevId", "PT", "Array<PT>" ],
//     right: function( rp: OtWrapperString, flags : number, dev_id : DevId, loc_id : number, pos : Array<number> ) : boolean {
//         return OtWrapperString.can_read.ok( flags ); // TODO: something like "only an user can rem its cursors"
//     },
//     valid: function( rp: OtWrapperString, dev_id : DevId, loc_id : number, pos : Array<number> ) : boolean {
//         return loc_id != 0;
//     },
//     apply_op( rp: OtWrapperString, aod, dev_id : DevId, loc_id : number, pos : Array<number> ) : void {
//         for( let c of rp.cursors ) {
//             if ( c.dev_id.val == dev_id.val && c.loc_id == loc_id ) {
//                 c.carets = condensed_to_caret_list( pos );
//                 break;
//             }
//         }
//     },
//     undo( rp: OtWrapperString, aod, dev_id : DevId, loc_id : number, pos : Array<number> ) : void {
//         throw new Error( "TODO (a priori impossible to undo)" );
//     },
//     update_patch_data_l0( rp: OtWrapperString, args ) : void {
//     }
// };
// static op_set_cursor_und = {
//     name: "set_cursor_und",
//     attrs: [ "DevId", "PT", "Array<PT>", "Array<PT>" ],
//     right: function( rp: OtWrapperString, flags : number, dev_id : DevId, loc_id : number, pos : Array<number>, old_pos : Array<number> ) : boolean {
//         return OtWrapperString.op_set_cursor.right( rp, flags, dev_id, loc_id, pos );
//     },
//     valid: function( rp: OtWrapperString, flags : number, dev_id : DevId, loc_id : number, pos : Array<number>, old_pos : Array<number> ) : boolean {
//         return loc_id != 0;
//     },
//     apply_op( rp: OtWrapperString, aod, dev_id : DevId, loc_id : number, pos : Array<number>, old_pos : Array<number> ) : void {
//         OtWrapperString.op_set_cursor.apply_op( rp, aod, dev_id, loc_id, pos );
//     },
//     undo( rp: OtWrapperString, aod, dev_id : DevId, loc_id : number, pos : Array<number>, old_pos : Array<number> ) : void {
//         OtWrapperString.op_set_cursor.apply_op( rp, aod, dev_id, loc_id, old_pos );
//     },
//     update_patch_data_l0( rp: OtWrapperString, args ) : void {
//     }
// };
// static op_rem_cursor = {
//     name: "rem_cursor",
//     attrs: [ "DevId", "PT" ],
//     right: function( rp: OtWrapperString, flags : number, dev_id : DevId, loc_id : number ) : boolean {
//         return OtWrapperString.can_read.ok( flags ); // TODO: something like "only an user can rem its cursors"
//     },
//     valid: function( rp: OtWrapperString, dev_id : DevId, loc_id : number ) : boolean {
//         return loc_id != 0;
//     },
//     apply_op( rp: OtWrapperString, aod, dev_id : DevId, loc_id : number ) : void {
//         for( let i = 0; i < rp.cursors.length; ++i ) {
//             if ( rp.cursors[ i ].dev_id.val == dev_id.val && rp.cursors[ i ].loc_id == loc_id ) {
//                 rp.cursors.splice( i, 1 );
//                 rp.sig_change();
//                 return;
//             }
//         }
//     },
//     undo( rp: OtWrapperString, aod, dev_id : DevId, loc_id : number ) : void {
//         throw new Error( "TODO (a priori impossible to undo)" );
//     },
//     update_patch_data_l0( rp: OtWrapperString, args ) : void {
//     }
// };
// static op_rem_cursor_und = {
//     name: "rem_cursor_und",
//     attrs: [ "DevId", "PT", "Array<PT>", "UsrId" ],
//     right: function( rp: OtWrapperString, flags : number, dev_id : DevId, loc_id : number, old_pos : Array<number>, old_usr_id : UsrId ) : boolean {
//         return OtWrapperString.op_rem_cursor.right( rp, flags, dev_id, loc_id );
//     },
//     valid: function( rp: OtWrapperString, dev_id : DevId, loc_id : number, old_pos : Array<number>, old_usr_id : UsrId ) : boolean {
//         return loc_id != 0;
//     },
//     apply_op( rp: OtWrapperString, aod, dev_id : DevId, loc_id : number, old_pos : Array<number>, old_usr_id : UsrId ) : void {
//         OtWrapperString.op_rem_cursor.apply_op( rp, aod, dev_id, loc_id );
//     },
//     undo( rp: OtWrapperString, aod, dev_id : DevId, loc_id : number, old_pos : Array<number>, old_usr_id : UsrId ) : void {
//         OtWrapperString.op_new_cursor.apply_op( rp, { usr: old_usr_id, type: aod.type }, dev_id, loc_id, old_pos );
//     },
//     update_patch_data_l0( rp: OtWrapperString, args ) : void {
//     }
// };

// static op_types = OtWrapper.op_types.concat( [
//     OtWrapperString.op_insert,
//     OtWrapperString.op_remove,
//     OtWrapperString.op_remove_und,
//     OtWrapperString.op_new_cursor,
//     OtWrapperString.op_set_cursor,
//     OtWrapperString.op_set_cursor_und,
//     OtWrapperString.op_rem_cursor,
//     OtWrapperString.op_rem_cursor_und
// ] );

// // right types
// static can_read = {
//     name: "read",
//     flags: 4,
//     ok( flags : number ) : boolean { return ( flags & 4 ) != 0; }
// }
// static can_insert = {
//     name: "insert",
//     flags: 8,
//     ok( flags : number ) : boolean { return ( flags & 8 ) != 0; }
// }
// static can_remove = {
//     name: "remove",
//     flags: 16,
//     ok( flags : number ) : boolean { return ( flags & 16 ) != 0; }
// }
// static can_append = {
//     name: "append",
//     flags: 32,
//     ok( flags : number ) : boolean { return ( flags & 32 ) != 0; }
// }

// static right_types = OtWrapper.right_types.concat( [
//     OtWrapperString.can_read,
//     OtWrapperString.can_insert,
//     OtWrapperString.can_remove,
//     OtWrapperString.can_append
// ] );

// write_to( bw : BinaryWriter, only_for_a_copy : boolean, cur_dev : DevId, cur_usr : UsrId ) {
//     if ( only_for_a_copy ) {
//         bw.write_Variable( this.val, only_for_a_copy, cur_dev, cur_usr );
//     } else {
//         bw.write_PT( DeclTypesForRW.num.OtWrapperString );
//         bw.write_Variable( this.val, only_for_a_copy, cur_dev, cur_usr );
//         this.write_OtWrapper_data_to( bw, only_for_a_copy, cur_dev, cur_usr );
//         bw.write_list( this.cursors, only_for_a_copy, cur_dev, cur_usr );
//         for( let l of [ this.new_cursors, this.mod_cursors, this.rem_cursors ] ) {
//             bw.write_PT( l.length );
//             for( let c of l ) {
//                 bw.write_DevId( c.dev_id, cur_dev );
//                 bw.write_PT   ( c.loc_id );
//             }
//         }
//     }
// }

// static _read_OtWrapperString_data_from( br: BinaryReader, cur_dev : DevId, cur_usr : UsrId ) : String {
//     let val = br.read_Variable( cur_dev, cur_usr );
//     let res = new OtWrapperString( val.rp );
//     OtWrapper.read_OtWrapper_data_from( res, br, cur_dev, cur_usr );
//     res.cursors = br.read_list<Cursor>( Cursor, cur_dev, cur_usr );
//     for( let l of [ res.new_cursors, res.mod_cursors, res.rem_cursors ] ) {
//         for( let n = br.read_PT(); n--; ) {
//             let dev_id = br.read_DevId( cur_dev );
//             let loc_id = br.read_PT   ();
//             for( let c of res.cursors ) {
//                 if ( c.dev_id.equ( dev_id ) && c.loc_id == loc_id ) {
//                     l.push( c );
//                     break;
//                 }
//             }
//         }
//     }
//     return new String( res );
// }

// static _skip_OtWrapperString_data_from( br: BinaryReader ) : void {
//     br.skip_Variable();
//     OtWrapper.skip_OtWrapper_data_from( br );
//     br.skip_list( Cursor );
//     for( let i = 0; i < 3; ++i ) {
//         for( let n = br.read_PT(); n--; ) {
//             br.skip_DevId();
//             br.skip_PT   ();
//         }
//     }
// }

// get_merge_effect( bw : BinaryWriter, siht : OtWrapperString, as_usr : UsrId ) : void {
//     // string
//     let off = this.val.length.val;
//     OtWrapperString.reg_op( bw, OtWrapperString.op_insert, off, siht.val.toString() );

//     // cursors
//     if ( this.cursors.length )
//         for( let c of this.cursors )
//             OtWrapperString.reg_op( bw, OtWrapperString.op_new_cursor, c.dev_id, c.loc_id, caret_to_condensed_list( c.carets ) );
// }

// new_cursor( carets = new Array<Caret>(), usr_id = new UsrId ) : number {
//     if ( OtWrapperString.can_read.ok( this.usr_right( usr_id ) ) ) {
//         let cursor    = new Cursor;
//         cursor.carets = ListUtil.copy_obj( carets );
//         cursor.usr_id = usr_id;
//         cursor.dev_id = new DevId;
//         cursor.loc_id = cur_loc_id += 1;

//         this.new_cursors.push( cursor );
//         this.cursors.push( cursor );
//         this.sig_change();

//         return cursor.loc_id;
//     }
//     return 0;
// }

// rem_cursor( dev_id : DevId, loc_id : number, usr_id = new UsrId ) {
//     if ( OtWrapperString.can_read.ok( this.usr_right( usr_id ) ) ) {
//         for( let i = 0; i < this.cursors.length; i += 1 ) {
//             let c = this.cursors[ i ];
//             if ( c.dev_id.val == dev_id.val && c.loc_id == loc_id ) {
//                 this.cursors.splice( i, 1 );
//                 this.rem_cursors.push( c );
//                 this.sig_change();
//                 break;
//             }
//         }
//     }
// }

// get_cursor( dev_id : DevId, loc_id : number, usr_id = new UsrId ) {
//     if ( OtWrapperString.can_read.ok( this.usr_right( usr_id ) ) )
//         for ( let c of this.cursors )
//             if ( c.dev_id.val == dev_id.val && c.loc_id == loc_id )
//                 return ListUtil.copy_obj( c.carets );
//     return null;
// }

// set_cursor( dev_id : DevId, loc_id : number, carets : Array<Caret>, usr_id = new UsrId ) {
//     if ( OtWrapperString.can_read.ok( this.usr_right( usr_id ) ) ) {
//         for ( let c of this.cursors ) {
//             if ( c.dev_id.val == dev_id.val && c.loc_id == loc_id && ! ListUtil.equ_obj( c.carets, carets ) ) {
//                 ListUtil.push_unique( this.mod_cursors, c )
//                 c.carets = ListUtil.copy_obj( carets );
//                 this.sig_change();
//                 break;
//             }
//         }
//     }
// }

// get_cursor_ids( usr_id = new UsrId ) {
//     let res = [];
//     if ( OtWrapperString.can_read.ok( this.usr_right( usr_id ) ) )
//         for( let c of this.cursors )
//             res.push( { dev_id: c.dev_id, loc_id: c.loc_id} );
//     return res;
// }

// freeze_last_changes() {
//     if ( this.mod_cursors.length ) {
//         for( let c of this.mod_cursors ) {
//             if ( this.new_cursors.indexOf( c ) >= 0 || this.rem_cursors.indexOf( c ) >= 0 || ListUtil.equ_obj( c.old_carets, c.carets ) )
//                 continue;
//             OtWrapperString.reg_op( this.last_ops, OtWrapperString.op_set_cursor_und, c.dev_id, c.loc_id, caret_to_condensed_list( c.carets ), caret_to_condensed_list( c.old_carets ) );
//             c.old_carets = ListUtil.copy_obj( c.carets );
//         }
//         this.mod_cursors = [];
//     }
//     if ( this.new_cursors.length ) {
//         for( let c of this.new_cursors ) {
//             if ( this.rem_cursors.indexOf( c ) >= 0 )
//                 continue;
//             OtWrapperString.reg_op( this.last_ops, OtWrapperString.op_new_cursor, c.dev_id, c.loc_id, caret_to_condensed_list( c.carets ) );
//             c.old_carets = ListUtil.copy_obj( c.carets );
//         }
//         this.new_cursors = [];
//     }
//     if ( this.rem_cursors.length ) {
//         for( let c of this.rem_cursors )
//             OtWrapperString.reg_op( this.last_ops, OtWrapperString.op_rem_cursor_und, c.dev_id, c.loc_id, caret_to_condensed_list( c.carets ), c.usr_id );
//         this.rem_cursors = [];
//     }
// }

// function caret_to_condensed_list( l : Array<Caret> ) : Array<number> {
//     let res = Array<number>();
//     for( let c of l ) {
//         res.push( c.beg );
//         res.push( c.end );
//         res.push( c.foc );
//     }
//     return res;
// }

// function condensed_to_caret_list( l : Array<number> ) : Array<Caret> {
//     let res = Array<Caret>();
//     for( let i = 0; i < l.length - 2; i += 3 ) {
//         let c = new Caret;
//         c.beg = l[ i + 0 ];
//         c.end = l[ i + 1 ];
//         c.foc = l[ i + 2 ];
//         res.push( c );
//     }
//     return res;
// }

// //
// class Cursor {
//     constructor() {
//         this.old_carets = new Array<Caret>();
//         this.carets     = new Array<Caret>();
//         this.loc_id     = 0;
//         this.dev_id     = new DevId;
//         this.usr_id     = new UsrId;
//     }
//     write_to( bw : BinaryWriter, only_for_a_copy : boolean, cur_dev : DevId, cur_usr : UsrId ) {
//         bw.write_list ( this.carets, only_for_a_copy, cur_dev, cur_usr );
//         bw.write_PT   ( this.loc_id );
//         bw.write_DevId( this.dev_id, cur_dev );
//         bw.write_UsrId( this.usr_id, cur_dev, cur_usr );
//     }
//     static read_from( br: BinaryReader, cur_dev : DevId, cur_usr : UsrId ) : Cursor {
//         let res    = new Cursor();
//         res.carets = br.read_list<Caret>( Caret, cur_dev, cur_usr );
//         res.loc_id = br.read_PT();
//         res.dev_id = br.read_DevId( cur_dev );
//         res.usr_id = br.read_UsrId( cur_dev, cur_usr );
//         return res;
//     }
//     static skip_from( br: BinaryReader ) : void {
//         br.skip_list ( Caret );
//         br.skip_PT   ();
//         br.skip_DevId();
//         br.skip_UsrId();
//     }
//     old_carets: Array<Caret>; // old position in the text
//     carets    : Array<Caret>; // position in the text
//     loc_id    : number; // id of the cursor (to be used with dev_id)
//     dev_id    : DevId; // from which dev
//     usr_id    : UsrId; // from which usr
// }

// var cur_loc_id = 0;

// DeclTypesForRW.readers[ DeclTypesForRW.num.OtWrapperString ] = OtWrapperString._read_OtWrapperString_data_from;

// PatchTypes.readers[ PatchTypes.num.OtWrapperString_GenString ] = function( br : BinaryReader ) : String {
//     return new String( new OtWrapperString( new GenString( "" ) ) );
// }

// // unk_new_or_new_unk
// OtWrapperString.reg_nuf( OtWrapperString.op_insert, OtWrapperString.op_insert, function( asu_n : UsrId, bq_n : BinaryWriter, bq_o : BinaryWriter, args_n, args_o ) {
//     if ( args_o[ 0 ] > args_n[ 0 ] ) {
//         // orig 01234
//         // real 0123unk4    INS(args_o[ 0 ]=4,args_o[ 1 ]=unk)
//         // imag 0new1234    INS(args_n[ 0 ]=1,args_n[ 1 ]=new)
//         // obj  0new123unk4 (real -> obj = new: INS 1,new; imag -> obj = unk: INS 7,unk)
//         args_o[ 0 ] += args_n[ 1 ].length;
//     } else {
//         // orig 01234
//         // real 0123unk4    INS(args_o[ 0 ]=4,args_o[ 1 ]=unk)
//         // imag 0new1234    INS(args_n[ 0 ]=1,args_n[ 1 ]=new)
//         // obj  0new123unk4 (real -> obj = new: INS 1,new; imag -> obj = unk: INS 7,unk)
//         args_n[ 0 ] += args_o[ 1 ].length;
//     }
// } );

// OtWrapperString.reg_nuf( OtWrapperString.op_insert, OtWrapperString.op_remove, function( asu_n : UsrId, bq_n : BinaryWriter, bq_o : BinaryWriter, args_n, args_o ) {
//     if ( args_n[ 0 ] >= args_o[ 0 ] + args_o[ 1 ] ) {
//         // orig 012345
//         // real 0345      REM(args_o[ 0 ]=1,args_o[ 1 ]=2)
//         // imag 01234new5 INS(args_n[ 0 ]=5,args_n[ 1 ]=new)
//         // obj  034new5   (real -> obj = new: INS 3,new; imag -> obj = unk: REM 1,2)
//         args_n[ 0 ] -= args_o[ 1 ]; // 3
//     } else if ( args_n[ 0 ] <= args_o[ 0 ] ) {
//         // orig 012345
//         // real 0125      REM(args_o[ 0 ]=3,args_o[ 1 ]=2)
//         // imag 0new12345 INS(args_n[ 0 ]=1,args_n[ 1 ]=new)
//         // obj  0new125   (real -> obj = new: INS 1,new; imag -> obj = unk: REM 6,2)
//         args_o[ 0 ] += args_n[ 1 ].length;
//     } else {
//         // orig 012345
//         // real 05        REM(args_o[ 0 ]=1,args_o[ 1 ]=4)
//         // imag 012new345 INS(args_n[ 0 ]=3,args_n[ 1 ]=new)
//         // obj  0new5     (real -> obj = new: INS 1,new; imag -> obj = unk: REM 1,2 + REM 4,2)
//         OtWrapperString.reg_op( bq_o, OtWrapperString.op_remove, args_o[ 0 ], args_n[ 0 ] - args_o[ 0 ] ); // 1, 2
//         args_o[ 1 ] -= args_n[ 0 ] - args_o[ 0 ];  // 2
//         args_n[ 0 ] = args_o[ 0 ];                 // 1
//         args_o[ 0 ] += args_n[ 1 ].length;         // 4
//     }
// } );


// OtWrapperString.reg_nuf( OtWrapperString.op_insert, OtWrapperString.op_remove_und, function( asu_n : UsrId, bq_n : BinaryWriter, bq_o : BinaryWriter, args_n, args_o ) {
//     if ( args_n[ 0 ] >= args_o[ 0 ] + args_o[ 1 ].length ) {
//         // orig 012345
//         // real 0345      REM(args_o[ 0 ]=1,args_o[ 1 ]=2)
//         // imag 01234new5 INS(args_n[ 0 ]=5,args_n[ 1 ]=new)
//         // obj  034new5   (real -> obj = new: INS 3,new; imag -> obj = unk: REM 1,2)
//         args_n[ 0 ] -= args_o[ 1 ].length; // 3
//     } else if ( args_n[ 0 ] <= args_o[ 0 ] ) {
//         // orig 012345
//         // real 0125      REM(args_o[ 0 ]=3,args_o[ 1 ]=2)
//         // imag 0new12345 INS(args_n[ 0 ]=1,args_n[ 1 ]=new)
//         // obj  0new125   (real -> obj = new: INS 1,new; imag -> obj = unk: REM 6,2)
//         args_o[ 0 ] += args_n[ 1 ].length;
//     } else {
//         // orig 012345
//         // real 05        REM(args_o[ 0 ]=1,args_o[ 1 ]=4)
//         // imag 012new345 INS(args_n[ 0 ]=3,args_n[ 1 ]=new)
//         // obj  0new5     (real -> obj = new: INS 1,new; imag -> obj = unk: REM 1,'01' + REM 4,'34')
//         OtWrapperString.reg_op( bq_o, OtWrapperString.op_remove_und, args_o[ 0 ], args_o[ 1 ].slice( 0, args_n[ 0 ] - args_o[ 0 ] ) ); // 1, '12'
//         args_o[ 1 ] = args_o[ 1 ].substr( args_n[ 0 ] - args_o[ 0 ] );  // '34'
//         args_n[ 0 ] = args_o[ 0 ];                                      // 1
//         args_o[ 0 ] += args_n[ 1 ].length;                              // 4
//     }
// } );

// OtWrapperString.reg_nuf( OtWrapperString.op_insert, OtWrapperString.op_new_cursor, function( asu_n : UsrId, bq_n : BinaryWriter, bq_o : BinaryWriter, args_n, args_o ) {
//     for( let i = 0; i < args_o[ 2 ].length - 2; i += 3 )
//         for( let j = i; j < i + 2; ++j )
//             if ( args_o[ 2 ][ j ] >= args_n[ 0 ] )
//                 args_o[ 2 ][ j ] += args_n[ 1 ].length;
// } );

// OtWrapperString.reg_nuf( OtWrapperString.op_insert, OtWrapperString.op_set_cursor, function( asu_n : UsrId, bq_n : BinaryWriter, bq_o : BinaryWriter, args_n, args_o ) {
//     for( let i = 0; i < args_o[ 2 ].length - 2; i += 3 )
//         for( let j = i; j < i + 2; ++j )
//             if ( args_o[ 2 ][ j ] >= args_n[ 0 ] )
//                 args_o[ 2 ][ j ] += args_n[ 1 ].length;
// } );

// OtWrapperString.reg_nuf( OtWrapperString.op_insert, OtWrapperString.op_set_cursor_und, function( asu_n : UsrId, bq_n : BinaryWriter, bq_o : BinaryWriter, args_n, args_o ) {
//     for( let i = 0; i < args_o[ 2 ].length - 2; i += 3 )
//         for( let j = i; j < i + 2; ++j )
//             if ( args_o[ 2 ][ j ] >= args_n[ 0 ] )
//                 args_o[ 2 ][ j ] += args_n[ 1 ].length;
// } );

// OtWrapperString.reg_nuf( OtWrapperString.op_remove, OtWrapperString.op_insert, function( asu_n : UsrId, bq_n : BinaryWriter, bq_o : BinaryWriter, args_n, args_o ) {
//     if ( args_o[ 0 ] <= args_n[ 0 ] ) {
//         // orig 01234567
//         // real 01ab234567 INS(args_o[ 0 ]=2,args_o[ 1 ]=ab)
//         // imag 012347     REM(args_n[ 0 ]=5,args_n[ 1 ]=2)
//         // obj  01ab2347   (real -> obj = new: REM 7,2; imag -> obj = unk: INS 2,ab)
//         args_n[ 0 ] += args_o[ 1 ].length;
//     } else if ( args_o[ 0 ] >= args_n[ 0 ] + args_n[ 1 ] ) {
//         // orig 01234567
//         // real 01234ab567 INS(args_o[ 0 ]=5,args_o[ 1 ]=ab)
//         // imag 014567     REM(args_n[ 0 ]=2,args_n[ 1 ]=2)
//         // obj  014ab567   (real -> obj = new: REM 2,2; imag -> obj = unk: INS 3,ab)
//         args_o[ 0 ] -= args_n[ 1 ];
//     } else {
//         // orig 01234567
//         // real 0123ab4567 INS(args_o[ 0 ]=4,args_o[ 1 ]=ab)
//         // imag 0167       REM(args_n[ 0 ]=2,args_n[ 1 ]=4)
//         // obj  01ab67     (real -> obj = new: REM 2,2 + REM 4,2; imag -> obj = unk: INS 2,ab)
//         OtWrapperString.reg_op( bq_n, OtWrapperString.op_remove_und, args_n[ 0 ], <OtWrapperString>this._substring( args_n[ 0 ], args_o[ 0 ] - args_n[ 0 ] ) ); // 2, 2
//         OtWrapperString.op_remove.apply_op( <OtWrapperString>this, { usr: asu_n }, args_n[ 0 ], args_o[ 0 ] - args_n[ 0 ] );
//         args_n[ 1 ] -= args_o[ 0 ] - args_n[ 0 ]; // 2
//         args_o[ 0 ] = args_n[ 0 ];                // 2
//         args_n[ 0 ] += args_o[ 1 ].length;        // 4
//     }
// } );

// OtWrapperString.reg_nuf( OtWrapperString.op_remove, OtWrapperString.op_remove, function( asu_n : UsrId, bq_n : BinaryWriter, bq_o : BinaryWriter, args_n, args_o ) {
//     if ( args_o[ 0 ] <= args_n[ 0 ] ) {
//         if ( args_o[ 0 ] + args_o[ 1 ] <= args_n[ 0 ] ) {
//             // orig 0123456789
//             // real 0456789    REM(args_o[ 0 ]=1,args_o[ 1 ]=3)
//             // imag 01234589   REM(args_n[ 0 ]=6,args_n[ 1 ]=2)
//             // obj  04589      (real -> obj = new: REM 3,2; imag -> obj = unk: REM 1,3)
//             args_n[ 0 ] -= args_o[ 1 ]; // 3
//         } else if ( args_o[ 0 ] + args_o[ 1 ] <= args_n[ 0 ] + args_n[ 1 ] ) {
//             // orig 0123456789
//             // real 06789      REM(args_o[ 0 ]=1,args_o[ 1 ]=5)
//             // imag 012389     REM(args_n[ 0 ]=4,args_n[ 1 ]=4)
//             // obj  089        (real -> obj = new: REM 1,2; imag -> obj = unk: REM 1,3)
//             let d = args_o[ 0 ] + args_o[ 1 ] - args_n[ 0 ]; // 2
//             args_n[ 1 ] -= d;  // 2
//             args_o[ 1 ] -= d;  // 3
//             args_n[ 0 ] = args_o[ 0 ]; // 1
//         } else {
//             // orig 0123456789
//             // real 09         REM(args_o[ 0 ]=1,args_o[ 1 ]=8)
//             // imag 01236789   REM(args_n[ 0 ]=4,args_n[ 1 ]=2)
//             // obj  09         (real -> obj = new: VOID; imag -> obj = unk: REM 1,6)
//             args_o[ 1 ] -= args_n[ 1 ];
//             args_n[ 1 ] = 0;
//         }
//     } else {
//         if ( args_n[ 0 ] + args_n[ 1 ] <= args_o[ 0 ] ) {
//             // orig 0123456789
//             // real 01234589   REM(args_o[ 0 ]=6,args_o[ 1 ]=2)
//             // imag 0456789    REM(args_n[ 0 ]=1,args_n[ 1 ]=3)
//             // obj  04589      (real -> obj = new: REM 1,3; imag -> obj = unk: REM 3,2)
//             args_o[ 0 ] -= args_n[ 1 ]; // 3
//         } else if ( args_n[ 0 ] + args_n[ 1 ] <= args_o[ 0 ] + args_o[ 1 ] ) {
//             // orig 0123456789
//             // real 012389     REM(args_o[ 0 ]=4,args_o[ 1 ]=4)
//             // imag 06789      REM(args_n[ 0 ]=1,args_n[ 1 ]=5)
//             // obj  089        (real -> obj = new: REM 1,3; imag -> obj = unk: REM 1,2)
//             let d = args_n[ 0 ] + args_n[ 1 ] - args_o[ 0 ]; // 2
//             args_o[ 1 ] -= d;  // 2
//             args_n[ 1 ] -= d;  // 3
//             args_o[ 0 ] = args_n[ 0 ]; // 1
//         } else {
//             // orig 0123456789
//             // real 01236789   REM(args_o[ 0 ]=4,args_o[ 1 ]=2)
//             // imag 09         REM(args_n[ 0 ]=1,args_n[ 1 ]=8)
//             // obj  09         (real -> obj = new: REM 1,6; imag -> obj = unk: VOID)
//             args_n[ 1 ] -= args_o[ 1 ];
//             args_o[ 1 ] = 0;
//         }
//     }
// } );

// OtWrapperString.reg_nuf( OtWrapperString.op_remove, OtWrapperString.op_remove_und, function( asu_n : UsrId, bq_n : BinaryWriter, bq_o : BinaryWriter, args_n, args_o ) {
//     if ( args_o[ 0 ] <= args_n[ 0 ] ) {
//         if ( args_o[ 0 ] + args_o[ 1 ].length <= args_n[ 0 ] ) {
//             // orig 0123456789
//             // real 0456789    REM(args_o[ 0 ]=1,args_o[ 1 ]='123')
//             // imag 01234589   REM(args_n[ 0 ]=6,args_n[ 1 ]=2)
//             // obj  04589      (real -> obj = new: REM 3,2; imag -> obj = unk: REM_UND 1,'123')
//             args_n[ 0 ] -= args_o[ 1 ].length; // 3
//         } else if ( args_o[ 0 ] + args_o[ 1 ].length <= args_n[ 0 ] + args_n[ 1 ] ) {
//             // orig 0123456789
//             // real 06789      REM_UND(args_o[ 0 ]=1,args_o[ 1 ]='12345')
//             // imag 012389     REM(args_n[ 0 ]=4,args_n[ 1 ]=4)
//             // obj  089        (real -> obj = new: REM 1,2; imag -> obj = unk: REM_UND 1,'123')
//             let d = args_o[ 0 ] + args_o[ 1 ].length - args_n[ 0 ]; // 2
//             args_o[ 1 ] = args_o[ 1 ].slice( 0, args_o[ 1 ].length - d ); // '123'
//             args_n[ 1 ] -= d;  // 2
//             args_n[ 0 ] = args_o[ 0 ]; // 1
//         } else {
//             // orig 0123456789
//             // real 09         REM_UND(args_o[ 0 ]=1,args_o[ 1 ]='12345678')
//             // imag 01236789   REM(args_n[ 0 ]=4,args_n[ 1 ]=2)
//             // obj  09         (real -> obj = new: VOID; imag -> obj = unk: REM_UND 1,'123678')
//             let t = args_o[ 1 ].substr( 0, args_n[ 0 ] - args_o[ 0 ] ) +
//                     args_o[ 1 ].substr( args_n[ 0 ] - args_o[ 0 ] + args_n[ 1 ] );
//             args_o[ 1 ] = t;
//             args_n[ 1 ] = 0;
//         }
//     } else {
//         if ( args_n[ 0 ] + args_n[ 1 ] <= args_o[ 0 ] ) {
//             // orig 0123456789
//             // real 01234589   REM_UND(args_o[ 0 ]=6,args_o[ 1 ]='67')
//             // imag 0456789    REM(args_n[ 0 ]=1,args_n[ 1 ]=3)
//             // obj  04589      (real -> obj = new: REM 1,3; imag -> obj = unk: REM_UND 3,'67')
//             args_o[ 0 ] -= args_n[ 1 ]; // 3
//         } else if ( args_n[ 0 ] + args_n[ 1 ] <= args_o[ 0 ] + args_o[ 1 ].length ) {
//             // orig 0123456789
//             // real 012389     REM_UND(args_o[ 0 ]=4,args_o[ 1 ]='4567')
//             // imag 06789      REM(args_n[ 0 ]=1,args_n[ 1 ]=5)
//             // obj  089        (real -> obj = new: REM 1,3; imag -> obj = unk: REM_UND 1,'67')
//             let d = args_n[ 0 ] + args_n[ 1 ] - args_o[ 0 ]; // 2
//             args_o[ 1 ] = args_o[ 1 ].substr( d ); // '67'
//             args_n[ 1 ] -= d;  // 3
//             args_o[ 0 ] = args_n[ 0 ]; // 1
//         } else {
//             // orig 0123456789
//             // real 01236789   REM(args_o[ 0 ]=4,args_o[ 1 ].length=2)
//             // imag 09         REM(args_n[ 0 ]=1,args_n[ 1 ]=8)
//             // obj  09         (real -> obj = new: REM 1,6; imag -> obj = unk: VOID)
//             args_n[ 1 ] -= args_o[ 1 ].length;
//             args_o[ 1 ] = "";
//         }
//     }
// } );

// OtWrapperString.reg_nuf( OtWrapperString.op_remove, OtWrapperString.op_new_cursor, function( asu_n : UsrId, bq_n : BinaryWriter, bq_o : BinaryWriter, args_n, args_o ) {
//     for( let i = 0; i < args_o[ 2 ].length - 2; i += 3 ) {
//         for( let j = i; j < i + 2; ++j ) {
//             if ( args_o[ 2 ][ j ] >= args_n[ 0 ] + args_n[ 1 ] )
//                 args_o[ 2 ][ j ] -= args_n[ 1 ];
//             else if ( args_o[ 2 ][ j ] > args_n[ 0 ] )
//                 args_o[ 2 ][ j ] = args_n[ 0 ];
//         }
//     }
// } );

// OtWrapperString.reg_nuf( OtWrapperString.op_remove, OtWrapperString.op_set_cursor, function( asu_n : UsrId, bq_n : BinaryWriter, bq_o : BinaryWriter, args_n, args_o ) {
//     for( let i = 0; i < args_o[ 2 ].length - 2; i += 3 ) {
//         for( let j = i; j < i + 2; ++j ) {
//             if ( args_o[ 2 ][ j ] >= args_n[ 0 ] + args_n[ 1 ] )
//                 args_o[ 2 ][ j ] -= args_n[ 1 ];
//             else if ( args_o[ 2 ][ j ] > args_n[ 0 ] )
//                 args_o[ 2 ][ j ] = args_n[ 0 ];
//             }
//         }
// } );

// OtWrapperString.reg_nuf( OtWrapperString.op_remove, OtWrapperString.op_set_cursor_und, function( asu_n : UsrId, bq_n : BinaryWriter, bq_o : BinaryWriter, args_n, args_o ) {
//     for( let i = 0; i < args_o[ 2 ].length - 2; i += 3 ) {
//         for( let j = i; j < i + 2; ++j ) {
//             if ( args_o[ 2 ][ j ] >= args_n[ 0 ] + args_n[ 1 ] )
//                 args_o[ 2 ][ j ] -= args_n[ 1 ];
//             else if ( args_o[ 2 ][ j ] > args_n[ 0 ] )
//                 args_o[ 2 ][ j ] = args_n[ 0 ];
//             }
//         }
// } );

// OtWrapperString.reg_nuf( OtWrapperString.op_remove_und, OtWrapperString.op_insert, function( asu_n : UsrId, bq_n : BinaryWriter, bq_o : BinaryWriter, args_n, args_o ) {
//     if ( args_o[ 0 ] <= args_n[ 0 ] ) {
//         // orig 01234567
//         // real 01ab234567 INS(args_o[ 0 ]=2,args_o[ 1 ]=ab)
//         // imag 012347     REM(args_n[ 0 ]=5,args_n[ 1 ]=2)
//         // obj  01ab2347   (real -> obj = new: REM 7,2; imag -> obj = unk: INS 2,ab)
//         args_n[ 0 ] += args_o[ 1 ].length;
//     } else if ( args_o[ 0 ] >= args_n[ 0 ] + args_n[ 1 ].length ) {
//         // orig 01234567
//         // real 01234ab567 INS(args_o[ 0 ]=5,args_o[ 1 ]=ab)
//         // imag 014567     REM(args_n[ 0 ]=2,args_n[ 1 ]=2)
//         // obj  014ab567   (real -> obj = new: REM 2,2; imag -> obj = unk: INS 3,ab)
//         args_o[ 0 ] -= args_n[ 1 ].length;
//     } else {
//         // orig 01234567
//         // real 0123ab4567 INS(p_1=4,d_1=ab)
//         // imag 0167       REM_UND(p_0=2,d_0=2345)
//         // obj  01ab67     (real -> obj = unk: REM_UND 2,23 + REM_UND 4,45; imag -> obj = new: INS 2,ab)
//         OtWrapperString.reg_op( bq_n, OtWrapperString.op_remove_und, args_n[ 0 ], args_n[ 1 ].substr( 0, args_o[ 0 ] - args_n[ 0 ] ) ); // 2, 23
//         OtWrapperString.op_remove.apply_op( this, { usr: asu_n }, args_n[ 0 ], args_o[ 0 ] - args_n[ 0 ] );
//         args_n[ 1 ] = args_n[ 1 ].substr( args_o[ 0 ] - args_n[ 0 ] ); // 45
//         args_o[ 0 ] = args_n[ 0 ];                                     // 2
//         args_n[ 0 ] += args_o[ 1 ].length;                             // 4
//     }
// } );

// OtWrapperString.reg_nuf( OtWrapperString.op_remove_und, OtWrapperString.op_remove, function( asu_n : UsrId, bq_n : BinaryWriter, bq_o : BinaryWriter, args_n, args_o ) {
//     if ( args_o[ 0 ] <= args_n[ 0 ] ) {
//         if ( args_o[ 0 ] + args_o[ 1 ] <= args_n[ 0 ] ) {
//             // orig 0123456789
//             // real 0456789    REM(args_o[ 0 ]=1,args_o[ 1 ]=3)
//             // imag 01234589   REM_UND(args_n[ 0 ]=6,args_n[ 1 ]='67')
//             // obj  04589      (real -> obj = new: REM 3,2; imag -> obj = _n: REM 1,3)
//             args_n[ 0 ] -= args_o[ 1 ]; // 3
//         } else if ( args_o[ 0 ] + args_o[ 1 ] <= args_n[ 0 ] + args_n[ 1 ].length ) {
//             // orig 0123456789
//             // real 06789      REM(args_o[ 0 ]=1,args_o[ 1 ]=5)
//             // imag 012389     REM_UND(args_n[ 0 ]=4,args_n[ 1 ]='4567')
//             // obj  089        (real -> obj = _n: REM_UND 1,'67'; imag -> obj = _o: REM_UND 1,'123')
//             let d = args_o[ 0 ] + args_o[ 1 ] - args_n[ 0 ]; // 2
//             args_o[ 1 ] -= d; // '123'
//             args_n[ 1 ] = args_n[ 1 ].substr( d );  // '67'
//             args_n[ 0 ] = args_o[ 0 ]; // 1
//         } else {
//             // orig 0123456789
//             // real 09         REM(args_o[ 0 ]=1,args_o[ 1 ]=8)
//             // imag 01236789   REM_UND(args_n[ 0 ]=4,args_n[ 1 ]='45')
//             // obj  09         (real -> obj = _n: VOID; imag -> obj = _n: REM 1,'123678')
//             args_o[ 1 ] -= args_n[ 1 ].length;
//             args_n[ 1 ].free();
//         }
//     } else {
//         if ( args_n[ 0 ] + args_n[ 1 ].length <= args_o[ 0 ] ) {
//             // orig 0123456789
//             // real 01234589   REM(args_o[ 0 ]=6,args_o[ 1 ]=2)
//             // imag 0456789    REM_UND(args_n[ 0 ]=1,args_n[ 1 ]='123')
//             // obj  04589      (real -> obj = _n: REM 1,'123'; imag -> obj = _n: REM 3,2)
//             args_o[ 0 ] -= args_n[ 1 ].length; // 3
//         } else if ( args_n[ 0 ] + args_n[ 1 ].length <= args_o[ 0 ] + args_o[ 1 ] ) {
//             // orig 0123456789
//             // real 012389     REM(args_o[ 0 ]=4,args_o[ 1 ]=4)
//             // imag 06789      REM_UND(args_n[ 0 ]=1,args_n[ 1 ]='12345')
//             // obj  089        (real -> obj = _n: REM 1,'123'; imag -> obj = _n: REM 1,2)
//             let d = args_n[ 0 ] + args_n[ 1 ].length - args_o[ 0 ]; // 2
//             args_o[ 1 ] -= d;  // 2
//             args_n[ 1 ] = args_n[ 1 ].slice( 0, args_n[ 1 ].length - d );  // '123'
//             args_o[ 0 ] = args_n[ 0 ]; // 1
//         } else {
//             // orig 0123456789
//             // real 01236789   REM(args_o[ 0 ]=4,args_o[ 1 ]=2)
//             // imag 09         REM_UND(args_n[ 0 ]=1,args_n[ 1 ]='12345678')
//             // obj  09         (real -> obj = _n: REM 1,'123678'; imag -> obj = _n: VOID)
//             let t = args_n[ 1 ].substr( 0, args_o[ 0 ] - args_n[ 0 ] ) +
//                     args_n[ 1 ].substr( args_o[ 0 ] - args_n[ 0 ] + args_o[ 1 ] );
//             args_n[ 1 ] = t;
//             args_o[ 1 ] = 0;
//         }
//     }
// } );

// OtWrapperString.reg_nuf( OtWrapperString.op_remove_und, OtWrapperString.op_remove_und, function( asu_n : UsrId, bq_n : BinaryWriter, bq_o : BinaryWriter, args_n, args_o ) {
//     if ( args_o[ 0 ] <= args_n[ 0 ] ) {
//         if ( args_o[ 0 ] + args_o[ 1 ].length <= args_n[ 0 ] ) {
//             // orig 0123456789
//             // real 0456789    REM(args_o[ 0 ]=1,args_o[ 1 ]='123')
//             // imag 01234589   REM(args_n[ 0 ]=6,args_n[ 1 ]='67')
//             // obj  04589      (real -> obj = _n: REM_UND 3,2; imag -> obj = _o: REM_UND 1,'123')
//             args_n[ 0 ] -= args_o[ 1 ].length; // 3
//         } else if ( args_o[ 0 ] + args_o[ 1 ].length <= args_n[ 0 ] + args_n[ 1 ].length ) {
//             // orig 0123456789
//             // real 06789      REM_UND(args_o[ 0 ]=1,args_o[ 1 ]='12345')
//             // imag 012389     REM(args_n[ 0 ]=4,args_n[ 1 ]='4567')
//             // obj  089        (real -> obj = _n: REM_UND 1,'67'; imag -> obj = _o: REM_UND 1,'123')
//             let d = args_o[ 0 ] + args_o[ 1 ].length - args_n[ 0 ]; // 2
//             args_o[ 1 ] = args_o[ 1 ].substr( 0, args_o[ 1 ].length - d ); // '123'
//             args_n[ 1 ] = args_n[ 1 ].substr( d );  // '67'
//             args_n[ 0 ] = args_o[ 0 ]; // 1
//         } else {
//             // orig 0123456789
//             // real 09         REM_UND(args_o[ 0 ]=1,args_o[ 1 ]='12345678')
//             // imag 01236789   REM(args_n[ 0 ]=4,args_n[ 1 ].length=2)
//             // obj  09         (real -> obj = _n: VOID; imag -> obj = _o: REM_UND 1,'123678')
//             let t = args_o[ 1 ].substr( 0, args_n[ 0 ] - args_o[ 0 ] ) +
//                     args_o[ 1 ].substr( args_n[ 0 ] - args_o[ 0 ] + args_n[ 1 ].length );
//             args_o[ 1 ] = t;
//             args_n[ 1 ] = "";
//         }
//     } else {
//         if ( args_n[ 0 ] + args_n[ 1 ].length <= args_o[ 0 ] ) {
//             // orig 0123456789
//             // real 01234589   REM_UND(args_o[ 0 ]=6,args_o[ 1 ]='67')
//             // imag 0456789    REM(args_n[ 0 ]=1,args_n[ 1 ].length='123')
//             // obj  04589      (real -> obj = _n: REM_UND 1,'123'; imag -> obj = _o: REM_UND 3,'67')
//             args_o[ 0 ] -= args_n[ 1 ].length; // 3
//         } else if ( args_n[ 0 ] + args_n[ 1 ].length <= args_o[ 0 ] + args_o[ 1 ].length ) {
//             // orig 0123456789
//             // real 012389     REM_UND(args_o[ 0 ]=4,args_o[ 1 ]='4567')
//             // imag 06789      REM(args_n[ 0 ]=1,args_n[ 1 ]='12345')
//             // obj  089        (real -> obj = _n: REM_UND 1,'123'; imag -> obj = _o: REM_UND 1,'67')
//             let d = args_n[ 0 ] + args_n[ 1 ].length - args_o[ 0 ]; // 2
//             args_o[ 1 ] = args_o[ 1 ].substr( d ); // '67'
//             args_n[ 1 ] = args_n[ 1 ].substr( 0, args_n[ 1 ].length - d ); // '123'
//             args_o[ 0 ] = args_n[ 0 ]; // 1
//         } else {
//             // orig 0123456789
//             // real 01236789   REM(args_o[ 0 ]=4,args_o[ 1 ]='45')
//             // imag 09         REM(args_n[ 0 ]=1,args_n[ 1 ]='12345678')
//             // obj  09         (real -> obj = _n: REM_UND 1,'123678'; imag -> obj = _o: VOID)
//             let t = args_n[ 1 ].substr( 0, args_o[ 0 ] - args_n[ 0 ] ) +
//                     args_n[ 1 ].substr( args_o[ 0 ] - args_n[ 0 ] + args_o[ 1 ].length ); // p_o - p_n + d_o.length
//             args_n[ 1 ] = t;
//             args_o[ 1 ] = "";
//         }
//     }
// } );

// OtWrapperString.reg_nuf( OtWrapperString.op_remove_und, OtWrapperString.op_new_cursor, function( asu_n : UsrId, bq_n : BinaryWriter, bq_o : BinaryWriter, args_n, args_o ) {
//     for( let i = 0; i < args_o[ 2 ].length - 2; i += 3 ) {
//         for( let j = i; j < i + 2; ++j ) {
//             if ( args_o[ 2 ][ j ] >= args_n[ 0 ] + args_n[ 1 ].length )
//                 args_o[ 2 ][ j ] -= args_n[ 1 ].length;
//             else if ( args_o[ 2 ][ j ] > args_n[ 0 ] )
//                 args_o[ 2 ][ j ] = args_n[ 0 ];
//         }
//     }
// } );

// OtWrapperString.reg_nuf( OtWrapperString.op_remove_und, OtWrapperString.op_set_cursor, function( asu_n : UsrId, bq_n : BinaryWriter, bq_o : BinaryWriter, args_n, args_o ) {
//     for( let i = 0; i < args_o[ 2 ].length - 2; i += 3 ) {
//         for( let j = i; j < i + 2; ++j ) {
//             if ( args_o[ 2 ][ j ] >= args_n[ 0 ] + args_n[ 1 ].length )
//                 args_o[ 2 ][ j ] -= args_n[ 1 ].length;
//             else if ( args_o[ 2 ][ j ] > args_n[ 0 ] )
//                 args_o[ 2 ][ j ] = args_n[ 0 ];
//         }
//     }
// } );

// OtWrapperString.reg_nuf( OtWrapperString.op_remove_und, OtWrapperString.op_set_cursor_und, function( asu_n : UsrId, bq_n : BinaryWriter, bq_o : BinaryWriter, args_n, args_o ) {
//     for( let i = 0; i < args_o[ 2 ].length - 2; i += 3 ) {
//         for( let j = i; j < i + 2; ++j ) {
//             if ( args_o[ 2 ][ j ] >= args_n[ 0 ] + args_n[ 1 ].length )
//                 args_o[ 2 ][ j ] -= args_n[ 1 ].length;
//             else if ( args_o[ 2 ][ j ] > args_n[ 0 ] )
//                 args_o[ 2 ][ j ] = args_n[ 0 ];
//         }
//     }
// } );

// OtWrapperString.reg_nuf( OtWrapperString.op_new_cursor, OtWrapperString.op_insert, function( asu_n : UsrId, bq_n : BinaryWriter, bq_o : BinaryWriter, args_n, args_o ) {
//     for( let i = 0; i < args_n[ 2 ].length - 2; i += 3 )
//         for( let j = i; j < i + 2; ++j )
//             if ( args_n[ 2 ][ j ] >= args_o[ 0 ] )
//                 args_n[ 2 ][ j ] += args_o[ 1 ].length;
// } );

// OtWrapperString.reg_nuf( OtWrapperString.op_new_cursor, OtWrapperString.op_remove, function( asu_n : UsrId, bq_n : BinaryWriter, bq_o : BinaryWriter, args_n, args_o ) {
//     for( let i = 0; i < args_n[ 2 ].length - 2; i += 3 ) {
//         for( let j = i; j < i + 2; ++j ) {
//             if ( args_n[ 2 ][ j ] >= args_o[ 0 ] + args_o[ 1 ] )
//                 args_n[ 2 ][ j ] -= args_o[ 1 ];
//             else if ( args_n[ 2 ][ j ] > args_o[ 0 ] )
//                 args_n[ 2 ][ j ] = args_o[ 0 ];
//         }
//     }
// } );

// OtWrapperString.reg_nuf( OtWrapperString.op_new_cursor, OtWrapperString.op_remove_und, function( asu_n : UsrId, bq_n : BinaryWriter, bq_o : BinaryWriter, args_n, args_o ) {
//     for( let i = 0; i < args_n[ 2 ].length - 2; i += 3 ) {
//         for( let j = i; j < i + 2; ++j ) {
//             if ( args_n[ 2 ][ j ] >= args_o[ 0 ] + args_o[ 1 ].length )
//                 args_n[ 2 ][ j ] -= args_o[ 1 ].length;
//             else if ( args_n[ 2 ][ j ] > args_o[ 0 ] )
//                 args_n[ 2 ][ j ] = args_o[ 0 ];
//         }
//     }
// } );

// OtWrapperString.reg_nuf( OtWrapperString.op_set_cursor, OtWrapperString.op_insert, function( asu_n : UsrId, bq_n : BinaryWriter, bq_o : BinaryWriter, args_n, args_o ) {
//     for( let i = 0; i < args_n[ 2 ].length - 2; i += 3 )
//         for( let j = i; j < i + 2; ++j )
//             if ( args_n[ 2 ][ j ] >= args_o[ 0 ] )
//                 args_n[ 2 ][ j ] += args_o[ 1 ].length;
// } );

// OtWrapperString.reg_nuf( OtWrapperString.op_set_cursor, OtWrapperString.op_remove, function( asu_n : UsrId, bq_n : BinaryWriter, bq_o : BinaryWriter, args_n, args_o ) {
//     for( let i = 0; i < args_n[ 2 ].length - 2; i += 3 ) {
//         for( let j = i; j < i + 2; ++j ) {
//             if ( args_n[ 2 ][ j ] >= args_o[ 0 ] + args_o[ 1 ] )
//                 args_n[ 2 ][ j ] -= args_o[ 1 ];
//             else if ( args_n[ 2 ][ j ] > args_o[ 0 ] )
//                 args_n[ 2 ][ j ] = args_o[ 0 ];
//         }
//     }
// } );
// OtWrapperString.reg_nuf( OtWrapperString.op_set_cursor, OtWrapperString.op_remove_und, function( asu_n : UsrId, bq_n : BinaryWriter, bq_o : BinaryWriter, args_n, args_o ) {
//     for( let i = 0; i < args_n[ 2 ].length - 2; i += 3 ) {
//         for( let j = i; j < i + 2; ++j ) {
//             if ( args_n[ 2 ][ j ] >= args_o[ 0 ] + args_o[ 1 ].length )
//                 args_n[ 2 ][ j ] -= args_o[ 1 ].length;
//             else if ( args_n[ 2 ][ j ] > args_o[ 0 ] )
//                 args_n[ 2 ][ j ] = args_o[ 0 ];
//         }
//     }
// } );

// OtWrapperString.reg_nuf( OtWrapperString.op_set_cursor, OtWrapperString.op_new_cursor, function( asu_n : UsrId, bq_n : BinaryWriter, bq_o : BinaryWriter, args_n, args_o ) {
//     // it is impossible to have the same ids
// } );

// OtWrapperString.reg_nuf( OtWrapperString.op_set_cursor, OtWrapperString.op_set_cursor, function( asu_n : UsrId, bq_n : BinaryWriter, bq_o : BinaryWriter, args_n, args_o ) {
//     if ( args_o[ 0 ] == args_n[ 0 ] && args_o[ 1 ] == args_n[ 1 ] )
//         args_o[ 1 ] = 0;
// } );


// OtWrapperString.reg_nuf( OtWrapperString.op_set_cursor, OtWrapperString.op_set_cursor_und, function( asu_n : UsrId, bq_n : BinaryWriter, bq_o : BinaryWriter, args_n, args_o ) {
//     if ( args_o[ 0 ] == args_n[ 0 ] && args_o[ 1 ] == args_n[ 1 ] )
//         args_o[ 1 ] = 0;
// } );

// OtWrapperString.reg_nuf( OtWrapperString.op_set_cursor, OtWrapperString.op_rem_cursor, function( asu_n : UsrId, bq_n : BinaryWriter, bq_o : BinaryWriter, args_n, args_o ) {
//     if ( args_o[ 0 ] == args_n[ 0 ] && args_o[ 1 ] == args_n[ 1 ] ) // cursor has been deleted in an unk patch -> do not keep it
//         args_n[ 1 ] = 0;
// } );

// OtWrapperString.reg_nuf( OtWrapperString.op_set_cursor, OtWrapperString.op_rem_cursor_und, function( asu_n : UsrId, bq_n : BinaryWriter, bq_o : BinaryWriter, args_n, args_o ) {
//     if ( args_o[ 0 ] == args_n[ 0 ] && args_o[ 1 ] == args_n[ 1 ] ) // cursor has been deleted in an unk patch -> do not keep it
//         args_n[ 1 ] = 0;
// } );

// OtWrapperString.reg_nuf( OtWrapperString.op_set_cursor_und, OtWrapperString.op_insert, function( asu_n : UsrId, bq_n : BinaryWriter, bq_o : BinaryWriter, args_n, args_o ) {
//     for( let i = 0; i < args_n[ 2 ].length - 2; i += 3 )
//         for( let j = i; j < i + 2; ++j )
//             if ( args_n[ 2 ][ j ] >= args_o[ 0 ] )
//                 args_n[ 2 ][ j ] += args_o[ 1 ].length;
// } );

// OtWrapperString.reg_nuf( OtWrapperString.op_set_cursor_und, OtWrapperString.op_remove, function( asu_n : UsrId, bq_n : BinaryWriter, bq_o : BinaryWriter, args_n, args_o ) {
//     for( let i = 0; i < args_n[ 2 ].length - 2; i += 3 ) {
//         for( let j = i; j < i + 2; ++j ) {
//             if ( args_n[ 2 ][ j ] >= args_o[ 0 ] + args_o[ 1 ] )
//                 args_n[ 2 ][ j ] -= args_o[ 1 ];
//             else if ( args_n[ 2 ][ j ] > args_o[ 0 ] )
//                 args_n[ 2 ][ j ] = args_o[ 0 ];
//         }
//     }
// } );

// OtWrapperString.reg_nuf( OtWrapperString.op_set_cursor_und, OtWrapperString.op_remove_und, function( asu_n : UsrId, bq_n : BinaryWriter, bq_o : BinaryWriter, args_n, args_o ) {
//     for( let i = 0; i < args_n[ 2 ].length - 2; i += 3 ) {
//         for( let j = i; j < i + 2; ++j ) {
//             if ( args_n[ 2 ][ j ] >= args_o[ 0 ] + args_o[ 1 ].length )
//                 args_n[ 2 ][ j ] -= args_o[ 1 ].length;
//             else if ( args_n[ 2 ][ j ] > args_o[ 0 ] )
//                 args_n[ 2 ][ j ] = args_o[ 0 ];
//         }
//     }
// } );

// OtWrapperString.reg_nuf( OtWrapperString.op_set_cursor_und, OtWrapperString.op_new_cursor, function( asu_n : UsrId, bq_n : BinaryWriter, bq_o : BinaryWriter, args_n, args_o ) {
//     // it is impossible to have the same ids
// } );

// OtWrapperString.reg_nuf( OtWrapperString.op_set_cursor_und, OtWrapperString.op_set_cursor, function( asu_n : UsrId, bq_n : BinaryWriter, bq_o : BinaryWriter, args_n, args_o ) {
//     if ( args_o[ 0 ] == args_n[ 0 ] && args_o[ 1 ] == args_n[ 1 ] )
//         args_o[ 1 ] = 0;
// } );

// OtWrapperString.reg_nuf( OtWrapperString.op_set_cursor_und, OtWrapperString.op_set_cursor_und, function( asu_n : UsrId, bq_n : BinaryWriter, bq_o : BinaryWriter, args_n, args_o ) {
//     if ( args_o[ 0 ] == args_n[ 0 ] && args_o[ 1 ] == args_n[ 1 ] )
//         args_o[ 1 ] = 0;
// } );

// OtWrapperString.reg_nuf( OtWrapperString.op_set_cursor_und, OtWrapperString.op_rem_cursor, function( asu_n : UsrId, bq_n : BinaryWriter, bq_o : BinaryWriter, args_n, args_o ) {
//     if ( args_o[ 0 ] == args_n[ 0 ] && args_o[ 1 ] == args_n[ 1 ] ) // cursor has been deleted in an unk patch -> do not keep it
//         args_n[ 1 ] = 0;
// } );

// OtWrapperString.reg_nuf( OtWrapperString.op_set_cursor_und, OtWrapperString.op_rem_cursor_und, function( asu_n : UsrId, bq_n : BinaryWriter, bq_o : BinaryWriter, args_n, args_o ) {
//     if ( args_o[ 0 ] == args_n[ 0 ] && args_o[ 1 ] == args_n[ 1 ] ) // cursor has been deleted in an unk patch -> do not keep it
//         args_n[ 1 ] = 0;
// } );

// OtWrapperString.reg_nuf( OtWrapperString.op_rem_cursor, OtWrapperString.op_new_cursor, function( asu_n : UsrId, bq_n : BinaryWriter, bq_o : BinaryWriter, args_n, args_o ) {
//     if ( args_o[ 0 ] == args_n[ 0 ] && args_o[ 1 ] == args_n[ 1 ] )
//         args_o[ 1 ] = 0;
// } );

// OtWrapperString.reg_nuf( OtWrapperString.op_rem_cursor, OtWrapperString.op_set_cursor, function( asu_n : UsrId, bq_n : BinaryWriter, bq_o : BinaryWriter, args_n, args_o ) {
//     if ( args_o[ 0 ] == args_n[ 0 ] && args_o[ 1 ] == args_n[ 1 ] )
//         args_o[ 1 ] = 0;
// } );

// OtWrapperString.reg_nuf( OtWrapperString.op_rem_cursor, OtWrapperString.op_set_cursor_und, function( asu_n : UsrId, bq_n : BinaryWriter, bq_o : BinaryWriter, args_n, args_o ) {
//     if ( args_o[ 0 ] == args_n[ 0 ] && args_o[ 1 ] == args_n[ 1 ] )
//         args_o[ 1 ] = 0;
// } );

// OtWrapperString.reg_nuf( OtWrapperString.op_rem_cursor, OtWrapperString.op_rem_cursor, function( asu_n : UsrId, bq_n : BinaryWriter, bq_o : BinaryWriter, args_n, args_o ) {
//     if ( args_o[ 0 ] == args_n[ 0 ] && args_o[ 1 ] == args_n[ 1 ] )
//         args_o[ 1 ] = 0;
// } );

// OtWrapperString.reg_nuf( OtWrapperString.op_rem_cursor, OtWrapperString.op_rem_cursor_und, function( asu_n : UsrId, bq_n : BinaryWriter, bq_o : BinaryWriter, args_n, args_o ) {
//     if ( args_o[ 0 ] == args_n[ 0 ] && args_o[ 1 ] == args_n[ 1 ] )
//         args_o[ 1 ] = 0;
// } );

// OtWrapperString.reg_nuf( OtWrapperString.op_rem_cursor_und, OtWrapperString.op_new_cursor, function( asu_n : UsrId, bq_n : BinaryWriter, bq_o : BinaryWriter, args_n, args_o ) {
//     if ( args_o[ 0 ] == args_n[ 0 ] && args_o[ 1 ] == args_n[ 1 ] )
//         args_o[ 1 ] = 0;
// } );

// OtWrapperString.reg_nuf( OtWrapperString.op_rem_cursor_und, OtWrapperString.op_set_cursor, function( asu_n : UsrId, bq_n : BinaryWriter, bq_o : BinaryWriter, args_n, args_o ) {
//     if ( args_o[ 0 ] == args_n[ 0 ] && args_o[ 1 ] == args_n[ 1 ] )
//         args_o[ 1 ] = 0;
// } );

// OtWrapperString.reg_nuf( OtWrapperString.op_rem_cursor_und, OtWrapperString.op_set_cursor_und, function( asu_n : UsrId, bq_n : BinaryWriter, bq_o : BinaryWriter, args_n, args_o ) {
//     if ( args_o[ 0 ] == args_n[ 0 ] && args_o[ 1 ] == args_n[ 1 ] )
//         args_o[ 1 ] = 0;
// } );

// OtWrapperString.reg_nuf( OtWrapperString.op_rem_cursor_und, OtWrapperString.op_rem_cursor, function( asu_n : UsrId, bq_n : BinaryWriter, bq_o : BinaryWriter, args_n, args_o ) {
//     if ( args_o[ 0 ] == args_n[ 0 ] && args_o[ 1 ] == args_n[ 1 ] )
//         args_o[ 1 ] = 0;
// } );

// OtWrapperString.reg_nuf( OtWrapperString.op_rem_cursor_und, OtWrapperString.op_rem_cursor_und, function( asu_n : UsrId, bq_n : BinaryWriter, bq_o : BinaryWriter, args_n, args_o ) {
//     if ( args_o[ 0 ] == args_n[ 0 ] && args_o[ 1 ] == args_n[ 1 ] )
//         args_o[ 1 ] = 0;
// } );

// OtWrapperString.reg_usc( OtWrapperString.op_insert, OtWrapperString.op_insert, function( asu_s : UsrId, bq_u : BinaryWriter, args_s, args_u ) {
//     if ( args_u[ 0 ] > args_s[ 0 ] ) {
//         //  orig 012
//         //  +unk 01a2  INS args_u[ 0 ]=2,args_u[ 1 ]=a
//         //  +std 0b1a2 INS args_s[ 0 ]=1,args_s[ 1 ]=b
//         // same as
//         //  +std 0b12  INS args_s[ 0 ]=1,args_s[ 1 ]=b
//         //  +unk 0b1a2 INS args_u[ 0 ]=3,args_u[ 1 ]=a
//         args_u[ 0 ] += args_s[ 1 ].length;
//     } else {
//         //  orig 012
//         //  +unk 0a12  INS args_u[ 0 ]=1,args_u[ 1 ]=a
//         //  +std 0a1b2 INS args_s[ 0 ]=3,args_s[ 1 ]=b
//         // same as
//         //  +std 01b2  INS args_s[ 0 ]=2,args_s[ 1 ]=b
//         //  +unk 0a1b2 INS args_u[ 0 ]=1,args_u[ 1 ]=a
//         args_s[ 0 ] -= args_u[ 1 ].length;
//     }
// } );

// OtWrapperString.reg_usc( OtWrapperString.op_insert, OtWrapperString.op_remove,  function( asu_s : UsrId, bq_u : BinaryWriter, args_s, args_u ) {
//     if ( args_s[ 0 ] > args_u[ 0 ] ) {
//         //  orig 012345678
//         //  +unk 045678     REM args_u[ 0 ]=1,args_u[ 1 ]=3
//         //  +std 04b5678    INS args_s[ 0 ]=2,args_s[ 1 ]=b
//         // same as
//         //  +std 01234b5678 INS args_s[ 0 ]=5,args_s[ 1 ]=b
//         //  +unk 04b5678    REM args_u[ 0 ]=3,args_u[ 1 ]=a
//         args_s[ 0 ] += args_u[ 1 ];
//     } else {
//         //  orig 012345678
//         //  +unk 012348     REM args_u[ 0 ]=5,args_u[ 1 ]=3
//         //  +std 01b2348    INS args_s[ 0 ]=2,args_s[ 1 ]=b
//         // same as
//         //  +std 01b2345678 INS args_s[ 0 ]=5,args_s[ 1 ]=b
//         //  +unk 01b2348    REM args_u[ 0 ]=6,args_u[ 1 ]=a
//         args_u[ 0 ] += args_s[ 1 ].length;
//     }
// } );

// OtWrapperString.reg_usc( OtWrapperString.op_insert, OtWrapperString.op_remove_und,  function( asu_s : UsrId, bq_u : BinaryWriter, args_s, args_u ) {
//     if ( args_s[ 0 ] > args_u[ 0 ] ) {
//         //  orig 012345678
//         //  +unk 045678     REM args_u[ 0 ]=1,args_u[ 1 ]=3
//         //  +std 04b5678    INS args_s[ 0 ]=2,args_s[ 1 ]=b
//         // same as
//         //  +std 01234b5678 INS args_s[ 0 ]=5,args_s[ 1 ]=b
//         //  +unk 04b5678    REM args_u[ 0 ]=3,args_u[ 1 ]=a
//         args_s[ 0 ] += args_u[ 1 ].length;
//     } else {
//         //  orig 012345678
//         //  +unk 012348     REM args_u[ 0 ]=5,args_u[ 1 ]=3
//         //  +std 01b2348    INS args_s[ 0 ]=2,args_s[ 1 ]=b
//         // same as
//         //  +std 01b2345678 INS args_s[ 0 ]=5,args_s[ 1 ]=b
//         //  +unk 01b2348    REM args_u[ 0 ]=6,args_u[ 1 ]=a
//         args_u[ 0 ] += args_s[ 1 ].length;
//     }
// } );

// OtWrapperString.reg_usc( OtWrapperString.op_insert, OtWrapperString.op_new_cursor,  function( asu_s : UsrId, bq_u : BinaryWriter, args_s, args_u ) {
//     for( let i = 0; i < args_u[ 2 ].length - 2; i += 3 ) {
//         for( let j = i; j < i + 2; ++j ) {
//             if ( args_u[ 2 ][ j ] >= args_s[ 0 ] ) {
//                 //  orig 012
//                 //  +unk 01|2  NEW_CURSOR pos=2
//                 //  +std 0b1|2 INS args_s[ 0 ]=1,args_s[ 1 ]=b
//                 // same as
//                 //  +std 0b12  INS args_s[ 0 ]=1,args_s[ 1 ]=b
//                 //  +unk 0b1|2 NEW_CURSOR pos=3
//                 args_u[ 2 ][ j ] += args_s[ 1 ].length;
//             }
//         }
//     }
// } );

// OtWrapperString.reg_usc( OtWrapperString.op_insert, OtWrapperString.op_set_cursor,  function( asu_s : UsrId, bq_u : BinaryWriter, args_s, args_u ) {
//     for( let i = 0; i < args_u[ 2 ].length - 2; i += 3 ) {
//         for( let j = i; j < i + 2; ++j ) {
//             if ( args_u[ 2 ][ j ] >= args_s[ 0 ] ) {
//                 //  orig 012
//                 //  +unk 01|2  NEW_CURSOR pos=2
//                 //  +std 0b1|2 INS args_s[ 0 ]=1,args_s[ 1 ]=b
//                 // same as
//                 //  +std 0b12  INS args_s[ 0 ]=1,args_s[ 1 ]=b
//                 //  +unk 0b1|2 NEW_CURSOR pos=3
//                 args_u[ 2 ][ j ] += args_s[ 1 ].length;
//             }
//         }
//     }
// } );

// OtWrapperString.reg_usc( OtWrapperString.op_insert, OtWrapperString.op_set_cursor_und,  function( asu_s : UsrId, bq_u : BinaryWriter, args_s, args_u ) {
//     for( let i = 0; i < args_u[ 2 ].length - 2; i += 3 ) {
//         for( let j = i; j < i + 2; ++j ) {
//             if ( args_u[ 2 ][ j ] >= args_s[ 0 ] ) {
//                 //  orig 012
//                 //  +unk 01|2  NEW_CURSOR pos=2
//                 //  +std 0b1|2 INS args_s[ 0 ]=1,args_s[ 1 ]=b
//                 // same as
//                 //  +std 0b12  INS args_s[ 0 ]=1,args_s[ 1 ]=b
//                 //  +unk 0b1|2 NEW_CURSOR pos=3
//                 args_u[ 2 ][ j ] += args_s[ 1 ].length;
//             }
//         }
//     }
// } );

// OtWrapperString.reg_usc( OtWrapperString.op_remove, OtWrapperString.op_insert,  function( asu_s : UsrId, bq_u : BinaryWriter, args_s, args_u ) {
//     if ( args_u[ 0 ] > args_s[ 0 ] ) {
//         //  orig 012345678
//         //  +unk 0123456b78 INS args_u[ 0 ]=7,args_u[ 1 ]=b
//         //  +std 0456b78    REM args_s[ 0 ]=1,args_s[ 1 ]=3
//         // same as
//         //  +std 045678     REM args_s[ 0 ]=1,args_s[ 1 ]=3
//         //  +unk 0456b78    INS args_u[ 0 ]=4,args_u[ 1 ]=b
//         args_u[ 0 ] -= args_s[ 1 ];
//     } else {
//         //  orig 012345678
//         //  +unk 01b2345678 INS args_u[ 0 ]=2,args_u[ 1 ]=b
//         //  +std 01b2348    REM args_s[ 0 ]=6,args_s[ 1 ]=3
//         // same as
//         //  +std 012348     REM args_s[ 0 ]=5,args_s[ 1 ]=3
//         //  +unk 01b2348    INS args_u[ 0 ]=2,args_u[ 1 ]=b
//         args_s[ 0 ] -= args_u[ 1 ].length;
//     }
// } );

// OtWrapperString.reg_usc( OtWrapperString.op_remove, OtWrapperString.op_remove,  function( asu_s : UsrId, bq_u : BinaryWriter, args_s, args_u ) {
//     if ( args_s[ 0 ] >= args_u[ 0 ] ) {
//         //  orig 012345678
//         //  +unk 0345678    REM args_u[ 0 ]=1,args_u[ 1 ]=2
//         //  +std 03478      REM args_s[ 0 ]=3,args_s[ 1 ]=2
//         // same as
//         //  +std 0123478    REM args_s[ 0 ]=5,args_s[ 1 ]=3
//         //  +unk 03478      REM args_u[ 0 ]=1,args_u[ 1 ]=2
//         args_s[ 0 ] += args_u[ 1 ];
//     } else if ( args_s[ 0 ] + args_s[ 1 ] <= args_u[ 0 ] ) {
//         //  orig 012345678
//         //  +unk 0123458    REM args_u[ 0 ]=6,args_u[ 1 ]=2
//         //  +std 03458      REM args_s[ 0 ]=1,args_s[ 1 ]=2
//         // same as
//         //  +std 0345678    REM args_s[ 0 ]=1,args_s[ 1 ]=2
//         //  +unk 03458      REM args_u[ 0 ]=4,args_u[ 1 ]=2
//         args_u[ 0 ] -= args_s[ 1 ];
//     } else {
//         //  orig 012345678
//         //  +unk 0123678    REM args_u[ 0 ]=4,args_u[ 1 ]=2
//         //  +std 08         REM args_s[ 0 ]=1,args_s[ 1 ]=5
//         // same as
//         //  +std 0678       REM args_s[ 0 ]=1,args_s[ 1 ]=5
//         //  +unk 08         REM args_u[ 0 ]=1,args_u[ 1 ]=2
//         args_u[ 0 ] = args_s[ 0 ];
//     }
// } );

// OtWrapperString.reg_usc( OtWrapperString.op_remove, OtWrapperString.op_remove_und,  function( asu_s : UsrId, bq_u : BinaryWriter, args_s, args_u ) {
//     if ( args_s[ 0 ] >= args_u[ 0 ] ) {
//         //  orig 012345678
//         //  +unk 0345678    REM args_u[ 0 ]=1,args_u[ 1 ]=2
//         //  +std 03478      REM args_s[ 0 ]=3,args_s[ 1 ]=2
//         // same as
//         //  +std 0123478    REM args_s[ 0 ]=5,args_s[ 1 ]=3
//         //  +unk 03478      REM args_u[ 0 ]=1,args_u[ 1 ]=2
//         args_s[ 0 ] += args_u[ 1 ].length;
//     } else if ( args_s[ 0 ] + args_s[ 1 ] <= args_u[ 0 ] ) {
//         //  orig 012345678
//         //  +unk 0123458    REM args_u[ 0 ]=6,args_u[ 1 ]=2
//         //  +std 03458      REM args_s[ 0 ]=1,args_s[ 1 ]=2
//         // same as
//         //  +std 0345678    REM args_s[ 0 ]=1,args_s[ 1 ]=2
//         //  +unk 03458      REM args_u[ 0 ]=4,args_u[ 1 ]=2
//         args_u[ 0 ] -= args_s[ 1 ];
//     } else {
//         //  orig 012345678
//         //  +unk 0123678    REM args_u[ 0 ]=4,args_u[ 1 ]=2
//         //  +std 08         REM args_s[ 0 ]=1,args_s[ 1 ]=5
//         // same as
//         //  +std 0678       REM args_s[ 0 ]=1,args_s[ 1 ]=5
//         //  +unk 08         REM args_u[ 0 ]=1,args_u[ 1 ]=2
//         args_u[ 0 ] = args_s[ 0 ];
//     }
// } );

// OtWrapperString.reg_usc( OtWrapperString.op_remove, OtWrapperString.op_new_cursor,  function( asu_s : UsrId, bq_u : BinaryWriter, args_s, args_u ) {
//     for( let i = 0; i < args_u[ 2 ].length - 2; i += 3 ) {
//         for( let j = i; j < i + 2; ++j ) {
//             if ( args_u[ 2 ][ j ] >= args_s[ 0 ] + args_s[ 1 ] ) {
//                 //  orig 012345
//                 //  +unk 01234|5  NEW_CURSOR pos=5
//                 //  +std 014|5    REM args_s[ 0 ]=2,args_s[ 1 ]=2
//                 // same as
//                 //  +std 0145     REM args_s[ 0 ]=2,args_s[ 1 ]=2
//                 //  +unk 014|5    NEW_CURSOR pos=3
//                 args_u[ 2 ][ j ] -= args_s[ 1 ];
//             } else if ( args_u[ 2 ][ j ] > args_s[ 0 ] ) {
//                 //  orig 012345
//                 //  +unk 012|345  NEW_CURSOR pos=3
//                 //  +std 01|45    REM args_s[ 0 ]=2,args_s[ 1 ]=2
//                 // same as
//                 //  +std 0145     REM args_s[ 0 ]=2,args_s[ 1 ]=2
//                 //  +unk 01|45    NEW_CURSOR pos=2
//                 args_u[ 2 ][ j ] = args_s[ 0 ];
//             }
//         }
//     }
// } );

// OtWrapperString.reg_usc( OtWrapperString.op_remove, OtWrapperString.op_set_cursor,  function( asu_s : UsrId, bq_u : BinaryWriter, args_s, args_u ) {
//     for( let i = 0; i < args_u[ 2 ].length - 2; i += 3 ) {
//         for( let j = i; j < i + 2; ++j ) {
//             if ( args_u[ 2 ][ j ] >= args_s[ 0 ] + args_s[ 1 ] ) {
//                 //  orig 012345
//                 //  +unk 01234|5  NEW_CURSOR pos=5
//                 //  +std 014|5    REM args_s[ 0 ]=2,args_s[ 1 ]=2
//                 // same as
//                 //  +std 0145     REM args_s[ 0 ]=2,args_s[ 1 ]=2
//                 //  +unk 014|5    NEW_CURSOR pos=3
//                 args_u[ 2 ][ j ] -= args_s[ 1 ];
//             } else if ( args_u[ 2 ][ j ] > args_s[ 0 ] ) {
//                 //  orig 012345
//                 //  +unk 012|345  NEW_CURSOR pos=3
//                 //  +std 01|45    REM args_s[ 0 ]=2,args_s[ 1 ]=2
//                 // same as
//                 //  +std 0145     REM args_s[ 0 ]=2,args_s[ 1 ]=2
//                 //  +unk 01|45    NEW_CURSOR pos=2
//                 args_u[ 2 ][ j ] = args_s[ 0 ];
//             }
//         }
//     }
// } );

// OtWrapperString.reg_usc( OtWrapperString.op_remove, OtWrapperString.op_set_cursor_und,  function( asu_s : UsrId, bq_u : BinaryWriter, args_s, args_u ) {
//     for( let i = 0; i < args_u[ 2 ].length - 2; i += 3 ) {
//         for( let j = i; j < i + 2; ++j ) {
//             if ( args_u[ 2 ][ j ] >= args_s[ 0 ] + args_s[ 1 ] ) {
//                 //  orig 012345
//                 //  +unk 01234|5  SET_CURSOR pos=5
//                 //  +std 014|5    REM args_s[ 0 ]=2,args_s[ 1 ]=2
//                 // same as
//                 //  +std 0145     REM args_s[ 0 ]=2,args_s[ 1 ]=2
//                 //  +unk 014|5    NEW_CURSOR pos=3
//                 args_u[ 2 ][ j ] -= args_s[ 1 ];
//             } else if ( args_u[ 2 ][ j ] > args_s[ 0 ] ) {
//                 //  orig 012345
//                 //  +unk 012|345  SET_CURSOR pos=3
//                 //  +std 01|45    REM args_s[ 0 ]=2,args_s[ 1 ]=2
//                 // same as
//                 //  +std 0145     REM args_s[ 0 ]=2,args_s[ 1 ]=2
//                 //  +unk 01|45    NEW_CURSOR pos=2
//                 args_u[ 2 ][ j ] = args_s[ 0 ];
//             }
//         }
//     }
// } );

// OtWrapperString.reg_usc( OtWrapperString.op_remove_und, OtWrapperString.op_insert,  function( asu_s : UsrId, bq_u : BinaryWriter, args_s, args_u ) {
//     if ( args_u[ 0 ] > args_s[ 0 ] ) {
//         //  orig 012345678
//         //  +unk 0123456b78 INS args_u[ 0 ]=7,args_u[ 1 ]=b
//         //  +std 0456b78    REM args_s[ 0 ]=1,args_s[ 1 ]=3
//         // same as
//         //  +std 045678     REM args_s[ 0 ]=1,args_s[ 1 ]=3
//         //  +unk 0456b78    INS args_u[ 0 ]=4,args_u[ 1 ]=b
//         args_u[ 0 ] -= args_s[ 1 ].length;
//     } else {
//         //  orig 012345678
//         //  +unk 01b2345678 INS args_u[ 0 ]=2,args_u[ 1 ]=b
//         //  +std 01b2348    REM args_s[ 0 ]=6,args_s[ 1 ]=3
//         // same as
//         //  +std 012348     REM args_s[ 0 ]=5,args_s[ 1 ]=3
//         //  +unk 01b2348    INS args_u[ 0 ]=2,args_u[ 1 ]=b
//         args_s[ 0 ] -= args_u[ 1 ].length;
//     }
// } );

// OtWrapperString.reg_usc( OtWrapperString.op_remove_und, OtWrapperString.op_remove,  function( asu_s : UsrId, bq_u : BinaryWriter, args_s, args_u ) {
//     if ( args_s[ 0 ] >= args_u[ 0 ] ) {
//         //  orig 012345678
//         //  +unk 0345678    REM args_u[ 0 ]=1,args_u[ 1 ]=2
//         //  +std 03478      REM args_s[ 0 ]=3,args_s[ 1 ]=2
//         // same as
//         //  +std 0123478    REM args_s[ 0 ]=5,args_s[ 1 ]=3
//         //  +unk 03478      REM args_u[ 0 ]=1,args_u[ 1 ]=2
//         args_s[ 0 ] += args_u[ 1 ];
//     } else if ( args_s[ 0 ] + args_s[ 1 ].length <= args_u[ 0 ] ) {
//         //  orig 012345678
//         //  +unk 0123458    REM args_u[ 0 ]=6,args_u[ 1 ]=2
//         //  +std 03458      REM args_s[ 0 ]=1,args_s[ 1 ]=2
//         // same as
//         //  +std 0345678    REM args_s[ 0 ]=1,args_s[ 1 ]=2
//         //  +unk 03458      REM args_u[ 0 ]=4,args_u[ 1 ]=2
//         args_u[ 0 ] -= args_s[ 1 ].length;
//     } else {
//         //  orig 012345678
//         //  +unk 0123678    REM args_u[ 0 ]=4,args_u[ 1 ]=2
//         //  +std 08         REM args_s[ 0 ]=1,args_s[ 1 ]=5
//         // same as
//         //  +std 0678       REM args_s[ 0 ]=1,args_s[ 1 ]=5
//         //  +unk 08         REM args_u[ 0 ]=1,args_u[ 1 ]=2
//         args_u[ 0 ] = args_s[ 0 ];
//     }
// } );

// OtWrapperString.reg_usc( OtWrapperString.op_remove_und, OtWrapperString.op_remove_und,  function( asu_s : UsrId, bq_u : BinaryWriter, args_s, args_u ) {
//     if ( args_s[ 0 ] >= args_u[ 0 ] ) {
//         //  orig 012345678
//         //  +unk 0345678    REM args_u[ 0 ]=1,args_u[ 1 ]=2
//         //  +std 03478      REM args_s[ 0 ]=3,args_s[ 1 ]=2
//         // same as
//         //  +std 0123478    REM args_s[ 0 ]=5,args_s[ 1 ]=3
//         //  +unk 03478      REM args_u[ 0 ]=1,args_u[ 1 ]=2
//         args_s[ 0 ] += args_u[ 1 ].length;
//     } else if ( args_s[ 0 ] + args_s[ 1 ].length <= args_u[ 0 ] ) {
//         //  orig 012345678
//         //  +unk 0123458    REM args_u[ 0 ]=6,args_u[ 1 ]=2
//         //  +std 03458      REM args_s[ 0 ]=1,args_s[ 1 ]=2
//         // same as
//         //  +std 0345678    REM args_s[ 0 ]=1,args_s[ 1 ]=2
//         //  +unk 03458      REM args_u[ 0 ]=4,args_u[ 1 ]=2
//         args_u[ 0 ] -= args_s[ 1 ].length;
//     } else {
//         //  orig 012345678
//         //  +unk 0123678    REM args_u[ 0 ]=4,args_u[ 1 ]=2
//         //  +std 08         REM args_s[ 0 ]=1,args_s[ 1 ]=5
//         // same as
//         //  +std 0678       REM args_s[ 0 ]=1,args_s[ 1 ]=5
//         //  +unk 08         REM args_u[ 0 ]=1,args_u[ 1 ]=2
//         args_u[ 0 ] = args_s[ 0 ];
//     }
// } );

// OtWrapperString.reg_usc( OtWrapperString.op_remove_und, OtWrapperString.op_new_cursor,  function( asu_s : UsrId, bq_u : BinaryWriter, args_s, args_u ) {
//     for( let i = 0; i < args_u[ 2 ].length - 2; i += 3 ) {
//         for( let j = i; j < i + 2; ++j ) {
//             if ( args_u[ 2 ][ j ] >= args_s[ 0 ] + args_s[ 1 ].length ) {
//                 //  orig 012345
//                 //  +unk 01234|5  NEW_CURSOR pos=5
//                 //  +std 014|5    REM args_s[ 0 ]=2,args_s[ 1 ]=2
//                 // same as
//                 //  +std 0145     REM args_s[ 0 ]=2,args_s[ 1 ]=2
//                 //  +unk 014|5    NEW_CURSOR pos=3
//                 args_u[ 2 ][ j ] -= args_s[ 1 ].length;
//             } else if ( args_u[ 2 ][ j ] > args_s[ 0 ] ) {
//                 //  orig 012345
//                 //  +unk 012|345  NEW_CURSOR pos=3
//                 //  +std 01|45    REM args_s[ 0 ]=2,args_s[ 1 ]=2
//                 // same as
//                 //  +std 0145     REM args_s[ 0 ]=2,args_s[ 1 ]=2
//                 //  +unk 01|45    NEW_CURSOR pos=2
//                 args_u[ 2 ][ j ] = args_s[ 0 ];
//             }
//         }
//     }
// } );

// OtWrapperString.reg_usc( OtWrapperString.op_remove_und, OtWrapperString.op_set_cursor,  function( asu_s : UsrId, bq_u : BinaryWriter, args_s, args_u ) {
//     for( let i = 0; i < args_u[ 2 ].length - 2; i += 3 ) {
//         for( let j = i; j < i + 2; ++j ) {
//             if ( args_u[ 2 ][ j ] >= args_s[ 0 ] + args_s[ 1 ].length ) {
//                 //  orig 012345
//                 //  +unk 01234|5  NEW_CURSOR pos=5
//                 //  +std 014|5    REM args_s[ 0 ]=2,args_s[ 1 ]=2
//                 // same as
//                 //  +std 0145     REM args_s[ 0 ]=2,args_s[ 1 ]=2
//                 //  +unk 014|5    NEW_CURSOR pos=3
//                 args_u[ 2 ][ j ] -= args_s[ 1 ].length;
//             } else if ( args_u[ 2 ][ j ] > args_s[ 0 ] ) {
//                 //  orig 012345
//                 //  +unk 012|345  NEW_CURSOR pos=3
//                 //  +std 01|45    REM args_s[ 0 ]=2,args_s[ 1 ]=2
//                 // same as
//                 //  +std 0145     REM args_s[ 0 ]=2,args_s[ 1 ]=2
//                 //  +unk 01|45    NEW_CURSOR pos=2
//                 args_u[ 2 ][ j ] = args_s[ 0 ];
//             }
//         }
//     }
// } );

// OtWrapperString.reg_usc( OtWrapperString.op_remove_und, OtWrapperString.op_set_cursor_und,  function( asu_s : UsrId, bq_u : BinaryWriter, args_s, args_u ) {
//     for( let i = 0; i < args_u[ 2 ].length - 2; i += 3 ) {
//         for( let j = i; j < i + 2; ++j ) {
//             if ( args_u[ 2 ][ j ] >= args_s[ 0 ] + args_s[ 1 ].length ) {
//                 //  orig 012345
//                 //  +unk 01234|5  SET_CURSOR pos=5
//                 //  +std 014|5    REM args_s[ 0 ]=2,args_s[ 1 ]=2
//                 // same as
//                 //  +std 0145     REM args_s[ 0 ]=2,args_s[ 1 ]=2
//                 //  +unk 014|5    NEW_CURSOR pos=3
//                 args_u[ 2 ][ j ] -= args_s[ 1 ].length;
//             } else if ( args_u[ 2 ][ j ] > args_s[ 0 ] ) {
//                 //  orig 012345
//                 //  +unk 012|345  SET_CURSOR pos=3
//                 //  +std 01|45    REM args_s[ 0 ]=2,args_s[ 1 ]=2
//                 // same as
//                 //  +std 0145     REM args_s[ 0 ]=2,args_s[ 1 ]=2
//                 //  +unk 01|45    NEW_CURSOR pos=2
//                 args_u[ 2 ][ j ] = args_s[ 0 ];
//             }
//         }
//     }
// } );

// OtWrapperString.reg_usc( OtWrapperString.op_set_cursor, OtWrapperString.op_new_cursor,  function( asu_s : UsrId, bq_u : BinaryWriter, args_s, args_u ) {
//     if ( args_s[ 0 ] == args_u[ 0 ] && args_s[ 1 ] == args_u[ 1 ] )
//         args_u[ 1 ] = 0;
// } );

// OtWrapperString.reg_usc( OtWrapperString.op_set_cursor, OtWrapperString.op_set_cursor,  function( asu_s : UsrId, bq_u : BinaryWriter, args_s, args_u ) {
//     if ( args_s[ 0 ] == args_u[ 0 ] && args_s[ 1 ] == args_u[ 1 ] )
//         args_u[ 1 ] = 0;
// } );

// OtWrapperString.reg_usc( OtWrapperString.op_set_cursor_und, OtWrapperString.op_set_cursor_und,  function( asu_s : UsrId, bq_u : BinaryWriter, args_s, args_u ) {
//     if ( args_s[ 0 ] == args_u[ 0 ] && args_s[ 1 ] == args_u[ 1 ] )
//         args_u[ 1 ] = 0;
// } );

// OtWrapperString.reg_usc( OtWrapperString.op_set_cursor_und, OtWrapperString.op_new_cursor,  function( asu_s : UsrId, bq_u : BinaryWriter, args_s, args_u ) {
//     if ( args_s[ 0 ] == args_u[ 0 ] && args_s[ 1 ] == args_u[ 1 ] )
//         args_u[ 1 ] = 0;
// } );

// OtWrapperString.reg_usc( OtWrapperString.op_set_cursor_und, OtWrapperString.op_set_cursor,  function( asu_s : UsrId, bq_u : BinaryWriter, args_s, args_u ) {
//     if ( args_s[ 0 ] == args_u[ 0 ] && args_s[ 1 ] == args_u[ 1 ] )
//         args_u[ 1 ] = 0;
// } );

// OtWrapperString.reg_usc( OtWrapperString.op_set_cursor_und, OtWrapperString.op_set_cursor_und,  function( asu_s : UsrId, bq_u : BinaryWriter, args_s, args_u ) {
//     if ( args_s[ 0 ] == args_u[ 0 ] && args_s[ 1 ] == args_u[ 1 ] )
//         args_u[ 1 ] = 0;
// } );

// OtWrapperString.reg_usc( OtWrapperString.op_rem_cursor, OtWrapperString.op_new_cursor,  function( asu_s : UsrId, bq_u : BinaryWriter, args_s, args_u ) {
//     if ( args_s[ 0 ] == args_u[ 0 ] && args_s[ 1 ] == args_u[ 1 ] )
//         args_u[ 1 ] = 0;
// } );

// OtWrapperString.reg_usc( OtWrapperString.op_rem_cursor, OtWrapperString.op_set_cursor,  function( asu_s : UsrId, bq_u : BinaryWriter, args_s, args_u ) {
//     if ( args_s[ 0 ] == args_u[ 0 ] && args_s[ 1 ] == args_u[ 1 ] )
//         args_u[ 1 ] = 0;
// } );

// OtWrapperString.reg_usc( OtWrapperString.op_rem_cursor, OtWrapperString.op_set_cursor_und,  function( asu_s : UsrId, bq_u : BinaryWriter, args_s, args_u ) {
//     if ( args_s[ 0 ] == args_u[ 0 ] && args_s[ 1 ] == args_u[ 1 ] )
//         args_u[ 1 ] = 0;
// } );

// OtWrapperString.reg_usc( OtWrapperString.op_rem_cursor_und, OtWrapperString.op_new_cursor,  function( asu_s : UsrId, bq_u : BinaryWriter, args_s, args_u ) {
//     if ( args_s[ 0 ] == args_u[ 0 ] && args_s[ 1 ] == args_u[ 1 ] )
//         args_u[ 1 ] = 0;
// } );

// OtWrapperString.reg_usc( OtWrapperString.op_rem_cursor_und, OtWrapperString.op_set_cursor,  function( asu_s : UsrId, bq_u : BinaryWriter, args_s, args_u ) {
//     if ( args_s[ 0 ] == args_u[ 0 ] && args_s[ 1 ] == args_u[ 1 ] )
//         args_u[ 1 ] = 0;
// } );

// OtWrapperString.reg_usc( OtWrapperString.op_rem_cursor_und, OtWrapperString.op_set_cursor_und,  function( asu_s : UsrId, bq_u : BinaryWriter, args_s, args_u ) {
//     if ( args_s[ 0 ] == args_u[ 0 ] && args_s[ 1 ] == args_u[ 1 ] )
//         args_u[ 1 ] = 0;
// } );



// methods["self_set_o"].add_surdef( 2, [ OtWrapperString, "to_String__b" ], function( str : String, sup ) : boolean {
//     if ( ! str.equ_immediate( sup ) ) {
//         str.clear();
//         str.insert( 0, sup.toString() );
//         return true;
//     }
//     return false;
// } );

// methods["self_set_b"].add_surdef( 2, [ OtWrapperString, "to_String__b" ], function( str : String, sup ) : boolean {
//     if ( ! str.equ_immediate( sup ) ) {
//         str.clear();
//         str.insert( 0, sup.toString() );
//         return true;
//     }
//     return false;
// } );

// methods["self_remove_bb"].add_surdef( 2, [ OtWrapperString, "to_Number__b", "to_Number__b" ], function( str, pos, len ) : boolean {
//     s = methods["to_Number__b"].call_1( pos );
//     n = methods["to_Number__b"].call_1( len );
//     return str.rp._self_remove( npos, nlen );
// } );

// methods["self_ot_merge__sb"].add_surdef( 2, [ OtWrapperString, "to_String__b" ], function( str: { rp: OtWrapperString }, oth ) : boolean {
//      = methods["to_String__b"].call_1( oth );
//     return str.rp._self_insert( str.rp.val.length.val, sth );
// } );
// methods["self_ot_merge__sb"].add_surdef( 3, [ OtWrapperString, OtWrapperString ], function( str: { rp: OtWrapperString }, oth: { rp: OtWrapperString }, as_usr : UsrId ) : boolean {
//     str.rp.on_change_par = str.rp.on_change_par.concat( oth.rp.on_change_par );
//     console.error( "merge" );
//     for( let i in oth.rp.on_change_callbacks )
//         str.rp.on_change_callbacks[ i ] = oth.rp.on_change_callbacks[ i ];

//      = methods["to_String__b"].call_1( oth );
//     return str.rp._self_insert( str.rp.val.length.val, sth );
// } );

// import BinaryWriter from "../../System/BinaryWriter";
// import BinaryReader from "../../System/BinaryReader";
// import ListUtil     from "../../System/ListUtil";
// import DevId        from "../../System/DevId";
// import UsrId        from "../../System/UsrId";
// import Variable     from "../Core/Variable";
// import GenString    from "./GenString";
// import String       from "../LvString";
// import Number       from "../LvNumber";
