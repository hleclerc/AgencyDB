import Graphviz                    from "../Core/Graphviz"
import { VarAnc }                  from "../Core/Variable"
import methods                     from "../Core/methods"
import Rp                          from "../Core/Rp"
import { BN_PT }                   from "../LvNumber/Bn"
import BlockCodegen                from "./BlockCodegen"
import Operation, { make_op }      from "./Operation"
import While, { WhileInp }         from "./While"
import Sym, { Link }               from "./Sym"
import If, { IfInp }               from "./If"

export
class CodegenData {
    var_names         = new Array<string>();
    in_ext_blk        = 10000000;                  /** used by sep_sub_blocks to know if var is used in blocks and in a sub blocks */
    parents           = new Array<Sym>();
    num_ch_in_parents = new Array<number>();
    bus_id            = 0;
    done              = false;
    num_in_sched      = -1;

    static cur_bus_id = 0;
}

export
function cd( v: Sym ): CodegenData {
    return v.op_mp.codegen_data as CodegenData;
}

class Target {
    variable: Sym;
    name    : string;
}

function init_codegen_data_of_sym( pos_codegen_data: Array<Sym>, v: Sym ) {
    v.op_mp.codegen_data = new CodegenData;
    pos_codegen_data.push( v );
}

function init_codegen_data( pos_codegen_data: Array<Sym>, targets: Array<Sym> ) {
    Sym.dfs_unique( targets, function( v: Sym ) {
        init_codegen_data_of_sym( pos_codegen_data, v );
    }, true );
}

function externalize_ext_vars( pos_codegen_data: Array<Sym>, inst: If | While, inp: IfInp | WhileInp, target: Sym, cond_var: boolean, num_sub_block: number ) {
    //
    Sym.dfs_unique( [ target ], ( pa: Sym ) => {
        if ( cd( pa ).in_ext_blk <= num_sub_block )
            return;
        for( let ch of pa.children ) {
            if ( cd( ch.item ).in_ext_blk <= num_sub_block ) {
                let nout = inst.children.findIndex( item => item.item == ch.item && item.nout == ch.nout );
                if ( nout < 0 ) {
                    if ( cond_var ) {
                        nout = inst.children.length - 1;
                        inst.children.push( inst.children[ nout ] ); // move condition variable at the end
                        inst.children[ nout ] = ch; // replace by externalized variable
                    } else
                        nout = inst.children.push( ch ) - 1;
                }
                pa.replace_child( ch, { item: inp, nout } );
            }
        }
    }, true );

    //
    sep_sub_blocks( pos_codegen_data, [ target ], num_sub_block + 1 );
} 

/**
 * A => E0
 * B => E1
 * C => E2
 * 
 * Il faut 
*/
function sep_sub_blocks( pos_codegen_data: Array<Sym>, targets: Array<Sym>, num_sub_block: number ) {
    // in_ext_blk for each inst of block of targets
    let if_out_vars = new Array<If>();
    let wh_out_vars = new Array<While>();
    Sym.dfs_unique( targets, ( v: Sym, n: number ) => {
        if ( cd( v ).in_ext_blk > num_sub_block )
            cd( v ).in_ext_blk = num_sub_block;
        if ( v instanceof If )
            if_out_vars.push( v );
        else if ( v instanceof While )
            wh_out_vars.push( v );
    } );

    // for each out_var, look if there is a sub var with a in_ext_blk
    for( let out_var of if_out_vars ) {
        if ( ! out_var.inp_ok.op_mp.codegen_data ) init_codegen_data_of_sym( pos_codegen_data, out_var.inp_ok ); // inp_xx may have been ignored (because not used before)
        if ( ! out_var.inp_ko.op_mp.codegen_data ) init_codegen_data_of_sym( pos_codegen_data, out_var.inp_ko );
        externalize_ext_vars( pos_codegen_data, out_var, out_var.inp_ok, out_var.out_ok, true , num_sub_block );
        externalize_ext_vars( pos_codegen_data, out_var, out_var.inp_ko, out_var.out_ko, true , num_sub_block );
    }
    for( let out_var of wh_out_vars ) {
        if ( ! out_var.inp_co.op_mp.codegen_data ) init_codegen_data_of_sym( pos_codegen_data, out_var.inp_co ); // inp_xx may have been ignored (because not used before)
        if ( ! out_var.inp_bk.op_mp.codegen_data ) init_codegen_data_of_sym( pos_codegen_data, out_var.inp_bk );
        externalize_ext_vars( pos_codegen_data, out_var, out_var.inp_co, out_var.out_co, true , num_sub_block );
        externalize_ext_vars( pos_codegen_data, out_var, out_var.inp_bk, out_var.out_bk, false, num_sub_block );
    }
}

