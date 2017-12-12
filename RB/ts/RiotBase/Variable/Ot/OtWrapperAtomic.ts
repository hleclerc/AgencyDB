import Method    from "../Core/Method";
import OtWrapper from "./OtWrapper";

//
export default
class OtWrapperAtomic extends OtWrapper {
    // patch types
    static op_set = {
        attrs: [ "Variable" ],
        right( rp: OtWrapperAtomic, flags : number, val : Variable ) : boolean {
            return OtWrapperAtomic.can_write.ok( flags );
        },
        valid( rp: OtWrapperAtomic, val : Variable ) : boolean {
            return val.rp != null;
        },
        apply_op( rp: OtWrapperAtomic, aod, val : Variable ) : void {
            rp.old = val;
            rp.val = val;
            rp.sig_change();
        },
        undo( rp: OtWrapperAtomic, aod, val : Variable ) : void {
            rp.old = val;
        },
        update_patch_data_l0( rp: OtWrapperAtomic, args ) : void {
        }
    };

    static op_set_und = {
        attrs: [ "Variable", "Variable" ],
        right( rp: OtWrapperAtomic, flags : number, val : Variable, old : Variable ) : boolean {
            return OtWrapperAtomic.op_set.right( rp, flags, val );
        },
        valid( rp: OtWrapperAtomic, val : Variable, old : Variable ) : boolean {
            return val.rp != null;
        },
        apply_op( rp: OtWrapperAtomic, aod, val : Variable, old : Variable ) : void {
            OtWrapperAtomic.op_set.apply_op( rp, aod, val );
        },
        undo( rp: OtWrapperAtomic, aod, val : Variable, old : Variable ) : void {
            rp.old = val;
            rp.val = old;
        },
        update_patch_data_l0( rp: OtWrapperAtomic, args ) : void {
        }
    };

    static op_types = OtWrapper.op_types.concat( [
        OtWrapperAtomic.op_set,
        OtWrapperAtomic.op_set_und
    ] );

    // right types
    static can_read = {
        flags: 4,
        name: "read",
        ok( flags : number ) : boolean { return ( flags & 4 ) != 0; }
    }
    static can_write = {
        flags: 8,
        name: "write",
        ok( flags : number ) : boolean { return ( flags & 8 ) != 0; }
    }

    static right_types = OtWrapper.right_types.concat( [
        OtWrapperAtomic.can_read,
        OtWrapperAtomic.can_write
    ] );

    constructor( val : Variable, old : Variable, usr_id = new UsrId ) {
        super( OtWrapperAtomic, usr_id );
        this.old = old;
        this.val = val;
    }

    prox_ref() : Variable {
        return this.val;
    }

    freeze_last_changes() {
        if ( ! this.val.equ_immediate( this.old ) ) {
            OtWrapperAtomic.reg_op( this.last_ops, OtWrapperAtomic.op_set_und, this.val, this.old );
            this.old = (<any>this.val).copy();
        }
    }

    old: Variable;
    val: Variable;
}
OtWrapper.make_templ( OtWrapperAtomic );


Method.methods["self_set_b"].add_surdef( 2, [ OtWrapperAtomic, ( t ) => t.term_value ], function( str : { rp : OtWrapperAtomic }, sup ) : boolean {
    if ( ! str.rp.val.equ_immediate( sup ) ) {
        str.rp.val = sup.copy();
        str.rp.sig_change();
        return true;
    }
    return false;
} );

Method.methods["self_set_o"].add_surdef( 2, [ OtWrapperAtomic, ( t ) => t.term_value ], function( str : { rp : OtWrapperAtomic }, sup ) : boolean {
    if ( ! str.rp.val.equ_immediate( sup ) ) {
        str.rp.val = sup;
        str.rp.sig_change();
        return true;
    }
    return false;
} );

Method.methods["self_ot_merge_b"].add_surdef( 2, [ OtWrapperAtomic, ( t ) => t.term_value ], function( str : { rp : OtWrapperAtomic }, sup, as_usr : UsrId ) : boolean {
    let res = Method.methods["self_ot_merge_b"].call_2( str.rp.val, sup, as_usr );
    if ( res ) str.rp.sig_change();
    return res;
} );

import UsrId    from "../../System/UsrId";
import Variable from "../Core/Variable";
