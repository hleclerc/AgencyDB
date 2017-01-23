import methods      from "../Core/methods"
import BlockCodegen from "./BlockCodegen"
import Sym, { Link} from "./Sym"

/** 
 * Goal: make a single Rp from a Link 
*/
export default 
class GetNout extends Sym {
    constructor( rp: Link ) {
        super( GetNout );
        this.children.push( rp );
    }

    to_String__b(): string {
        return `GetNout(${ methods[ "to_String__b" ].call_1( this.children[ 0 ].item ) },${ this.children[ 0 ].nout })`;
    }

    repr_graphviz(): string {
        return `GN`;
    }

    block_code( cg: BlockCodegen, options ): void {
    }

    need_output() {
        return false;
    }
} 
Sym.make_templ( GetNout );


export
function get_nout( item: Sym, nout: number ) {
    return nout ? new GetNout( { item, nout } ) : item;
}
 
export
function unget_nout( item: Sym ): Link {
    return item instanceof GetNout ? item.children[ 0 ] : { item, nout: 0 }; 
}

