import Codegen  from "../Symbol/Codegen";
import LvNumber from "../LvNumber";
import LvString from "../LvString";
import LvUsrId  from "../LvUsrId";
import * as fs  from "fs";

export { _if }  from "../Symbol/If";

export type OpWriter = ( op: any ) => void;
export class Op {}

class OpInfo<UT> {
    inst   : any; /** instance of symbolic repr */
    num    : number;
    apply ?: ( d: UT, o: Op ) => any;
    undo  ?: ( d: UT, o: Op ) => any;
    store ?: ( d: UT, o: Op ) => Array<{type:any,data:any}>;
};

/** helper */
let nb_sp = 0;
function wl( str = "" ) {
    process.stdout.write( " ".repeat( nb_sp ) + str + "\n" );
}

// struct op_add_grp_right { static const char *name() { return "add_grp_right"; } static bool valid( GrpId, unsigned v ) { return v; } };
// struct op_rem_grp_right { static const char *name() { return "rem_grp_right"; } static bool valid( GrpId, unsigned v ) { return v; } };
// struct op_add_usr_right { static const char *name() { return "add_usr_right"; } static bool valid( UsrId, unsigned v ) { return v; } };
// struct op_rem_usr_right { static const char *name() { return "rem_usr_right"; } static bool valid( UsrId, unsigned v ) { return v; } };
class AddUsrRight { usr = new LvUsrId(); flags = new LvNumber(); };

export default 
class GenOperation<UT> {
    static AddUsrRight = AddUsrRight;

    constructor( class_: any, sym_corr = {} as { [ name: string ]: string } ) {
        this.sym_corr = sym_corr;
        this.cl_inst  = new class_;
    }

    add_right_flags( ...right_flags: Array<string> ) {
        if ( right_flags.length == 0 )
            throw "right_flags is expected to be non void";
        // if first time, add flags and operations for right mgmt
        if ( this.right_flags.length == 0 ) {
            this.right_flags.push( "add_rights", "rem_rights" );

        }
        this.right_flags.push( ...right_flags );
    }

    apply( op_type, cb: ( d: UT, o: Op ) => any ) {
        this.reg_op( op_type ).apply = cb;
    }

    undo( op_type, cb: ( d: UT, o: Op ) => any ) {
        this.reg_op( op_type ).undo = cb;
    }

    store( op_type, cb: ( d: UT, o: Op ) => Array<{type:any,data:any}> ) {
        this.reg_op( op_type ).store = cb;
    }

    reg_op( op_type ): OpInfo<UT> {
        const ind = this.operations.find( x => x.inst.constructor == op_type );
        if ( ind )
            return ind;
        const res = { inst: new op_type, num: this.operations.length } as OpInfo<UT>;
        this.operations.push( res );
        return res;
    }

    fwd_trans( o_op_type, n_op_type, cb: ( o, n ) => void ) {
        this.trans_rules.set( `${ ( new o_op_type ).constructor.name } ${ ( new n_op_type ).constructor.name }`, cb );
    }

    // /** declare a new operation */
    // add_op( name: string, args: Array<string>, info: OpInfo ): void {
    //     this.operations.push( new Operation( name, args.map( x => { const a = x.split(':'); return { name: a[ 0 ], type: a[ 1 ] }; } ), 
    //             this.operations.length, info.apply, info.undo ) );
    // }

    // /** add forward translation rule */
    // add_ftr( o_op: string, n_op: string, cb: ( o, n ) => void ) {
    //     this.trans_rules.set( `${ o_op } ${ n_op }`, cb );
    // }

