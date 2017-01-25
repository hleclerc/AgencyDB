import RegCallbacks from "../System/RegCallbacks";
import BinaryWriter from "../System/BinaryWriter";
import BinaryReader from "../System/BinaryReader";
import PatchId      from "../System/PatchId";
import DevId        from "../System/DevId";
import UsrId        from "../System/UsrId";
import GrpId        from "../System/GrpId";
import PatchManager from "./PatchManager";
import OtMerger     from "./OtMerger";
import methods      from "./methods";
import Rp           from "./Rp";

// interface OtWrapperOp {
//     attrs               : Array<string>,
//     right               : ( OtWrapper, number, ...any ) => boolean,
//     valid               : ( OtWrapper, ...any         ) => boolean,
//     apply_op            : ( OtWrapper, ...any         ) => void,
//     undo                : ( OtWrapper, ...any         ) => void,
//     update_patch_data_l0: ( OtWrapper, any            ) => void
// }

//
abstract class OtWrapper extends Rp {
    static data_loss_if_prox = 10;
    static term_value        = true;

    static make_templ( cl ) {
        Rp.make_templ( cl );
    }

    constructor( cl, usr_id = new UsrId ) {
        super( cl );

        // this.on_change_callbacks = {};
        // this.on_change_par       = [];
        // this.last_ops            = new BinaryWriter;
        // this.usr_rights          = {}; // usr_id -> unsigned (right flags)
        // this.grp_rights          = {}; // grp_id -> unsigned (right flags)
        this.usr_id              = usr_id; // creator (who has all the right at the beginning)

        // this.usr_rights[ this.usr_id.toString() ] = ( 1 << cl.right_types.length ) - 1;
    }

    // write_OtWrapper_data_to( bw : BinaryWriter, only_for_a_copy : boolean, cur_dev : DevId, cur_usr : UsrId ) {
    //     bw.write_Uint8Array( this.last_ops.to_Uint8Array() );

    //     let nur = 0; for( let i in this.usr_rights ) ++nur;
    //     bw.write_PT( nur );
    //     for( let i in this.usr_rights ) {
    //         bw.write_UsrId( UsrId.fromString( i ), cur_dev, cur_usr );
    //         bw.write_PT( this.usr_rights[ i ] );
    //     }

    //     let ngr = 0; for( let i in this.grp_rights ) ++ngr;
    //     bw.write_PT( ngr );
    //     for( let i in this.grp_rights ) {
    //         bw.write_GrpId( GrpId.fromString( i ), cur_dev );
    //         bw.write_PT( this.grp_rights[ i ] );
    //     }

    //     bw.write_UsrId( this.usr_id, cur_dev, cur_usr );
    // }

    // static read_OtWrapper_data_from( res : OtWrapper, br : BinaryReader, cur_dev : DevId, cur_usr : UsrId ) : void {
    //     res.last_ops.write_some( br.read_Uint8Array() );

    //     for( let nur = br.read_PT(); nur--; ) {
    //         let usr = br.read_UsrId( cur_dev, cur_usr );
    //         res.usr_rights[ usr.toString() ] = br.read_PT();
    //     }

    //     for( let ngr = br.read_PT(); ngr--; ) {
    //         let grp = br.read_GrpId( cur_dev );
    //         res.grp_rights[ grp.toString() ] = br.read_PT();
    //     }

    //     res.usr_id = br.read_UsrId( cur_dev, cur_usr );
    // }

    // static skip_OtWrapper_data_from( br: BinaryReader ) {
    //     br.skip_Uint8Array();

    //     for( let nur = br.read_PT(); nur--; ) {
    //         br.skip_UsrId();
    //         br.skip_PT();
    //     }

    //     for( let ngr = br.read_PT(); ngr--; ) {
    //         br.skip_GrpId();
    //         br.skip_PT();
    //     }

    //     br.skip_UsrId();
    // }

    on_change__s( callback, immediate ): Rp {
        this.on_change.set( callback, immediate );
        return this;
    }

    rm_on_change__s( callback ) {
        this.on_change.delete( callback );
        return this;
    }

    add_patch_manager__s( patch_manager ): Rp {
        this.patch_managers.push( patch_manager );
        return this;
    }

    new_ot_merger__b( patch_manager: PatchManager ) {
        return new OtMerger( this, patch_manager );
    }

