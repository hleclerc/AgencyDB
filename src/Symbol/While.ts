import Graphviz           from "../Core/Graphviz"
import { VarAnc }         from "../Core/Variable"
import methods            from "../Core/methods"
import Method             from "../Core/Method"
import Surdef             from "../Core/Surdef"
import Rp                 from "../Core/Rp"
import LvNumber           from "../LvNumber"
import { skv_link_o }     from "./SymbolicKnownValue"
import Interceptor        from "./Interceptor"
import Operation, { slo } from "./Operation"
import BlockCodegen       from "./BlockCodegen"
import { cd }             from "./Codegen"
import { get_nout }       from "./GetNout"
import Sym, { Link }      from "./Sym"
import { _if }            from "./If"

/**
 * An expression without input,
 * with outputs representing the while inputs
 */
export
class WhileInp extends Sym {
    constructor() {
        super( WhileInp );
    }

    to_String__b(): string {
        return "WhileInp";
    }

    nb_outputs() {
        return this.while_inst.children.length; // not the condition
    }

    while_inst: While;
}
Sym.make_templ( WhileInp );

/** */
export
class WhileOutCond extends Sym {
    constructor( expr: Array<Link> ) {
        super( WhileOutCond );
        this.children = expr;
    }

    to_String__b(): string {
        return "WhileOutCond";
    }

    block_code( cg: BlockCodegen, options ): void {
        if ( options.condition_code ) {
            options.condition_code.ok = cg.inline_code( this.children[ this.children.length - 1 ], 1e6, { conv_to_boolean: true } );
            options.condition_code.ko = "!" + cg.inline_code( this.children[ this.children.length - 1 ], Operation.prec.NOT, { conv_to_boolean: true } );
        }
    }

    while_inst: While;
}
Sym.make_templ( WhileOutCond );

/** */
export
class WhileOut extends Sym {
    constructor( expr: Array<Link> ) {
        super( WhileOut );
        this.children = expr;
    }

    to_String__b(): string {
        return "WhileOut";
    }

    while_inst: While;
}
Sym.make_templ( WhileOut );

/**
 * cond => first child
 */
export default 
class While extends Sym {
    constructor( expr: Array<Link>, inp_co: WhileInp, out_co: WhileOutCond, inp_bk: WhileInp, out_bk: WhileOut ) {
        super( While );
        this.children = expr;
        this.inp_co   = inp_co;
        this.out_co   = out_co;
        this.inp_bk   = inp_bk;
        this.out_bk   = out_bk;

        inp_co.while_inst = this;
        out_co.while_inst = this;
        inp_bk.while_inst = this;
        out_bk.while_inst = this;
    }

    to_String__b(): string {
        return `While`;
    }

    nb_outputs() {
        return this.out_bk.children.length;
    }

    block_code( cg: BlockCodegen, options ): void {
        // assign IfInp outputs var_names
        for( let ch of this.children ) {
            const name = cg.checked_var_name( ch );
            cd( this.inp_co ).var_names.push( name );
            cd( this.inp_bk ).var_names.push( name );
            cd( this        ).var_names.push( name );
        }
                
        //
        //const code_co = cg.block_code( this.out_co.children[ this.out_co.children.length - 1 ], 0, { conv_to_boolean: true } );
        let options_co = { condition_code: { ok: '', ko: '' } };
        const code_co = cg.block_code( [ this.out_co ], options_co );
        const code_bk = cg.block_code( [ this.out_bk ] );
        cg.inst( code_co.length ? `while(true){${ code_co }if(${ options_co.condition_code.ko })break;${ code_bk }}` : `while(${ options_co.condition_code.ok }){${ code_bk }}` );
    }

    self_ops(): Array<number> {
        let res = new Array<number>();
        for( let n = 0; n < this.out_bk.children.length; ++n )
            res.push( n );
        return res;
    }