    write( lang: string ): void {
        wl( `import BinaryWriter    from "../System/BinaryWriter"` );
        wl( `import BinaryReader    from "../System/BinaryReader"` );
        wl( `import UsrId           from "../System/UsrId"`        );
        wl();

        // bin_repr
        wl( `var bin_repr = {` );
        for( const op of this.operations )
            wl( `    ${ op.inst.constructor.name }: function( bw: BinaryWriter, ${ this.func_args_op( lang, op ) } ): void { bw.write_PI8( ${ op.num } ); ${ this.bw_write_op( lang, op ) } },` );
        wl( `}` );
        wl();

        // read
        wl( `function read( br: BinaryReader, cb: ( type: string, args: any ) => void ) {` );
        wl( `    switch ( br.read_PI8() ) {` );
        for( const op of this.operations )
            wl( `        case ${ op.num }: cb( "${ op.inst.constructor.name }", { ${ this.br_read_obj( lang, op ) } } ); break;` );
        wl( `        default: cb( null, {} ); br.clear(); break;` );
        wl( `    }` );
        wl( `}` );
        wl();

        // skip
        wl( `function skip( br: BinaryReader ): Array<number> {` );
        wl( `    let res = new Array<number>();` );
        wl( `    while ( br.size ) {` );
        wl( `        res.push( br.cursor );` );
        wl( `        switch ( br.read_PI8() ) {` );
        for( const op of this.operations )
            wl( `            case ${ op.num }: ${ this.br_skip_obj( lang, op ) } break;` );
        wl( `        }` );
        wl( `    }` );
        wl( `    return res;` );
        wl( `}` );
        wl();

        // undo_patch
        //const ul = this.loc_type( lang, new this.underlying_class );
        wl( `function undo_patch( val: ${ this.cl_inst.constructor.name }, br: BinaryReader, as_usr: UsrId ) {` );
        wl( `    const res = skip( br );` );
        wl( `    for( let n = res.length; n--; ) {` );
        wl( `        br.cursor = res[ n ];` );
        wl( `        switch ( br.read_PI8() ) {` );
        for( const op of this.operations ) {
            if ( op.undo ) {
                wl( `        case ${ op.num }: {` );
                nb_sp += 12;
                this.write_undo_patch( lang, op );
                nb_sp -= 12;
                wl( `            break;` );
                wl( `        }` );
            }
        }
        wl( `        }` );
        wl( `    }` );
        wl( `    return val;` );
        wl( `}` );
        wl();
        // new_patch
        wl( `function new_patch( val: ${ this.cl_inst.constructor.name }, bw_new: BinaryWriter, br_new: BinaryReader, as_usr: UsrId, cq_unk: BinaryWriter ) {` );
        wl( `    while ( br_new.size ) {` );
        wl( `        switch ( br_new.read_PI8() ) {` );
        for( const op of this.operations ) {
            wl( `        case ${ op.num }: {` );
            nb_sp += 12;
            this.write_new_patch( lang, op );
            nb_sp -= 12;
            wl( `            break;` );
            wl( `        }` );
        }
        wl( `        }` );
        wl( `    }` );
        wl( `    return val;` );
        wl( `}` );
        wl();


        // exports
        wl( `export default { read, bin_repr, new_patch, undo_patch };` );
    }

    func_args_op( lang: string, op: OpInfo<UT> ): string {
        return Object.keys( op.inst ).map( x => `${ x }: ${ this.loc_type( lang, op.inst[ x ] ) }` ).join( ', ' );
    }

    bw_write_op( lang: string, op: OpInfo<UT>, bw = "bw", suffix = "" ): string {
        return Object.keys( op.inst ).map( x => `${ bw }.write_${ this.gen_type( op.inst[ x ] ) }( ${ x }${ suffix } );` ).join( ' ' );
    }

    br_read_obj( lang: string, op: OpInfo<UT> ): string {
        return Object.keys( op.inst ).map( x => `${ x }: br.read_${ this.gen_type( op.inst[ x ] ) }()` ).join( ', ' );
    }

    br_skip_obj( lang: string, op: OpInfo<UT> ): string {
        return Object.keys( op.inst ).map( x => `br.skip_${ this.gen_type( op.inst[ x ] ) }();` ).join( ' ' );
    }

    br_read_var( lang: string, op: OpInfo<UT>, br = "br", suffix = "" ): string {
        const keys = Object.keys( op.inst );
        return keys.length ? "let " + keys.map( x => `${ x }${ suffix } = ${ br }.read_${ this.gen_type( op.inst[ x ] ) }()` ).join( ', ' ) + ";" : '';
    }

    loc_type( lang: string, inst ): string {
        switch ( inst.constructor.name ) {
            case "LvString": return "string";
            case "LvNumber": return "number";
        }
        throw "Unknown loc type for " + inst.constructor.name;
    }

    gen_type( inst ): string {
        switch ( inst.constructor.name ) {
            case "LvString": return "String";
            case "LvNumber": return "PT";
        }
        throw "Unknown gen type for " + inst.constructor.name;
    }

