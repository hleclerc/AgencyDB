// //// nsmake ifndef NO_OPERATIONAL_TRANSFORM
// import OtWrapperString from "./OtWrapperString"
// //// nsmake endif // NO_OPERATIONAL_TRANSFORM
import methods         from "../Core/methods"
import Rp              from "../Core/Rp"
import LvNumber        from "../LvNumber"
import LvObj           from "../LvObj"

/**
 * 
 */
export default 
class GenObj extends Rp {
    constructor() {
        super( GenObj );
    }

    to_String__b(): string {
        return this.data;
    }

    js_repr__b( prec: number ): string {
        return `"${ this.data }"`;
    }

    variable_type__b() {
        return LvObj;
    }

    // copy__b(): Rp {
    //     return new GenObj( this.data );
    // }

    // to_Boolean__b(): boolean {
    //     return Boolean( this.data );
    // }

    // get_size__b(): LvNumber {
    //     return new LvNumber( this.data.length );
    // }

    obj_type__b() {
        return Object;
    }

    data: string;
} 
Rp.make_templ( GenObj );