    // rm_on_change__s( callback_id: PatchId ) {
    //     delete this.on_change_callbacks[ callback_id.toString() ];
    // }

    // pull_ot_changes( changes ) {
    //     this.freeze_last_changes();
    //     changes.write_some( this.last_ops.to_Uint8Array() );
    //     this.last_ops.clear();
    // }

    // freeze_last_changes() {
    // }

    // usr_right( usr_id = new UsrId ) : number {
    //     let val = this.usr_rights[ usr_id.toString() ];
    //     return val != null ? val : 0;
    // }

    // grp_right( grp_id = new GrpId ) : number {
    //     let val = this.grp_rights[ grp_id.toString() ];
    //     return val != null ? val : 0;
    // }

    sig_change( wr: ( bw: BinaryWriter ) => void, src_patch_manager = null as PatchManager ) {
        // for( let p of this.on_change_par )
        //     p.par.sig_change( p.arg );
        // this.on_change.forEach( cb => cb(  ) );
        this.on_change.forEach( ( immediate, cb ) =>  {
            immediate ? cb() : RegCallbacks.add( cb );
        } );
        for( let pm of this.patch_managers )
            if ( pm != src_patch_manager )
                pm.new_modification( wr );
    }

    abstract new_patch ( res: BinaryWriter, msg: BinaryReader, as_usr: UsrId, cq_unk: BinaryWriter );
    abstract undo_patch( msg: BinaryReader, as_usr: UsrId );

    on_change      = new Map<() => void, boolean>();
    patch_managers = new Array<PatchManager>();
    // on_change_par       = new Array<{ par: Variable, arg: any }>();
    // last_ops            = new BinaryWriter();
    usr_rights     = {} as { [usr_id: string]: number }; // usr_id -> unsigned (right flags)
    grp_rights     = {} as { [grp_id: string]: number }; // grp_id -> unsigned (right flags)
    usr_id         : UsrId; // creator (who has all the right at the beginning)
}
export default OtWrapper;

// import BinaryReader from "../../System/BinaryReader";
// import Variable     from "../Core/Variable";
// import OtMerger     from "./OtMerger";

// static op_add_grp_right = { // add_grp_right
//     attrs: [ "GrpId", "PI32" ],
//     right( rp: OtWrapper, flags : number, grp : GrpId, val : number ) : boolean {
//         return OtWrapper.can_add_rights.ok( flags );
//     },
//     valid( rp: OtWrapper, grp : GrpId, val : number ) : boolean {
//         return val != 0;
//     },
//     apply_op( rp: OtWrapper, aod, grp : GrpId, val : number ) : void {
//         if ( val &= ~ rp.grp_right( grp ) ) {
//             rp.grp_rights[ grp.toString() ] |= val;
//             rp.sig_change();
//         }
//     },
//     undo( rp: OtWrapper, aod, grp : GrpId, val : number ) : void {
//         OtWrapper.op_rem_grp_right.apply_op( rp, aod, grp, val );
//     },
//     update_patch_data_l0( rp: OtWrapper, args ) : void {
//         args[ 1 ] &= ~ rp.grp_right( args[ 0 ] );
//     }
// };
// static op_rem_grp_right = {
//     attrs: [ "GrpId", "PI32" ],
//     right( rp: OtWrapper, flags : number, grp : GrpId, val : number ) : boolean {
//         return OtWrapper.can_rem_rights.ok( flags );
//     },
//     valid( rp: OtWrapper, grp : GrpId, val : number ) : boolean {
//         return val != 0;
//     },
//     apply_op( rp: OtWrapper, aod, grp : GrpId, val : number ) : void {
//         if ( val &= rp.grp_right( grp ) ) {
//             rp.grp_rights[ grp.toString() ] &= ~val;
//             rp.sig_change();
//         }
//     },
//     undo( rp: OtWrapper, aod, grp : GrpId, val : number ) : void {
//         OtWrapper.op_add_grp_right.apply_op( rp, aod, grp, val );
//     },
//     update_patch_data_l0( rp: OtWrapper, args ) : void {
//         args[1] &= rp.grp_right( args[ 0 ] );
//     }
// };
// static op_add_usr_right = {
//     attrs: [ "UsrId", "PI32" ],
//     right( rp: OtWrapper, flags : number, usr : UsrId, val : number ) : boolean {
//         return OtWrapper.can_add_rights.ok( flags );
//     },
//     valid( rp: OtWrapper, usr : UsrId, val : number ) : boolean {
//         return val != 0;
//     },
//     apply_op( rp: OtWrapper, aod, usr : UsrId, val : number ) : void {
//         if ( val &= ~ rp.usr_right( usr ) ) {
//             rp.usr_rights[ usr.toString() ] |= val;
//             rp.sig_change();
//         }
//     },
//     undo( rp: OtWrapper, aod, usr : UsrId, val : number ) : void {
//         OtWrapper.op_rem_usr_right.apply_op( rp, aod, usr, val );
//     },
//     update_patch_data_l0( rp: OtWrapper, args ) : void {
//         args[ 1 ] &= ~rp.usr_right( args[ 0 ] );
//     }
// };
// static op_rem_usr_right = {
//     attrs: [ "UsrId", "PI32" ],
//     right( rp: OtWrapper, flags : number, usr : UsrId, val : number ) : boolean {
//         return OtWrapper.can_rem_rights.ok( flags );
//     },
//     valid( rp: OtWrapper, usr : UsrId, val : number ) : boolean {
//         return val != 0;
//     },
//     apply_op( rp: OtWrapper, aod, usr : UsrId, val : number ) : void  {
//         if ( val &= rp.usr_right( usr ) ) {
//             rp.usr_rights[ usr.toString() ] &= ~val;
//             return rp.sig_change();
//         }
//     },
//     undo( rp: OtWrapper, aod, usr : UsrId, val : number ) : void  {
//         OtWrapper.op_add_usr_right.apply_op( rp, aod, usr, val );
//     },
//     update_patch_data_l0( rp: OtWrapper, args ) : void {
//         args[ 1 ] &= rp.usr_right( args[ 0 ] );
//     }
// };

