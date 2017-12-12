import DeclTypesForRW from "../Core/DeclTypesForRW";
import UsrId          from "../../System/UsrId";
import OtWrapper      from "../Ot/OtWrapper";
import OtMerger       from "../Ot/OtMerger";
import Method         from "../Core/Method";

//
export default
class GenArray extends OtWrapper {
    // patch types
    static op_insert = {
        attrs: [ "PT", "Array<Variable>" ],
        right( rp: GenArray, flags : number, pos : number, val : Variable[] ) : boolean {
            return GenArray.can_insert.ok( flags ) || ( pos == rp.data.length && GenArray.can_append.ok( flags ) );
        },
        valid( rp: GenArray, pos : number, v : Variable[] ) : boolean {
            return v.length != 0;
        },
        apply_op( rp: GenArray, aod, pos : number, val : Variable[] ) : void {
            for( let v of val ) {
                let changes = new BinaryWriter;
                v.pull_ot_changes( changes );
                v.onchange_par( new Variable( rp ), v );
            }
            rp.data = rp.data.slice( 0, pos ).concat( val ).concat( rp.data.slice( pos ) );
            rp.sig_change();
        },
        undo( rp: GenArray, aod, pos : number, val : Variable[] ) : void {
            throw new Error( "TODO" );
        },
        update_patch_data_l0( rp: GenArray, args ) : void {
        }
    };
    static op_remove = {
        attrs: [ "PT", "PT" ],
        right: function( rp: GenArray, flags : number, pos : number, len : number ) : boolean {
            return GenArray.can_remove.ok( flags );
        },
        valid( rp: GenArray, pos : number, len : number ) : boolean {
            return len != 0;
        },
        apply_op: function( rp: GenArray, aod, pos : number, len : number ) : void {
            for( let i = pos; i < pos + len; ++i ) {
                let j = rp.mod_nodes.indexOf( rp.data[ i ] );
                if ( j >= 0 )
                    rp.mod_nodes.splice( j, 1 );
            }
            rp.data.splice( pos, len );
            rp.sig_change();
        },
        undo: function( rp: GenArray, aod, pos : number, len : number ) : void {
            throw new Error( "TODO" );
        },
        update_patch_data_l0: function( rp: GenArray, args ) : void {
        }
    };
    static op_remove_und = {
        attrs: [ "PT", "Array<Variable>" ],
        right: function( rp: GenArray, flags : number, pos : number, items : Variable[] ) : boolean {
            return GenArray.can_remove.ok( flags );
        },
        valid( rp: GenArray, pos : number, items : Variable[] ) : boolean {
            return items.length != 0;
        },
        apply_op: function( rp: GenArray, aod, pos : number, items : Variable[] ) : void {
            GenArray.op_remove.apply_op( rp, aod, pos, items.length );
        },
        undo: function( rp: GenArray, aod, pos : number, items : Variable[] ) : void {
            throw new Error( "TODO" );
        },
        update_patch_data_l0: function( rp: GenArray, arg ) : void {
        }
    }
    static op_mod_item = {
        attrs: [ "PT", "Buffer" ],
        right: function( rp: GenArray, flags : number, pos : number, msg : Uint8Array ) : boolean {
            return GenArray.can_modify.ok( flags );
        },
        valid( rp: GenArray, pos : number, msg : Uint8Array ) : boolean {
            return msg.byteLength != 0;
        },
        apply_op: function( rp: GenArray, aod, pos : number, msg : Uint8Array ) : void {
            let pm = rp.data[ pos ].new_ot_merger();
            pm.apply_op( aod, msg );
        },
        undo: function( rp: GenArray, aod, pos : number, msg : Uint8Array ) : void {
            let pm = rp.data[ pos ].new_ot_merger();
            pm.undo_patch( msg, aod.usr );
        },
        update_patch_data_l0: function( rp: GenArray, arg ) : void {
        }
    }

    static op_types = OtWrapper.op_types.concat( [
        GenArray.op_insert,
        GenArray.op_remove,
        GenArray.op_remove_und,
        GenArray.op_mod_item
    ] );

    // right types
    static can_read = {
        flags: 4,
        name: "read",
        ok( flags : number ) : boolean { return ( flags & 4 ) != 0; }
    }
    static can_insert = {
        flags: 8,
        name: "insert",
        ok( flags : number ) : boolean { return ( flags & 8 ) != 0; }
    }
    static can_remove = {
        flags: 16,
        name: "remove",
        ok( flags : number ) : boolean { return ( flags & 16 ) != 0; }
    }
    static can_append = {
        flags: 32,
        name: "append",
        ok( flags : number ) : boolean { return ( flags & 32 ) != 0; }
    }
    static can_modify = {
        flags: 64,
        name: "modify",
        ok( flags : number ) : boolean { return ( flags & 64 ) != 0; }
    }

    static right_types = OtWrapper.right_types.concat( [
        GenArray.can_read,
        GenArray.can_insert,
        GenArray.can_remove,
        GenArray.can_append,
        GenArray.can_modify
    ] );

    constructor( array, usr_id = new UsrId ) {
        super( GenArray );

        this.data = [];
        for( let v of array )
            this.data.push( Variable.rp_conv_cp( v ) );

        let va = new Variable( this );
        for( let d of this.data )
            d.onchange_par( va, d );

        this.mod_nodes = [];
    }

    to_String() : string {
        return `[${ this.data }]`
    }

    to_Array() : Array<any> {
        let res = [];
        for( let v of this.data )
            res.push( (<any>v ).val );
        return res;
    }

    get_size() : Number {
         return new Number( this.data.length );
    }

    copy() : Variable {
         return new _Array<any>( this.data );
    }

    write_to( bw : BinaryWriter, only_for_a_copy : boolean,  cur_dev : DevId, cur_usr : UsrId ) : void {
        bw.write_PT( DeclTypesForRW.num.GenArray );
        bw.write_list( this.data, only_for_a_copy,  cur_dev, cur_usr );
    }

    static read_GenArray_from( br : BinaryReader, dev_id : DevId, usr_id : UsrId ) : _Array<Variable> {
        return new _Array<Variable>( br.read_list( Variable, dev_id, usr_id ) );
    }

    get_patch_type( bw : BinaryWriter ) {
        bw.write_PT( PatchTypes.num.GenArray );
    }

