// //// nsmake ifndef NO_OPERATIONAL_TRANSFORM
// import OtWrapperString from "./OtWrapperString"
// //// nsmake endif // NO_OPERATIONAL_TRANSFORM
import methods         from "../Core/methods"
import Rp              from "../Core/Rp"
import LvNumber        from "../LvNumber"
import LvObj           from "../LvObj"
import RpObjItem      from "./RpObjItem"

/**
 * generic object. Stores
 * 
 * Proposition de format: 
 */
export default 
class RpObj extends Rp {
    static type_map = new Map<any,number>();

    constructor() {
        super( RpObj );
    }

    // to_String__b(): string {
    //     return this.data;
    // }

    // js_repr__b( prec: number ): string {
    //     return `"${ this.data }"`;
    // }

    variable_type__b() {
        return LvObj;
    }

    // copy__b(): Rp {
    //     return new RpObj( this.data );
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

    data : Array<RpObjItem>; /** */
    entry: number;            /** index of entry point in data */
} 
Rp.make_templ( RpObj );

