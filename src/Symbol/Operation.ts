import Variable                           from "../Core/Variable"
import methods                            from "../Core/methods"
import Method                             from "../Core/Method"
import Surdef                             from "../Core/Surdef"
import Rp                                 from "../Core/Rp"
import SymbolicKnownValue, { skv_link_o } from "./SymbolicKnownValue"
import RpListSymbolic                     from "./RpListSymbolic"
import BlockCodegen                       from "./BlockCodegen"
import { cd }                             from "./Codegen"
import { unget_nout, get_nout }           from "./GetNout"
import Sym, { Link }                      from "./Sym"
  
import LvBuffer                           from "../LvBuffer"
import LvNumber                           from "../LvNumber"

export default 
class Operation extends Sym {
    constructor( method: Method, ...args ) {
        super( Operation );
        this.method        = method;
        this.self_args     = method.pattern.split( "" ).map( ( val, ind ) => val == "s" ? ind : -1 ).filter( x => x >= 0 );
        this.children      = args.slice( 0, method.nb_rp_args );
        this.args          = args.slice( method.nb_rp_args );
        this.variable_type = this._find_variable_type();
    }

    variable_type__b() {
        return this.variable_type;
    }

    to_String__b(): string {
        const lst = [ ...this.children.map( x => methods[ "to_String__b" ].call_1( x.item ) ), ...this.args.map( x => x.toString() ) ];
        return `${ this.base_name }(${ lst.join( ',' ) })`;
    }

    key_type__b() {
        if ( this.method.pattern[ 0 ] == "s" )
            return methods["key_type__b"].call_1( this.children[ 0 ].item );
        throw new Error( "TODO" );
    }

    val_type__b() {
        if ( this.method.pattern[ 0 ] == "s" )
            return methods["val_type__b"].call_1( this.children[ 0 ].item );
        throw new Error( "TODO" );
    }

    /** this.method.base_name, with a '_s' if self method */
    get base_name(): string {
        const ind_und = this.method.name.indexOf( '__' );
        return this.method.name.substr( 0, ind_und ) + ( this.method.name[ ind_und + 2 ] == 's' ? '_s' : '' );
    }

    repr_graphviz(): string {
        return this.base_name;
    }

    self_ops(): Array<number> {
        return this.self_args;
    }

    static prec = {
        GROUP   :  0, MEMBER  : 1, NEW     : 1, CALL    :  2, INC_SUF : 3, DEC_SUF : 3,
        NOT     :  4, NEG     : 4, INC_PRE : 4, DEC_PRE :  4, TYPEOF  : 4, DELETE  : 4,
        MUL     :  5, EXPO    : 5, DIV     : 5, REM     :  5,
        ADD     :  6, SUB     : 6, LFT_SHT : 7, RGT_SHT :  7, SRGT_SHT: 7, SLFT_SHT: 7,
        INF     :  8, INF_EQ  : 8, SUP     : 8, SUP_EQ  :  8, IN      : 8, INST_OF : 8,
        EQU     :  9, NEQ     : 9, EQU_S   : 9, NEQ_S   :  9,
        AND_BIN : 10, XOR_BIN : 11, OR_BIN : 12, AND_LOG: 13, OR_LOG  : 14,
        COND_OP : 15, ASSIGN  : 16, YIELD  : 17, COMMA  : 19, MAX_PREC: 20
    }

    block_code( cg: BlockCodegen, options ): void {
        // we can skip this instruction (to be written later in inline mode) ?
        if ( this.self_args.length == 0 && cd( this ).parents.length <= 1 ) {
            cd( this ).done = false;
            return null;
        }
        // not a self op ? => block_code was called to create a new variable
        if ( this.self_args.length == 0 ) {
            cd( this ).var_names[ 0 ] = cg.new_var( prec => this.inline_code( cg, 0, prec, {} ) ); 
            return;
        }
        // 
        const num_self = this.self_args[ 0 ];
        cd( this ).var_names.push( cd( this.children[ num_self ].item ).var_names[ this.children[ num_self ].nout ] );
        cg.inst( `${ this.inline_code( cg, 0, 1e6, {} ) };` );
    }