    cmp_patch_type( br : BinaryReader ) : boolean {
        return br.read_PT() == PatchTypes.num.GenArray;
    }

    obj_type() {
        return Array;
    }

    sig_change( rp? ) {
        if ( rp != null && this.mod_nodes.indexOf( rp ) < 0 )
            this.mod_nodes.push( rp );
        OtWrapper.prototype.sig_change.call( this, rp );
    }

    freeze_last_changes() {
        if ( this.mod_nodes.length ) {
            for( let m of this.mod_nodes ) {
                let pos = this.data.indexOf( m );
                let d = new BinaryWriter;
                m.pull_ot_changes( d );
                if ( d.size && pos >= 0 )
                    GenArray.reg_op( this.last_ops, GenArray.op_mod_item, pos, d.to_Uint8Array() );
            }
            this.mod_nodes = [];
        }
    }

    // beware: items are owned by this
    _self_insert( pos : number, items : Variable[], usr_id = this.usr_id ) : boolean {
        if ( GenArray.op_insert.right( this, this.usr_right( usr_id ), pos, items ) ) {
            let changes = new BinaryWriter;
            for( let v of items )
                v.pull_ot_changes( changes );
            GenArray.reg_op( this.last_ops, GenArray.op_insert, pos, items );
            GenArray.op_insert.apply_op( this, { usr: usr_id, type: 0 }, pos, items );
            return true;
        }
        return false;
    }

    _self_remove( pos, len, usr_id = this.usr_id ) : boolean {
        if ( len && GenArray.op_remove.right( this, this.usr_right( usr_id ), pos, len ) ){
            let lst = this.data.slice( pos, pos + len );
            GenArray.reg_op( this.last_ops, GenArray.op_remove_und, pos, lst );
            GenArray.op_remove_und.apply_op( this, { usr: usr_id, type: 0 }, pos, lst );
            return true;
        }
        return false;
    }

    data      : Array<Variable>;
    mod_nodes : Array<Variable>;
}
OtWrapper.make_templ( GenArray );

DeclTypesForRW.readers[ DeclTypesForRW.num.GenArray ] = GenArray.read_GenArray_from;

// unk_new_or_new_unk
GenArray.reg_nuf( GenArray.op_insert, GenArray.op_insert, function( asu_n : UsrId, bq_n : BinaryWriter, bq_o : BinaryWriter, args_n, args_o ) {
    if ( args_o[ 0 ] > args_n[ 0 ] ) {
        // orig 01234
        // real 0123unk4    INS(args_o[ 0 ]=4,args_o[ 1 ]=unk)
        // imag 0new1234    INS(args_n[ 0 ]=1,args_n[ 1 ]=new)
        // obj  0new123unk4 (real -> obj = new: INS 1,new; imag -> obj = unk: INS 7,unk)
        args_o[ 0 ] += args_n[ 1 ].length;
    } else {
        // orig 01234
        // real 0123unk4    INS(args_o[ 0 ]=4,args_o[ 1 ]=unk)
        // imag 0new1234    INS(args_n[ 0 ]=1,args_n[ 1 ]=new)
        // obj  0new123unk4 (real -> obj = new: INS 1,new; imag -> obj = unk: INS 7,unk)
        args_n[ 0 ] += args_o[ 1 ].length;
    }
} );

GenArray.reg_nuf( GenArray.op_insert, GenArray.op_remove, function( asu_n : UsrId, bq_n : BinaryWriter, bq_o : BinaryWriter, args_n, args_o ) {
    if ( args_n[ 0 ] >= args_o[ 0 ] + args_o[ 1 ] ) {
        // orig 012345
        // real 0345      REM(args_o[ 0 ]=1,args_o[ 1 ]=2)
        // imag 01234new5 INS(args_n[ 0 ]=5,args_n[ 1 ]=new)
        // obj  034new5   (real -> obj = new: INS 3,new; imag -> obj = unk: REM 1,2)
        args_n[ 0 ] -= args_o[ 1 ]; // 3
    } else if ( args_n[ 0 ] <= args_o[ 0 ] ) {
        // orig 012345
        // real 0125      REM(args_o[ 0 ]=3,args_o[ 1 ]=2)
        // imag 0new12345 INS(args_n[ 0 ]=1,args_n[ 1 ]=new)
        // obj  0new125   (real -> obj = new: INS 1,new; imag -> obj = unk: REM 6,2)
        args_o[ 0 ] += args_n[ 1 ].length;
    } else {
        // orig 012345
        // real 05        REM(args_o[ 0 ]=1,args_o[ 1 ]=4)
        // imag 012new345 INS(args_n[ 0 ]=3,args_n[ 1 ]=new)
        // obj  0new5     (real -> obj = new: INS 1,new; imag -> obj = unk: REM 1,2 + REM 4,2)
        GenArray.reg_op( bq_o, GenArray.op_remove, args_o[ 0 ], args_n[ 0 ] - args_o[ 0 ] ); // 1, 2
        args_o[ 1 ] -= args_n[ 0 ] - args_o[ 0 ];  // 2
        args_n[ 0 ] = args_o[ 0 ];                 // 1
        args_o[ 0 ] += args_n[ 1 ].length;         // 4
    }
} );


GenArray.reg_nuf( GenArray.op_insert, GenArray.op_remove_und, function( asu_n : UsrId, bq_n : BinaryWriter, bq_o : BinaryWriter, args_n, args_o ) {
    if ( args_n[ 0 ] >= args_o[ 0 ] + args_o[ 1 ].length ) {
        // orig 012345
        // real 0345      REM(args_o[ 0 ]=1,args_o[ 1 ]=2)
        // imag 01234new5 INS(args_n[ 0 ]=5,args_n[ 1 ]=new)
        // obj  034new5   (real -> obj = new: INS 3,new; imag -> obj = unk: REM 1,2)
        args_n[ 0 ] -= args_o[ 1 ].length; // 3
    } else if ( args_n[ 0 ] <= args_o[ 0 ] ) {
        // orig 012345
        // real 0125      REM(args_o[ 0 ]=3,args_o[ 1 ]=2)
        // imag 0new12345 INS(args_n[ 0 ]=1,args_n[ 1 ]=new)
        // obj  0new125   (real -> obj = new: INS 1,new; imag -> obj = unk: REM 6,2)
        args_o[ 0 ] += args_n[ 1 ].length;
    } else {
        // orig 012345
        // real 05        REM(args_o[ 0 ]=1,args_o[ 1 ]=4)
        // imag 012new345 INS(args_n[ 0 ]=3,args_n[ 1 ]=new)
        // obj  0new5     (real -> obj = new: INS 1,new; imag -> obj = unk: REM 1,'01' + REM 4,'34')
        GenArray.reg_op( bq_o, GenArray.op_remove_und, args_o[ 0 ], args_o[ 1 ].slice( 0, args_n[ 0 ] - args_o[ 0 ] ) ); // 1, '12'
        args_o[ 1 ] = args_o[ 1 ].substr( args_n[ 0 ] - args_o[ 0 ] );  // '34'
        args_n[ 0 ] = args_o[ 0 ];                                      // 1
        args_o[ 0 ] += args_n[ 1 ].length;                              // 4
    }
} );

