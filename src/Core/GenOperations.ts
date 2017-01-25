//// nsmake define NO_OPERATIONAL_TRANSFORM
import Codegen  from "../Symbol/Codegen";
import LvNumber from "../LvNumber";
import LvString from "../LvString";
import * as fs  from "fs";

export { _if }  from "../Symbol/If";

export type OpWriter = ( op: any ) => void;
export class Op {}


class OpInfo<UT> {
    op_type: any;
    num    : number;
    apply ?:( d: UT, o: Op ) => any
    undo  ?:( d: UT, o: Op ) => any
    // store?: ( b: OpWriter, o: any ) => void;
};

/** helper */
let nb_sp = 0;
function wl( str = "" ) {
    process.stdout.write( " ".repeat( nb_sp ) + str + "\n" );
}

export default 
class GenOperation<UT> {
    constructor( class_name: string, underlying_class ) {
        this.underlying_class = underlying_class;
        this.class_name       = class_name;
    }

    apply( op_type, cb: ( d: UT, o: Op ) => any ) {
        this.reg_op( op_type ).apply = cb;
    }

    undo( op_type, cb: ( d: UT, o: Op ) => any ) {
        this.reg_op( op_type ).undo = cb;
    }

    reg_op( op_type ): OpInfo<UT> {
        const ind = this.operations.find( x => x.op_type == op_type );
        if ( ind )
            return ind;
        const res = { op_type, num: this.operations.length } as OpInfo<UT>;
        this.operations.push( res );
        return res;
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
            wl( `    ${ op.op_type.prototype.constructor.name }: function( bw: BinaryWriter, ${ this.func_args_op( lang, op ) } ): void { bw.write_PI8( ${ op.num } ); ${ this.bw_write_op( lang, op ) } },` );
        wl( `}` );
        wl();

        // read
        wl( `function read( br: BinaryReader, cb: ( type: string, args: any ) => void ) {` );
        wl( `    switch ( br.read_PI8() ) {` );
        for( const op of this.operations )
            wl( `        case ${ op.num }: cb( "${ ( new op.op_type ).constructor.name }", { ${ this.br_read_obj( lang, op ) } } ); break;` );
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
        wl( `function undo_patch( val: ${ this.loc_type( lang, new this.underlying_class ) }, br: BinaryReader, as_usr: UsrId ) {` );
        wl( `    const res = skip( br );` );
        wl( `    for( let n = res.length; n--; ) {` );
        wl( `        br.cursor = res[ n ];` );
        wl( `        switch ( br.read_PI8() ) {` );
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
        // new_patch
        wl( `function new_patch( val: ${ this.class_name }, bw_new: BinaryWriter, br_new: BinaryReader, as_usr: UsrId, cq_unk: BinaryWriter ) {` );
        // wl( `    while ( br_new.size ) {` );
        // wl( `        switch ( br_new.read_PI8() ) {` );
        // for( const op of this.operations ) {
        //     wl( `        case ${ op.num }: {` );
        //     nb_sp += 12;
        //     this.write_new_patch( lang, op );
        //     nb_sp -= 12;
        //     wl( `            break;` );
        //     wl( `        }` );
        // }
        // wl( `        }` );
        // wl( `    }` );
        wl( `}` );
        wl();


        // exports
        wl( `export default { read, bin_repr, new_patch, undo_patch };` );
    }

    func_args_op( lang: string, op: OpInfo<UT> ): string {
        const inst = new op.op_type;
        return Object.keys( inst ).map( x => `${ x }: ${ this.loc_type( lang, inst[ x ] ) }` ).join( ', ' );
    }

    bw_write_op( lang: string, op: OpInfo<UT> ): string {
        const inst = new op.op_type;
        return Object.keys( inst ).map( x => `bw.write_${ this.gen_type( inst[ x ] ) }( ${ x } );` ).join( ' ' );
    }

    br_read_obj( lang: string, op: OpInfo<UT> ): string {
        const inst = new op.op_type;
        return Object.keys( inst ).map( x => `${ x }: br.read_${ this.gen_type( inst[ x ] ) }()` ).join( ', ' );
    }

    br_skip_obj( lang: string, op: OpInfo<UT> ): string {
        const inst = new op.op_type;
        return Object.keys( inst ).map( x => `br.skip_${ this.gen_type( inst[ x ] ) }();` ).join( ' ' );
    }

    br_read_var( lang: string, op: OpInfo<UT>, br = "br", suffix = "" ): string {
        const inst = new op.op_type, keys = Object.keys( inst );
        return keys.length ? "let " + keys.map( x => `${ x }${ suffix } = br.read_${ this.gen_type( inst[ x ] ) }()` ).join( ', ' ) + ";" : '';
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

    make_symbolic_data( op: OpInfo<UT>, suffix = "" ) {
        const res = {} as any;
        const inst = new op.op_type;
        for( const key of Object.keys( inst ) ) {
            switch ( inst[ key ].constructor.name ) {
                case 'LvNumber': res[ key ] = LvNumber.symbol( key + suffix ); break;
                case 'LvString': res[ key ] = LvString.symbol( key + suffix ); break;
                default: throw "TODO: make_symbolic_data for " + inst[ key ].constructor.name;
            }
        }
        return res;
    }

    // write_new_patch( lang: string, op_new: Operation ) {
    //     // read data
    //     wl( op_new.br_read_var( "br_new", "_new" ) );

    //     // read unk data
    //     wl( `let br_unk = new BinaryReader( cq_unk.to_Uint8Array() );` );
    //     wl( `while ( br_unk.size ) {` );
    //     wl( `    const num_unk = br_unk.read_PI8();` );
    //     wl( `    switch ( num_unk ) {` );
    //     for( const op_unk of this.operations ) {
    //         wl( `        case ${ op_unk.num }: {` );
    //         nb_sp += 12;
    //         wl( op_unk.br_read_var( "br_unk", "_unk" ) );
    //         // wl( `console.log( "unk:", { ${ op_unk.args.map( x => x.name ).join(',') } } );` );

    //         let cb = this.trans_rules.get( `${ op_unk.name } ${ op_new.name }` );
    //         if ( cb ) {
    //             let data_unk = op_unk.make_symbolic_data( "_unk" );
    //             let data_new = op_new.make_symbolic_data( "_new" );
    //             cb( data_unk, data_new );
    //             wl( Codegen.make_code( [
    //                 ...Object.keys( data_unk ).map( k => data_unk[ k ] ),
    //                 ...Object.keys( data_new ).map( k => data_new[ k ] ),
    //             ] ) );
    //         }

    //         nb_sp -= 12;
    //         wl( `            break;` );
    //         wl( `        }` );
    //     }
    //     wl( `        default: break;` );
    //     wl( `    }` );
    //     wl( `}` );

    //     // apply operation
    //     wl( op_new.apply_op[ lang ]( "val", "_new" ) );

    //     // write back the new data
    //     wl( `bw_new.write_PI8( ${ op_new.num } ); ${ op_new.bw_write( "_new" ) }` );
    // }

    write_undo_patch( lang: string, op: OpInfo<UT> ) {
        let d = this.underlying_class.symbol( "val" );
        let o = this.make_symbolic_data( op );
        op.undo( d, o );
        wl( this.br_read_var( lang, op ) );
        wl( Codegen.make_code( [ d ] ) );
    }

    operations      = new Array<OpInfo<UT>>();
    class_name      : string;
    underlying_class: any;

    // operations      = new Array<Operation>();
    // trans_rules     = new Map<string,(o,n)=>void>();
}
