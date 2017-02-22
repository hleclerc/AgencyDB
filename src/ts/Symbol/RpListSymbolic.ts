import Sym, { Link } from '../Symbol/Sym'
import methods       from '../Core/methods'  
import Rp            from '../Core/Rp'  

/** */
export default
class RpListSymbolic extends Sym {
    constructor( lst: Array<Link> ) {
        super( RpListSymbolic );
        lst.forEach( x => this.children.push( x ) );
    }

    to_String__b(): string {
        return `[${ this.children.map( x => methods["to_String__b"].call_1( x.item ) ).join( "," ) }]`;
    }
}
Rp.make_templ( RpListSymbolic );

// methods["symbolic_known_value__b"].add_surdef( 3, [ RpListKnown ], function( rl: RpListKnown ) {
//     return new RpListSymbolic( rl.lst.map( x => methods["symbolic_known_value__b"].call_1( x ) ) );
// } );
