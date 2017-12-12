import Lazyfier from "../Variable/Symbol/Lazyfier";
import Variable from "../Variable/Core/Variable";
import Rp       from "../Variable/Core/Rp";
import Map      from "./Map";

//
export default
class RenderGraphviz {
    static new_img( elem : HTMLElement = document.getElementById( 'test_div' ) ) : HTMLImageElement {
        let img = document.createElement( "img" );
        elem.appendChild( img );
        return img;
    }

    static disp_graph( obj_list : Array<Variable>, img : HTMLImageElement = RenderGraphviz.new_img() ) {
        let rg = new RenderGraphviz;
        for( let obj of obj_list )
            rg._add_obj_rec( obj.rp instanceof Lazyfier ? (<Lazyfier>obj.rp).sym : obj );
        img.src = `/_graph?gr=${ encodeURIComponent( rg.make_string() ) }`;
    }

    constructor() {
        this.objects = new Map<Rp,string>();
        this.cpt     = 0;
    }

    make_string() : string {
        let res = "";
        this.objects.forEach( ( n, rbj ) => {
            let obj = new Variable( rbj );
            res += `${ n } [label="${ obj.graphviz_repr() }"];`;

            for( let ch of obj.symbolic_children() )
                res += `${ n }->${ this.objects.get( ch.rp ) };`;

            let csd = obj.common_symbolic_data();
            if ( csd ) {
                // parents
                for( let pa of csd.parents ) {
                    if ( this.objects.get( pa ) )
                        res += `${ this.objects.get( pa ) }->${ n } [color=red];`;
                    else
                        res += `${ new Variable( pa ).graphviz_repr() }->${ n } [color=red];`;
                }
                // lazyfier_parents
                for( let pa of csd.lazyfier_parents ) {
                    this.cpt += 1;
                    res += `${ this.cpt } [shape=rect,label=LZ];`;
                    res += `${ this.cpt }->${ n } [color=green];`;
                }
            }
        } );
        return res;
    }

    _add_obj_rec( obj : Variable ) {
        if ( this.objects.has( obj.rp ) )
            return;
        this.objects.set( obj.rp, '_' + ( this.cpt += 1 ) );

        for( let ch of obj.symbolic_children() )
            this._add_obj_rec( ch );
    }

    objects: Map<Rp,string>;
    cpt    : number;
}