GenArray.reg_nuf( GenArray.op_remove, GenArray.op_insert, function( asu_n : UsrId, bq_n : BinaryWriter, bq_o : BinaryWriter, args_n, args_o ) {
    if ( args_o[ 0 ] <= args_n[ 0 ] ) {
        // orig 01234567
        // real 01ab234567 INS(args_o[ 0 ]=2,args_o[ 1 ]=ab)
        // imag 012347     REM(args_n[ 0 ]=5,args_n[ 1 ]=2)
        // obj  01ab2347   (real -> obj = new: REM 7,2; imag -> obj = unk: INS 2,ab)
        args_n[ 0 ] += args_o[ 1 ].length;
    } else if ( args_o[ 0 ] >= args_n[ 0 ] + args_n[ 1 ] ) {
        // orig 01234567
        // real 01234ab567 INS(args_o[ 0 ]=5,args_o[ 1 ]=ab)
        // imag 014567     REM(args_n[ 0 ]=2,args_n[ 1 ]=2)
        // obj  014ab567   (real -> obj = new: REM 2,2; imag -> obj = unk: INS 3,ab)
        args_o[ 0 ] -= args_n[ 1 ];
    } else {
        // orig 01234567
        // real 0123ab4567 INS(args_o[ 0 ]=4,args_o[ 1 ]=ab)
        // imag 0167       REM(args_n[ 0 ]=2,args_n[ 1 ]=4)
        // obj  01ab67     (real -> obj = new: REM 2,2 + REM 4,2; imag -> obj = unk: INS 2,ab)
        GenArray.reg_op( bq_n, GenArray.op_remove_und, args_n[ 0 ], <GenArray>this._substring( args_n[ 0 ], args_o[ 0 ] - args_n[ 0 ] ) ); // 2, 2
        GenArray.op_remove.apply_op( <GenArray>this, { usr: asu_n }, args_n[ 0 ], args_o[ 0 ] - args_n[ 0 ] );
        args_n[ 1 ] -= args_o[ 0 ] - args_n[ 0 ]; // 2
        args_o[ 0 ] = args_n[ 0 ];                // 2
        args_n[ 0 ] += args_o[ 1 ].length;        // 4
    }
} );

GenArray.reg_nuf( GenArray.op_remove, GenArray.op_remove, function( asu_n : UsrId, bq_n : BinaryWriter, bq_o : BinaryWriter, args_n, args_o ) {
    if ( args_o[ 0 ] <= args_n[ 0 ] ) {
        if ( args_o[ 0 ] + args_o[ 1 ] <= args_n[ 0 ] ) {
            // orig 0123456789
            // real 0456789    REM(args_o[ 0 ]=1,args_o[ 1 ]=3)
            // imag 01234589   REM(args_n[ 0 ]=6,args_n[ 1 ]=2)
            // obj  04589      (real -> obj = new: REM 3,2; imag -> obj = unk: REM 1,3)
            args_n[ 0 ] -= args_o[ 1 ]; // 3
        } else if ( args_o[ 0 ] + args_o[ 1 ] <= args_n[ 0 ] + args_n[ 1 ] ) {
            // orig 0123456789
            // real 06789      REM(args_o[ 0 ]=1,args_o[ 1 ]=5)
            // imag 012389     REM(args_n[ 0 ]=4,args_n[ 1 ]=4)
            // obj  089        (real -> obj = new: REM 1,2; imag -> obj = unk: REM 1,3)
            let d = args_o[ 0 ] + args_o[ 1 ] - args_n[ 0 ]; // 2
            args_n[ 1 ] -= d;  // 2
            args_o[ 1 ] -= d;  // 3
            args_n[ 0 ] = args_o[ 0 ]; // 1
        } else {
            // orig 0123456789
            // real 09         REM(args_o[ 0 ]=1,args_o[ 1 ]=8)
            // imag 01236789   REM(args_n[ 0 ]=4,args_n[ 1 ]=2)
            // obj  09         (real -> obj = new: VOID; imag -> obj = unk: REM 1,6)
            args_o[ 1 ] -= args_n[ 1 ];
            args_n[ 1 ] = 0;
        }
    } else {
        if ( args_n[ 0 ] + args_n[ 1 ] <= args_o[ 0 ] ) {
            // orig 0123456789
            // real 01234589   REM(args_o[ 0 ]=6,args_o[ 1 ]=2)
            // imag 0456789    REM(args_n[ 0 ]=1,args_n[ 1 ]=3)
            // obj  04589      (real -> obj = new: REM 1,3; imag -> obj = unk: REM 3,2)
            args_o[ 0 ] -= args_n[ 1 ]; // 3
        } else if ( args_n[ 0 ] + args_n[ 1 ] <= args_o[ 0 ] + args_o[ 1 ] ) {
            // orig 0123456789
            // real 012389     REM(args_o[ 0 ]=4,args_o[ 1 ]=4)
            // imag 06789      REM(args_n[ 0 ]=1,args_n[ 1 ]=5)
            // obj  089        (real -> obj = new: REM 1,3; imag -> obj = unk: REM 1,2)
            let d = args_n[ 0 ] + args_n[ 1 ] - args_o[ 0 ]; // 2
            args_o[ 1 ] -= d;  // 2
            args_n[ 1 ] -= d;  // 3
            args_o[ 0 ] = args_n[ 0 ]; // 1
        } else {
            // orig 0123456789
            // real 01236789   REM(args_o[ 0 ]=4,args_o[ 1 ]=2)
            // imag 09         REM(args_n[ 0 ]=1,args_n[ 1 ]=8)
            // obj  09         (real -> obj = new: REM 1,6; imag -> obj = unk: VOID)
            args_n[ 1 ] -= args_o[ 1 ];
            args_o[ 1 ] = 0;
        }
    }
} );

