import DeclTypesForRW from "../Core/DeclTypesForRW";
import Method         from "../Core/Method"
import Rp             from "../Core/Rp"

//
export default
class GenString extends Rp {
    static known_value = true;
    static term_value = true;

    constructor( data: string ) {
        super( GenString );
        this.data = data;
    }

    to_String() : string {
        return this.data;
    }

    get_size() : Number {
        return new Number( this.data.length );
    }

    obj_type() {
        return String;
    }

    make_ot_wrapper() : Rp {
        return new OtWrapperString( this );
    }

    get_patch_type( bw : BinaryWriter ) {
        bw.write_PT( PatchTypes.num.OtWrapperString_GenString );
    }

    cmp_patch_type( br : BinaryReader ) : boolean {
        return br.read_PT() == PatchTypes.num.OtWrapperString_GenString;
    }

    copy() {
        return new String( this.data );
    }

    write_to( bw : BinaryWriter, only_for_a_copy : boolean, cur_dev : DevId, cur_usr : UsrId ) {
        bw.write_PT( DeclTypesForRW.num.GenString );
        bw.write_String( this.data );
    }

    static read_GenString_from( br: BinaryReader, dev_id : DevId, usr_id : UsrId ) : String {
        return new String( br.read_String() );
    }

    data: string;
}
Rp.make_templ( GenString );

DeclTypesForRW.readers[ DeclTypesForRW.num.GenString ] = GenString.read_GenString_from;

Method.methods["select_bb"].add_surdef( 2, [ GenString, "to_Number" ], ( str, ind : Variable ) =>
    new String( str.rp.data[ ind.toNumber() ] )
);

Method.methods["self_add_b"].add_surdef( 2, [ GenString, "to_String" ], ( str, sup : Variable ) => {
    let add = sup.toString();
    str.rp.data += Method.methods["to_String"].call_1( sup )
    return add.length != 0;
} );

Method.methods["self_insert_bb"].add_surdef( 2, [ GenString, "to_Number", "to_String" ], ( str, pos : Variable, sup : Variable ) => {
    let npos = pos.toNumber();
    let nsup = sup.toString();
    str.rp.data = str.rp.data.slice( 0, npos ) + nsup + str.rp.data.slice( npos );
    return nsup.length != 0;
} );

Method.methods["self_remove_bb"].add_surdef( 2, [ GenString, "to_Number","to_Number" ],( str, pos : Variable, len : Variable ) => {
    let npos = pos.toNumber();
    let nlen = len.toNumber();
    str.rp.data = str.rp.data.slice( 0, npos ) + str.rp.data.slice( npos + nlen );
    return nlen != 0;
} );

// Method.methods["self_ot_merge_b"].add_surdef( 2, [ GenString, GenString ],( str : Variable, pos : Variable, as_usr : UsrId ) => {
//     console.log( "Merging stuff " + str + " " + pos );
//     Method.methods["self_add_b"].call_2( str, pos );
//     return true;
// } );

Method.methods["new_cursor"].add_surdef( 2, [ GenString ], ( obj, dev_id, loc_id ) => {
    obj.rp = Method.methods["make_ot_wrapper"].call_1( obj );
    return Method.methods["new_cursor"].call_1( obj, dev_id, loc_id );
} );

Method.methods["rem_cursor"].add_surdef( 2, [ GenString ], ( obj, dev_id, loc_id ) =>
    false
);

Method.methods["get_cursor"].add_surdef( 2, [ GenString ], ( obj, dev_id, loc_id ) =>
    null
)

Method.methods["self_set_b"].add_surdef( 2, [ GenString, "to_String" ], ( obj, val ) =>
    obj.rp.data = val.toString()
)

Method.methods["equ_immediate"].add_surdef( 1, [ GenString, GenString ], ( obj, val ) =>
    obj.toString() == val.toString()
);

Method.methods["inf_immediate"].add_surdef( 1, [ GenString, GenString ], ( obj, val ) =>
    obj.toString() < val.toString()
);

Method.methods["sup_immediate"].add_surdef( 1, [ GenString, GenString ], ( obj, val ) =>
    obj.toString() > val.toString()
);

import OtWrapperString from "./OtWrapperString";

import BinaryReader    from "../../System/BinaryReader";
import BinaryWriter    from "../../System/BinaryWriter";
import DevId           from "../../System/DevId";
import UsrId           from "../../System/UsrId";
import PatchTypes      from "../Ot/PatchTypes";
import Variable        from "../Core/Variable";
import String          from "../String";
import Number          from "../Number";