    inline_code( cg: BlockCodegen, nout: number, prec: number, options ): string {
        // self op
        if ( this.self_args.length ) {
            const name = cg.checked_var_name( this.children[ 0 ] );

            if ( this.method.select ) {
                // where smurf.get(x).self_op(y) works
                // else, it should be a graph transformation as add__sbb( map, key, val ) => set__sbb( map, key, select_bb( map, key ) + val )
                //
                const bin_sop = ( op: string ) => {
                    const res = `${ name }${ rl.children.length > 1 ? rl.children.slice( 0, rl.children.length - 1 ).map( x => `.get(${ cg.inline_code( x, Operation.prec.MAX_PREC ) })` ).join( "" ) :
                             "" }.get(${ cg.inline_code( rl.children[ rl.children.length - 1 ], Operation.prec.MAX_PREC ) })${op}${ cg.inline_code( this.children[ 2 ], Operation.prec.COMMA ) }`;
                    return Operation.prec.ASSIGN > prec ? `(${ res })` : res;
                }
                // const bin = ( op: string ) => {
                //     const base
                //     const res = `${ name }${ rl.children.length > 1 ? rl.children.slice( 0, rl.children.length - 1 ).map( x => `.get(${ cg.inline_code( x, Operation.prec.MAX_PREC ) })` ).join( "" ) :
                //              "" }.get(${ cg.inline_code( rl.children[ rl.children.length - 1 ], Operation.prec.MAX_PREC ) })${op}${ cg.inline_code( this.children[ 2 ], Operation.prec.COMMA ) }`;
                //     return Operation.prec.ASSIGN > prec ? `(${ res })` : res;
                // }

                const rl = this.children[ 1 ].item;
                if ( rl instanceof RpListSymbolic ) {
                    switch ( this.method.base_name ) {
                        // binary
                        case "set":
                            return `${ name }${ rl.children.length > 1 ? rl.children.slice( 0, rl.children.length - 1 ).map( x => `.get(${ cg.inline_code( x, Operation.prec.MAX_PREC ) })` ).join( "" ) :
                             "" }.set(${ cg.inline_code( rl.children[ rl.children.length - 1 ], Operation.prec.COMMA ) },${ cg.inline_code( this.children[ 2 ], Operation.prec.COMMA ) })`;

                        // case "and_bin": return bin_sop( "+=" );
                            
                        default:
                            return `${ name }.${ this.method.name }(${ [
                                ...this.children.slice( 1 ).map( ch => cg.inline_code( ch, Operation.prec.COMMA ) ),
                                ...this.args
                            ].join( ',' ) })`;
                    }
                } else
                    throw new Error( "TODO" );
            }

            const bin = ( op: string ) => {
                const res = name + op + cg.inline_code( this.children[ 1 ], Operation.prec.ASSIGN );
                return Operation.prec.ASSIGN > prec ? `(${ res })` : res;
            }
            const bim = ( op: string ) => {
                const res = name + "." + op + "(" + cg.inline_code( this.children[ 1 ], Operation.prec.MAX_PREC ) + ")";
                return Operation.prec.MEMBER > prec ? `(${ res })` : res;
            }
            switch ( this.method.base_name ) {
                // binary
                case "add"    : return bin( '+=' );
                case "sub"    : return bin( '-=' );
                case "mul"    : return bin( '*=' );
                case "div"    : return bin( '/=' );
                case "or_bin" : return bin( '|=' );
                case "and_bin": return bin( '&=' );
                case "xor_bin": return bin( '^=' );
                case "set"    : return bin( '='  );
                case "push"   : return bim( 'push' );
                default:
                    return `${ name }.${ this.method.name }(${ [
                        ...this.children.slice( 1 ).map( ch => cg.inline_code( ch, Operation.prec.COMMA ) ),
                        ...this.args
                    ].join( ',' ) })`;
            }
        }

        // helpers
        const par = ( prec_op: number, op: string ) => prec_op > prec ? `(${ op })` : op;
        const bin = ( prec_op: number, op: string ) => par( prec_op, cg.inline_code( this.children[ 0 ], prec_op ) + op + cg.inline_code( this.children[ 1 ], prec_op ) );
        const una = ( prec_op: number, op: string ) => par( prec_op, op + cg.inline_code( this.children[ 0 ], prec_op ) );

        // content
        switch ( this.method.base_name ) {
            // unary
            case "to_BooleanVariable": return par( Operation.prec.CALL, options.conv_to_boolean ? cg.inline_code( this.children[ 0 ], prec, options ) : `Boolean(${ cg.inline_code( this.children[ 0 ], 1e6 ) })` );
            case "copy"              : return cg.inline_code( this.children[ 0 ], prec );
            case "get_size"          : return par( Operation.prec.MEMBER, `${ cg.inline_code( this.children[ 0 ], Operation.prec.MEMBER ) }.${ this.children[ 0 ].item.variable_type__b() == LvBuffer ? "byteLength" : "length" }` );
            case "not_log"           : return una( Operation.prec.NOT     , "!"   );
            case "not_bin"           : return una( Operation.prec.NOT     , "~"   );
            // binary
            case "add"               : return bin( Operation.prec.ADD     , "+"   );
            case "sub"               : return bin( Operation.prec.SUB     , "-"   );
            case "mul"               : return bin( Operation.prec.MUL     , "*"   );
            case "div"               : return bin( Operation.prec.DIV     , "/"   );
            case "mod"               : return bin( Operation.prec.REM     , "%"   );
            case "or_bin"            : return bin( Operation.prec.OR_BIN  , "|"   );
            case "xor_bin"           : return bin( Operation.prec.AND_BIN , "^"   );
            case "and_bin"           : return bin( Operation.prec.AND_BIN , "&"   );
            case "or_log"            : return bin( Operation.prec.OR_LOG  , "||"  );
            case "and_log"           : return bin( Operation.prec.AND_LOG , "&&"  );
            case "is_equ"            : return bin( Operation.prec.EQU     , "=="  );
            case "is_inf"            : return bin( Operation.prec.INF     , "<"   );
            case "is_infeq"          : return bin( Operation.prec.INF_EQ  , "<="  );
            case "is_sup"            : return bin( Operation.prec.SUP     , ">"   );
            case "is_supeq"          : return bin( Operation.prec.SUP_EQ  , ">="  );
            case "signed_shift_left" : return bin( Operation.prec.SLFT_SHT, "<<"  );
            case "signed_shift_right": return bin( Operation.prec.SRGT_SHT, ">>"  );
            case "zfill_shift_right" : return bin( Operation.prec.RGT_SHT , ">>>" );
            case "select"            :
                if ( this.children[ 1 ].item instanceof RpListSymbolic ) {
                    const lst = this.children[ 1 ].item.children;
                    if ( this.children[ 0 ].item.variable_type__b().use_get_for_select )
                        return par( Operation.prec.MEMBER, cg.inline_code( this.children[ 0 ], Operation.prec.MEMBER ) + 
                            lst.map( ch => ".get(" + cg.inline_code( ch, Operation.prec.MAX_PREC ) + ")" ).join( "" ) );
                    return par( Operation.prec.CALL, cg.inline_code( this.children[ 0 ], Operation.prec.CALL ) + 
                            lst.map( ch => "[" + cg.inline_code( ch, Operation.prec.MAX_PREC ) + "]" ).join( "" ) );
                    // par( Operation.prec.CALL, cg.inline_code( this.children[ 0 ], Operation.prec.CALL ) + "[" + cg.inline_code( this.children[ 1 ], Operation.prec.MAX_PREC ) + "]" )
                }
                console.error( this.children[ 1 ].item );
                throw new Error( "TODO" );
            case "heads"             : return par( Operation.prec.MEMBER, `${ cg.inline_code( this.children[ 0 ], Operation.prec.MEMBER ) }.substr(0,${ cg.inline_code( this.children[ 1 ], Operation.prec.COMMA ) })` );
            case "tails"             : return par( Operation.prec.MEMBER, `${ cg.inline_code( this.children[ 0 ], Operation.prec.MEMBER ) }.substr(${ cg.inline_code( this.children[ 1 ], Operation.prec.MAX_PREC ) })` );
            // ternary
            case "slice"             : return par( Operation.prec.MEMBER, `${ cg.inline_code( this.children[ 0 ], Operation.prec.MEMBER ) }.substring(${ cg.inline_code( this.children[ 1 ], Operation.prec.COMMA ) },${ cg.inline_code( this.children[ 2 ], Operation.prec.COMMA ) })` );
            // default
            default:
                return par( Operation.prec.CALL, `${ this.method.base_name }(${ [
                    ...this.children.map( ch => cg.inline_code( ch, Operation.prec.COMMA ) ),
                    ...this.args
                ].join( ',' ) })` );
        }
    }

