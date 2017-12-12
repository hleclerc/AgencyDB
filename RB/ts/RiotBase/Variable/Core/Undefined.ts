import Method from "../Core/Method"
import Rp     from "../Core/Rp"

//
export default
class Undefined extends Rp {
    static term_value = true;

    constructor() {
        super( Undefined );
    }

    to_String() : string {
        return "undefined";
    }

    obj_type() {
        return Variable;
    }

    copy() {
        return new Variable( this );
    }

    write_to( bw : BinaryWriter, cur_dev : DevId, cur_usr : UsrId ) {
        throw new Error( "TODO" );
        // bw.write_PT( Undefined.num_for_RW );
        // bw.write_PT( 0 );
    }

    self_on_change_par( par, arg ) {
        this.on_change_par.push( { par: par, arg: arg } );
    }

    pull_ot_changes( changes ) {
    }

    static read_from( br : BinaryReader, dev_id : DevId, usr_id : UsrId ) : Variable {
        return new Variable( new Undefined );
    }

    on_change_par : Array<{ par: Variable, arg: any }> = new Array();
}
Rp.make_templ( Undefined );

Method.methods["self_set_o"].add_surdef( 2, [ Undefined, () => true ], function( obj, val : Variable ) {
    if ( val.rp instanceof Undefined )
        return false;
    let on_change_par = obj.rp.on_change_par;
    (<any>Object).setPrototypeOf( obj, val );
    obj.rp = val.rp;
    for( let i of on_change_par )
        Method.methods["self_on_change_par"].call_1( obj, i.par, i.arg );
    return true;
} );

import BinaryReader    from "../../System/BinaryReader";
import BinaryWriter    from "../../System/BinaryWriter";
import DevId           from "../../System/DevId";
import UsrId           from "../../System/UsrId";
import Variable        from "../Core/Variable";
