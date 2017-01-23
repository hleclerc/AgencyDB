import Graphviz      from "../Core/Graphviz";
import methods       from "../Core/methods";
import Rp            from "../Core/Rp";
import { cd }        from "./Codegen";
import BlockCodegen  from "./BlockCodegen";

export
class Link {
    item: Sym;
    nout: number;
}

//
export default
class Sym extends Rp {
    static watch_self_op = true;

    constructor( cl ) {
        super( cl );
    }

    replace_child( old_ch: Link, new_ch: Link ) {
        for( let n in this.children )
            if ( this.children[ n ].item == old_ch.item && this.children[ n ].nout == old_ch.nout )
                this.children[ n ] = new_ch;
    }

    for_each_sub_block( cb: ( v: Sym ) => void ) {
    }

    variable_type__b() {
        console.error( "variable_type__b should be redefined for", this.constructor.name );
        return null;
    }

    write_graphviz__b( gr: Graphviz ) {
        gr.node( this, this.nb_inputs(), this.nb_outputs(), this.repr_graphviz() );
        this.children.forEach( ( ch, ind ) => gr.edge( this, ind, ch.item, ch.nout ) );
    }

    repr_graphviz() {
        return methods["to_String__b"].call_1( this );
    }

    nb_inputs() {
        return this.children.length;
    }
    
    nb_outputs() {
        return 1;
    }

    /** num of children that will be "selfed" (i.e. modified) */
    self_ops(): Array<number> {
        return [];
    }

    inline_code( cg: BlockCodegen, nout: number, prec: number, options: any ): string {
        return cd( this ).var_names[ nout ];
    }

    block_code( cg: BlockCodegen, options ): void {
    }

    static make_templ( type ) {
        Rp.make_templ( type );
    }

    /** Depth First Search unique (items are visited only once if they are of type Sym) */
    static dfs_u( v: Array<Sym>, cb: ( x: Sym, ssb: number ) => void, go_into_subblocks = false ) {
        ++Sym.cur_op_id;
        for( const x of v )
            _dfs_u_rec( x, cb, go_into_subblocks, 0 );
    } 


    children            = new Array<Link>();
    op_mp               = {} as any;
    op_id               = 0;

    static cur_op_id    = 0;
    static cur_sched_id = 0;
}

function _dfs_u_rec( v: Sym, cb: ( x: Sym, ssb: number ) => void, go_into_subblocks, ssb = 0 ) {
    if ( v.op_id == Sym.cur_op_id )
        return;
    v.op_id = Sym.cur_op_id;

    // rec
    for( const ch of v.children )
        _dfs_u_rec( ch.item, cb, go_into_subblocks, ssb );
    if ( go_into_subblocks )
        v.for_each_sub_block( sb => _dfs_u_rec( sb, cb, go_into_subblocks, ssb + 1 ) );

    // call
    cb( v, ssb );
}