    _find_variable_type(): typeof Variable {
        for( const k of Object.keys( Operation._variable_type_finders ) ) {
            for( const f of Operation._variable_type_finders[ k ] ) {
                const res = f( this.method, this.args );
                if ( res )
                    return res;
            }
        }
        return null;
    }

    static add_variable_type_finder( prio: number, finder: ( method: Method, args ) => typeof Variable ) {
        let l = Operation._variable_type_finders[ - prio ] || ( Operation._variable_type_finders[ - prio ] = [] );
        l.push( finder );
    }

    static _variable_type_finders = {} as { [ key: number ]: Array<( method: Method, args ) => typeof Variable> };

    method       : Method;
    self_args    : Array<number>;   /** Sym args that will be modified */
    args         : Array<any>;      /** not the Sym stuff (which is in children) */
    variable_type: typeof Variable; /** like String, Buffer, ... */
} 
Sym.make_templ( Operation );

export function make_op( method: string | Method, ...args ): Link {
    if ( typeof method == "string" )
        return make_op( methods[ method ], ...args );

    // if everything is known => compute directly the result
    if ( args.slice( 0, method.nb_rp_args ).every( x => x.item instanceof SymbolicKnownValue ) ) {
        return methods["symbolic_known_value__o"].call_1( method.call_gen(
            ...args.slice( 0, method.nb_rp_args ).map( x => ( x.item as SymbolicKnownValue ).value ),
            ...args.slice( method.nb_rp_args )
        ) );
    }

    return { item: new Operation( method, ...args ), nout: 0 };
}