    for_each_sub_block( cb: ( v: Sym ) => void ) {
        cb( this.out_co );
        cb( this.out_bk );
    }
    
    write_graphviz__b( gr: Graphviz ) {
        gr.node( this, this.nb_inputs(), this.nb_outputs(), `While` );
        gr.edge( this, -1, this.out_co, -1, { label: "co", style: "dashed", subgraph: { color: "yellow", style: "dotted" } } );
        gr.edge( this, -1, this.out_bk, -1, { label: "bk", style: "dashed", subgraph: { color: "green" , style: "dotted" } } );
        this.children.forEach( ( ch, num_ch ) => { gr.edge( this, num_ch, ch.item, ch.nout ); } );
    }

    inp_co  : WhileInp;
    out_co  : WhileOutCond;
    inp_bk  : WhileInp;
    out_bk  : WhileOut;
} 
Sym.make_templ( While );

export 
function _while( cond_cb: () => any, block_cb?: () => void ) {
    // intercept calls to get condition and modified variables
    let old_le = 0;
    let inp_co = new WhileInp();
    let inp_bk = new WhileInp();
    let inp_wh = new Array<VarAnc>(); // variable modified during the while (cond or block)
    let out_co = null as Array<Link>; // [ ...mod_co.entries() ].map( ( [ v, oan ] ) => skv_link( oan.n ) ).concat( cond );
    let out_bk = null as Array<Link>; // [ ...mod_co.entries() ].map( ( [ v, oan ] ) => skv_link( oan.n ) ).concat( cond );
    let mod_co = new Map<VarAnc,{ o: Rp, b: Rp, n: Rp }>();
    while ( true ) {
        // condition
        let cond = null as Link;
        Interceptor.run( mod_co, function() {
            const var_cond = new LvNumber( cond_cb() ).toBooleanVariable;
            cond = slo( var_cond.rp );
        }, function( val: VarAnc ) {
            const ind = inp_wh.indexOf( val );
            return get_nout( inp_co, ind >= 0 ? ind : inp_wh.push( val ) - 1 );
        } );

        // save condition + modified variables 
        out_co = [ ...mod_co.entries() ].map( ( [ v, oan ] ) => skv_link_o( oan.n ) ).concat( cond );

        // entry variables for the block
        let mod_bk = new Map<VarAnc,{ o: Rp, b: Rp, n: Rp }>();
        mod_co.forEach( ( oan, val ) => {
            const n = get_nout( inp_bk, inp_wh.indexOf( val ) );
            mod_bk.set( val, { o: oan.o, b: n, n } );
        } );

        // block (with mod_bk initialized by mod_co)
        Interceptor.run( mod_bk, block_cb, function( val: VarAnc ) {
            const ind = inp_wh.indexOf( val );
            return get_nout( inp_bk, ind >= 0 ? ind : inp_wh.push( val ) - 1 );
        } );

        // save modified variables 
        out_bk = [ ...mod_bk.entries() ].map( ( [ v, oan ] ) => skv_link_o( oan.n ) );

        // nb of modifications has not changed ?
        if ( old_le == mod_bk.size )
            break;
        old_le = mod_bk.size;

        // variables modified during bk, not seen by co
        mod_bk.forEach( ( oan, val ) => {
            if ( ! mod_co.get( val ) ) {
                const n = get_nout( inp_co, inp_wh.indexOf( val ) );
                mod_co.set( val, { o: oan.o, b: n, n } );
            }
        } );
    }

    // a While inst is basically a value modifier
    if ( Method.int_call_s )
        inp_wh.forEach( ( v, num ) => { Method.int_call_s( v ); } );

    // modify variables to take while outputs
    let rp_wh = new While( inp_wh.map( x => skv_link_o( x.rp ) ), inp_co, new WhileOutCond( out_co ), inp_bk, new WhileOut( out_bk ) );
    inp_wh.forEach( ( v, num ) => { v.rp = get_nout( rp_wh, num ); } );
}

