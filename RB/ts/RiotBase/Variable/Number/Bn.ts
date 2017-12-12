import DeclTypesForRW from "../Core/DeclTypesForRW";
import Method         from "../Core/Method";
import Rp             from "../Core/Rp";

// declare class BinaryWriter { write_PT( val: number ); }
// declare class BinaryReader { read_PT() : number; }
declare class DevId {}
declare class UsrId {}

//
function Bn( T ) {
    class _Bn extends Rp {
        constructor( val : number ) {
            super( _Bn );
            this.val = val
        }

        static term_value = true;
        static base_type  = T;

        to_String() : string {
            return '' + this.val;
        }

        to_Number() : number {
            return this.val;
        }

        graphviz_repr() : string {
            return this.to_String();
        }

        copy() : Number {
            return new Number( this.val );
        }

        write_to( bw : BinaryWriter, cur_dev : DevId, cur_usr : UsrId ) {
            if ( Math.floor( this.val ) != this.val ) {
                bw.write_PT( DeclTypesForRW.num.Bn_FP64 );
                bw.write_FP64( this.val );
            } else if ( this.val < 0 ) {
                throw new Error( "TODO" );
            } else if ( this.val >= 4294967296 ) {
                throw new Error( "TODO (64 bit integers)" );
            } else {
                bw.write_PT( DeclTypesForRW.num.Bn_PI32 );
                bw.write_PT( this.val );
            }
        }

        static read_from( br : BinaryReader, dev_id : DevId, usr_id : UsrId ) : Number {
            return new Number( br.read_PT() ); // IMPORTANT_TODO: may be signed, etc...
        }

        obj_type() {
            return Number;
        }

        make_ot_wrapper() {
            return new OtWrapperNumber( this );
        }

        val: number;
    }
    Rp.make_templ( _Bn, "Bn_" + T );

    Method.methods["self_set_b"].add_surdef( 2, [ _Bn, "to_Number" ], function( obj, val ) {
        let nv = val.toNumber();
        if ( obj.rp.val != nv ) {
            obj.rp.val = nv;
            return true;
        }
        return false;
    } );

    Method.methods["self_set_o"].add_surdef( 2, [ _Bn, "to_Number" ], function( obj, val ) {
        let nv = val.toNumber();
        if ( obj.rp.val != nv ) {
            obj.rp.val = nv;
            return true;
        }
        return false;
    } );

    return _Bn;
}

export var BN_FP64 = Bn( "FP64" );

DeclTypesForRW.readers[ DeclTypesForRW.num.Bn_PI32 ] = function( br: BinaryReader, dev_id : DevId, usr_id : UsrId ) : Number {
    return new Number( new BN_FP64( br.read_PT() ) );
};

DeclTypesForRW.readers[ DeclTypesForRW.num.Bn_PI64 ] = function( br: BinaryReader, dev_id : DevId, usr_id : UsrId ) : Number {
    throw new Error( "TODO" );
};

DeclTypesForRW.readers[ DeclTypesForRW.num.Bn_FP32 ] = function( br: BinaryReader, dev_id : DevId, usr_id : UsrId ) : Number {
    return new Number( new BN_FP64( br.read_FP32() ) );
};

DeclTypesForRW.readers[ DeclTypesForRW.num.Bn_FP64 ] = function( br: BinaryReader, dev_id : DevId, usr_id : UsrId ) : Number {
    return new Number( new BN_FP64( br.read_FP64() ) );
};


let num_methods = {
    is_eqz:   ( obj      ) => new Number( Method.methods["to_Number"].call_1( obj ) == 0 ),
    is_pos:   ( obj      ) => new Number( Method.methods["to_Number"].call_1( obj ) >  0 ),
    not:      ( obj      ) => new Number(  !  Method.methods["to_Number"].call_1( obj ) ),
    neg:      ( obj      ) => new Number(  -  Method.methods["to_Number"].call_1( obj ) ),
    is_equ:   ( o_0, o_1 ) => new Number( Method.methods["to_Number"].call_1( o_0 ) == Method.methods["to_Number"].call_1( o_1 ) ),
    is_inf:   ( o_0, o_1 ) => new Number( Method.methods["to_Number"].call_1( o_0 ) <  Method.methods["to_Number"].call_1( o_1 ) ),
    is_infeq: ( o_0, o_1 ) => new Number( Method.methods["to_Number"].call_1( o_0 ) <= Method.methods["to_Number"].call_1( o_1 ) ),
    add:      ( o_0, o_1 ) => new Number( Method.methods["to_Number"].call_1( o_0 ) +  Method.methods["to_Number"].call_1( o_1 ) ),
    sub:      ( o_0, o_1 ) => new Number( Method.methods["to_Number"].call_1( o_0 ) -  Method.methods["to_Number"].call_1( o_1 ) ),
    mul:      ( o_0, o_1 ) => new Number( Method.methods["to_Number"].call_1( o_0 ) *  Method.methods["to_Number"].call_1( o_1 ) ),
    div:      ( o_0, o_1 ) => new Number( Method.methods["to_Number"].call_1( o_0 ) /  Method.methods["to_Number"].call_1( o_1 ) )
}

// unary
for( let n of [ "is_eqz", "is_pos", "not", "neg" ] )
    Method.methods[ n + "_b" ].add_surdef( 1, [ "to_Number" ], num_methods[ n ] );

// binary
for( let n of [ "is_equ", "is_inf", "is_infeq", "add", "sub", "mul", "div" ] )
    Method.methods[ n + "_bb" ].add_surdef( 1, [ "to_Number", "to_Number" ], num_methods[ n ] );


import BinaryWriter    from "../../System/BinaryWriter";
import BinaryReader    from "../../System/BinaryReader";
import OtWrapperNumber from "./OtWrapperNumber"
import Number          from "../Number"