GenArray.reg_nuf( GenArray.op_remove, GenArray.op_remove_und, function( asu_n : UsrId, bq_n : BinaryWriter, bq_o : BinaryWriter, args_n, args_o ) {
    if ( args_o[ 0 ] <= args_n[ 0 ] ) {
        if ( args_o[ 0 ] + args_o[ 1 ].length <= args_n[ 0 ] ) {
            // orig 0123456789
            // real 0456789    REM(args_o[ 0 ]=1,args_o[ 1 ]='123')
            // imag 01234589   REM(args_n[ 0 ]=6,args_n[ 1 ]=2)
            // obj  04589      (real -> obj = new: REM 3,2; imag -> obj = unk: REM_UND 1,'123')
            args_n[ 0 ] -= args_o[ 1 ].length; // 3
        } else if ( args_o[ 0 ] + args_o[ 1 ].length <= args_n[ 0 ] + args_n[ 1 ] ) {
            // orig 0123456789
            // real 06789      REM_UND(args_o[ 0 ]=1,args_o[ 1 ]='12345')
            // imag 012389     REM(args_n[ 0 ]=4,args_n[ 1 ]=4)
            // obj  089        (real -> obj = new: REM 1,2; imag -> obj = unk: REM_UND 1,'123')
            let d = args_o[ 0 ] + args_o[ 1 ].length - args_n[ 0 ]; // 2
            args_o[ 1 ] = args_o[ 1 ].slice( 0, args_o[ 1 ].length - d ); // '123'
            args_n[ 1 ] -= d;  // 2
            args_n[ 0 ] = args_o[ 0 ]; // 1
        } else {
            // orig 0123456789
            // real 09         REM_UND(args_o[ 0 ]=1,args_o[ 1 ]='12345678')
            // imag 01236789   REM(args_n[ 0 ]=4,args_n[ 1 ]=2)
            // obj  09         (real -> obj = new: VOID; imag -> obj = unk: REM_UND 1,'123678')
            let t = args_o[ 1 ].substr( 0, args_n[ 0 ] - args_o[ 0 ] ) +
                    args_o[ 1 ].substr( args_n[ 0 ] - args_o[ 0 ] + args_n[ 1 ] );
            args_o[ 1 ] = t;
            args_n[ 1 ] = 0;
        }
    } else {
        if ( args_n[ 0 ] + args_n[ 1 ] <= args_o[ 0 ] ) {
            // orig 0123456789
            // real 01234589   REM_UND(args_o[ 0 ]=6,args_o[ 1 ]='67')
            // imag 0456789    REM(args_n[ 0 ]=1,args_n[ 1 ]=3)
            // obj  04589      (real -> obj = new: REM 1,3; imag -> obj = unk: REM_UND 3,'67')
            args_o[ 0 ] -= args_n[ 1 ]; // 3
        } else if ( args_n[ 0 ] + args_n[ 1 ] <= args_o[ 0 ] + args_o[ 1 ].length ) {
            // orig 0123456789
            // real 012389     REM_UND(args_o[ 0 ]=4,args_o[ 1 ]='4567')
            // imag 06789      REM(args_n[ 0 ]=1,args_n[ 1 ]=5)
            // obj  089        (real -> obj = new: REM 1,3; imag -> obj = unk: REM_UND 1,'67')
            let d = args_n[ 0 ] + args_n[ 1 ] - args_o[ 0 ]; // 2
            args_o[ 1 ] = args_o[ 1 ].substr( d ); // '67'
            args_n[ 1 ] -= d;  // 3
            args_o[ 0 ] = args_n[ 0 ]; // 1
        } else {
            // orig 0123456789
            // real 01236789   REM(args_o[ 0 ]=4,args_o[ 1 ].length=2)
            // imag 09         REM(args_n[ 0 ]=1,args_n[ 1 ]=8)
            // obj  09         (real -> obj = new: REM 1,6; imag -> obj = unk: VOID)
            args_n[ 1 ] -= args_o[ 1 ].length;
            args_o[ 1 ] = "";
        }
    }
} );

GenArray.reg_nuf( GenArray.op_remove_und, GenArray.op_insert, function( asu_n : UsrId, bq_n : BinaryWriter, bq_o : BinaryWriter, args_n, args_o ) {
    if ( args_o[ 0 ] <= args_n[ 0 ] ) {
        // orig 01234567
        // real 01ab234567 INS(args_o[ 0 ]=2,args_o[ 1 ]=ab)
        // imag 012347     REM(args_n[ 0 ]=5,args_n[ 1 ]=2)
        // obj  01ab2347   (real -> obj = new: REM 7,2; imag -> obj = unk: INS 2,ab)
        args_n[ 0 ] += args_o[ 1 ].length;
    } else if ( args_o[ 0 ] >= args_n[ 0 ] + args_n[ 1 ].length ) {
        // orig 01234567
        // real 01234ab567 INS(args_o[ 0 ]=5,args_o[ 1 ]=ab)
        // imag 014567     REM(args_n[ 0 ]=2,args_n[ 1 ]=2)
        // obj  014ab567   (real -> obj = new: REM 2,2; imag -> obj = unk: INS 3,ab)
        args_o[ 0 ] -= args_n[ 1 ].length;
    } else {
        // orig 01234567
        // real 0123ab4567 INS(p_1=4,d_1=ab)
        // imag 0167       REM_UND(p_0=2,d_0=2345)
        // obj  01ab67     (real -> obj = unk: REM_UND 2,23 + REM_UND 4,45; imag -> obj = new: INS 2,ab)
        GenArray.reg_op( bq_n, GenArray.op_remove_und, args_n[ 0 ], args_n[ 1 ].substr( 0, args_o[ 0 ] - args_n[ 0 ] ) ); // 2, 23
        GenArray.op_remove.apply_op( this, { usr: asu_n }, args_n[ 0 ], args_o[ 0 ] - args_n[ 0 ] );
        args_n[ 1 ] = args_n[ 1 ].substr( args_o[ 0 ] - args_n[ 0 ] ); // 45
        args_o[ 0 ] = args_n[ 0 ];                                     // 2
        args_n[ 0 ] += args_o[ 1 ].length;                             // 4
    }
} );

