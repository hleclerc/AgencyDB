import methods        from "../Core/methods"
import Rp             from "../Core/Rp"
import { unget_nout } from "./GetNout"
import Sym, { Link }  from "./Sym"

export default 
class SymbolicKnownValue extends Sym {
    constructor( rp: Rp ) {
        super( SymbolicKnownValue );
        this.value = rp;
    }

    to_String__b(): string {
        return methods[ "to_String__b" ].call_1( this.value );
    }

    repr_graphviz() {
        return methods[ "js_repr__b" ].call_1( this.value, 0 );
    }

    block_code( cg, options ): void {
    }

    inline_code( cg, nout: number, prec: number, options ): string {
        return methods[ "js_repr__b" ].call_1( this.value, prec );
    }

    value: Rp;
} 
Sym.make_templ( SymbolicKnownValue );


/** turn to symbolic known value if not symbolic */
export
function skv( value: Rp, need_cp = false ): Sym {
    if ( need_cp )
        value = methods[ "copy__b" ].call_1( value );
    return value instanceof Sym ? value : new SymbolicKnownValue( value );
}

/** turn to symbolic known value if not symbolic */
export
function skv_link( value: Rp, need_cp = false ): Link {
    if ( need_cp )
        value = methods[ "copy__b" ].call_1( value );
    return value instanceof Sym ? unget_nout( value ) : { item: new SymbolicKnownValue( value ), nout: 0 };
}
