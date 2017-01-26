import methods                      from "../Core/methods"
import Operation                    from "./Operation"
import Codegen, { CodegenData, cd } from "./Codegen"
import Sym, { Link }                from "./Sym"


/** */
function get_leaves( leaves: Array<Sym>, targets: Array<Sym> ) {
    Sym.dfs_unique( targets, function( op: Sym ) {
        if ( op.children.length == 0 )
            leaves.push( op );
    } );
}

/** */
function save_if_undone_parent_rec( need_save: Map<Sym,Set<number>>, ch: Link, pa: Sym, num_in_sched: number ) {
    if ( cd( pa ).num_in_sched > num_in_sched ) {
        if ( ! need_save.get( ch.item ) )
            need_save.set( ch.item, new Set<number>() );
        need_save.get( ch.item ).add( ch.nout );
        return;
    }
    if ( cd( pa ).done )
        return;
    // console.log( "ch", methods[ 'to_String' ].call_1( ch ), methods[ 'to_String' ].call_1( pa ), cd( pa ).num_in_sched, num_in_sched );
    cd( pa ).parents.forEach( ( npa, ind ) =>
        save_if_undone_parent_rec( need_save, { item: pa, nout: npa.children[ cd( pa ).num_ch_in_parents[ ind ] ].nout }, npa, num_in_sched )
    );
}

/** bottom up search, with dependencies */
function bus( leaves: Array<Sym>, cb: ( op: Sym ) => void ) {
    const bus_id = ++CodegenData.cur_bus_id;
    let front = [ ...leaves ];
    while ( front.length ) {
        // get next op. We try to execute self ops after the others
        let best_score = -1e6, best_pos = 0;
        for( let pos = 0; pos < front.length; ++pos ) {
            const score = - front[ pos ].self_ops().length;
            if ( best_score < score ) {
                best_score = score;
                best_pos   = pos;
            } 
        }
        let op = front[ best_pos ];
        front.splice( best_pos, 1 );

        // register, and call cb
        if ( cd( op ).bus_id == bus_id )
            continue;
        cd( op ).bus_id = bus_id;
        cb( op );

        // look into the parent list
        for( let parent of cd( op ).parents )
            if ( parent.children.every( ch => cd( ch.item ).bus_id >= bus_id ) )
                front.push( parent );
    }
}

/** */
export default
class BlockCodegen {
    constructor( cg: Codegen, parent = null as BlockCodegen ) {
        this.parent = parent;
        this.cg     = cg;
    }

    exec( targets: Array<Sym>, options = {} as any ): string {
        // get a schedule
        let leaves = new Array<Sym>(), sched = new Array<Sym>();
        get_leaves( leaves, targets );        
        bus( leaves, op => {
            cd( op ).num_in_sched = sched.length;
            sched.push( op );
        } );

        // execute
        for( let op of sched ) {
            // console.log( "op:", methods["to_String__b"].call_1( op ) );
            cd( op ).done = true;

            // we are going to modify a variable that still have to be used ?
            let need_save = new Map<Sym,Set<number>>(); // => num used outputs
            for( let num_ch of op.self_ops() ) {
                const ch = op.children[ num_ch ];
                cd( ch.item ).parents.forEach( ( pa, ind ) => {
                    const nout_used = pa.children[ cd( ch.item ).num_ch_in_parents[ ind ] ].nout;
                    if ( pa != op && nout_used == ch.nout )
                        save_if_undone_parent_rec( need_save, ch, pa, cd( op ).num_in_sched );
                } );
            }
            let to_be_replaced = new Array<{ lst, ind, val }>();
            for( let [ item, nouts ] of need_save ) {
                // console.log( "need save", methods["to_String__b"].call_1( item ), nouts );
                // for( const nout of nouts )
                //     console.log( "poh", nout, methods["to_String__b"].call_1( item ), cd( item ).var_names[ nout ] );
                
                // if already done, we manually save the variable
                if ( [ ...nouts ].some( nout => Boolean( cd( item ).var_names[ nout ] ) ) ) {
                    for( const nout of nouts ) {
                        const tmp = this.new_var( prec => cd( item ).var_names[ nout ] );
                        to_be_replaced.push( { lst: cd( item ).var_names, ind: nout, val: tmp } );
                    }
                } else {
                    // if not done, it means that it was inlined, meaning that there is only one single output
                    console.assert( nouts.size == 1 );
                    const nout = [ ...nouts ][ 0 ];
                    cd( item ).var_names[ nout ] = this.new_var( prec => item.inline_code( this, nout, prec, {} ) );
                    // const tmp = this.new_var();
                    // cd( item ).var_names[ nout ] = tmp;
                    // this.code += `${ tmp }=${ item.inline_code( this, nout, Operation.prec.ASSIGN, {} ) };`;
                }
            }

            // exec
            op.block_code( this, options );

            // manually modified var_names
            for( let r of to_be_replaced )
                r.lst[ r.ind ] = r.val;
        }
        if ( this.declarations.length )
            this.code = `var ${ this.declarations.map( x => x.var_name + ( x.code ? "=" + x.code : "" ) ).join(',') };${ this.code }`;
        return this.code;
    }

    inline_code( op: Link, prec, options = {} as any ): string {
        const n = cd( op.item ).var_names[ op.nout ]; 
        return n ? n : op.item.inline_code( this, op.nout, prec, options );
    }

    checked_var_name( ch: Link ): string {
        let name = cd( ch.item ).var_names[ ch.nout ];
        if ( ! name ) {
            cd( ch.item ).var_names[ ch.nout ] = name = this.new_var( prec => ch.item.inline_code( this, ch.nout, prec, {} ) );
        }
        return name;
    }

    /** Write an instruction */
    inst( code: string ): void {
        this.decl_mode = false;
        this.code += code;
    }

    new_var( cb?: ( prec: number ) => string ): string {
        // this.code += `${ tmp }=${  };`;
        const var_name = `T${ this.cg.nb_vars++ }`;
        if ( cb ) {
            if ( this.decl_mode ) {
                this.declarations.push( { var_name, code: cb( Operation.prec.COMMA ) } );
            } else {
                this.code += `${ var_name }=${ cb( Operation.prec.ASSIGN ) };`;
                this.declarations.push( { var_name, code: null } );
            }
        } else
            this.declarations.push( { var_name, code: null } );
        return var_name;
    }

    /** helper */
    block_code( targets: Array<Sym>, options = {} as any ): string {
        let bc = new BlockCodegen( this.cg, this.parent );
        return bc.exec( targets, options );
    } 

    declarations = new Array<{var_name:string,code:string}>();
    parent       : BlockCodegen;
    decl_mode    = true;  //
    code         = "";
    cg           : Codegen;
}