GenArray.reg_nuf( GenArray.op_remove_und, GenArray.op_remove, function( asu_n : UsrId, bq_n : BinaryWriter, bq_o : BinaryWriter, args_n, args_o ) {
    if ( args_o[ 0 ] <= args_n[ 0 ] ) {
        if ( args_o[ 0 ] + args_o[ 1 ] <= args_n[ 0 ] ) {
            // orig 0123456789
            // real 0456789    REM(args_o[ 0 ]=1,args_o[ 1 ]=3)
            // imag 01234589   REM_UND(args_n[ 0 ]=6,args_n[ 1 ]='67')
            // obj  04589      (real -> obj = new: REM 3,2; imag -> obj = _n: REM 1,3)
            args_n[ 0 ] -= args_o[ 1 ]; // 3
        } else if ( args_o[ 0 ] + args_o[ 1 ] <= args_n[ 0 ] + args_n[ 1 ].length ) {
            // orig 0123456789
            // real 06789      REM(args_o[ 0 ]=1,args_o[ 1 ]=5)
            // imag 012389     REM_UND(args_n[ 0 ]=4,args_n[ 1 ]='4567')
            // obj  089        (real -> obj = _n: REM_UND 1,'67'; imag -> obj = _o: REM_UND 1,'123')
            let d = args_o[ 0 ] + args_o[ 1 ] - args_n[ 0 ]; // 2
            args_o[ 1 ] -= d; // '123'
            args_n[ 1 ] = args_n[ 1 ].substr( d );  // '67'
            args_n[ 0 ] = args_o[ 0 ]; // 1
        } else {
            // orig 0123456789
            // real 09         REM(args_o[ 0 ]=1,args_o[ 1 ]=8)
            // imag 01236789   REM_UND(args_n[ 0 ]=4,args_n[ 1 ]='45')
            // obj  09         (real -> obj = _n: VOID; imag -> obj = _n: REM 1,'123678')
            args_o[ 1 ] -= args_n[ 1 ].length;
            args_n[ 1 ].free();
        }
    } else {
        if ( args_n[ 0 ] + args_n[ 1 ].length <= args_o[ 0 ] ) {
            // orig 0123456789
            // real 01234589   REM(args_o[ 0 ]=6,args_o[ 1 ]=2)
            // imag 0456789    REM_UND(args_n[ 0 ]=1,args_n[ 1 ]='123')
            // obj  04589      (real -> obj = _n: REM 1,'123'; imag -> obj = _n: REM 3,2)
            args_o[ 0 ] -= args_n[ 1 ].length; // 3
        } else if ( args_n[ 0 ] + args_n[ 1 ].length <= args_o[ 0 ] + args_o[ 1 ] ) {
            // orig 0123456789
            // real 012389     REM(args_o[ 0 ]=4,args_o[ 1 ]=4)
            // imag 06789      REM_UND(args_n[ 0 ]=1,args_n[ 1 ]='12345')
            // obj  089        (real -> obj = _n: REM 1,'123'; imag -> obj = _n: REM 1,2)
            let d = args_n[ 0 ] + args_n[ 1 ].length - args_o[ 0 ]; // 2
            args_o[ 1 ] -= d;  // 2
            args_n[ 1 ] = args_n[ 1 ].slice( 0, args_n[ 1 ].length - d );  // '123'
            args_o[ 0 ] = args_n[ 0 ]; // 1
        } else {
            // orig 0123456789
            // real 01236789   REM(args_o[ 0 ]=4,args_o[ 1 ]=2)
            // imag 09         REM_UND(args_n[ 0 ]=1,args_n[ 1 ]='12345678')
            // obj  09         (real -> obj = _n: REM 1,'123678'; imag -> obj = _n: VOID)
            let t = args_n[ 1 ].substr( 0, args_o[ 0 ] - args_n[ 0 ] ) +
                    args_n[ 1 ].substr( args_o[ 0 ] - args_n[ 0 ] + args_o[ 1 ] );
            args_n[ 1 ] = t;
            args_o[ 1 ] = 0;
        }
    }
} );

