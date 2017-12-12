import Variable from "./Core/Variable"

declare var global: any;

//
export default
class Number extends Variable {
    static NativeType = ( typeof self != "undefined" ? self : global ).Number // shortcut for people willing to import this as 'Number'

    constructor( val? ) {
        if      ( val instanceof Rp            ) super( val );
        else if ( val instanceof Number        ) super( Method.methods["copy"].call_1( val ) );
        else if ( typeof( val ) == "number"    ) super( new Bn.BN_FP64( val ) );
        else if ( typeof( val ) == "boolean"   ) super( new Bn.BN_FP64( val ) );
        else if ( typeof( val ) == "undefined" ) super( new Bn.BN_FP64( 0   ) );
        else throw new Error( "TODO Number from" + typeof( val ) );
    }

    copy() : Number {
        return Method.methods["copy"].call_1( this );
    }

    get val() : number {
        return this.toNumber();
    }

    set val( n : number ) {
        this.set( n );
    }

    self_add( val ) : Number { return Method.methods["self_add_b"].call_2( this, Number.rp_conv( val ) ); }
    self_sub( val ) : Number { return Method.methods["self_sub_b"].call_2( this, Number.rp_conv( val ) ); }
    self_mul( val ) : Number { return Method.methods["self_mul_b"].call_2( this, Number.rp_conv( val ) ); }
    self_div( val ) : Number { return Method.methods["self_div_b"].call_2( this, Number.rp_conv( val ) ); }

    self_not() : Number { return Method.methods["self_not"].call_1( this ); }
    self_neg() : Number { return Method.methods["self_neg"].call_1( this ); }

    add( val ) : Number { return Method.methods["add_bb"].call_2( this, Number.rp_conv( val ) ); }
    sub( val ) : Number { return Method.methods["sub_bb"].call_2( this, Number.rp_conv( val ) ); }
    mul( val ) : Number { return Method.methods["mul_bb"].call_2( this, Number.rp_conv( val ) ); }
    div( val ) : Number { return Method.methods["div_bb"].call_2( this, Number.rp_conv( val ) ); }

    not() : Number { return Method.methods["not_b"].call_1( this ); }
    neg() : Number { return Method.methods["neg_b"].call_1( this ); }

    is_equ  ( val ) : Number { return Method.methods["is_equ_bb"  ].call_2( this, Number.rp_conv( val ) ); }
    is_inf  ( val ) : Number { return Method.methods["is_inf_bb"  ].call_2( this, Number.rp_conv( val ) ); }
    is_infeq( val ) : Number { return Method.methods["is_infeq_bb"].call_2( this, Number.rp_conv( val ) ); }
    is_neq  ( val ) : Number { return this.is_equ  ( val ).not(); }
    is_sup  ( val ) : Number { return this.is_infeq( val ).not(); }
    is_supeq( val ) : Number { return this.is_inf  ( val ).not(); }
}


import Bn = require( "./Number/Bn" );
import Method from "./Core/Method"
import Rp     from "./Core/Rp"
