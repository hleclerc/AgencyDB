import methods         from "../Core/methods"
import { BN_FP64 }     from "../Number/Bn"
import Rp              from "../Core/Rp"
import LvNumber        from "../LvNumber"
import LvBuffer        from "../LvBuffer"

export default 
class GenBuffer extends Rp {
    constructor( data: Buffer ) {
        super( GenBuffer );
        this.data = data;
    }

    variable_type__b() {
        return LvBuffer;
    }

    to_String__b(): string {
        return this.data.toString();
    }

    js_repr__b( prec: number ): string {
        return `Buffer.from("${ this.data.toString() })")`;
    }

    copy__b(): Rp {
        return new GenBuffer( Buffer.from( this.data ) );
    }

    to_Boolean__b(): boolean {
        return Boolean( this.data );
    }

    get_size__b(): LvNumber {
        return new LvNumber( this.data.byteLength );
    }

    data: Buffer;
} 
Rp.make_templ( GenBuffer );

// methods[ "add__bb"    ].add_surdef( 10, [ GenBuffer, GenBuffer ], ( a: GenBuffer, b: GenBuffer ) => new GenBuffer( a.data + b.data ) );
// methods[ "set__sb"    ].add_surdef( 10, [ GenBuffer, GenBuffer ], ( a: GenBuffer, b: GenBuffer ) => { a.data = b.data; return a; } );

methods[ "set__sbb"   ].add_surdef( 10, [ GenBuffer, "to_Number__b", "to_Number__b" ], ( a: GenBuffer, b: Rp, c: Rp ) => { a.data[ methods[ "to_Number__b" ].call_1( b ) ] = methods[ "to_Number__b" ].call_1( c ); return a; } );

methods[ "select__bb" ].add_surdef( 10, [ GenBuffer, "to_Number__b" ], ( a: GenBuffer, b: Rp ) => new BN_FP64( a.data[ methods[ "to_Number__b" ].call_1( b ) ] ) );



