import Method from "./Method"
import Rp     from "./Rp"

declare var require: ( string ) => any;
declare class Number {}

//
export default
class Variable {
    // attempt to make a new RiotBase Variable type (like a String, ...)
    static rp_conv = function( val ) : Variable {
        return this.rp_conv_on( val ).v
    }

    // attempt to make a new RiotBase Variable type (like a String, ...)
    static rp_conv_cp = function( val ) : Variable {
        let rp = this.rp_conv_on( val );
        return rp.o ? rp.v : Method.methods["copy"].call_1( rp.v );
    }

    // attempt to make a RiotBase Variable type (like a String, ...)
    static rp_conv_on = function( val ) : { v : Variable, o: boolean } {
        if ( this != Variable           ) { return val instanceof this ? { v: val, o: false } : { v: new this( val ), o: true }; }
        if ( val instanceof Variable    ) { return { v : val, o: false }; }
        if ( typeof( val ) == "string"  ) { let T = require( "../String" ).default; return { v: new T( val ), o: true }; }
        if ( typeof( val ) == "number"  ) { let T = require( "../Number" ).default; return { v: new T( val ), o: true }; }
        if ( typeof( val ) == "boolean" ) { let T = require( "../Number" ).default; return { v: new T( val ), o: true }; }
        if ( val instanceof Array       ) { let T = require( "../Array"  ).default; return { v: new T( val ), o: true }; }
        return null;
    }

    constructor( rp: Rp ) {
        this.rp = rp;
    }

    set( val, opt? ) : boolean {
        let rc = ( <any>( this.constructor ) ).rp_conv_on( val );
        return rc.o ? Method.methods["self_set_o"].call_2( this, rc.v ) : Method.methods["self_set_b"].call_2( this, rc.v );
    }

    toString() : string {
        return Method.methods["to_String"].call_1( this );
    }

    rp_structure() : string {
        return Method.methods["rp_structure"].call_1( this );
    }

    toNumber() : number {
        return Method.methods["to_Number"].call_1( this );
    }

    toArray() : Array<any> {
        return Method.methods["to_Array"].call_1( this );
    }

    inf_immediate( val ) : boolean {
        return Method.methods["inf_immediate"].call_2( this, ( <any>( this.constructor ) ).rp_conv( val ) );
    }

    sup_immediate( val ) : boolean {
        return Method.methods["sup_immediate"].call_2( this, ( <any>( this.constructor ) ).rp_conv( val ) );
    }

    equ_immediate( val ) : boolean {
        return Method.methods["equ_immediate"].call_2( this, ( <any>( this.constructor ) ).rp_conv( val ) );
    }

    is_neg() : Number {
        return Method.methods["is_neg_b"].call_1( this );
    }

    is_pos() : Number {
        return Method.methods["is_pos_b"].call_1( this );
    }

    is_null() : Number {
        return Method.methods["is_eqz_b"].call_1( this );
    }

    is_non_null() : Number {
        return ( <any>( this.is_null() ) ).not();
    }

    onchange( callback ) : PatchId { // return a callback id
        return Method.methods["self_on_change"].call_1( this, callback );
    }

    sig_change( arg? ) { // arg is optionnal
        Method.methods["sig_change"].call_1( this, arg );
    }

    onchange_par( par, arg ) { //
        Method.methods["self_on_change_par"].call_1( this, par, arg );
    }

    rm_onchange( callback_id ) {
        Method.methods["self_rm_on_change"].call_1( this, callback_id );
    }

    pull_ot_changes( changes ) {
        Method.methods["pull_ot_changes"].call_1( this, changes );
    }

    new_ot_merger() : OtMerger {
        return Method.methods["new_ot_merger"].call_1( this );
    }

    self_glo_to_loc( cur_dev : DevId, cur_usr : UsrId ) : void {
        Method.methods["self_glo_to_loc"].call_1( this, cur_dev, cur_usr );
    }

    self_loc_to_glo( cur_dev : DevId, cur_usr : UsrId ) : void {
        Method.methods["self_loc_to_glo"].call_1( this, cur_dev, cur_usr );
    }

    obj_type() {
        return Method.methods["obj_type"].call_1( this );
    }

    get_possible_rights() : Array<string> {
        return Method.methods["get_possible_rights"].call_1( this );
    }

    add_usr_right( usr : UsrId, right_type : string ) : boolean {
        return Method.methods["add_usr_right"].call_1( this, usr, right_type );
    }

    rem_usr_right( usr : UsrId, right_type : string ) : boolean {
        return Method.methods["rem_usr_right"].call_1( this, usr, right_type );
    }

    get_users_in_acl() : Array<UsrId> {
        return Method.methods["get_users_in_acl"].call_1( this );
    }

    get_usr_rights( usr : UsrId ) : Array<string> {
        return Method.methods["get_usr_rights"].call_1( this, usr );
    }

    common_symbolic_data() : CommonSymbolicData {
        return Method.methods["common_symbolic_data"].call_1( this );
    }

    symbolic_children() : Array<Variable> {
        return Method.methods["symbolic_children"].call_1( this );
    }

    graphviz_repr() : string {
        return Method.methods["graphviz_repr"].call_1( this );
    }

    is_symbolic() : boolean {
        return Method.methods["is_symbolic"].call_1( this );
    }

    then( callback ) {
        Method.methods["then"].call_1( this, callback );
    }

    get_graph_data( graph_data, children, id ) : void {
        Method.methods["get_graph_data"].call_1( this, graph_data, children, id );
    }

    write_to( bw : BinaryWriter, only_for_a_copy : boolean, cur_dev : DevId, cur_usr : UsrId ) {
        bw.write_Variable( this, only_for_a_copy, cur_dev, cur_usr );
    }

    static read_from( br : BinaryReader, cur_dev : DevId, cur_usr : UsrId ) : Variable {
        return br.read_Variable( cur_dev, cur_usr );
    }

    static skip_from( br : BinaryReader ) : void {
        br.skip_Variable();
    }

    rp: Rp;
}

import CommonSymbolicData from "../Symbol/CommonSymbolicData";
import BinaryWriter       from "../../System/BinaryWriter";
import BinaryReader       from "../../System/BinaryReader";
import PatchId            from "../../System/PatchId";
import DevId              from "../../System/DevId";
import UsrId              from "../../System/UsrId";
import OtMerger           from "../Ot/OtMerger";