// static op_types : Array<OtWrapperOp> = [
//     OtWrapper.op_add_grp_right,
//     OtWrapper.op_rem_grp_right,
//     OtWrapper.op_add_usr_right,
//     OtWrapper.op_rem_usr_right
// ];

// //
// static can_add_rights = {
//     name: "add_rights",
//     flags: 1,
//     ok( flags : number ) : boolean { return ( flags & 1 ) != 0; }
// }
// static can_rem_rights = {
//     name: "rem_rights",
//     flags: 2,
//     ok( flags : number ) : boolean { return ( flags & 2 ) != 0; }
// }

// static right_types = [
//     OtWrapper.can_add_rights,
//     OtWrapper.can_rem_rights
// ];

// static reg_op( bw: BinaryWriter, op : OtWrapperOp, ...args ) {
//     // op_type
//     let i = this.op_types.indexOf( op );
//     if ( i < 0 )
//         throw new Error( `Op ${op} ! defined in op_types` );
//     bw.write_PT( i );
//     //
//     for( let i = 0; i < op.attrs.length; i += 1 ) {
//         switch ( op.attrs[ i ] ) {
//             case "PI32"           : bw.write_PT        ( args[ i ] ); break;
//             case "PT"             : bw.write_PT        ( args[ i ] ); break;
//             case "DevId"          : bw.write_DevId     ( args[ i ] ); break;
//             case "UsrId"          : bw.write_UsrId     ( args[ i ] ); break;
//             case "GrpId"          : bw.write_GrpId     ( args[ i ] ); break;
//             case "String"         : bw.write_String    ( args[ i ] ); break;
//             case "Buffer"         : bw.write_Uint8Array( args[ i ] ); break;
//             case "PatchId"        : bw.write_PatchId   ( args[ i ] ); break;
//             case "Variable"       : bw.write_Variable  ( args[ i ] ); break;
//             case "Array<PT>"      : bw.write_PT( args[ i ].length ); for( let a of args[ i ] ) bw.write_PT( a ); break;
//             case "Array<Variable>": bw.write_PT( args[ i ].length ); for( let a of args[ i ] ) bw.write_Variable( a ); break;
//             default: throw new Error( "TODO reg_op type " + op.attrs[ i ] );
//         }
//     }
// }

