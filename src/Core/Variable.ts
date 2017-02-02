import methods, { decl, dstd, dslf } from './methods';
import Rp                            from "./Rp"

/** Untyped variable (ancestor of something typed, with specific methods) */
export
class VarAnc {
    constructor( rp: Rp ) {
        this.date = VarAnc.date;
        this.rp   = rp;
    }

    toString(): string {
        return methods[ "to_String__b" ].call_1( this.rp );
    } 

    toBoolean(): boolean {
        return methods[ "to_Boolean__b" ].call_1( this.rp );
    }

    toNumber(): number {
        return methods[ "to_Number__b" ].call_1( this.rp );
    }

    toArray(): Array<any> {
        return methods[ "to_Array__b" ].call_1( this.rp );
    }

    get toBooleanVariable(): LvNumber {
        return new LvNumber( methods[ "to_BooleanVariable__b" ].call_1( this.rp ) );
    }

    rp_structure(): string {
        return methods[ "rp_structure__b" ].call_1( this.rp );
    }

    variable_type(): any {
        return methods[ "variable_type__b" ].call_1( this.rp );
    }

    infImmediate( val: VarAnc ): boolean {
        return methods[ "inf_immediate__bb" ].call_2( this.rp, val.rp );
    }

    supImmediate( val: VarAnc ): boolean {
        return methods[ "sup_immediate__bb" ].call_2( this.rp, val.rp );
    }

    equImmediate( val: VarAnc ): boolean {
        return methods[ "equ_immediate__bb" ].call_2( this.rp, val.rp );
    }

    isNeg(): LvNumber {
        return methods[ "is_neg__b" ].call_1( this.rp );
    }

    isPos(): LvNumber {
        return methods[ "is_pos__b" ].call_1( this.rp );
    }

    isNull(): LvNumber {
        return methods[ "is_eqz__b" ].call_1( this.rp );
    }

    isNonNull(): LvNumber {
        return this.isNull().notLog();
    }

    // sig_change( arg? ) { // arg is optionnal
    //     methods["sig_change"].call_1( this, arg );
    // }

    // onchange_par( par, arg ) { //
    //     methods["self_on_change_par"].call_1( this, par, arg );
    // }

    // rm_onchange( callback_id ) {
    //     methods["self_rm_on_change"].call_1( this, callback_id );
    // }

    // pull_ot_changes( changes ) {
    //     methods["pull_ot_changes"].call_1( this, changes );
    // }

    // new_ot_merger(): OtMerger {
    //     return methods["new_ot_merger"].call_1( this.rp );
    // }

    // self_glo_to_loc( cur_dev : DevId, cur_usr : UsrId ) : void {
    //     methods["self_glo_to_loc"].call_1( this, cur_dev, cur_usr );
    // }

    // self_loc_to_glo( cur_dev : DevId, cur_usr : UsrId ) : void {
    //     methods["self_loc_to_glo"].call_1( this, cur_dev, cur_usr );
    // }

    // obj_type() {
    //     return methods["obj_type"].call_1( this.rp );
    // }

    /** list of right names (e.g.: [ "add_usr_right", "append", ... ] ) */
    getPossibleRights(): Array<string> {
        return methods["get_possible_rights__b"].call_1( this.rp );
    }

    getUsersInAccessConstrolList() : Array<UsrId> {
        return methods["get_users_in_acl__b"].call_1( this.rp );
    }

    getUsrRights( usr: UsrId ) : Array<string> {
        return methods["get_usr_rights__b"].call_1( this.rp, usr );
    }

    addUsrRight( usr: UsrId, right_types: string | Array<string>, as_usr = new UsrId ): VarAnc {
        this.rp = methods["add_usr_right__s"].call_1s( this, usr, typeof right_types == "string" ? [ right_types ] : right_types, as_usr );
        return this;
    }

    remUsrRight( usr: UsrId, right_types: string | Array<string>, as_usr = new UsrId ): VarAnc {
        this.rp = methods["rem_usr_right__s"].call_1s( this, usr, typeof right_types == "string" ? [ right_types ] : right_types, as_usr );
        return this;
    }

    applyMethod( name, ...args ): VarAnc {
        this.rp = methods["apply_method__s"].call_1s( this, name, ...args.map( x => toRp( x ) ) );
        return this;
    }

