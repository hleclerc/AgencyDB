import Variable, { toLv_array } from "./Core/Variable"
import methods                  from "./Core/methods"
import Rp                       from "./Core/Rp"
             
import RpSymbol                 from "./Symbol/RpSymbol"
import { BN_FP64 }              from "./LvNumber/Bn"
             
//
export default
class LvNumber extends Variable<LvNumber> {
    static NativeType = Number;

    constructor( val = 0 as Rp | LvNumber | number | boolean ) {
        if      ( val instanceof Rp       ) super( val );
        else if ( val instanceof LvNumber ) super( methods[ "copy__b" ].call_1( val.rp ) );
        else                                super( LvNumber.makeRp( val ) );
    }

    static makeRp( val: number | boolean ) {
        return typeof( val ) == "number" ? new BN_FP64( val ) : new BN_FP64( val ? 1 : 0 );
    }

    static symbol( name: string ): LvNumber {
        return new LvNumber( new RpSymbol( LvNumber, name ) );
    }

    copy() : LvNumber {
        return new LvNumber( methods["copy__b"].call_1( this.rp ) );
    }

    get(): number {
        return this.toNumber();
    }

    set( nv: number | LvNumber ) {
        this.rp = typeof nv == 'number' ? methods[ "set__so" ].call_2s( this, new BN_FP64( nv ) ) : methods[ "set__sb" ].call_2s( this, ( nv as LvNumber ).rp );
        return this;
    }

    get val() : number {
        return this.get();
    }

    set val( n : number ) {
        this.set( n );
    }

    selfNotBin      (): LvNumber { this.rp = methods[ "not_bin__s" ].call_1s( this ); return this; }
    selfNotLog      (): LvNumber { this.rp = methods[ "not_log__s" ].call_1s( this ); return this; }
    selfNeg         (): LvNumber { this.rp = methods[ "neg__s"     ].call_1s( this ); return this; }
    
    notLog          (): LvNumber { return new LvNumber( methods[ "not_log__b" ].call_1( this.rp ) ); }
    notBin          (): LvNumber { return new LvNumber( methods[ "not_bin__b" ].call_1( this.rp ) ); }
    neg             (): LvNumber { return new LvNumber( methods[ "neg__b"     ].call_1( this.rp ) ); }

    add             ( val: number | LvNumber ): LvNumber { return new LvNumber( typeof val == 'number' ? methods[ "add__bo"                ].call_2( this.rp, new BN_FP64( val ) ) : methods[ "add__bb"                ].call_2( this.rp, val.rp ) ); }
    sub             ( val: number | LvNumber ): LvNumber { return new LvNumber( typeof val == 'number' ? methods[ "sub__bo"                ].call_2( this.rp, new BN_FP64( val ) ) : methods[ "sub__bb"                ].call_2( this.rp, val.rp ) ); }
    mul             ( val: number | LvNumber ): LvNumber { return new LvNumber( typeof val == 'number' ? methods[ "mul__bo"                ].call_2( this.rp, new BN_FP64( val ) ) : methods[ "mul__bb"                ].call_2( this.rp, val.rp ) ); }
    div             ( val: number | LvNumber ): LvNumber { return new LvNumber( typeof val == 'number' ? methods[ "div__bo"                ].call_2( this.rp, new BN_FP64( val ) ) : methods[ "div__bb"                ].call_2( this.rp, val.rp ) ); }
     
    orLog           ( val: number | LvNumber ): LvNumber { return new LvNumber( typeof val == 'number' ? methods[ "or_log__bo"             ].call_2( this.rp, new BN_FP64( val ) ) : methods[ "or_log__bb"             ].call_2( this.rp, val.rp ) ); }
    andLog          ( val: number | LvNumber ): LvNumber { return new LvNumber( typeof val == 'number' ? methods[ "and_log__bo"            ].call_2( this.rp, new BN_FP64( val ) ) : methods[ "and_log__bb"            ].call_2( this.rp, val.rp ) ); }
    xorLog          ( val: number | LvNumber ): LvNumber { return new LvNumber( typeof val == 'number' ? methods[ "xor_log__bo"            ].call_2( this.rp, new BN_FP64( val ) ) : methods[ "xor_log__bb"            ].call_2( this.rp, val.rp ) ); }
                        
    orBin           ( val: number | LvNumber ): LvNumber { return new LvNumber( typeof val == 'number' ? methods[ "or_bin__bo"             ].call_2( this.rp, new BN_FP64( val ) ) : methods[ "or_bin__bb"             ].call_2( this.rp, val.rp ) ); }
    andBin          ( val: number | LvNumber ): LvNumber { return new LvNumber( typeof val == 'number' ? methods[ "and_bin__bo"            ].call_2( this.rp, new BN_FP64( val ) ) : methods[ "and_bin__bb"            ].call_2( this.rp, val.rp ) ); }
    xorBin          ( val: number | LvNumber ): LvNumber { return new LvNumber( typeof val == 'number' ? methods[ "xor_bin__bo"            ].call_2( this.rp, new BN_FP64( val ) ) : methods[ "xor_bin__bb"            ].call_2( this.rp, val.rp ) ); }
                        