// static loc_to_glo( op, args, dev_id : DevId, usr_id : UsrId ) {
//     for( let i = 0; i < op.attrs.length; i += 1 ) {
//         switch ( op.attrs[ i ] ) {
//             case "DevId"          :
//             case "GrpId"          :
//             case "PatchId"        : args[ i ].self_loc_to_glo( dev_id ); break;
//             case "Variable"       : args[ i ] = args[ i ].copy(); args[ i ].self_loc_to_glo( dev_id ); break;
//             case "UsrId"          : args[ i ].self_loc_to_glo( dev_id, usr_id ); break;
//             case "Array<Variable>": for( let a of args[ i ] ) a.self_loc_to_glo( dev_id, usr_id ); break;
//             default:
//         }
//     }
// }

// static glo_to_loc( op, args, dev_id : DevId, usr_id : UsrId ) {
//     for( let i = 0; i < op.attrs.length; i += 1 ) {
//         switch ( op.attrs[ i ] ) {
//             case "DevId"          :
//             case "GrpId"          :
//             case "PatchId"        : args[ i ].self_glo_to_loc( dev_id ); break;
//             case "Variable"       : args[ i ] = args[ i ].copy(); args[ i ].self_glo_to_loc( dev_id ); break;
//             case "UsrId"          : args[ i ].self_glo_to_loc( dev_id, usr_id ); break;
//             case "Array<Variable>": for( let a of args[ i ] ) a.self_glo_to_loc( dev_id, usr_id ); break;
//             default:
//         }
//     }
// }

// static read_op( br: BinaryReader, op : OtWrapperOp, func : ( op: OtWrapperOp, ...args : any[] ) => any ) {
//     let args = [];
//     for( let i = 0; i < op.attrs.length; i += 1 ) {
//         switch ( op.attrs[ i ] ) {
//             case "PI32"           : args.push( br.read_PT        () ); break;
//             case "PT"             : args.push( br.read_PT        () ); break;
//             case "DevId"          : args.push( br.read_DevId     () ); break;
//             case "UsrId"          : args.push( br.read_UsrId     () ); break;
//             case "GrpId"          : args.push( br.read_GrpId     () ); break;
//             case "String"         : args.push( br.read_String    () ); break;
//             case "Buffer"         : args.push( br.read_Uint8Array() ); break;
//             case "PatchId"        : args.push( br.read_PatchId   () ); break;
//             case "Variable"       : args.push( br.read_Variable  () ); break;
//             case "Array<PT>"      : { let lst = []; for( let l = br.read_PT(); l--; ) lst.push( br.read_PT() ); args.push( lst ); break; }
//             case "Array<Variable>": args.push( br.read_list( Variable ) ); break;
//             default: throw new Error( "TODO read_op type " + op.attrs[ i ] );
//         }
//     }
//     return func( op, ...args );
// }

// // static nu_or_un_fun = []; // functions for new_unk || unk_new combinations. Will be redefined in sub classes
// // static usc_fun      = []; // functions for std_unk combinations. Will be redefined in sub classes

// static reg_nuf( op_0, op_1, fun ) {
//     (<any>this).nu_or_un_fun[ this.op_types.indexOf( op_0 ) * this.op_types.length + this.op_types.indexOf( op_1 ) ] = fun;
// }

// static reg_usc( op_0, op_1, fun ) {
//     (<any>this).usc_fun[ this.op_types.indexOf( op_0 ) * this.op_types.length + this.op_types.indexOf( op_1 ) ] = fun;
// }
// write_OtWrapper_data_to( bw : BinaryWriter, only_for_a_copy : boolean, cur_dev : DevId, cur_usr : UsrId ) {
//     bw.write_Uint8Array( this.last_ops.to_Uint8Array() );

//     let nur = 0; for( let i in this.usr_rights ) ++nur;
//     bw.write_PT( nur );
//     for( let i in this.usr_rights ) {
//         bw.write_UsrId( UsrId.fromString( i ), cur_dev, cur_usr );
//         bw.write_PT( this.usr_rights[ i ] );
//     }

//     let ngr = 0; for( let i in this.grp_rights ) ++ngr;
//     bw.write_PT( ngr );
//     for( let i in this.grp_rights ) {
//         bw.write_GrpId( GrpId.fromString( i ), cur_dev );
//         bw.write_PT( this.grp_rights[ i ] );
//     }

//     bw.write_UsrId( this.usr_id, cur_dev, cur_usr );
// }

