//// nsmake ifndef NO_OPERATIONAL_TRANSFORM
import OtWrapperString from "./OtWrapperString"
//// nsmake endif // NO_OPERATIONAL_TRANSFORM
import methods         from "../Core/methods"
import Rp              from "../Core/Rp"
import LvString        from "../LvString"
import LvNumber        from "../LvNumber"

export default 
class GenString extends Rp {
    constructor( data: string ) {
        super( GenString );
        this.data = data;
    }

    to_String__b(): string {
        return this.data;
    }

    js_repr__b( prec: number ): string {
        return `"${ this.data }"`;
    }

    variable_type__b() {
        return LvString;
    }

    copy__b(): Rp {
        return new GenString( this.data );
    }

    to_Boolean__b(): boolean {
        return Boolean( this.data );
    }

    get_size__b(): LvNumber {
        return new LvNumber( this.data.length );
    }

    obj_type__b() {
        return String;
    }

    make_ot_wrapper__s(): Rp {
        //// nsmake ifndef NO_OPERATIONAL_TRANSFORM
        return new OtWrapperString( this );
        //// nsmake endif // NO_OPERATIONAL_TRANSFORM
        //// nsmake uncomment_ifdef NO_OPERATIONAL_TRANSFORM
        // return null;
        //// nsmake endif // NO_OPERATIONAL_TRANSFORM
    }

    get_possible_rights__b(): Array<string> {
        //// nsmake ifndef NO_OPERATIONAL_TRANSFORM
        return OtWrapperString.prototype.get_possible_rights__b();
        //// nsmake endif // NO_OPERATIONAL_TRANSFORM
        //// nsmake uncomment_ifdef NO_OPERATIONAL_TRANSFORM
        // return ["NO_OPERATIONAL_TRANSFORM"];
        //// nsmake endif // NO_OPERATIONAL_TRANSFORM
    }

    //// nsmake endif // NO_OPERATIONAL_TRANSFORM

    // get_patch_type( bw : BinaryWriter ) {
    //     bw.write_PT( PatchTypes.num.OtWrapperString_GenString );
    // }

    // cmp_patch_type( br : BinaryReader ) : boolean {
    //     return br.read_PT() == PatchTypes.num.OtWrapperString_GenString;
    // }

    // write_to( bw : BinaryWriter, only_for_a_copy : boolean, cur_dev : DevId, cur_usr : UsrId ) {
    //     bw.write_PT( DeclTypesForRW.num.GenString );
    //     bw.write_String( this.data );
    // }

    // static read_GenString_from( br: BinaryReader, dev_id : DevId, usr_id : UsrId ) : String {
    //     return new String( br.read_String() );
    // }

    data: string;
} 
Rp.make_templ( GenString );

methods[ "add__bb"           ].add_surdef( 10, [ GenString, GenString ], ( a: GenString, b: GenString ) => new GenString( a.data + b.data ) );
methods[ "set__sb"           ].add_surdef( 10, [ GenString, GenString ], ( a: GenString, b: GenString ) => { a.data = b.data; return a; } );
       
methods[ "set__sbb"          ].add_surdef( 10, [ GenString, "to_Number__b", "to_String__b" ], ( a: GenString, b: Rp, c: Rp ) => { const n = methods[ "to_Number__b" ].call_1( b ); a.data = a.data.substr( 0, n ) + methods[ "to_String__b" ].call_1( c ) + a.data.substr( n + 1 ); return a; } );
methods[ "insert__sbb"       ].add_surdef( 10, [ GenString, "to_Number__b", "to_String__b" ], ( a: GenString, b: Rp, c: Rp ) => { const n = methods[ "to_Number__b" ].call_1( b ); a.data = a.data.substr( 0, n ) + methods[ "to_String__b" ].call_1( c ) + a.data.substr( n ); return a; } );
methods[ "remove__sbb"       ].add_surdef( 10, [ GenString, "to_Number__b", "to_Number__b" ], ( a: GenString, b: Rp, c: Rp ) => { const n = methods[ "to_Number__b" ].call_1( b ), q = methods[ "to_Number__b" ].call_1( c ); a.data = a.data.substr( 0, n ) + a.data.substr( n + q ); return a; } );
       
methods[ "select__bb"        ].add_surdef( 10, [ GenString, "to_Number__b" ], ( a: GenString, b: Rp ) => { return new GenString( a.data[ methods[ "to_Number__b" ].call_1( b ) ] ); } );
      
methods[ "slice__bbb"        ].add_surdef( 10, [ GenString, "to_Number__b", "to_Number__b" ], ( a: GenString, b: Rp, c: Rp ) => { return new GenString( a.data.substring( methods[ "to_Number__b" ].call_1( b ), methods[ "to_Number__b" ].call_1( c ) ) ); } );

methods[ "inf_immediate__bb" ].add_surdef( 10, [ GenString, GenString ], ( a: GenString, b: GenString ) => a.data < b.data );
methods[ "sup_immediate__bb" ].add_surdef( 10, [ GenString, GenString ], ( a: GenString, b: GenString ) => a.data > b.data );

