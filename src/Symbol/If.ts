import { VarAnc }        from "../Core/Variable"
import Graphviz          from "../Core/Graphviz"
import methods           from "../Core/methods"
import Method            from "../Core/Method"
import Surdef            from "../Core/Surdef"
import Rp                from "../Core/Rp"
import LvNumber          from "../LvNumber"
import { skv_link_o }    from "./SymbolicKnownValue"
import BlockCodegen      from "./BlockCodegen"
import { cd }            from "./Codegen"
import Interceptor       from "./Interceptor"
import Operation         from "./Operation"
import { get_nout }      from "./GetNout"
import Sym, { Link }     from "./Sym"

/**
 * An expression without input,
 * with outputs representing the if inputs
 */
export
class IfInp extends Sym {
    constructor() {
        super( IfInp );
    }

    to_String__b(): string {
        return `IfInp`;
    }

    block_code( cg: BlockCodegen, options ): void {
    }

    nb_outputs() {
        return this.if_inst.children.length - 1; // not the condition
    }

    if_inst: If;
}
Sym.make_templ( IfInp );

/** */
export
class IfOut extends Sym {
    constructor( expr: Array<Link> ) {
        super( IfOut );
        this.children = expr;
    }

    to_String__b(): string {
        return "IfOut";
    }

    block_code( cg: BlockCodegen, options ): void {
    }

    if_inst: If;
}
Sym.make_templ( IfOut );

/**
 * cond => first child
 */
export default 
class If extends Sym {
    constructor( expr: Array<Link>, inp_ok: IfInp, out_ok: IfOut, inp_ko: IfInp, out_ko: IfOut ) {
        super( If );
        this.children = expr;
        this.inp_ok   = inp_ok;
        this.out_ok   = out_ok;
        this.inp_ko   = inp_ko;
        this.out_ko   = out_ko;

        inp_ok.if_inst = this;
        out_ok.if_inst = this;
        inp_ko.if_inst = this;
        out_ko.if_inst = this;
    }

    to_String__b(): string {
        return `If`;
    }

    nb_outputs() {
        return this.out_ok.children.length;
    }

    block_code( cg: BlockCodegen, options ): void {
        // assign IfInp outputs var_names
        const num_cond = this.children.length - 1;
        for( let num_ch = 0; num_ch < num_cond; ++num_ch ) {
            const name = cg.checked_var_name( this.children[ num_ch ] );
            cd( this.inp_ok ).var_names.push( name );
            cd( this.inp_ko ).var_names.push( name );
            cd( this        ).var_names.push( name );
        }

        //
        const ok_code = cg.block_code( [ this.out_ok ] ); 
        const ko_code = cg.block_code( [ this.out_ko ] );

        let cond_options = { conv_to_boolean: true }, cond = this.children[ num_cond ];
        cg.inst( ok_code ? ( ko_code ?
            `if(${ cg.inline_code( cond, 1e6, cond_options ) }){${ ok_code }}else{${ ko_code }}` :
            `if(${ cg.inline_code( cond, 1e6, cond_options ) }){${ ok_code }}`
        ) : ( ko_code ?
            `if(!${ cg.inline_code( cond, Operation.prec.NOT, cond_options ) }){${ ko_code }}` :
            null
        ) );
    }

    self_ops(): Array<number> {
        let res = new Array<number>();
        for( let n = 0; n < this.out_ok.children.length; ++n )
            res.push( n );
        return res;
    }

    for_each_sub_block( cb: ( v: Sym ) => void ) {
        cb( this.out_ok );
        cb( this.out_ko );
    }
    
    write_graphviz__b( gr: Graphviz ) {
        if ( gr.node( this, this.children.length, this.out_ok.children.length, "If" ) ) {
            gr.edge( this, -1, this.out_ok, -1, { label: "ok", style: "dashed", subgraph: { color: "yellow", style: "dotted" } } );
            gr.edge( this, -1, this.out_ko, -1, { label: "ko", style: "dashed", subgraph: { color: "green" , style: "dotted" } } );
            this.children.forEach( ( ch, num_ch ) => { gr.edge( this, num_ch, ch.item, ch.nout ); } );
        }
    }

    inp_ok: IfInp;
    out_ok: IfOut;
    inp_ko: IfInp;
    out_ko: IfOut;
} 
Sym.make_templ( If );

export 
function _if( cond: any, ok: () => any, ...rem ) { // ko?: () => any
    // helper
    function ko() {
        if ( rem.length == 0 ) return;
        if ( rem.length == 1 ) return rem[ 0 ]();
        return _if( rem[ 0 ], rem[ 1 ], ...rem.slice( 2 ) );
    }

    if ( cond instanceof VarAnc ) {
        const b_cond = cond.toBooleanVariable;
        if ( ! ( b_cond.rp instanceof Sym ) )
            return b_cond.val ? ok() : ko();

        // intercept calls to get modified variables
        let inp_ok = new IfInp();
        let inp_if = new Array<Rp>(); // If inputs
        let mod_xx = new Map<VarAnc,number>();
        let mod_ok = new Map<VarAnc,{ o: Rp, b: Rp, n: Rp }>();
        Interceptor.run( mod_ok, ok, function( v: VarAnc ) {
            let ind = inp_if.indexOf( v.rp );
            if ( ind < 0 )
                ind = inp_if.push( v.rp ) - 1;
            mod_xx.set( v, ind );
            return get_nout( inp_ok, ind );
        } );

        let inp_ko = new IfInp();
        let mod_ko = new Map<VarAnc,{ o: Rp, b: Rp, n: Rp }>();
        Interceptor.run( mod_ko, ko, function( v: VarAnc ) {
            let ind = inp_if.indexOf( v.rp );
            if ( ind < 0 )
                ind = inp_if.push( v.rp ) - 1;
            mod_xx.set( v, ind );
            return get_nout( inp_ko, ind );
        } );        

        //
        let out_ok = new Array<Link>(), out_ko = new Array<Link>();
        for( let [ v, oan ] of mod_ok ) {
            out_ok.push( skv_link_o( oan.n ) );
            const oan_ko = mod_ko.get( v );
            out_ko.push( oan_ko ? skv_link_o( oan_ko.n ) : { item: inp_ko, nout: mod_xx.get( v ) } );
        }
        for( let [ v, oan ] of mod_ko ) {
            if ( mod_ok.get( v ) )
                continue;
            out_ok.push( { item: inp_ok, nout: mod_xx.get( v ) } );
            out_ko.push( skv_link_o( oan.n ) );
        }

        // excepted for condition variable, an If inst is basically a value modifier
        if ( Method.int_call_s )
            mod_xx.forEach( ( num, val ) => Method.int_call_s( val ) );

        // modify variables to take if outputs
        let rp_if = new If( [ ...inp_if, b_cond.rp ].map( skv_link_o ), inp_ok, new IfOut( out_ok ), inp_ko, new IfOut( out_ko ) );
        mod_xx.forEach( ( num, val ) => val.rp = get_nout( rp_if, num ) );
        return;
    }
    //
    return cond ? ok() : ko();
}