// static read_OtWrapper_data_from( res : OtWrapper, br : BinaryReader, cur_dev : DevId, cur_usr : UsrId ) : void {
//     res.last_ops.write_some( br.read_Uint8Array() );

//     for( let nur = br.read_PT(); nur--; ) {
//         let usr = br.read_UsrId( cur_dev, cur_usr );
//         res.usr_rights[ usr.toString() ] = br.read_PT();
//     }

//     for( let ngr = br.read_PT(); ngr--; ) {
//         let grp = br.read_GrpId( cur_dev );
//         res.grp_rights[ grp.toString() ] = br.read_PT();
//     }

//     res.usr_id = br.read_UsrId( cur_dev, cur_usr );
// }

// static skip_OtWrapper_data_from( br: BinaryReader ) {
//     br.skip_Uint8Array();

//     for( let nur = br.read_PT(); nur--; ) {
//         br.skip_UsrId();
//         br.skip_PT();
//     }

//     for( let ngr = br.read_PT(); ngr--; ) {
//         br.skip_GrpId();
//         br.skip_PT();
//     }

//     br.skip_UsrId();
// }

// self_on_change_par( par, arg ) {
//     this.on_change_par.push( { par: par, arg: arg } );
// }

// get_possible_rights() : Array<string> {
//     let res = [];
//     for( let i of (<any>this).constructor.right_types )
//         res.push( i.name );
//     // T::RightTypes::apply( [&]( auto st ) { res << decltype( st )::T::name(); } );
//     return res;
// }
// get_groups_in_acl() : Array<GrpId> {
//     throw new Error( "TODO" );
// }

// get_users_in_acl() : Array<UsrId> {
//     let res = [];
//     for( let p in this.usr_rights )
//         res.push( UsrId.fromString( p ) );
//     return res;
// }

// get_grp_rights( grp : GrpId ) : Array<String> { throw new Error( "TODO" ); } // Vec<std::string> res; unsigned r = grp_right( grp ); T::RightTypes::apply( [&]( auto st ) { if ( r & 1 ) res << decltype( st )::T::name(); r >>= 1; } ); return res; }

// get_usr_rights( usr : UsrId ) : Array<String> {
//     let res = [];
//     let rights = this.usr_right( usr );
//     for( let r of (<any>this).constructor.right_types )
//         if ( ( r.flags & rights ) == r.flags )
//             res.push( r.name );
//     return res;
// }

// add_grp_right ( grp : GrpId, right : string ) : boolean { throw new Error( "TODO" ); } // PT n = num_right( right ); return n < T::RightTypes::size ? _add_grp_right( grp, 1u << n ) : false; }

// add_usr_right ( usr : UsrId, right : string ) : boolean {
//     let flags = 0;
//     for( let n of right.split( " " ) ) {
//         for( let r of (<any>this).constructor.right_types ) {
//             if ( r.name == right ) {
//                 flags |= r.flags;
//                 break;
//             }
//         }
//     }
//     return this._add_usr_right( usr, flags );
// }

// rem_grp_right ( grp : GrpId, right : string ) : boolean { throw new Error( "TODO" ); } // PT n = num_right( right ); return n < T::RightTypes::size ? _rem_grp_right( grp, 1u << n ) : false; }

// rem_usr_right ( usr : UsrId, right : string ) : boolean {
//     let flags = 0;
//     for( let n of right.split( " " ) ) {
//         for( let r of (<any>this).constructor.right_types ) {
//             if ( r.name == right ) {
//                 flags |= r.flags;
//                 break;
//             }
//         }
//     }
//     return this._rem_usr_right( usr, flags );
// }

// get_merge_effect( bw : BinaryWriter, v, as_usr : UsrId ) : void {
//      throw new Error( "to do: merge effect for " + (<any>this).constructor.name );
// }

// get_ext_patch_merge_effect( siht : OtWrapper ) : void {
//     this.on_change_par = this.on_change_par.concat( siht.on_change_par );

//     for( let i in siht.on_change_callbacks )
//         this.on_change_callbacks[ i ] = siht.on_change_callbacks[ i ];
// }

// patch_data_visitor( data : Uint8Array, func : ( ...args : any[] ) => void ) {
//     let br = new BinaryReader( data );
//     while( br.size != 0 ) {
//         let num = br.read_PI8();
//         OtWrapper.read_op( br, ( <any>this ).constructor.op_types[ num ], func );
//     }
// }

