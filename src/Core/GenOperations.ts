import Codegen         from "../Symbol/Codegen"
import { _if }         from "../Symbol/If"
import Graphviz        from "../Core/Graphviz"
import methods         from "../Core/methods" 
import Rp              from "../Core/Rp"
import LvNumber        from "../LvNumber"
import LvString        from "../LvString"
import LvUsrId         from "../LvUsrId"
import MA, { LvArray } from "../LvArray"
import LvMap           from "../LvMap"
import LvAny           from "../LvAny"
import * as fs         from "fs"
    
export { _if }         from "../Symbol/If";

export class Op {}

const Map_LvUsrId_LvNumber = LvMap( LvUsrId, LvNumber );
export class OtWrapperWithRightFlags { right_flags = new Map_LvUsrId_LvNumber; }

class OpInfo<UT> {
    inst     : any; /** instance of symbolic repr */
    num      : number;
    apply   ?: ( d: UT, o: Op ) => any;
    undo    ?: ( d: UT, o: Op ) => any;
    store   ?: ( d: UT, o: Op ) => Array<{type:any,data:any}>;
    right_to?: ( d: UT, o, f: LvNumber, r: LvNumber ) => void;
};

class BwRepr {
    constructor( go, name = "bw" ) {
        this.sym      = LvString.symbol( name );
        this.orig_sym = this.sym.rp;
        this.go       = go;
    }
    push( op_type, data ) {
        const op_info = this.go.operations.find( x => x.inst.constructor == op_type );
        this.sym.applyMethod( "write_PI8", new LvNumber( op_info.num ) );
        Object.keys( op_info.inst ).forEach( name => {
            const gt = this.go.gen_type( data[ name ] );
            this.sym.applyMethod( "write_" + ( gt || "obj" ), data[ name ] );
        } );
    }
    sym     : LvString;
    go      : any; /** GenOperation<...> */
    orig_sym: Rp;
}

/** helper */
let nb_sp = 0;
function wl( str = "" ) {
    process.stdout.write( " ".repeat( nb_sp ) + str + "\n" );
}

export class AddUsrRight { usr = new LvUsrId(); flags = new LvNumber(); };
export class RemUsrRight { usr = new LvUsrId(); flags = new LvNumber(); };

declare type TransFunc = ( o, n, lo: BwRepr, ln: BwRepr ) => void;

export default 
class GenOperation<UT> {
    static AddUsrRight = AddUsrRight;

    constructor( class_: any, sym_corr = {} as { [ name: string ]: string } ) {
        this.sym_corr = sym_corr;
        this.cl_inst  = new class_;
    }

    /** this function install  */
    define_rights_by_flags( right_names: Array<string>, attr_name = "right_flags" ) {
        this.right_names = [ "add_usr_right", "rem_usr_right", ...right_names ];

        this.apply( AddUsrRight, ( d, o: AddUsrRight ) => { const v = ( d as any as OtWrapperWithRightFlags ).right_flags.get( o.usr ); v.selfOrBin( o.flags ); } );
        this.apply( RemUsrRight, ( d, o: RemUsrRight ) => { const v = ( d as any as OtWrapperWithRightFlags ).right_flags.get( o.usr ); v.selfAndBin( o.flags.notBin() ); } );

        this.undo ( AddUsrRight, ( d, o: AddUsrRight ) => ( d as any as OtWrapperWithRightFlags ).right_flags.get( o.usr ).selfXorBin( o.flags ) );
        this.undo ( RemUsrRight, ( d, o: RemUsrRight ) => ( d as any as OtWrapperWithRightFlags ).right_flags.get( o.usr ).selfXorBin( o.flags ) );

        this.right( AddUsrRight, ( d, o: AddUsrRight, f: LvNumber, r: LvNumber ) => r.set( f.andBin( this.flag( "add_usr_right" ) ) ) );
        this.right( RemUsrRight, ( d, o: RemUsrRight, f: LvNumber, r: LvNumber ) => r.set( f.andBin( this.flag( "rem_usr_right" ) ) ) );

        this.store( AddUsrRight, ( d, o: AddUsrRight ) => [ { type: AddUsrRight, data: { usr: o.usr, flags: o.flags.andBin( ( d as any as OtWrapperWithRightFlags ).right_flags.get( o.usr ).notBin() ) } as AddUsrRight } ] );
        this.store( RemUsrRight, ( d, o: RemUsrRight ) => [ { type: RemUsrRight, data: { usr: o.usr, flags: o.flags.andBin( ( d as any as OtWrapperWithRightFlags ).right_flags.get( o.usr )          ) } as RemUsrRight } ] );

        // i: 010011
        // o: 011111  ADD(001100)
        // n: 111011  ADD(101000)
        // r: 111111  (n:(o->r) = ADD(100000); o:(n->r) = unk: ADD(000100))
        this.fwd_trans( AddUsrRight, AddUsrRight, ( o: AddUsrRight, n: AddUsrRight ) => _if( o.usr.equ( n.usr ), () => {
            n.flags.selfAndBin( o.flags.notBin() );
            o.flags.selfAndBin( n.flags.notBin() );
        } ) );

        // i: 010011
        // o: 010000  REM(000011)
        // n: 000010  REM(010001)
        // r: 000000  (n:(o->r) = REM(010000); o:(n->r) = REM(000010))
        this.fwd_trans( RemUsrRight, RemUsrRight, ( o: RemUsrRight, n: RemUsrRight ) => _if( o.usr.equ( n.usr ), () => {
            n.flags.selfAndBin( o.flags.notBin() );
            o.flags.selfAndBin( n.flags.notBin() );
        } ) );
    }