function set_up_parents( targets: Array<Sym> ) {
    Sym.dfs_unique( targets, function( op: Sym ) {
        if ( op.children.length ) {
            op.children.forEach( ( ch, num_ch ) => {
                cd( ch.item ).parents.push( op );
                cd( ch.item ).num_ch_in_parents.push( num_ch );
            } );
        }
    }, true );
}

function base_instruction_selection( targets: Array<Sym>, lang: string ) {
    targets.splice( 0, targets.length, ...Sym.dfs_repl_unique( targets, function( op: Sym ): Array<Link> {
        if ( op instanceof Operation ) {
            switch ( op.base_name ) {
                // a.substring( 0, children[ 1 ] ) + a.substring( children[ 1 + 2 ] )
                case "remove_s":
                    return [ make_op( "set__sb", op.children[ 0 ], make_op( "add__bb",
                        make_op( "beginning__bb", op.children[ 0 ], op.children[ 1 ] ),
                        make_op( "ending__bb", op.children[ 0 ], make_op( "add__bb", op.children[ 1 ], op.children[ 2 ] ) )
                    ) ) ];
                // ( a.substring( 0, children[ 1 ] ) + children[ 2 ] ) + a.substring( children[ 1 ] )
                case "insert_s":
                    return [ make_op( "set__sb", op.children[ 0 ], make_op( "add__bb", 
                        make_op( "add__bb", make_op( "beginning__bb", op.children[ 0 ], op.children[ 1 ] ), op.children[ 2 ] ),
                        make_op( "ending__bb", op.children[ 0 ], op.children[ 1 ] )
                    ) ) ];
                // 
                case "beginning_s":
                case "ending_s":
                    return [ make_op( "set__sb", op.children[ 0 ], make_op( op.method.base_name + "__bb", op.children[ 0 ], op.children[ 1 ] ) ) ];
            }

            // ex: add__sbb( map, key, val ) => set__sbb( map, key, add__bb( select__bb( map, key ), val ) ) 
            if ( op.method.select && op.method.base_name != "set" ) {
                return [ make_op( "set__sbb", op.children[ 0 ], op.children[ 1 ], make_op( op.method.base_name + "__bb", make_op( "select__bb", op.children[ 0 ], op.children[ 1 ] ), op.children[ 2 ] ) ) ];
            }
        }
        return null;
    }, true ) );
}


// let cpr = 0, _targets;

/** */
export default
class Codegen {
    /** helper to call exec from Variables */
    static make_code( targets: Array<VarAnc>, lang = "ts" ) {
        let c = new Codegen;
        return c.exec( targets.map( x => x.rp ), lang );
    }

    exec( targets: Array<Rp>, lang: string, prec = 0 ): string {
        const res = this.exec_wo_free( targets, lang, prec );
        this.free();
        return res;
    }

    exec_wo_free( rp_targets: Array<Rp>, lang: string, prec = 0 ): string {
        const targets = rp_targets.filter( op => op instanceof Sym ) as Array<Sym>;

// ++cpr;
// _targets = targets;
//console.log( cpr );

// if ( cpr ==  25 ) Graphviz.display( _targets );
        // TODO: change _sxx ops to _xxx ops when possible (when the result is not in the targets)

        // change instructions that can't be written in $lang (may change targets)
        base_instruction_selection( targets, lang );
// if ( cpr ==  25 ) Graphviz.display( _targets );
    
        // assign CodegenData in op_mp.codegen_data
        init_codegen_data( this.pos_codegen_data, targets );

        // ensure that sub blocks (like the ones with IfOut above) are totally independant 
        sep_sub_blocks( this.pos_codegen_data, targets, 0 );
// if ( cpr ==  25 ) Graphviz.display( _targets );
        
        // set up parents, in current and sub blocks recursively
        set_up_parents( targets );

        //
        let bc = new BlockCodegen( this );
        const res = bc.exec( targets ); // `/*cpr=${ cpr }*/` + 
        if ( res.indexOf("17") >= 0 )
            Graphviz.display( targets );
        return res; 
    }

    free() {
        this.pos_codegen_data.forEach( v => delete v.op_mp.codegen_data );
    }

    pos_codegen_data = new Array<Sym>();
    nb_vars          = 0;
}