    // then( callback ) {
    //     methods["then"].call_1( this, callback );
    // }

    // write_to( bw : BinaryWriter, only_for_a_copy : boolean, cur_dev : DevId, cur_usr : UsrId ) {
    //     bw.write_Variable( this, only_for_a_copy, cur_dev, cur_usr );
    // }

    // static read_from( br : BinaryReader, cur_dev : DevId, cur_usr : UsrId ) : Variable {
    //     return br.read_Variable( cur_dev, cur_usr );
    // }

    // static skip_from( br : BinaryReader ) : void {
    //     br.skip_Variable();
    // }
    static date = 0;

    rp  : Rp;
    date: number; /** creation "date" */
}

/**  */
export default
class Variable<T> extends VarAnc {
    // constructor( rp: Rp ) {
    //     super( rp );
    // }

    /**  */ 
    onChange( callback: ( val: T ) => void, immediate = false ): Variable<T> {
        this.rp = methods[ "on_change__s" ].call_1s( this, callback, immediate );
        return this;
    }

    deleteOnChange( callback: ( val: T ) => void ): Variable<T> {
        this.rp = methods[ "rm_on_change__s" ].call_1s( this, callback );
        return this;
    }
} 

/** */
export var toLv_array = new Array< ( val: any ) => VarAnc >();
export function toRp( val: any ): Rp {
    if ( val instanceof VarAnc )
        return methods["copy__b"].call_1( val.rp );
    for( const f of toLv_array ) {
        let r = f( val );
        if ( r )
            return r.rp;
    }
    return null;
}

/** to register methods... */
export function od( cb ) { if ( base_methods_are_defined ) cb(); else waiting_bmar_cbs.push( cb ); }
let base_methods_are_defined = false, waiting_bmar_cbs = [];

// representation
dstd( "to_String"              , 1, false ); // returns a native String
dstd( "write_graphviz"         , 1, false ); // param: ( gr: Graphviz )
dstd( "js_repr"                , 1, false ); // param: ( precedance ). Return a string.
dstd( "variable_type"          , 1, false ); // the variable type that should surround this Rp

// conversion
dstd( "to_Number"              , 1, false ); // returns a native Number
dstd( "to_Boolean"             , 1, false ); // returns a native Boolean
dstd( "to_Array"               , 1, false ); // returns a native Array
dstd( "to_Inode"               , 1, false ); // returns an Inode object
dstd( "to_Buffer"              , 1, false ); // returns an Buffer object
dstd( "to_native_type"         , 1, false ); // 

// proxy
dstd( "prox_ptr"               , 1, false ); // returns a Variable, which can be modified
dstd( "prox_ref"               , 1, false ); // returns an existing Variable, which should not be modified. prox_ref__o return the variable owned by the receiver. prox_ref__b return a variable that may have to be copied for a later use.
dstd( "prox_val"               , 1, false ); // returns a computed Variable

// generic calls
dslf( "apply_method"           , 1        ); // takes a string (name of the method) + a list of parameters (Rp types)


// OT   
dslf( "add_patch_manager"      , 1        ); // enable notably to add a DbItem (which can be considered as a patch manager) 
dslf( "make_ot_wrapper"        , 1, false ); // returns an OtWrapper
decl( "get_patch_type__b"      , false    ); // return a number representing the patch type (e.g. OtWrapperAtomic[ObjectId_Inode]...)
decl( "cmp_patch_type__b"      , false    ); // return true if same type
decl( "new_ot_merger__b"       , false    ); // arg: orig patch_manager. returns an OtMerger
// decl( "pull_ot_changes"        , "r"   ); // takes a BinaryWriter as arg
// dslf( "ot_merge"               , 2     ); // merge two objects into one
// decl( "cmp_patch_type"         , "c"   ); // return a number representing the patch type (e.g. OtWrapperAtomic[ObjectId_Inode]...)
   
