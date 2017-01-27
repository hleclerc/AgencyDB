import { VarAnc }   from "../Core/Variable"
import methods      from "../Core/methods"
import { BN_PT }    from "../Number/Bn"
import Rp           from "../Core/Rp"
import GenMap       from "./GenMap"

/**
 */
export default 
class SelectGenMap extends Rp {
    constructor( variable: VarAnc, key: Rp ) {
        super( SelectGenMap );
        this.variable = variable;
        this.key      = key;
    }

    variable_type__b() {
        return ( this.variable.rp as GenMap ).val_type;
    }

    // to_String__b(): string {
    //     return `proute`;
    // }

    // js_repr__b( prec: number ): string {
    //     return `Buffer.from("${ this.data.toString() })")`;
    // }

    // copy__b(): Rp {
    //     return new Map( Buffer.from( this.data ) );
    // }

    // to_Boolean__b(): boolean {
    //     return Boolean( this.data );
    // }

    // get_size__b(): LvNumber {
    //     return new LvNumber( this.data.byteLength );
    // }

    variable: VarAnc;
    key     : Rp;
} 
Rp.make_templ( SelectGenMap );
