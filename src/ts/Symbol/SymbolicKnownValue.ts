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

methods["symbolic_known_value__o"].add_surdef( 2, type_0 => type_0.symbolic_value, function( sym: Sym ) {
    return { item: sym, nout: 0 } as Link;
} );

methods["symbolic_known_value__o"].add_surdef( 1, [ () => true ], function( obj: Rp ) {
    return { item: new SymbolicKnownValue( obj ), nout: 0 };
} );


/** turn to symbolic known value if not symbolic */
export
function skv_link_o( value: Rp ): Link {
    return value instanceof Sym ? unget_nout( value ) : methods[ "symbolic_known_value__o" ].call_1( value );
}

export
function skv_link_b( value: Rp ): Link {
    return value instanceof Sym ? unget_nout( value ) : methods[ "symbolic_known_value__b" ].call_1( value );
}