    /** add description of permission */
    right( op_type, cb: ( d: UT, o, f: LvNumber, r: LvNumber ) => void ) {
        this.reg_op( op_type ).right_to = cb;
    }

    /** flag associated with name */
    flag( name: string ): number {
        return Math.pow( 2, this.right_names.indexOf( name ) );
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

    fwd_trans( o_op_type, n_op_type, cb: TransFunc ) {
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
        wl( `import BinaryWriter from "../System/BinaryWriter"` );
        wl( `import BinaryReader from "../System/BinaryReader"` );
        wl( `import UsrId        from "../System/UsrId"`        );
        wl( `import DevId        from "../System/DevId"`        );
        wl();

        // bin_repr
        wl( `var bin_repr = {` );
        for( const op of this.operations )
            wl( `    ${ op.inst.constructor.name }: function( bw: BinaryWriter, ${ this.func_args_op( lang, op ) } ): void { bw.write_PI8( ${ op.num } ); ${ this.bw_write_obj( lang, op ) } },` );
        wl( `}` );
        wl();

        // right to
        wl( `var right_to = {` );
        for( const op of this.operations )
            wl( `    ${ op.inst.constructor.name }: function( val: ${ this.cl_inst.constructor.name }, _as_usr: UsrId, ${ this.func_args_op( lang, op ) } ): boolean { ${ this._write_right_to( lang, op ) } },` );
        wl( `}` );
        wl();

        // read
        wl( `function read( br: BinaryReader, cb: ( type: string, args: any ) => void, src_dev?: DevId, src_usr?: UsrId, cur_dev?: DevId, cur_usr?: UsrId ) {` );
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
                wl( `        case ${ op.num }: { // ${ op.inst.constructor.name }` );
                nb_sp += 12;
                this._write_undo_patch( lang, op );
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
        wl( `function new_patch( val: ${ this.cl_inst.constructor.name }, bw_new: BinaryWriter, br_new: BinaryReader, as_usr: UsrId, cq_unk: BinaryWriter, src_dev?: DevId, src_usr?: UsrId, cur_dev?: DevId, cur_usr?: UsrId ) {` );
        wl( `    while ( br_new.size ) {` );
        wl( `        let br_unk = new BinaryReader( cq_unk.to_Uint8Array() );` );
        wl( `        let bw_unk = new BinaryWriter;` );
        wl( `        switch ( br_new.read_PI8() ) {` );
        for( const op_new of this.operations ) {
            wl( `        case ${ op_new.num }: { // ${ op_new.inst.constructor.name }` );
            nb_sp += 12;
            this._write_new_patch( lang, op_new );
            nb_sp -= 12;
            wl( `            break;` );
            wl( `        }` );
        }
        wl( `        }` );
        wl( `    }` );
        wl( `    return val;` );
        wl( `}` );
        wl();

        // get_possible_rights__b
        wl( `function get_possible_rights__b(): Array<string> {` );
        wl( `    return ${ JSON.stringify( this.right_names ) };` );
        wl( `}` );
        wl();


        // exports
        wl( `export default { read, right_to, bin_repr, new_patch, undo_patch, get_possible_rights__b };` );
    }

    func_args_op( lang: string, op: OpInfo<UT>, types = true, suffix = "" ): string {
        return Object.keys( op.inst ).map( x => x + suffix + ( types ? ": " + this.loc_type( lang, op.inst[ x ] ) : "" ) ).join( ', ' );
    }

    bw_write( lang: string, name: string, inst: any, bw = "bw", suffix = "" ): string {
        const gt = this.gen_type( inst );
        return gt ? `${ bw }.write_${ gt }( ${ name }${ suffix } );` : `${ name }${ suffix }.write_to( ${ bw } );`;
    }

    bw_write_obj( lang: string, op: OpInfo<UT>, bw = "bw", suffix = "" ): string {
        return Object.keys( op.inst ).map( x => this.bw_write( lang, x, op.inst[ x ], bw, suffix ) ).join( ' ' );
    }

    br_read( lang: string, inst: any, br = "br", corr_read = true ): string {
        const gt = this.gen_type( inst );
        return gt ? `${ br }.read_${ gt }()` : `${ inst.constructor.NativeType.name }.read_from( ${ br }${
            corr_read && inst.constructor.NativeType.need_dev_for_read ? ", src_dev" : "" }${
            corr_read && inst.constructor.NativeType.need_usr_for_read ? ", src_usr" : "" }${
            corr_read && inst.constructor.NativeType.need_dev_for_read ? ", cur_dev" : "" }${
            corr_read && inst.constructor.NativeType.need_usr_for_read ? ", cur_usr" : ""
        } )`;
    }

    br_read_obj( lang: string, op: OpInfo<UT> ): string {
        return Object.keys( op.inst ).map( x => `${ x }: ${ this.br_read( lang, op.inst[ x ] ) }` ).join( ', ' );
    }

    br_skip( lang: string, inst: any, br = "br" ): string {
        const gt = this.gen_type( inst );
        return gt ? `${ br }.skip_${ gt }();` : `${ inst.constructor.NativeType.name }.skip_from( ${ br } );`;
    }

    br_skip_obj( lang: string, op: OpInfo<UT> ): string {
        return Object.keys( op.inst ).map( x => this.br_skip( lang, op.inst[ x ] ) ).join( ' ' );
    }

    br_read_var( lang: string, op: OpInfo<UT>, br = "br", suffix = "", corr_read = true ): string {
        const keys = Object.keys( op.inst );
        return keys.length ? "let " + keys.map( x => `${ x }${ suffix } = ${ this.br_read( lang, op.inst[ x ], br, corr_read ) }` ).join( ', ' ) + ";" : '';
    }

    loc_type( lang: string, inst ): string {
        switch ( inst.constructor.name ) {
            case "LvString": return "string";
            case "LvNumber": return "number";
        }
        return inst.constructor.NativeType.name;
    }

    gen_type( inst ): string {
        switch ( inst.constructor.name ) {
            case "LvString": return "String";
            case "LvNumber": return "PT";
        }
        return null;
    }

    make_symbolic_data( inst: any, suffix = "", prefix = "", correction = {} as { [ key: string ]: string } ) {
        const res = {} as any;
        for( const key of Object.keys( inst ) ) {
            res[ key ] = inst[ key ].constructor.symbol( prefix + ( correction[ key ] || key ) + suffix );
            // switch ( inst[ key ].constructor.name ) {
            //     case 'LvNumber': res[ key ] = inst[ key ].constructor.symbol( prefix + ( correction[ key ] || key ) + suffix ); break;
            //     case 'LvString': res[ key ] = inst[ key ].constructor.symbol( prefix + ( correction[ key ] || key ) + suffix ); break;
            //     case '_LvMap'  : res[ key ] = inst[ key ].constructor.symbol( prefix + ( correction[ key ] || key ) + suffix ); break;
            //     default: throw "TODO: make_symbolic_data for " + inst[ key ].constructor.name;
            // }
        }
        return res;
    }

    _write_new_patch( lang: string, op_new: OpInfo<UT> ) {
        // read data
        wl( this.br_read_var( lang, op_new, "br_new", "_new" ) );

        // read unk data
        wl( `while ( br_unk.size ) {` );
        wl( `    const num_unk = br_unk.read_PI8();` );
        wl( `    switch ( num_unk ) {` );
        for( const op_unk of this.operations ) {
            wl( `        case ${ op_unk.num }: { // ${ op_unk.inst.constructor.name }` );
            nb_sp += 12;
            wl( `console.log( '${ op_unk.inst.constructor.name }', '${ op_new.inst.constructor.name }' );` );
            wl( this.br_read_var( lang, op_unk, "br_unk", "_unk", false ) );
            
            let cb = this.trans_rules.get( `${ op_unk.inst.constructor.name } ${ op_new.inst.constructor.name }` ), inv = false;
            if ( ! cb )
                cb = this.trans_rules.get( `${ op_new.inst.constructor.name } ${ op_unk.inst.constructor.name }` ), inv = true;
            if ( cb ) {
                let data_unk = this.make_symbolic_data( op_unk.inst, "_unk" );
                let data_new = this.make_symbolic_data( op_new.inst, "_new" );
                let lo = new BwRepr( this, "bw_unk" );
                let ln = new BwRepr( this, "bw_new" );
                if ( inv )
                    cb( data_new, data_unk, ln, lo );
                else
                    cb( data_unk, data_new, lo, ln );

                // if ( lo.sym.rp != lo.orig_sym ) {
                //     Graphviz.display( [
                //         ...Object.keys( data_unk ).map( k => data_unk[ k ].rp ),
                //         ...Object.keys( data_new ).map( k => data_new[ k ].rp ),
                //         lo.sym.rp,
                //         ln.sym.rp,
                //     ] );
                // }

                wl( Codegen.make_code( [
                    ...Object.keys( data_unk ).map( k => data_unk[ k ] ),
                    ...Object.keys( data_new ).map( k => data_new[ k ] ),
                    lo.sym,
                    ln.sym,
                ], lang ) );
            }
            wl( `bw_unk.write_PI8( ${ op_unk.num } ); ${ this.bw_write_obj( lang, op_unk, "bw_unk", "_unk" ) }` );
            nb_sp -= 12;
            wl( `            break;` );
            wl( `        }` );
        }
        wl( `    }` );
        wl( `}` );
        
        // test rights
        wl( `if ( right_to.${ op_new.inst.constructor.name }( val, as_usr, ${ this.func_args_op( lang, op_new, false, "_new" ) } ) ) {` );
        nb_sp += 4;

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
                wl( `bw_new.write_PI8( ${ op.num } );` + this.bw_write_obj( lang, op, "bw_new", "_tmp" ) );
                nb_sp -= 4;
                wl( `}` ); 
            }
        } else {
            wl( `bw_new.write_PI8( ${ op_new.num } ); ${ this.bw_write_obj( lang, op_new, "bw_new", "_new" ) }` );
        }

