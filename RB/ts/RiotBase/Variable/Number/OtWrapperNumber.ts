import OtWrapper from "../Ot/OtWrapper";
import Method    from "../Core/Method";

//
export default
class OtWrapperNumber extends OtWrapper {
    // patch types
    static op_set = {
        attrs: [ "Variable" ],
        right( rp: OtWrapperNumber, flags : number, nv : Variable ) : boolean {
            return nv.sup_immediate( rp.val ) ? OtWrapperNumber.can_increase.ok( flags ) : nv.equ_immediate( rp.val ) || OtWrapperNumber.can_increase.ok( flags );
        },
        valid( rp: OtWrapperNumber, nv : Variable ) : boolean {
            throw new Error( "TODO" );
        },
        apply_op( rp: OtWrapperNumber, aod, nv : Variable ) : void {
            Method.methods["self_set_b"].call_2( rp.val, nv );
            rp.sig_change();
        },
        undo( rp: OtWrapperNumber, aod, nv : Variable ) : void {
            throw new Error( "Weird" );
        },
        update_patch_data_l0( rp: OtWrapperNumber, args ) : void {
        }
    };
    static op_set_und = {
        attrs: [ "Variable", "Variable" ],
        right( rp: OtWrapperNumber, flags : number, val : Variable, old : Variable ) : boolean {
            return OtWrapperNumber.op_set.right( rp, flags, val );
        },
        valid( rp: OtWrapperNumber, val : Variable, old : Variable ) : boolean {
            throw new Error( "TODO" );
        },
        apply_op( rp: OtWrapperNumber, aod, val : Variable, old : Variable ) : void {
            OtWrapperNumber.op_set.apply_op( rp, aod, val );
        },
        undo( rp: OtWrapperNumber, aod, val : Variable, old : Variable ) : void {
            throw new Error( "TODO" );
        },
        update_patch_data_l0( rp: OtWrapperNumber, args ) : void {
        }
    };
    static op_add = {
        attrs: [ "Variable" ],
        right( rp: OtWrapperNumber, flags : number, nv : Variable ) : boolean {
            return nv.sup_immediate( 0 ) ? OtWrapperNumber.can_increase.ok( flags ) : nv.equ_immediate( 0 ) || OtWrapperNumber.can_decrease.ok( flags );
        },
        valid( rp: OtWrapperNumber, nv : Variable ) : boolean {
            throw new Error( "TODO" );
        },
        apply_op( rp: OtWrapperNumber, aod, nv : Variable ) : void {
            Method.methods["self_add_b"].call_2( rp.val, nv );
            rp.sig_change();
        },
        undo( rp: OtWrapperNumber, aod, nv : Variable ) : void {
            throw new Error( "TODO" );
        },
        update_patch_data_l0( rp: OtWrapperNumber, args ) : void {
        }
    };

    static op_types = OtWrapper.op_types.concat( [
        OtWrapperNumber.op_set,
        OtWrapperNumber.op_set_und,
        OtWrapperNumber.op_add
    ] );

    // right types
    static can_read = {
        name: "read",
        flags: 4,
        ok( flags : number ) : boolean { return ( flags & 4 ) != 0; }
    }
    static can_increase = {
        name: "increase",
        flags: 8,
        ok( flags : number ) : boolean { return ( flags & 8 ) != 0; }
    }
    static can_decrease = {
        name: "decrease",
        flags: 16,
        ok( flags : number ) : boolean { return ( flags & 16 ) != 0; }
    }

    static right_types = OtWrapper.right_types.concat( [
        OtWrapperNumber.can_read,
        OtWrapperNumber.can_increase,
        OtWrapperNumber.can_decrease
    ] );

    constructor( rp : Rp ) {
        super( OtWrapperNumber );
        this.val = new Number( rp );

        // first patch
        if ( ! this.val.equ_immediate( 0 ) )
            OtWrapperNumber.reg_op( this.last_ops, OtWrapperNumber.op_set, this.val );
    }

    prox_ref() : Variable {
        return this.val;
    }

    _self_set( nv : Variable, usr_id = this.usr_id ) {
        if ( ! nv.equ_immediate( this.val ) && OtWrapperNumber.op_set_und.right( this, this.usr_right( usr_id ), nv, this.val ) ) {
            OtWrapperNumber.reg_op( this.last_ops, OtWrapperNumber.op_set_und, nv, this.val );
            OtWrapperNumber.op_set_und.apply_op( this, { usr: usr_id, type: 0 }, nv, this.val );
            return true;
        }
        return false;
    }

    _self_add( nv : Variable, usr_id = this.usr_id ) {
        if ( ! nv.equ_immediate( 0 ) && OtWrapperNumber.op_add.right( this, this.usr_right( usr_id ), nv ) ) {
            OtWrapperNumber.reg_op( this.last_ops, OtWrapperNumber.op_add, nv );
            OtWrapperNumber.op_add.apply_op( this, { usr: usr_id, type: 0 }, nv );
            return true;
        }
        return false;
    }

    val : Variable;
}
OtWrapper.make_templ( OtWrapperNumber );


Method.methods["self_set_o"].add_surdef( 2, [ OtWrapperNumber, "to_Number" ], function( num : { rp: OtWrapperNumber }, sup : Variable ) {
    return num.rp._self_set( sup );
} );

Method.methods["self_set_b"].add_surdef( 2, [ OtWrapperNumber, "to_Number" ], function( num : { rp: OtWrapperNumber }, sup : Number ) {
    return num.rp._self_set( sup.copy() );
} );

Method.methods["self_add_b"].add_surdef( 2, [ OtWrapperNumber, "to_Number" ], function( num : { rp: OtWrapperNumber }, sup : Variable ) {
    return num.rp._self_add( sup );
} );

Method.methods["self_sub_b"].add_surdef( 2, [ OtWrapperNumber, "to_Number" ], function( num : { rp: OtWrapperNumber }, sup : Variable ) {
    return num.rp._self_add( Method.methods["neg_b"].call_1( sup ) );
} );

import Variable from "../Core/Variable";
import Rp       from "../Core/Rp";
import Number   from "../Number";
