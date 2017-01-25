//// nsmake define NO_OPERATIONAL_TRANSFORM
import Codegen  from "../Symbol/Codegen";
import LvNumber from "../LvNumber";
import LvString from "../LvString";
import * as fs  from "fs";

export { _if }  from "../Symbol/If";

function bw_write( type: string, name: string, suffix: string ) {
    return `bw${ suffix }.write_${ type }( ${ name }${ suffix } );`;
}

function br_read( br: string, type: string ) {
    return `${ br }.read_${ type }()`;
}

function type_ts( type: string ) {
    switch ( type ) {
        case 'PT'    : return 'number';
        case 'String': return 'string';
        default: return type;
    }
}

class Operation {
    constructor( readonly name: string, readonly args: Array<{name:string,type:string}>, readonly num: number, readonly apply_op: { [lang: string]: ( name: string, suffix: string ) => string }, readonly undo_op: { [lang: string]: ( name: string, suffix: string ) => string } ) {
    }
    func_args_ts(): string {
        return this.args.map( x => x.name + ': ' + type_ts( x.type ) ).join( ', ' );
    }
    bw_write( suffix = "" ): string {
        return this.args.map( x => bw_write( x.type, x.name, suffix ) ).join( ' ' );
    }
    br_read_obj( br = "br" ): string {
        return this.args.map( x => `${ x.name }: ${ br_read( br, x.type ) }` ).join( ', ' );
    }
    br_read_var( br = "br", suffix = "" ): string {
        return this.args.length ? "let " + this.args.map( x => `${ x.name }${ suffix } = ${ br_read( br, x.type ) }` ).join( ', ' ) + ";" : '';
    }
    make_symbolic_data( suffix = "" ) {
        let res = {} as any;
        for( const arg of this.args ) {
            switch ( arg.type ) {
                case 'PT'    : res[ arg.name ] = LvNumber.symbol( arg.name + suffix ); break;
                case 'String': res[ arg.name ] = LvString.symbol( arg.name + suffix ); break;
            }
            
        }
        return res;
    }
}

let nb_sp = 0;
function wl( str = "" ) {
    process.stdout.write( " ".repeat( nb_sp ) + str + "\n" );
}

export default 
class GenOperation {
    constructor( class_name: string ) {
        this.class_name = class_name;
    }

    /** declare a new operation */
    add_op( name: string, args: Array<string>, apply_op: { [lang: string]: ( name: string, suffix: string ) => string }, undo_op: { [lang: string]: ( name: string, suffix: string ) => string } ): void {
        this.operations.push( new Operation( name, args.map( x => { const a = x.split(':'); return { name: a[ 0 ], type: a[ 1 ] }; } ), this.operations.length, apply_op, undo_op ) );
    }

    /** add forward translation rule */
    add_ftr( o_op: string, n_op: string, cb: ( o, n ) => void ) {
        this.trans_rules.set( `${ o_op } ${ n_op }`, cb );
    }

    write( lang: string ): void {
        wl( `import BinaryWriter    from "../System/BinaryWriter"` );
        wl( `import BinaryReader    from "../System/BinaryReader"` );
        wl( `import UsrId           from "../System/UsrId"`        );
        wl();

        // bin_repr
        wl( `var bin_repr = {` );
        for( const op of this.operations )
            wl( `    ${ op.name }: function( ${ op.func_args_ts() } ): Uint8Array { let bw = new BinaryWriter; bw.write_PI8( ${ op.num } ); ${ op.bw_write() } return bw.to_Uint8Array(); },` );
        wl( `}` );
        wl();

        // read
        wl( `function read( br: BinaryReader, cb: ( type: string, args: any ) => void ) {` );
        wl( `    const num = br.read_PI8();` );
        wl( `    switch ( num ) {` );
        for( const op of this.operations )
            wl( `        case ${ op.num }: cb( "${ op.name }", { ${ op.br_read_obj() } } ); break;` );
        wl( `        default: cb( null, {} ); br.clear(); break;` );
        wl( `    }` );
        wl( `}` );
        wl();

        // new_patch
        wl( `function new_patch( val: ${ this.class_name }, bw_new: BinaryWriter, br_new: BinaryReader, as_usr: UsrId, cq_unk: BinaryWriter ) {` );
        wl( `    while ( br_new.size ) {` );
        wl( `        const num = br_new.read_PI8();` );
        wl( `        switch ( num ) {` );
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
        wl( `}` );
        wl();

        // undo_patch
        wl( `function undo_patch( val: ${ this.class_name }, br: BinaryReader, as_usr: UsrId ) {` );
        wl( `    while ( br.size ) {` );
        wl( `        const num = br.read_PI8();` );
        wl( `        switch ( num ) {` );
        for( const op of this.operations ) {
            wl( `        case ${ op.num }: {` );
            nb_sp += 12;
            this.write_undo_patch( lang, op );
            nb_sp -= 12;
            wl( `            break;` );
            wl( `        }` );
        }
        wl( `        }` );
        wl( `    }` );
        wl( `}` );
        wl();

        // exports
        wl( `export default { read, bin_repr, new_patch, undo_patch };` );
    }

    write_new_patch( lang: string, op_new: Operation ) {
        // read data
        wl( op_new.br_read_var( "br_new", "_new" ) );

        // read unk data
        wl( `let br_unk = new BinaryReader( cq_unk.to_Uint8Array() );` );
        wl( `while ( br_unk.size ) {` );
        wl( `    const num_unk = br_unk.read_PI8();` );
        wl( `    switch ( num_unk ) {` );
        for( const op_unk of this.operations ) {
            wl( `        case ${ op_unk.num }: {` );
            nb_sp += 12;
            wl( op_unk.br_read_var( "br_unk", "_unk" ) );
            // wl( `console.log( "unk:", { ${ op_unk.args.map( x => x.name ).join(',') } } );` );

            let cb = this.trans_rules.get( `${ op_unk.name } ${ op_new.name }` );
            if ( cb ) {
                let data_unk = op_unk.make_symbolic_data( "_unk" );
                let data_new = op_new.make_symbolic_data( "_new" );
                cb( data_unk, data_new );
                wl( Codegen.make_code( [
                    ...Object.keys( data_unk ).map( k => data_unk[ k ] ),
                    ...Object.keys( data_new ).map( k => data_new[ k ] ),
                ] ) );
            }

            nb_sp -= 12;
            wl( `            break;` );
            wl( `        }` );
        }
        wl( `        default: break;` );
        wl( `    }` );
        wl( `}` );

        // apply operation
        wl( op_new.apply_op[ lang ]( "val", "_new" ) );

        // write back the new data
        wl( `bw_new.write_PI8( ${ op_new.num } ); ${ op_new.bw_write( "_new" ) }` );
    }

    write_undo_patch( lang: string, op: Operation ) {
        // read data
        wl( op.br_read_var( "br" ) );

        // apply operation
        wl( op.undo_op[ lang ]( "val", "" ) );
    }

    class_name  : string;
    operations  = new Array<Operation>();
    trans_rules = new Map<string,(o,n)=>void>();
}