// reverse_patch_data_visitor( data : Uint8Array, func : ( ...args : any[] ) => void ) {
//     // forward
//     let br = new BinaryReader( data );
//     let offsets = [];
//     while( br.size != 0 ) {
//         offsets.push( br.cursor );
//         let num = br.read_PI8();
//         OtWrapper.read_op( br, ( <any>this ).constructor.op_types[ num ], () => null );
//     }

//     // backward
//     for( let offset = offsets.length - 1; offset >= 0; offset -= 1 ) {
//         br.cursor = offset;
//         let num = br.read_PI8();
//         OtWrapper.read_op( br, ( <any>this ).constructor.op_types[ num ], func );
//     }
// }

// new_ot_merger() {
//     return new OtMerger( this );
// }

// unk_new_or_new_unk( op_new, op_unk, as_usr : UsrId, data_new : BinaryWriter, data_unk : BinaryWriter, args_new, args_unk ) : void {
//     let cl = (<any>this).constructor;
//     cl.nu_or_un_fun[ cl.op_types.indexOf( op_new ) * cl.op_types.length + cl.op_types.indexOf( op_unk ) ].call( this, as_usr, data_new, data_unk, args_new, args_unk );
// }

// unk_std_comb( op_std, op_unk, as_usr : UsrId, data_unk : BinaryWriter, args_std, args_unk ) : void {
//     let cl = (<any>this).constructor;
//     cl.usc_fun[ cl.op_types.indexOf( op_std ) * cl.op_types.length + cl.op_types.indexOf( op_unk ) ].call( this, as_usr, data_unk, args_std, args_unk );
// }

// _add_usr_right( usr : UsrId, flags : number ) : boolean {
//     if ( ! OtWrapper.can_add_rights.ok( this.usr_right() ) || ! flags )
//         return false;

//     let usrs = usr.toString();
//     if ( this.usr_rights[ usrs ] == null )
//         this.usr_rights[ usrs ] = 0;

//     flags &= ~ this.usr_rights[ usrs ];
//     if ( flags ) {
//         OtWrapper.reg_op( this.last_ops, OtWrapper.op_add_usr_right, usr, flags );
//         this.usr_rights[ usrs ] |= flags;
//         this.sig_change();
//         return true;
//     }
//     return false;
// }

// _rem_usr_right( usr : UsrId, flags : number ) : boolean {
//     if ( ! OtWrapper.can_rem_rights.ok( this.usr_right() ) || ! flags )
//         return false;

//     let usrs = usr.toString();
//     if ( this.usr_rights[ usrs ] == null )
//         return false;

//     flags &= this.usr_rights[ usrs ];
//     if ( flags ) {
//         OtWrapper.reg_op( this.last_ops, OtWrapper.op_rem_usr_right, usr, flags );
//         this.usr_rights[ usrs ] &= ~flags;
//         this.sig_change();
//         return true;
//     }
//     return false;
// }
// // register base patch combination funcs
// cl.nu_or_un_fun = [];
// for( let i = 0; i < cl.op_types.length * cl.op_types.length; ++i )
//     cl.nu_or_un_fun.push( function() {} );

// cl.usc_fun = [];
// for( let i = 0; i < cl.op_types.length * cl.op_types.length; ++i )
//     cl.usc_fun.push( function() {} );

