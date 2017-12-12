import Variable from "../Core/Variable";
import Method   from "../Core/Method";
import Rp       from "../Core/Rp";
import GenSym   from "./GenSym";
import Map      from "../../System/Map";

var cur_then_list_id = 0;
var then_map         = new Map<number,Array<(Variable)=>void>>();

//
export default
class Lazyfier extends Rp {
    static unknown_value = true;
    static term_value    = true;

    constructor( sym : Variable ) {
        super( Lazyfier );
        this.sym = sym;

        let csd = sym.common_symbolic_data();
        if ( csd )
            csd.lazyfier_parents.push( this );
    }

    copy() : Variable {
        let ot = Method.methods["obj_type"].call_1( this.sym );
        return new ot( new Lazyfier( Method.methods["copy"].call_1( this.sym ) ) );
    }

    prox_ref() : Variable {
        return this.sym;
    }

    freeze_last_mod() {
    }

    pull_ot_changes() {
        return new Uint8Array( [] );
    }

    // helpers
    static copy_rp_without_lazifier( val : Variable ) : Variable {
         return Method.methods["copy"].call_1( val.rp.type_num == Lazyfier.type_num ? ( val.rp as Lazyfier ).sym : val );
    }

    static ref_rp_without_lazifier( val : Variable ) : Variable {
         return val.rp.type_num == Lazyfier.type_num ? ( val.rp as Lazyfier ).sym : val;
    }

    static new_symbol<T extends Variable>( cl: { new( Rp ): T ;}, name: string ) : T {
        return new cl( new Lazyfier( new cl( new GenSym( cl, name ) ) ) );
    }

    obj_type() {
        return this.sym.obj_type();
    }

    get_then_list_id() {
        if ( this.then_list.length ) {
            let res = ++cur_then_list_id;
            then_map.set( res, this.then_list );
            return res;
        }
        return null;
    }

    then( callback ) {
        this.then_list.push( callback );
    }

    sym       : Variable;
    then_list = new Array<(Variable)=>void>();
}

Rp.make_templ( Lazyfier );

declare function require( s: string ) : void;
require( "./SymOp" );