//
// methods[ "copy__b" ].add_surdef( 2, ( type_0 ) => type_0.prototype instanceof Sym, ( a: Sym ) => new Operation( methods[ "copy__b" ], unget_nout( a ) ) );
function uncopy( value: Sym ): Sym { return value instanceof Operation && value.method.name == "copy__b" ? get_nout( value.children[ 0 ].item, value.children[ 0 ].nout ) : value; }

export function slb( value: Rp ): Link { return value instanceof Sym ? unget_nout( uncopy( value ) ) : methods["symbolic_known_value__b"].call_1( value ); }
export function slo( value: Rp ): Link { return value instanceof Sym ? unget_nout( uncopy( value ) ) : methods["symbolic_known_value__o"].call_1( value ); }
export function sls( value: Rp ): Link { return value instanceof Sym ? unget_nout(         value   ) : methods["symbolic_known_value__o"].call_1( value ); }

//
Method.plugins.push( function( test_pf, for_a_test, method, type_0, type_1, type_2, type_4 ) {
    if ( ! method.ok_sym )
        return;
        
    // copy_b( symbol ) => 
    switch ( method.nb_rp_args ) {
    case 1:
        if ( type_0.prototype instanceof Sym ) {
            switch ( method.pattern ) {
                case 'b': test_pf( new Surdef( 1, null, ( a: Rp, ...args ) => new Operation( method, slb( a ), ...args ) ) ); break;
                case 'o': test_pf( new Surdef( 1, null, ( a: Rp, ...args ) => new Operation( method, slo( a ), ...args ) ) ); break;
                case 's': test_pf( new Surdef( 1, null, ( a: Rp, ...args ) => new Operation( method, sls( a ), ...args ) ) ); break;
            }
        }
        break;
    case 2:
        if ( type_0.prototype instanceof Sym || type_1.prototype instanceof Sym ) {
            switch ( method.pattern ) {
                case 'bb': test_pf( new Surdef( 1, null, ( a: Rp, b: Rp, ...args ) => new Operation( method, slb( a ), slb( b ), ...args ) ) ); break;
                case 'bo': test_pf( new Surdef( 1, null, ( a: Rp, b: Rp, ...args ) => new Operation( method, slb( a ), slo( b ), ...args ) ) ); break;
                case 'ob': test_pf( new Surdef( 1, null, ( a: Rp, b: Rp, ...args ) => new Operation( method, slo( a ), slb( b ), ...args ) ) ); break;
                case 'oo': test_pf( new Surdef( 1, null, ( a: Rp, b: Rp, ...args ) => new Operation( method, slo( a ), slo( b ), ...args ) ) ); break;
                case 'sb': test_pf( new Surdef( 1, null, ( a: Rp, b: Rp, ...args ) => new Operation( method, sls( a ), slb( b ), ...args ) ) ); break;
                case 'so': test_pf( new Surdef( 1, null, ( a: Rp, b: Rp, ...args ) => new Operation( method, sls( a ), slo( b ), ...args ) ) ); break;
            }
        }
        break;
    case 3:
        if ( type_0.prototype instanceof Sym || type_1.prototype instanceof Sym || type_2.prototype instanceof Sym ) {
            switch ( method.pattern ) {
                case 'bbb': test_pf( new Surdef( 1, null, ( a: Rp, b: Rp, c: Rp, ...args ) => new Operation( method, slb( a ), slb( b ), slb( c ), ...args ) ) ); break;
                case 'bbo': test_pf( new Surdef( 1, null, ( a: Rp, b: Rp, c: Rp, ...args ) => new Operation( method, slb( a ), slb( b ), slo( c ), ...args ) ) ); break;
                case 'bob': test_pf( new Surdef( 1, null, ( a: Rp, b: Rp, c: Rp, ...args ) => new Operation( method, slb( a ), slo( b ), slb( c ), ...args ) ) ); break;
                case 'boo': test_pf( new Surdef( 1, null, ( a: Rp, b: Rp, c: Rp, ...args ) => new Operation( method, slb( a ), slo( b ), slo( c ), ...args ) ) ); break;

                case 'obb': test_pf( new Surdef( 1, null, ( a: Rp, b: Rp, c: Rp, ...args ) => new Operation( method, slo( a ), slb( b ), slb( c ), ...args ) ) ); break;
                case 'obo': test_pf( new Surdef( 1, null, ( a: Rp, b: Rp, c: Rp, ...args ) => new Operation( method, slo( a ), slb( b ), slo( c ), ...args ) ) ); break;
                case 'oob': test_pf( new Surdef( 1, null, ( a: Rp, b: Rp, c: Rp, ...args ) => new Operation( method, slo( a ), slo( b ), slb( c ), ...args ) ) ); break;
                case 'ooo': test_pf( new Surdef( 1, null, ( a: Rp, b: Rp, c: Rp, ...args ) => new Operation( method, slo( a ), slo( b ), slo( c ), ...args ) ) ); break;

                case 'sbb': test_pf( new Surdef( 1, null, ( a: Rp, b: Rp, c: Rp, ...args ) => new Operation( method, sls( a ), slb( b ), slb( c ), ...args ) ) ); break;
                case 'sbo': test_pf( new Surdef( 1, null, ( a: Rp, b: Rp, c: Rp, ...args ) => new Operation( method, sls( a ), slb( b ), slo( c ), ...args ) ) ); break;
                case 'sob': test_pf( new Surdef( 1, null, ( a: Rp, b: Rp, c: Rp, ...args ) => new Operation( method, sls( a ), slo( b ), slb( c ), ...args ) ) ); break;
                case 'soo': test_pf( new Surdef( 1, null, ( a: Rp, b: Rp, c: Rp, ...args ) => new Operation( method, sls( a ), slo( b ), slo( c ), ...args ) ) ); break;
            }
        }
        break;
    }
} )

// default js_type definition
Operation.add_variable_type_finder( 0, function( method, args ) {
    // -> bool
    if ( [ "not_bin", "not_log", "neg", "is_pos", "is_neg", "is_eqz", "is_equ", "is_inf", "is_infeq", "get_size" ].indexOf( method.base_name ) >= 0 )
        return LvNumber;
    // self ops
    // if ( method.pattern[ 0 ] == "s" || method.base_name == "set" )
    //     return args[ 0 ].variable_type__b();
    // by default, we return the first argument 
    return args.length ? args[ 0 ].variable_type__b() : null;
} );
