import methods       from "../Core/methods";
import Graphviz      from "../Core/Graphviz";
import Rp            from "../Core/Rp";
import BlockCodegen  from "./BlockCodegen";
import { cd }        from "./Codegen";
import GetNout       from "./GetNout";

export
interface Link {
    item: Sym;
    nout: number;
}

//
export default
class Sym extends Rp {
    static watch_self_op = true;
    static symbolic_value = true;

    constructor( cl ) {
        super( cl );
    }

    add_child( ch: Link ) {
        this.children.push( ch );
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
        if ( gr.node( this, this.nb_inputs(), this.nb_outputs(), this.repr_graphviz() ) )
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
    static dfs_unique( v: Array<Sym>, cb: ( x: Sym, ssb: number ) => void, go_into_subblocks = false ): void {
        ++Sym.cur_op_id;
        for( const x of v )
            _dfs_unique_rec( x, cb, go_into_subblocks, 0 );
    } 

    /** replace items. if cb return null, items are unchanged. */
    static dfs_repl_unique( v: Array<Sym>, cb: ( x: Sym ) => Array<Link>, go_into_subblocks = false ): Array<Sym> {
        ++Sym.cur_op_id;
        return [].concat( ...v.map( x => _dfs_repl_unique_rec( x, cb, go_into_subblocks ).map( l => l.item ) ) );
    }

    children            = new Array<Link>();
    op_mp               = {} as any;
    op_id               = 0;
    get_nouts           = new Array<GetNout>();

    static cur_op_id    = 0;
    static cur_sched_id = 0;
}

function _dfs_unique_rec( v: Sym, cb: ( x: Sym, ssb: number ) => void, go_into_subblocks, ssb = 0 ) {
    if ( v.op_id == Sym.cur_op_id )
        return;
    v.op_id = Sym.cur_op_id;

    // rec
    for( const ch of v.children )
        _dfs_unique_rec( ch.item, cb, go_into_subblocks, ssb );
    if ( go_into_subblocks )
        v.for_each_sub_block( sb => _dfs_unique_rec( sb, cb, go_into_subblocks, ssb + 1 ) );

    // call
    cb( v, ssb );
}

/** Return a Link for each output */
function _dfs_repl_unique_rec( v: Sym, cb: ( x: Sym ) => Array<Link>, go_into_subblocks ): Array<Link> {
    if ( v.op_id == Sym.cur_op_id )
        return v.op_mp as Array<Link>;
    v.op_id = Sym.cur_op_id;

    // call and register the result
    let res = cb( v );
    if ( ! res ) {
        res = [];
        for( let nout = 0; nout < v.nb_outputs(); ++nout )
            res.push( { item: v, nout } );
    }
    v.op_mp = res;

    // recursion
    for( let i = 0; i < res.length; ++i ) {
        for( let j = 0; ; ++j ) {
            if ( j == i ) {
                let n = res[ i ].item;
                if ( n instanceof Sym ) {
                    for( let i = 0; i < n.children.length; ++i ) {
                        const arr = _dfs_repl_unique_rec( n.children[ i ].item, cb, go_into_subblocks );
                        n.children[ i ] = arr[ n.children[ i ].nout ];
                    }
                    if ( go_into_subblocks )
                        n.for_each_sub_block( sb => _dfs_repl_unique_rec( sb, cb, go_into_subblocks ) );
                }
                break;
            }
            if ( res[ i ].item == res[ j ].item )
                break;
        }
    }

    return res;
}

require( "./SelectSymbolic" );