        // register new unk data
        wl( `bw_unk.transfer_to( cq_unk );` );

        // apply operation
        let d = this.make_symbolic_data( this.cl_inst, "", "val.", this.sym_corr );
        let o = this.make_symbolic_data( op_new.inst, "_new" );
        op_new.apply( d, o );
        wl( Codegen.make_code( Object.keys( d ).map( n => d[ n ] ), lang ) );

        nb_sp -= 4;
        wl( `}` );
    }

    _write_undo_patch( lang: string, op: OpInfo<UT> ) {
        let d = this.make_symbolic_data( this.cl_inst, "", "val.", this.sym_corr );
        let o = this.make_symbolic_data( op.inst );
        op.undo( d, o );
        wl( this.br_read_var( lang, op, "br", "", false ) );
        wl( Codegen.make_code( Object.keys( d ).map( n => d[ n ] ), lang ) );
    }

    _write_right_to( lang: string, op: OpInfo<UT> ): string {
        let d = this.make_symbolic_data( this.cl_inst, "", "val.", this.sym_corr );
        let o = this.make_symbolic_data( op.inst );
        let f = LvNumber.symbol( "_f" );
        let r = LvNumber.symbol( "_r" );
        op.right_to( d, o, f, r );
        return "var _r,_f=val.right_flags.get(_as_usr);" + Codegen.make_code( [ r ], lang ) + "return _r;";
    }

    cl_inst     : any; /** instance of symbolic repr of class */
    sym_corr    : { [ name: string ]: string }; /** names in symbolic repr to name in dst code */
    operations  = new Array<OpInfo<UT>>();
    trans_rules = new Map<string,TransFunc>();
    right_names = new Array<string>();
}