    make_symbolic_data( inst: any, suffix = "", prefix = "", correction = {} as { [ key: string ]: string } ) {
        const res = {} as any;
        for( const key of Object.keys( inst ) ) {
            switch ( inst[ key ].constructor.name ) {
                case 'LvNumber': res[ key ] = LvNumber.symbol( prefix + ( correction[ key ] || key ) + suffix ); break;
                case 'LvString': res[ key ] = LvString.symbol( prefix + ( correction[ key ] || key ) + suffix ); break;
                default: throw "TODO: make_symbolic_data for " + inst[ key ].constructor.name;
            }
        }
        return res;
    }

    write_new_patch( lang: string, op_new: OpInfo<UT> ) {
        // read data
        wl( this.br_read_var( lang, op_new, "br_new", "_new" ) );

        // read unk data
        wl( `let br_unk = new BinaryReader( cq_unk.to_Uint8Array() );` );
        wl( `let bw_unk = new BinaryWriter;` );
        wl( `while ( br_unk.size ) {` );
        wl( `    const num_unk = br_unk.read_PI8();` );
        wl( `    switch ( num_unk ) {` );
        for( const op_unk of this.operations ) {
            wl( `        case ${ op_unk.num }: {` );
            nb_sp += 12;
            wl( this.br_read_var( lang, op_unk, "br_unk", "_unk" ) );
            
            let cb = this.trans_rules.get( `${ op_unk.inst.constructor.name } ${ op_new.inst.constructor.name }` );
            if ( cb ) {
                let data_unk = this.make_symbolic_data( op_unk.inst, "_unk" );
                let data_new = this.make_symbolic_data( op_new.inst, "_new" );
                cb( data_unk, data_new );
                wl( Codegen.make_code( [
                    ...Object.keys( data_unk ).map( k => data_unk[ k ] ),
                    ...Object.keys( data_new ).map( k => data_new[ k ] ),
                ], lang ) );
            }
            wl( `bw_unk.write_PI8( ${ op_unk.num } ); ${ this.bw_write_op( lang, op_unk, "bw_unk", "_unk" ) }` );
            nb_sp -= 12;
            wl( `            break;` );
            wl( `        }` );
        }
        wl( `    }` );
        wl( `}` );

        // write back the new data
        if ( op_new.store ) {
            let d = this.make_symbolic_data( this.cl_inst, "", "val.", this.sym_corr );
            let o = this.make_symbolic_data( op_new.inst, "_new" );
            for( const new_op of op_new.store( d, o ) ) {
                const op = this.reg_op( new_op.type );
                const li = Object.keys( new_op.data ).map( x => op.inst[ x ].constructor.symbol( x + "_tmp" ).set( new_op.data[ x ] ) );
                wl( `{` ); 
                nb_sp += 4;
                wl( `let ${ Object.keys( op.inst ).map( x => x + "_tmp" ).join( ', ' ) };` );
                wl( Codegen.make_code( li, lang ) );
                wl( `bw_new.write_PI8( ${ op.num } );` + Object.keys( op.inst ).map( x => ` bw_new.write_${ this.gen_type( op.inst[ x ] ) }( ${ x }_tmp );` ).join( '' ) );
                nb_sp -= 4;
                wl( `}` ); 
            }
        } else {
            wl( `bw_new.write_PI8( ${ op_new.num } ); ${ this.bw_write_op( lang, op_new, "bw_new", "_new" ) }` );
        }

        // register new unk data
        wl( `bw_unk.transfer_to( cq_unk );` );

        // apply operation
        let d = this.make_symbolic_data( this.cl_inst, "", "val.", this.sym_corr );
        let o = this.make_symbolic_data( op_new.inst, "_new" );
        op_new.apply( d, o );
        wl( Codegen.make_code( Object.keys( d ).map( n => d[ n ] ), lang ) );
    }

    write_undo_patch( lang: string, op: OpInfo<UT> ) {
        let d = this.make_symbolic_data( this.cl_inst, "", "val.", this.sym_corr );
        let o = this.make_symbolic_data( op.inst );
        op.undo( d, o );
        wl( this.br_read_var( lang, op ) );
        wl( Codegen.make_code( Object.keys( d ).map( n => d[ n ] ), lang ) );
    }

    cl_inst     : any; /** instance of symbolic repr of class */
    sym_corr    : { [ name: string ]: string }; /** names in symbolic repr to name in dst code */
    operations  = new Array<OpInfo<UT>>();
    trans_rules = new Map<string,(o,n)=>void>();
    right_flags = new Array<string>();
}
