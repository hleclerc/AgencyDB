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
        return this.isNull().not();
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

    // get_possible_rights(): Array<string> {
    //     return methods["get_possible_rights"].call_1( this.rp );
    // }

    // add_usr_right( usr : UsrId, right_type : string ) : boolean {
    //     return methods["add_usr_right"].call_1( this, usr, right_type );
    // }

    // rem_usr_right( usr : UsrId, right_type : string ) : boolean {
    //     return methods["rem_usr_right"].call_1( this, usr, right_type );
    // }

    // get_users_in_acl() : Array<UsrId> {
    //     return methods["get_users_in_acl"].call_1( this.rp );
    // }

    // get_usr_rights( usr : UsrId ) : Array<string> {
    //     return methods["get_usr_rights"].call_1( this, usr );
    // }

    // symbolic_children() : Array<Variable> {
    //     return methods["symbolic_children"].call_1( this.rp );
    // }

    // graphviz_repr() : string {
    //     return methods["graphviz_repr"].call_1( this.rp );
    // }

    // is_symbolic() : boolean {
    //     return methods["is_symbolic"].call_1( this.rp );
    // }

    // then( callback ) {
    //     methods["then"].call_1( this, callback );
    // }

    // get_graph_data( graph_data, children, id ) : void {
    //     methods["get_graph_data"].call_1( this, graph_data, children, id );
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

// proxy
dstd( "prox_ptr"               , 1, false ); // returns a Variable, which can be modified
dstd( "prox_ref"               , 1, false ); // returns an existing Variable, which should not be modified. prox_ref__o return the variable owned by the receiver. prox_ref__b return a variable that may have to be copied for a later use.
dstd( "prox_val"               , 1, false ); // returns a computed Variable

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
// decl( "key_type"               , "c"   );
// decl( "val_type"               , "c"   );
   
//    
dstd( "inf_immediate"          , 2, false );
dstd( "sup_immediate"          , 2, false );
dstd( "equ_immediate"          , 2, false );
   
dstd( "select"                 , 2        ); // select for read
dstd( "heads"                  , 2        ); // item, end => items at the beginning (up to end)
dstd( "tails"                  , 2        ); // item, beg => items up to beg
dstd( "slice"                  , 3        ); // lst, beg, end (get a copy)
   
dstd( "get_size"               , 1        ); // return a Variable (probably a Number)
   
dstd( "to_BooleanVariable"     , 1        ); // conversion to Boolean variable (can be symbolic, ...)
   

dstd( "copy"                   , 1        );

[ "not", "neg"                              ].forEach( x => dslf( x, 1 ) );   
[ "set", "add", "sub", "mul", "div"         ].forEach( x => dslf( x, 2 ) );   
[ "is_pos", "is_neg", "is_eqz"              ].forEach( x => dstd( x, 1 ) );
[ "is_equ", "is_inf", "is_infeq"            ].forEach( x => dstd( x, 2 ) );
[ "or_log", "and_log", "xor_log"            ].forEach( x => dstd( x, 2 ) );
[ "signed_shift_left", "signed_shift_right" ].forEach( x => dstd( x, 2 ) );
[ "zfill_shift_left"                        ].forEach( x => dstd( x, 2 ) );

dslf( "remove"                 , 3        );
dslf( "insert"                 , 3        );
   
// changes
dslf( "on_change"              , 1        ); // return an id (type PatchId), that can be used to remove the callback
dstd( "sig_change"             , 1        ); //
dslf( "rm_on_change"           , 1        ); // takes the id (type PatchId) that has been used for `on_change`
dslf( "on_change_par"          , 1        ); // add a parent object for ob_change (if this is changed, sig_change will be called on this parent). Optionnal argument: something that will be sent to parent sig_change
   
// sym   
// decl( "ww_launch"              , "c"   ); // called from the scheduler WebWorker to launch the query
// decl( "then"                   , "c"   ); // executed a function when value is known
   
// db   
// decl( "bind_by_path"           , "c"   ); // path , db, category, load_only_once
// decl( "bind_by_object_id"      , "c"   ); // inode, db, category, load_only_once
   
// rights   
// decl( "get_possible_rights"    , "c"   );
// decl( "get_groups_in_acl"      , "c"   );
// decl( "get_users_in_acl"       , "c"   );
// decl( "get_grp_rights"         , "c"   );
// decl( "get_usr_rights"         , "c"   );
// decl( "add_grp_right"          , "r"   );
// decl( "add_usr_right"          , "r"   );
// decl( "rem_grp_right"          , "r"   );
// decl( "rem_usr_right"          , "r"   );

import LvNumber  from "../LvNumber"
require( "./DefaultMethods" );
// require( "./RefProxy" );