// decl( "write_to"               , "c"   ); // takes a BinaryWriter as arg
// decl( "self_loc_to_glo"        , "r"   ); // + DevID + UsrId
// decl( "self_glo_to_loc"        , "r"   ); // + DevID + UsrId
// decl( "new_cursor"             , "r"   ); // return a loc_id (PI64)
// decl( "get_cursor"             , "c"   ); // + DevID + loc_id (PI64). Return [{beg:number,end:number,foc:number}]
// decl( "set_cursor"             , "c"   ); // + DevID + loc_id (PI64) + [{beg:number,end:number,foc:number}]
// decl( "rem_cursor"             , "r"   ); // + DevID + loc_id (PI64)
// decl( "get_cursor_ids"         , "c"   );
// decl( "mod_select"             , "rc"  ); // select for modification
// decl( "rem_item"               , "rc"  ); // remove item (e.g. using a key)
// decl( "obj_type"               , "c"   ); // variable type (ex: GenString -> String)
dstd( "key_type"               , 1, false );
dstd( "val_type"               , 1, false );
   
//    
dstd( "inf_immediate"          , 2, false );
dstd( "sup_immediate"          , 2, false );
dstd( "equ_immediate"          , 2, false );
   
dstd( "select_ref"             , 2        ); // make a Select (or a SelectSym, ...)
dstd( "select"                 , 2        ); // select for read
dslf( "beginning"              , 2        ); // item, end => items at the beginning (up to end)
dslf( "ending"                 , 2        ); // item, beg => items up to beg
dslf( "slice"                  , 3        ); // lst, beg, end (get a copy)
   
dstd( "get_size"               , 1        ); // return a Variable (probably a number)
   
dstd( "to_BooleanVariable"     , 1        ); // conversion to Boolean variable (can be symbolic, ...)
   

dstd( "copy"                   , 1        );

dslf( "not_log"                , 1        );
dslf( "not_bin"                , 1        );
dslf( "neg"                    , 1        );
       
dslf( "set"                    , 2        );
dslf( "add"                    , 2        );
dslf( "sub"                    , 2        );
dslf( "mul"                    , 2        );
dslf( "div"                    , 2        );
dslf( "or_log"                 , 2        );
dslf( "and_log"                , 2        );
dslf( "xor_log"                , 2        );
dslf( "or_bin"                 , 2        );
dslf( "and_bin"                , 2        );
dslf( "xor_bin"                , 2        );
dslf( "signed_shift_left"      , 2        );
dslf( "signed_shift_right"     , 2        );
dslf( "zfill_shift_left"       , 2        );
       
dstd( "is_pos"                 , 1        );
dstd( "is_neg"                 , 1        );
dstd( "is_eqz"                 , 1        );
       
dstd( "is_equ"                 , 2        );
dstd( "is_inf"                 , 2        );
dstd( "is_infeq"               , 2        );
dstd( "is_sup"                 , 2        );
dstd( "is_supeq"               , 2        );

dslf( "remove"                 , 3        );
dslf( "insert"                 , 3        );
dslf( "push"                   , 2        );
   
// sym   
// decl( "ww_launch"              , "c"   ); // called from the scheduler WebWorker to launch the query
// decl( "then"                   , "c"   ); // executed a function when value is known
dstd( "symbolic_known_value"   , 1        ); // return a Link with symbol that wraps the first argument

// changes
dslf( "on_change"              , 1        ); // return an id (type PatchId), that can be used to remove the callback
dstd( "sig_change"             , 1        ); //
dslf( "rm_on_change"           , 1        ); // takes the id (type PatchId) that has been used for `on_change`
dslf( "on_change_par"          , 1        ); // add a parent object for ob_change (if this is changed, sig_change will be called on this parent). Optionnal argument: something that will be sent to parent sig_change

// db   
// decl( "bind_by_path"           , "c"   ); // path , db, category, load_only_once
// decl( "bind_by_object_id"      , "c"   ); // inode, db, category, load_only_once
   
// rights   
decl( "get_possible_rights__b" , false );
decl( "get_users_in_acl__b"    , false );
decl( "get_usr_rights__b"      , false );
decl( "add_usr_right__s"       , false );
decl( "rem_usr_right__s"       , false );


base_methods_are_defined = true;
waiting_bmar_cbs.forEach( cb => cb() );
waiting_bmar_cbs.length = 0;

import UsrId    from "../System/UsrId"
import LvNumber from "../LvNumber"
require( "./DefaultMethods" );
require( "./Select" );