GenArray.reg_nuf( GenArray.op_remove_und, GenArray.op_remove_und, function( asu_n : UsrId, bq_n : BinaryWriter, bq_o : BinaryWriter, args_n, args_o ) {
    if ( args_o[ 0 ] <= args_n[ 0 ] ) {
        if ( args_o[ 0 ] + args_o[ 1 ].length <= args_n[ 0 ] ) {
            // orig 0123456789
            // real 0456789    REM(args_o[ 0 ]=1,args_o[ 1 ]='123')
            // imag 01234589   REM(args_n[ 0 ]=6,args_n[ 1 ]='67')
            // obj  04589      (real -> obj = _n: REM_UND 3,2; imag -> obj = _o: REM_UND 1,'123')
            args_n[ 0 ] -= args_o[ 1 ].length; // 3
        } else if ( args_o[ 0 ] + args_o[ 1 ].length <= args_n[ 0 ] + args_n[ 1 ].length ) {
            // orig 0123456789
            // real 06789      REM_UND(args_o[ 0 ]=1,args_o[ 1 ]='12345')
            // imag 012389     REM(args_n[ 0 ]=4,args_n[ 1 ]='4567')
            // obj  089        (real -> obj = _n: REM_UND 1,'67'; imag -> obj = _o: REM_UND 1,'123')
            let d = args_o[ 0 ] + args_o[ 1 ].length - args_n[ 0 ]; // 2
            args_o[ 1 ] = args_o[ 1 ].substr( 0, args_o[ 1 ].length - d ); // '123'
            args_n[ 1 ] = args_n[ 1 ].substr( d );  // '67'
            args_n[ 0 ] = args_o[ 0 ]; // 1
        } else {
            // orig 0123456789
            // real 09         REM_UND(args_o[ 0 ]=1,args_o[ 1 ]='12345678')
            // imag 01236789   REM(args_n[ 0 ]=4,args_n[ 1 ].length=2)
            // obj  09         (real -> obj = _n: VOID; imag -> obj = _o: REM_UND 1,'123678')
            let t = args_o[ 1 ].substr( 0, args_n[ 0 ] - args_o[ 0 ] ) +
                    args_o[ 1 ].substr( args_n[ 0 ] - args_o[ 0 ] + args_n[ 1 ].length );
            args_o[ 1 ] = t;
            args_n[ 1 ] = "";
        }
    } else {
        if ( args_n[ 0 ] + args_n[ 1 ].length <= args_o[ 0 ] ) {
            // orig 0123456789
            // real 01234589   REM_UND(args_o[ 0 ]=6,args_o[ 1 ]='67')
            // imag 0456789    REM(args_n[ 0 ]=1,args_n[ 1 ].length='123')
            // obj  04589      (real -> obj = _n: REM_UND 1,'123'; imag -> obj = _o: REM_UND 3,'67')
            args_o[ 0 ] -= args_n[ 1 ].length; // 3
        } else if ( args_n[ 0 ] + args_n[ 1 ].length <= args_o[ 0 ] + args_o[ 1 ].length ) {
            // orig 0123456789
            // real 012389     REM_UND(args_o[ 0 ]=4,args_o[ 1 ]='4567')
            // imag 06789      REM(args_n[ 0 ]=1,args_n[ 1 ]='12345')
            // obj  089        (real -> obj = _n: REM_UND 1,'123'; imag -> obj = _o: REM_UND 1,'67')
            let d = args_n[ 0 ] + args_n[ 1 ].length - args_o[ 0 ]; // 2
            args_o[ 1 ] = args_o[ 1 ].substr( d ); // '67'
            args_n[ 1 ] = args_n[ 1 ].substr( 0, args_n[ 1 ].length - d ); // '123'
            args_o[ 0 ] = args_n[ 0 ]; // 1
        } else {
            // orig 0123456789
            // real 01236789   REM(args_o[ 0 ]=4,args_o[ 1 ]='45')
            // imag 09         REM(args_n[ 0 ]=1,args_n[ 1 ]='12345678')
            // obj  09         (real -> obj = _n: REM_UND 1,'123678'; imag -> obj = _o: VOID)
            let t = args_n[ 1 ].substr( 0, args_o[ 0 ] - args_n[ 0 ] ) +
                    args_n[ 1 ].substr( args_o[ 0 ] - args_n[ 0 ] + args_o[ 1 ].length ); // p_o - p_n + d_o.length
            args_n[ 1 ] = t;
            args_o[ 1 ] = "";
        }
    }
} );


GenArray.reg_usc( GenArray.op_insert, GenArray.op_insert, function( asu_s : UsrId, bq_u : BinaryWriter, args_s, args_u ) {
    if ( args_u[ 0 ] > args_s[ 0 ] ) {
        //  orig 012
        //  +unk 01a2  INS args_u[ 0 ]=2,args_u[ 1 ]=a
        //  +std 0b1a2 INS args_s[ 0 ]=1,args_s[ 1 ]=b
        // same as
        //  +std 0b12  INS args_s[ 0 ]=1,args_s[ 1 ]=b
        //  +unk 0b1a2 INS args_u[ 0 ]=3,args_u[ 1 ]=a
        args_u[ 0 ] += args_s[ 1 ].length;
    } else {
        //  orig 012
        //  +unk 0a12  INS args_u[ 0 ]=1,args_u[ 1 ]=a
        //  +std 0a1b2 INS args_s[ 0 ]=3,args_s[ 1 ]=b
        // same as
        //  +std 01b2  INS args_s[ 0 ]=2,args_s[ 1 ]=b
        //  +unk 0a1b2 INS args_u[ 0 ]=1,args_u[ 1 ]=a
        args_s[ 0 ] -= args_u[ 1 ].length;
    }
} );

GenArray.reg_usc( GenArray.op_insert, GenArray.op_remove,  function( asu_s : UsrId, bq_u : BinaryWriter, args_s, args_u ) {
    if ( args_s[ 0 ] > args_u[ 0 ] ) {
        //  orig 012345678
        //  +unk 045678     REM args_u[ 0 ]=1,args_u[ 1 ]=3
        //  +std 04b5678    INS args_s[ 0 ]=2,args_s[ 1 ]=b
        // same as
        //  +std 01234b5678 INS args_s[ 0 ]=5,args_s[ 1 ]=b
        //  +unk 04b5678    REM args_u[ 0 ]=3,args_u[ 1 ]=a
        args_s[ 0 ] += args_u[ 1 ];
    } else {
        //  orig 012345678
        //  +unk 012348     REM args_u[ 0 ]=5,args_u[ 1 ]=3
        //  +std 01b2348    INS args_s[ 0 ]=2,args_s[ 1 ]=b
        // same as
        //  +std 01b2345678 INS args_s[ 0 ]=5,args_s[ 1 ]=b
        //  +unk 01b2348    REM args_u[ 0 ]=6,args_u[ 1 ]=a
        args_u[ 0 ] += args_s[ 1 ].length;
    }
} );

GenArray.reg_usc( GenArray.op_insert, GenArray.op_remove_und,  function( asu_s : UsrId, bq_u : BinaryWriter, args_s, args_u ) {
    if ( args_s[ 0 ] > args_u[ 0 ] ) {
        //  orig 012345678
        //  +unk 045678     REM args_u[ 0 ]=1,args_u[ 1 ]=3
        //  +std 04b5678    INS args_s[ 0 ]=2,args_s[ 1 ]=b
        // same as
        //  +std 01234b5678 INS args_s[ 0 ]=5,args_s[ 1 ]=b
        //  +unk 04b5678    REM args_u[ 0 ]=3,args_u[ 1 ]=a
        args_s[ 0 ] += args_u[ 1 ].length;
    } else {
        //  orig 012345678
        //  +unk 012348     REM args_u[ 0 ]=5,args_u[ 1 ]=3
        //  +std 01b2348    INS args_s[ 0 ]=2,args_s[ 1 ]=b
        // same as
        //  +std 01b2345678 INS args_s[ 0 ]=5,args_s[ 1 ]=b
        //  +unk 01b2348    REM args_u[ 0 ]=6,args_u[ 1 ]=a
        args_u[ 0 ] += args_s[ 1 ].length;
    }
} );


