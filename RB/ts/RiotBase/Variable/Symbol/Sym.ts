import Rp from "../Core/Rp";

//
export default
class Sym extends Rp {
    static unknown_value = true;
    static term_value    = true;

    constructor( cl ) {
        super( cl );
        this.csd = new CommonSymbolicData;
    }

    common_symbolic_data() {
        return this.csd;
    }

    copy() : Variable {
        return this.as_a_Variable();
    }

    ww_set_lazy_value( val: Variable ) {
        this.csd.ww_then( val );
    }

    csd : CommonSymbolicData;
}
Rp.make_templ( Sym );

import CommonSymbolicData from "./CommonSymbolicData";
import Variable           from "../Core/Variable";

declare function require( s: string );
require( "./SymOp" );