// //
// cl.reg_nuf( cl.op_add_grp_right, cl.op_add_grp_right, function( asu_n : UsrId, bq_n : BinaryWriter, bq_o : BinaryWriter, args_n, args_o ) { if ( args_o[ 0 ] == args_n[ 0 ] ) args_o[ 1 ] &= ~ args_n[ 1 ]; } );
// cl.reg_nuf( cl.op_add_grp_right, cl.op_rem_grp_right, function( asu_n : UsrId, bq_n : BinaryWriter, bq_o : BinaryWriter, args_n, args_o ) { if ( args_o[ 0 ] == args_n[ 0 ] ) args_o[ 1 ] &= ~ args_n[ 1 ]; } );
// cl.reg_nuf( cl.op_rem_grp_right, cl.op_add_grp_right, function( asu_n : UsrId, bq_n : BinaryWriter, bq_o : BinaryWriter, args_n, args_o ) { if ( args_o[ 0 ] == args_n[ 0 ] ) args_o[ 1 ] &= ~ args_n[ 1 ]; } );
// cl.reg_nuf( cl.op_rem_grp_right, cl.op_rem_grp_right, function( asu_n : UsrId, bq_n : BinaryWriter, bq_o : BinaryWriter, args_n, args_o ) { if ( args_o[ 0 ] == args_n[ 0 ] ) args_o[ 1 ] &= ~ args_n[ 1 ]; } );
// cl.reg_nuf( cl.op_add_usr_right, cl.op_add_usr_right, function( asu_n : UsrId, bq_n : BinaryWriter, bq_o : BinaryWriter, args_n, args_o ) { if ( args_o[ 0 ] == args_n[ 0 ] ) args_o[ 1 ] &= ~ args_n[ 1 ]; } );
// cl.reg_nuf( cl.op_add_usr_right, cl.op_rem_usr_right, function( asu_n : UsrId, bq_n : BinaryWriter, bq_o : BinaryWriter, args_n, args_o ) { if ( args_o[ 0 ] == args_n[ 0 ] ) args_o[ 1 ] &= ~ args_n[ 1 ]; } );
// cl.reg_nuf( cl.op_rem_usr_right, cl.op_add_usr_right, function( asu_n : UsrId, bq_n : BinaryWriter, bq_o : BinaryWriter, args_n, args_o ) { if ( args_o[ 0 ] == args_n[ 0 ] ) args_o[ 1 ] &= ~ args_n[ 1 ]; } );
// cl.reg_nuf( cl.op_rem_usr_right, cl.op_rem_usr_right, function( asu_n : UsrId, bq_n : BinaryWriter, bq_o : BinaryWriter, args_n, args_o ) { if ( args_o[ 0 ] == args_n[ 0 ] ) args_o[ 1 ] &= ~ args_n[ 1 ]; } );

// cl.reg_usc( cl.op_add_grp_right, cl.op_add_grp_right, function( asu_s : UsrId, bq_u : BinaryWriter, args_s, args_u ) { if ( args_s[ 0 ] == args_u[ 0 ] ) args_u[ 1 ] &= ~ args_s[ 1 ]; } );
// cl.reg_usc( cl.op_add_grp_right, cl.op_rem_grp_right, function( asu_s : UsrId, bq_u : BinaryWriter, args_s, args_u ) { if ( args_s[ 0 ] == args_u[ 0 ] ) args_u[ 1 ] &= ~ args_s[ 1 ]; } );
// cl.reg_usc( cl.op_rem_grp_right, cl.op_add_grp_right, function( asu_s : UsrId, bq_u : BinaryWriter, args_s, args_u ) { if ( args_s[ 0 ] == args_u[ 0 ] ) args_u[ 1 ] &= ~ args_s[ 1 ]; } );
// cl.reg_usc( cl.op_rem_grp_right, cl.op_rem_grp_right, function( asu_s : UsrId, bq_u : BinaryWriter, args_s, args_u ) { if ( args_s[ 0 ] == args_u[ 0 ] ) args_u[ 1 ] &= ~ args_s[ 1 ]; } );
// cl.reg_usc( cl.op_add_usr_right, cl.op_add_usr_right, function( asu_s : UsrId, bq_u : BinaryWriter, args_s, args_u ) { if ( args_s[ 0 ] == args_u[ 0 ] ) args_u[ 1 ] &= ~ args_s[ 1 ]; } );
// cl.reg_usc( cl.op_add_usr_right, cl.op_rem_usr_right, function( asu_s : UsrId, bq_u : BinaryWriter, args_s, args_u ) { if ( args_s[ 0 ] == args_u[ 0 ] ) args_u[ 1 ] &= ~ args_s[ 1 ]; } );
// cl.reg_usc( cl.op_rem_usr_right, cl.op_add_usr_right, function( asu_s : UsrId, bq_u : BinaryWriter, args_s, args_u ) { if ( args_s[ 0 ] == args_u[ 0 ] ) args_u[ 1 ] &= ~ args_s[ 1 ]; } );
// cl.reg_usc( cl.op_rem_usr_right, cl.op_rem_usr_right, function( asu_s : UsrId, bq_u : BinaryWriter, args_s, args_u ) { if ( args_s[ 0 ] == args_u[ 0 ] ) args_u[ 1 ] &= ~ args_s[ 1 ]; } );