GenArray.reg_usc( GenArray.op_remove, GenArray.op_insert,  function( asu_s : UsrId, bq_u : BinaryWriter, args_s, args_u ) {
    if ( args_u[ 0 ] > args_s[ 0 ] ) {
        //  orig 012345678
        //  +unk 0123456b78 INS args_u[ 0 ]=7,args_u[ 1 ]=b
        //  +std 0456b78    REM args_s[ 0 ]=1,args_s[ 1 ]=3
        // same as
        //  +std 045678     REM args_s[ 0 ]=1,args_s[ 1 ]=3
        //  +unk 0456b78    INS args_u[ 0 ]=4,args_u[ 1 ]=b
        args_u[ 0 ] -= args_s[ 1 ];
    } else {
        //  orig 012345678
        //  +unk 01b2345678 INS args_u[ 0 ]=2,args_u[ 1 ]=b
        //  +std 01b2348    REM args_s[ 0 ]=6,args_s[ 1 ]=3
        // same as
        //  +std 012348     REM args_s[ 0 ]=5,args_s[ 1 ]=3
        //  +unk 01b2348    INS args_u[ 0 ]=2,args_u[ 1 ]=b
        args_s[ 0 ] -= args_u[ 1 ].length;
    }
} );

GenArray.reg_usc( GenArray.op_remove, GenArray.op_remove,  function( asu_s : UsrId, bq_u : BinaryWriter, args_s, args_u ) {
    if ( args_s[ 0 ] >= args_u[ 0 ] ) {
        //  orig 012345678
        //  +unk 0345678    REM args_u[ 0 ]=1,args_u[ 1 ]=2
        //  +std 03478      REM args_s[ 0 ]=3,args_s[ 1 ]=2
        // same as
        //  +std 0123478    REM args_s[ 0 ]=5,args_s[ 1 ]=3
        //  +unk 03478      REM args_u[ 0 ]=1,args_u[ 1 ]=2
        args_s[ 0 ] += args_u[ 1 ];
    } else if ( args_s[ 0 ] + args_s[ 1 ] <= args_u[ 0 ] ) {
        //  orig 012345678
        //  +unk 0123458    REM args_u[ 0 ]=6,args_u[ 1 ]=2
        //  +std 03458      REM args_s[ 0 ]=1,args_s[ 1 ]=2
        // same as
        //  +std 0345678    REM args_s[ 0 ]=1,args_s[ 1 ]=2
        //  +unk 03458      REM args_u[ 0 ]=4,args_u[ 1 ]=2
        args_u[ 0 ] -= args_s[ 1 ];
    } else {
        //  orig 012345678
        //  +unk 0123678    REM args_u[ 0 ]=4,args_u[ 1 ]=2
        //  +std 08         REM args_s[ 0 ]=1,args_s[ 1 ]=5
        // same as
        //  +std 0678       REM args_s[ 0 ]=1,args_s[ 1 ]=5
        //  +unk 08         REM args_u[ 0 ]=1,args_u[ 1 ]=2
        args_u[ 0 ] = args_s[ 0 ];
    }
} );

GenArray.reg_usc( GenArray.op_remove, GenArray.op_remove_und,  function( asu_s : UsrId, bq_u : BinaryWriter, args_s, args_u ) {
    if ( args_s[ 0 ] >= args_u[ 0 ] ) {
        //  orig 012345678
        //  +unk 0345678    REM args_u[ 0 ]=1,args_u[ 1 ]=2
        //  +std 03478      REM args_s[ 0 ]=3,args_s[ 1 ]=2
        // same as
        //  +std 0123478    REM args_s[ 0 ]=5,args_s[ 1 ]=3
        //  +unk 03478      REM args_u[ 0 ]=1,args_u[ 1 ]=2
        args_s[ 0 ] += args_u[ 1 ].length;
    } else if ( args_s[ 0 ] + args_s[ 1 ] <= args_u[ 0 ] ) {
        //  orig 012345678
        //  +unk 0123458    REM args_u[ 0 ]=6,args_u[ 1 ]=2
        //  +std 03458      REM args_s[ 0 ]=1,args_s[ 1 ]=2
        // same as
        //  +std 0345678    REM args_s[ 0 ]=1,args_s[ 1 ]=2
        //  +unk 03458      REM args_u[ 0 ]=4,args_u[ 1 ]=2
        args_u[ 0 ] -= args_s[ 1 ];
    } else {
        //  orig 012345678
        //  +unk 0123678    REM args_u[ 0 ]=4,args_u[ 1 ]=2
        //  +std 08         REM args_s[ 0 ]=1,args_s[ 1 ]=5
        // same as
        //  +std 0678       REM args_s[ 0 ]=1,args_s[ 1 ]=5
        //  +unk 08         REM args_u[ 0 ]=1,args_u[ 1 ]=2
        args_u[ 0 ] = args_s[ 0 ];
    }
} );

GenArray.reg_usc( GenArray.op_remove_und, GenArray.op_insert,  function( asu_s : UsrId, bq_u : BinaryWriter, args_s, args_u ) {
    if ( args_u[ 0 ] > args_s[ 0 ] ) {
        //  orig 012345678
        //  +unk 0123456b78 INS args_u[ 0 ]=7,args_u[ 1 ]=b
        //  +std 0456b78    REM args_s[ 0 ]=1,args_s[ 1 ]=3
        // same as
        //  +std 045678     REM args_s[ 0 ]=1,args_s[ 1 ]=3
        //  +unk 0456b78    INS args_u[ 0 ]=4,args_u[ 1 ]=b
        args_u[ 0 ] -= args_s[ 1 ].length;
    } else {
        //  orig 012345678
        //  +unk 01b2345678 INS args_u[ 0 ]=2,args_u[ 1 ]=b
        //  +std 01b2348    REM args_s[ 0 ]=6,args_s[ 1 ]=3
        // same as
        //  +std 012348     REM args_s[ 0 ]=5,args_s[ 1 ]=3
        //  +unk 01b2348    INS args_u[ 0 ]=2,args_u[ 1 ]=b
        args_s[ 0 ] -= args_u[ 1 ].length;
    }
} );