    isEqu           ( val: number | LvNumber ): LvNumber { return new LvNumber( typeof val == 'number' ? methods[ "is_equ__bo"             ].call_2( this.rp, new BN_FP64( val ) ) : methods[ "is_equ__bb"             ].call_2( this.rp, val.rp ) ); }
    isInf           ( val: number | LvNumber ): LvNumber { return new LvNumber( typeof val == 'number' ? methods[ "is_inf__bo"             ].call_2( this.rp, new BN_FP64( val ) ) : methods[ "is_inf__bb"             ].call_2( this.rp, val.rp ) ); }
    isInfEq         ( val: number | LvNumber ): LvNumber { return new LvNumber( typeof val == 'number' ? methods[ "is_infeq__bo"           ].call_2( this.rp, new BN_FP64( val ) ) : methods[ "is_infeq__bb"           ].call_2( this.rp, val.rp ) ); }
    isNeq           ( val: number | LvNumber ): LvNumber { return new LvNumber( typeof val == 'number' ? methods[ "is_neq__bo"             ].call_2( this.rp, new BN_FP64( val ) ) : methods[ "is_neq__bb"             ].call_2( this.rp, val.rp ) ); }
    isSup           ( val: number | LvNumber ): LvNumber { return new LvNumber( typeof val == 'number' ? methods[ "is_sup__bo"             ].call_2( this.rp, new BN_FP64( val ) ) : methods[ "is_sup__bb"             ].call_2( this.rp, val.rp ) ); }
    isSupEq         ( val: number | LvNumber ): LvNumber { return new LvNumber( typeof val == 'number' ? methods[ "is_supeq__bo"           ].call_2( this.rp, new BN_FP64( val ) ) : methods[ "is_supeq__bb"           ].call_2( this.rp, val.rp ) ); }

    signedShiftLeft ( val: number | LvNumber ): LvNumber { return new LvNumber( typeof val == 'number' ? methods[ "signed_shift_left__bo"  ].call_2( this.rp, new BN_FP64( val ) ) : methods[ "signed_shift_left__bb"  ].call_2( this.rp, val.rp ) ); }
    signedShiftRight( val: number | LvNumber ): LvNumber { return new LvNumber( typeof val == 'number' ? methods[ "signed_shift_right__bo" ].call_2( this.rp, new BN_FP64( val ) ) : methods[ "signed_shift_right__bb" ].call_2( this.rp, val.rp ) ); }
    zfillShiftRight ( val: number | LvNumber ): LvNumber { return new LvNumber( typeof val == 'number' ? methods[ "zfill_shift_right__bo"  ].call_2( this.rp, new BN_FP64( val ) ) : methods[ "zfill_shift_right__bb"  ].call_2( this.rp, val.rp ) ); }
    
    selfAdd         ( val: number | LvNumber ): LvNumber { this.rp = typeof val == 'number' ? methods[ "add__so"     ].call_2s( this, new BN_FP64( val ) ) : methods[ "add__sb"     ].call_2s( this, val.rp ); return this; }
    selfSub         ( val: number | LvNumber ): LvNumber { this.rp = typeof val == 'number' ? methods[ "sub__so"     ].call_2s( this, new BN_FP64( val ) ) : methods[ "sub__sb"     ].call_2s( this, val.rp ); return this; }
    selfMul         ( val: number | LvNumber ): LvNumber { this.rp = typeof val == 'number' ? methods[ "mul__so"     ].call_2s( this, new BN_FP64( val ) ) : methods[ "mul__sb"     ].call_2s( this, val.rp ); return this; }
    selfDiv         ( val: number | LvNumber ): LvNumber { this.rp = typeof val == 'number' ? methods[ "div__so"     ].call_2s( this, new BN_FP64( val ) ) : methods[ "div__sb"     ].call_2s( this, val.rp ); return this; }
     
    selfOrBin       ( val: number | LvNumber ): LvNumber { this.rp = typeof val == 'number' ? methods[ "or_bin__so"  ].call_2s( this, new BN_FP64( val ) ) : methods[ "or_bin__sb"  ].call_2s( this, val.rp ); return this; }
    selfXorBin      ( val: number | LvNumber ): LvNumber { this.rp = typeof val == 'number' ? methods[ "xor_bin__so" ].call_2s( this, new BN_FP64( val ) ) : methods[ "xor_bin__sb" ].call_2s( this, val.rp ); return this; }
    selfAndBin      ( val: number | LvNumber ): LvNumber { this.rp = typeof val == 'number' ? methods[ "and_bin__so" ].call_2s( this, new BN_FP64( val ) ) : methods[ "and_bin__sb" ].call_2s( this, val.rp ); return this; }
}

toLv_array.push( val =>
    typeof val == "number" || typeof val == "boolean" ? new LvNumber( val ) : null
);
