import Method from "../Core/Method";
import Sym    from "./Sym";

//
export default
class GenSym extends Sym {
    constructor( cl : { new( Rp ) : Variable; }, name : string ) {
        super( GenSym );
        this.name = name;
        this.cl   = cl;
    }

    to_String() : string {
        return this.name;
    }

    graphviz_repr() : string {
        return this.name;
    }

    obj_type() {
        return this.cl;
    }

    name: string;
    cl  : { new( Rp ) : Variable; };
}
Sym.make_templ( GenSym );

import Variable from "../Core/Variable";