GenArray.reg_usc( GenArray.op_remove_und, GenArray.op_remove,  function( asu_s : UsrId, bq_u : BinaryWriter, args_s, args_u ) {
    if ( args_s[ 0 ] >= args_u[ 0 ] ) {
        //  orig 012345678
        //  +unk 0345678    REM args_u[ 0 ]=1,args_u[ 1 ]=2
        //  +std 03478      REM args_s[ 0 ]=3,args_s[ 1 ]=2
        // same as
        //  +std 0123478    REM args_s[ 0 ]=5,args_s[ 1 ]=3
        //  +unk 03478      REM args_u[ 0 ]=1,args_u[ 1 ]=2
        args_s[ 0 ] += args_u[ 1 ];
    } else if ( args_s[ 0 ] + args_s[ 1 ].length <= args_u[ 0 ] ) {
        //  orig 012345678
        //  +unk 0123458    REM args_u[ 0 ]=6,args_u[ 1 ]=2
        //  +std 03458      REM args_s[ 0 ]=1,args_s[ 1 ]=2
        // same as
        //  +std 0345678    REM args_s[ 0 ]=1,args_s[ 1 ]=2
        //  +unk 03458      REM args_u[ 0 ]=4,args_u[ 1 ]=2
        args_u[ 0 ] -= args_s[ 1 ].length;
    } else {
        //  orig 012345678
        //  +unk 0123678    REM args_u[ 0 ]=4,args_u[ 1 ]=2
        //  +std 08         REM args_s[ 0 ]=1,args_s[ 1 ]=5
        // same as
        //  +std 0678       REM args_s[ 0 ]=1,args_s[ 1 ]=5
        //  +unk 08         REM args_u[ 0 ]=1,args_u[ 1 ]=2
        args_u[ 0 ] = args_s[ 0 ];
    }
} );

GenArray.reg_usc( GenArray.op_remove_und, GenArray.op_remove_und,  function( asu_s : UsrId, bq_u : BinaryWriter, args_s, args_u ) {
    if ( args_s[ 0 ] >= args_u[ 0 ] ) {
        //  orig 012345678
        //  +unk 0345678    REM args_u[ 0 ]=1,args_u[ 1 ]=2
        //  +std 03478      REM args_s[ 0 ]=3,args_s[ 1 ]=2
        // same as
        //  +std 0123478    REM args_s[ 0 ]=5,args_s[ 1 ]=3
        //  +unk 03478      REM args_u[ 0 ]=1,args_u[ 1 ]=2
        args_s[ 0 ] += args_u[ 1 ].length;
    } else if ( args_s[ 0 ] + args_s[ 1 ].length <= args_u[ 0 ] ) {
        //  orig 012345678
        //  +unk 0123458    REM args_u[ 0 ]=6,args_u[ 1 ]=2
        //  +std 03458      REM args_s[ 0 ]=1,args_s[ 1 ]=2
        // same as
        //  +std 0345678    REM args_s[ 0 ]=1,args_s[ 1 ]=2
        //  +unk 03458      REM args_u[ 0 ]=4,args_u[ 1 ]=2
        args_u[ 0 ] -= args_s[ 1 ].length;
    } else {
        //  orig 012345678
        //  +unk 0123678    REM args_u[ 0 ]=4,args_u[ 1 ]=2
        //  +std 08         REM args_s[ 0 ]=1,args_s[ 1 ]=5
        // same as
        //  +std 0678       REM args_s[ 0 ]=1,args_s[ 1 ]=5
        //  +unk 08         REM args_u[ 0 ]=1,args_u[ 1 ]=2
        args_u[ 0 ] = args_s[ 0 ];
    }
} );



Method.methods["self_set_o"].add_surdef( 2, [ GenArray, GenArray ], function( arr : { rp: GenArray }, val : { rp: GenArray } ) {
    let ol = arr.rp.data.length;
    let nl = val.rp.data.length;
    if ( ol > nl )
        arr.rp._self_remove( nl, ol - nl );
    for( let i = 0; i < Math.min( ol, nl ); ++i )
        Method.methods["self_set_o"].call_2( arr.rp.data[ i ], val.rp.data[ i ] );
    if ( nl > ol )
        arr.rp._self_insert( ol, val.rp.data.slice( ol, nl ) );
} );

Method.methods["self_set_b"].add_surdef( 2, [ GenArray, GenArray ], function( arr : { rp: GenArray }, val : { rp: GenArray } ) {
    let ol = arr.rp.data.length;
    let nl = val.rp.data.length;
    if ( ol > nl )
        arr.rp._self_remove( nl, ol - nl );
    for( let i = 0; i < Math.min( ol, nl ); i += 1 )
        Method.methods["self_set_o"].call_2( arr.rp.data[ i ], (<any>val).rp.data[ i ].copy() );
    if ( nl > ol )
        arr.rp._self_insert( ol, val.rp.data.slice( ol, nl ) );
} );

Method.methods["self_insert_bo"].add_surdef( 2, [ GenArray, "to_Number", () => true ], function( str : { rp: GenArray }, pos : Number, item : Variable ) {
    return str.rp._self_insert( pos.toNumber(), [ item ] );
} );

Method.methods["self_remove_bb"].add_surdef( 2, [ GenArray, "to_Number", "to_Number" ], function( str : { rp: GenArray }, pos : Number, len : Number ) {
    return str.rp._self_remove( pos.toNumber(), len.toNumber() );
} );

Method.methods["mod_select"].add_surdef( 2, [ GenArray, "to_Number" ], function( arr : { rp: GenArray }, pos : Number ) : Variable {
    return arr.rp.data[ pos.toNumber() ];
} );

Method.methods["equ_immediate"].add_surdef( 2, [ GenArray, GenArray ], function( a : { rp: GenArray }, b : { rp: GenArray } ) : boolean {
    if ( a.rp.data.length != b.rp.data.length )
        return false;
    for( let i = 0; i < a.rp.data.length; ++i )
        if ( ! a.rp.data[ i ].equ_immediate( b.rp.data[ i ] ) )
            return false;
    return true;
} );


import BinaryWriter from "../../System/BinaryWriter";
import BinaryReader from "../../System/BinaryReader";
import PatchId      from "../../System/PatchId";
import DevId        from "../../System/DevId";
import PatchTypes   from "../Ot/PatchTypes";
import Variable     from "../Core/Variable";
import Number       from "../Number";
import _Array       from "../Array";
