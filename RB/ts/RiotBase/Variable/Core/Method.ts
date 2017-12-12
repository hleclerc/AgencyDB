import Surdef from "./Surdef"

declare class Map<K,V> {}

function disp_pos( s : Surdef ) : string {
    let p = s.stack.split( "\n" );
    return "\n->" + ( p.length > 2 ? p[ 2 ] : s.stack );
}

///
export default
class Method {
    static methods : { [ name : string ] : Method; } = {};
    static slf_mts = new Array<{ name: string, nb_rp_args: number }>();
    static std_mts = new Array<{ name: string, nb_rp_args: number }>();

    static decl( name: string, constness : string ) : void {
        Method.methods[ name ] = new Method( name, constness );
    }

    static dstd( name, nb_rp_args ) {
        Method.std_mts.push( { name: name, nb_rp_args: nb_rp_args } );

        for( let v of Method.comb_bo( nb_rp_args ) ) {
            let constness = v.split( "" ).map( c => "c" ).join( "" ); // c == "o" ? "r" : "c"
            Method.decl( name + "_" + v, constness );
        }
    }

    static dslf( name, nb_rp_args ) {
        Method.slf_mts.push( { name: name, nb_rp_args: nb_rp_args } );

        // xxx_bb, xxx_bo, ...
        Method.dstd( name, nb_rp_args );

        // xxx_sb, self_xxx_so, ... + xxx_sb_extr, self_xxx_so_extr, ...
        for( let v of Method.comb_bo( nb_rp_args - 1 ) ) {
            let constness = v.split( "" ).map( c => "c" ).join( "" ); // c == "o" ? "r" : "c"
            Method.decl( name + "_s" + v          , "r" + constness );
            Method.decl( name + "_s" + v + "_extr", "r" + constness );
        }

        // self_xxx_b, self_xxx_o, ...
        for( let v of Method.comb_bo( nb_rp_args - 1 ) ) {
            let constness = v.split( "" ).map( c => "c" ).join( "" ); // c == "o" ? "r" : "c"
            Method.decl( "self_" + name + ( v.length ? "_" + v : "" ), "r" + constness );
        }
    }

    static comb_bo( nb_rp_args : number ) : Array<String> {
        switch ( nb_rp_args ) {
            case 0: return [ "" ]
            case 1: return [ "b" ,"o" ]
            case 2: return [ "bb", "bo", "ob", "oo" ]
            case 3: return [ "bbb", "bob", "obb", "oob", "bbo", "boo", "obo", "ooo" ]
        }
        throw new Error("TODO");
    }

    constructor( name : string, constness : string ) {
        this.function_list = [];
        this.function_map  = new Map<string,any>();
        this.nb_tabu_types = 0;
        this.nb_rp_args    = constness.length;
        this.constness     = constness;
        this.surdefs       = [];
        this.name          = name;

        this.resize( 8 );
    }

    resize( new_nb_tabu_types : number ) : void {
        if ( this.nb_tabu_types < new_nb_tabu_types ) {
            switch ( this.nb_rp_args ) {
                case 1: {// 1 rp arg
                    let unk = ( ...args ) => {
                        let type_0 = Rp.type_num_map[ args[ 0 ].rp.type_num ];
                        args[ 0 ].rp.type_off = type_0.potentially_updated_type_off()
                        this.function_list[ args[ 0 ].rp.type_off ] = this.find_best_func_1( type_0 ).func;
                        return this.function_list[ args[ 0 ].rp.type_off ].apply( null, args );
                    }

                    while ( this.function_list.length < new_nb_tabu_types )
                        this.function_list.push( unk );
                    break;
                } case 2: {
                    let unk = ( ...args ) => {
                        let type_0 = Rp.type_num_map[ args[ 0 ].rp.type_num ];
                        let type_1 = Rp.type_num_map[ args[ 1 ].rp.type_num ];
                        args[ 0 ].rp.type_off = type_0.potentially_updated_type_off();
                        args[ 1 ].rp.type_off = type_1.potentially_updated_type_off();
                        this.function_list[ args[ 0 ].rp.type_off * this.nb_tabu_types + args[ 1 ].rp.type_off ] = this.find_best_func_2( type_0, type_1 ).func;
                        return this.function_list[ args[ 0 ].rp.type_off * this.nb_tabu_types + args[ 1 ].rp.type_off ].apply( null, args );
                    }

                    let new_function_list = [];
                    for( let i = 0; i < new_nb_tabu_types; i++ )
                        for( let j = 0; j < new_nb_tabu_types; j++ )
                            new_function_list.push( unk );
                    for( let i = 0; i < this.nb_tabu_types; i++ )
                        for( let j = 0; j < this.nb_tabu_types; j++ )
                            new_function_list[ i * new_nb_tabu_types + j ] = this.function_list[ i * this.nb_tabu_types + j ];
                    this.function_list = new_function_list
                    break;
                } case 3: {
                    let unk = ( ...args ) => {
                        let type_0 = Rp.type_num_map[ args[ 0 ].rp.type_num ];
                        let type_1 = Rp.type_num_map[ args[ 1 ].rp.type_num ];
                        let type_2 = Rp.type_num_map[ args[ 2 ].rp.type_num ];
                        args[ 0 ].rp.type_off = type_0.potentially_updated_type_off()
                        args[ 1 ].rp.type_off = type_1.potentially_updated_type_off()
                        args[ 2 ].rp.type_off = type_2.potentially_updated_type_off()
                        let s = String( [ args[ 0 ].rp.type_off, args[ 1 ].rp.type_off, args[ 2 ].rp.type_off ] );
                        this.function_list[ s ] = this.find_best_func_3( type_0, type_1, type_2 ).func;
                        return this.function_list[ s ].apply( null, args );
                    }

                    if ( ! this.nb_tabu_types )
                        this.function_list[ String( [ 0, 0, 0 ] ) ] = unk;
                    break;
                } default:
                    throw new Error( "TODO" );
            }
            this.nb_tabu_types = new_nb_tabu_types
        }
    }


    add_surdef( pertinence : number, test : ( (...any) => boolean ) | Array<any>, func : (...any) => any ) : void {
        this.surdefs.push( new Surdef( pertinence, test, func ) );
    }

    call_1( ...args ) {
        return this.function_list[ args[ 0 ].rp.type_off ].apply( null, args );
    }

    call_2( ...args ) {
        return this.function_list[ args[ 0 ].rp.type_off * this.nb_tabu_types + args[ 1 ].rp.type_off ].apply( null, args );
    }

    call_3( ...args ) {
        let s = String( [ args[ 0 ].rp.type_off, args[ 1 ].rp.type_off, args[ 2 ].rp.type_off ] )
        if ( this.function_list[ s ] == null )
            this.function_list[ s ] = this.function_list[ String( [ 0, 0, 0 ] ) ];
        return this.function_list[ s ].apply( null, args );
    }

    surdef_for( ...args ) : Surdef {
        switch ( this.nb_rp_args ) {
            case 1: return this.find_best_func_1( args[ 0 ], true );
            case 2: return this.find_best_func_2( args[ 0 ], args[ 1 ], true );
            case 3: return this.find_best_func_3( args[ 0 ], args[ 1 ], args[ 2 ], true );
            default: throw new Error( "TODO" );
        }
    }

    pert_surdef_for( ...args ) : number {
        let s = this.surdef_for( ...args );
        return s != null ? s.pertinence : -1e300;
    }

    has_surdef_for( ...args ) : boolean {
        return this.surdef_for( ...args ) != null;
    }

    static test_arg_surdef( test, type ) {
        if ( typeof test == 'string' )
            return Method.methods[ test ].has_surdef_for( type );
        return test.make_templ ? test == type : test( type );
    }

    find_best_func_1( type_0, for_a_test = false ) : Surdef {
        let best_surdefs = new Array<Surdef>();

        function test_pf( surdef : Surdef ) {
            if ( best_surdefs.length == 0 || best_surdefs[ 0 ].pertinence <= surdef.pertinence ) {
                if ( best_surdefs.length && best_surdefs[ 0 ].pertinence < surdef.pertinence )
                    best_surdefs = []
                best_surdefs.push( surdef );
            }
        }

        // ext surdefs
        for( let s of this.surdefs ) {
            if ( s.test instanceof Array ) {
                if ( s.test.length != 1 )
                    throw new Error( "Wrong test array size" );
                if ( Method.test_arg_surdef( s.test[ 0 ], type_0 ) )
                    test_pf( s );
            } else if ( Method.test_arg_surdef( s.test, type_0 ) )
                test_pf( s );
        }

        // method in type_0 ?
        if ( type_0.prototype[ this.name ] != null )
            test_pf( new Surdef( 2, () => true, ( val_0, ...args ) => type_0.prototype[ this.name ].apply( val_0.rp, args ) ) );

        // proxy ?
        if ( ! for_a_test && [ "rp_structure", "prox_ptr", "prox_ref", "prox_val" ].indexOf( this.name ) < 0 ) {
            if ( this.constness[ 0 ] == "c" ) {
                if ( Method.methods[ 'prox_ref' ].has_surdef_for( type_0 ) )
                    test_pf( new Surdef( 1, () => true, ( val_0, ...args ) => this.call_1.apply( this, [ Method.methods[ 'prox_ref' ].call_1( val_0 ) ].concat( args ) ) ) );
                else if ( Method.methods[ 'prox_val' ].has_surdef_for( type_0 ) )
                    test_pf( new Surdef( 1, () => true, ( val_0, ...args ) => this.call_1.apply( this, [ Method.methods[ 'prox_val' ].call_1( val_0 ) ].concat( args ) ) ) );
                else if ( Method.methods[ 'prox_ptr' ].has_surdef_for( type_0 ) )
                    test_pf( new Surdef( 1, () => true, ( val_0, ...args ) => this.call_1.apply( this, [ Method.methods[ 'prox_ptr' ].call_1( val_0 ) ].concat( args ) ) ) );
            } else if ( Method.methods[ 'prox_ptr' ].has_surdef_for( type_0 ) ) // if we are going to modify the value, (currently) only prox_ptr is allowed
                test_pf( new Surdef( 1, () => true, ( val_0, ...args ) => this.call_1.apply( this, [ Method.methods[ 'prox_ptr' ].call_1( val_0 ) ].concat( args ) ) ) );
        }

        // summary
        if ( ! for_a_test ) {
            if ( best_surdefs.length == 0 )
                throw new Error( `impossible to find surdef for ${ this.name } (with type ${ type_0.name })` );
            if ( best_surdefs.length >= 2 )
                throw new Error( `Ambiguous overload for ${ this.name } with type=${ type_0.name } and pert=${ best_surdefs[ 0 ].pertinence }, possibilities are ${ best_surdefs.map( disp_pos ).join( "" ) }` );
        }
        return best_surdefs.length ? best_surdefs[ 0 ] : null;
    }


    find_best_func_2( type_0, type_1, for_a_test = false ) : Surdef {
        // surdefs
        let best_surdefs = new Array<Surdef>();

        function test_pf( surdef : Surdef ) : void {
            if ( best_surdefs.length == 0 || best_surdefs[ 0 ].pertinence <= surdef.pertinence ) {
                if ( best_surdefs.length && best_surdefs[ 0 ].pertinence < surdef.pertinence )
                    best_surdefs = [];
                best_surdefs.push( surdef );
            }
        }

        // ext surdefs
        for( let s of this.surdefs ) {
            if ( s.test instanceof Array ) {
                if ( s.test.length != 2 )
                    throw new Error( "Wrong test array size" );
                if ( Method.test_arg_surdef( s.test[ 0 ], type_0 ) && Method.test_arg_surdef( s.test[ 1 ], type_1 ) )
                    test_pf( s );
            } else if ( s.test( type_0, type_1 ) )
                test_pf( s );
        }

        // proxy ?
        if ( ! for_a_test ) {
            let dla_0 = 1 + 0.1 / ( 1 + type_0.data_loss_if_prox );
            let dla_1 = 1 + 0.1 / ( 1 + type_1.data_loss_if_prox );
            if ( dla_1 == dla_0 )
                dla_1 += 0.00001;

            if ( this.constness[ 0 ] == "c" ) {
                if ( Method.methods[ 'prox_ref' ].has_surdef_for( type_0 ) )
                    test_pf( new Surdef( dla_0, () => true, ( val_0, ...args ) => this.call_2( Method.methods[ 'prox_ref' ].call_1( val_0 ), ...args ) ) );
                else if ( Method.methods[ 'prox_val' ].has_surdef_for( type_0 ) )
                    test_pf( new Surdef( dla_0, () => true, ( val_0, ...args ) => this.call_2( Method.methods[ 'prox_val' ].call_1( val_0 ), ...args ) ) );
                else if ( Method.methods[ 'prox_ptr' ].has_surdef_for( type_0 ) )
                    test_pf( new Surdef( dla_0, () => true, ( val_0, ...args ) => this.call_2( Method.methods[ 'prox_ptr' ].call_1( val_0 ), ...args ) ) );
            } else if ( Method.methods[ 'prox_ptr' ].has_surdef_for( type_0 ) ) { // if we are going to modify the value, (currently) only prox_ptr is allowed
                test_pf( new Surdef( dla_0, () => true, ( val_0, ...args ) => this.call_2.apply( this, [ Method.methods[ 'prox_ptr' ].call_1( val_0 ) ].concat( args ) ) ) );
            }

            if ( this.constness[ 1 ] == "c" ) {
                if ( Method.methods[ 'prox_ref' ].has_surdef_for( type_1 ) )
                    test_pf( new Surdef( dla_1, () => true, ( val_0, val_1, ...args ) => this.call_2( val_0, Method.methods[ 'prox_ref' ].call_1( val_1 ), ...args ) ) );
                else if ( Method.methods[ 'prox_val' ].has_surdef_for( type_1 ) )
                    test_pf( new Surdef( dla_1, () => true, ( val_0, val_1, ...args ) => this.call_2( val_0, Method.methods[ 'prox_val' ].call_1( val_1 ), ...args ) ) );
                else if ( Method.methods[ 'prox_ptr' ].has_surdef_for( type_1 ) )
                    test_pf( new Surdef( dla_1, () => true, ( val_0, val_1, ...args ) => this.call_2( val_0, Method.methods[ 'prox_ptr' ].call_1( val_1 ), ...args ) ) );
            } else if ( Method.methods[ 'prox_ptr' ].has_surdef_for( type_1 ) ) { // if we are going to modify the value, (currently) only prox_ptr is allowed
                test_pf( new Surdef( dla_1, () => true, ( val_0, val_1, ...args ) => this.call_2( val_0, Method.methods[ 'prox_ptr' ].call_1( val_1 ), ...args ) ) );
            }
        }

        // summary
        if ( ! for_a_test ) {
            if ( best_surdefs.length == 0 )
                throw new Error( `Impossible to find surdef for '${ this.name }' with types '${ type_0.name }' and '${ type_1.name }'` );
            if ( best_surdefs.length >= 2 )
                throw new Error( `Ambiguous overload for '${ this.name }' with types '${ type_0.name }' and '${ type_1.name }' (pertinence=${ best_surdefs[ 0 ].pertinence }), possibilities are ${ best_surdefs.map( disp_pos ).join( "" ) }` );
        }
        return best_surdefs.length ? best_surdefs[ 0 ] : null;
    }

    find_best_func_3( type_0, type_1, type_2, for_a_test = false ) : Surdef {
        // surdefs
        let best_surdefs = new Array<Surdef>();

        function test_pf( surdef : Surdef ) : void {
            if ( best_surdefs.length == 0 || best_surdefs[ 0 ].pertinence <= surdef.pertinence ) {
                if ( best_surdefs.length && best_surdefs[ 0 ].pertinence < surdef.pertinence )
                    best_surdefs = [];
                best_surdefs.push( surdef );
            }
        }

        // ext surdefs
        for( let s of this.surdefs ) {
            if ( s.test instanceof Array ) {
                if ( s.test.length != 3 )
                    throw new Error( "Wrong test array size" );
                if ( Method.test_arg_surdef( s.test[ 0 ], type_0 ) && Method.test_arg_surdef( s.test[ 1 ], type_1 ) && Method.test_arg_surdef( s.test[ 2 ], type_2 ) )
                    test_pf( s );
            } else if ( s.test( type_0, type_1, type_2 ) )
                test_pf( s );
        }

        // proxy ?
        if ( ! for_a_test ) {
            let dla_0 = 1 + 0.1 / ( 1 + type_0.data_loss_if_prox );
            let dla_1 = 1 + 0.1 / ( 1 + type_1.data_loss_if_prox );
            let dla_2 = 1 + 0.1 / ( 1 + type_2.data_loss_if_prox );
            if ( dla_2 == dla_0 && dla_1 == dla_0 ) {
                dla_2 += 0.00002;
                dla_1 += 0.00001;
            } else if ( dla_1 == dla_0 ) {
                dla_1 += 0.00001;
            } else if ( dla_2 == dla_0 ) {
                dla_2 += 0.00001;
            } else if ( dla_2 == dla_1 ) {
                dla_2 += 0.00001;
            }

            if ( this.constness[ 0 ] == "c" ) {
                if ( Method.methods[ 'prox_ref' ].has_surdef_for( type_0 ) )
                    test_pf( new Surdef( dla_0, () => true, ( val_0, ...args ) => this.call_3.apply( this, [ Method.methods[ 'prox_ref' ].call_1( val_0 ) ].concat( args ) ) ) );
                else if ( Method.methods[ 'prox_val' ].has_surdef_for( type_0 ) )
                    test_pf( new Surdef( dla_0, () => true, ( val_0, ...args ) => this.call_3.apply( this, [ Method.methods[ 'prox_val' ].call_1( val_0 ) ].concat( args ) ) ) );
                else if ( Method.methods[ 'prox_ptr' ].has_surdef_for( type_0 ) )
                    test_pf( new Surdef( dla_0, () => true, ( val_0, ...args ) => this.call_3.apply( this, [ Method.methods[ 'prox_ptr' ].call_1( val_0 ) ].concat( args ) ) ) );
            } else if ( Method.methods[ 'prox_ptr' ].has_surdef_for( type_0 ) ) { // if we are going to modify the value, (currently) only prox_ptr is allowed
                test_pf( new Surdef( dla_0, () => true, ( val_0, ...args ) => this.call_3.apply( this, [ Method.methods[ 'prox_ptr' ].call_1( val_0 ) ].concat( args ) ) ) );
            }

            if ( this.constness[ 1 ] == "c" ) {
                if ( Method.methods[ 'prox_ref' ].has_surdef_for( type_1 ) )
                    test_pf( new Surdef( dla_1, () => true, ( val_0, val_1, ...args ) => this.call_3.apply( this, [ val_0, Method.methods[ 'prox_ref' ].call_1( val_1 ) ].concat( args ) ) ) );
                else if ( Method.methods[ 'prox_val' ].has_surdef_for( type_1 ) )
                    test_pf( new Surdef( dla_1, () => true, ( val_0, val_1, ...args ) => this.call_3.apply( this, [ val_0, Method.methods[ 'prox_val' ].call_1( val_1 ) ].concat( args ) ) ) );
                else if ( Method.methods[ 'prox_ptr' ].has_surdef_for( type_1 ) )
                    test_pf( new Surdef( dla_1, () => true, ( val_0, val_1, ...args ) => this.call_3.apply( this, [ val_0, Method.methods[ 'prox_ptr' ].call_1( val_1 ) ].concat( args ) ) ) );
            } else if ( Method.methods[ 'prox_ptr' ].has_surdef_for( type_1 ) ) { // if we are going to modify the value, (currently) only prox_ptr is allowed
                test_pf( new Surdef( dla_1, () => true, ( val_0, val_1, ...args ) => this.call_3.apply( this, [ val_0, Method.methods[ 'prox_ptr' ].call_1( val_1 ) ].concat( args ) ) ) );
            }

            if ( this.constness[ 2 ] == "c" ) {
                if ( Method.methods[ 'prox_ref' ].has_surdef_for( type_2 ) )
                    test_pf( new Surdef( dla_2, () => true, ( val_0, val_1, val_2, ...args ) => this.call_3.apply( this, [ val_0, val_1, Method.methods[ 'prox_ref' ].call_1( val_2 ) ].concat( args ) ) ) );
                else if ( Method.methods[ 'prox_val' ].has_surdef_for( type_2 ) )
                    test_pf( new Surdef( dla_2, () => true, ( val_0, val_1, val_2, ...args ) => this.call_3.apply( this, [ val_0, val_1, Method.methods[ 'prox_val' ].call_1( val_2 ) ].concat( args ) ) ) );
                else if ( Method.methods[ 'prox_ptr' ].has_surdef_for( type_2 ) )
                    test_pf( new Surdef( dla_2, () => true, ( val_0, val_1, val_2, ...args ) => this.call_3.apply( this, [ val_0, val_1, Method.methods[ 'prox_ptr' ].call_1( val_2 ) ].concat( args ) ) ) );
            } else if ( Method.methods[ 'prox_ptr' ].has_surdef_for( type_2 ) ) { // if we are going to modify the value, (currently) only prox_ptr is allowed
                test_pf( new Surdef( dla_2, () => true, ( val_0, val_1, val_2, ...args ) => this.call_3.apply( this, [ val_0, val_1, Method.methods[ 'prox_ptr' ].call_1( val_2 ) ].concat( args ) ) ) );
            }
        }

        // summary
        if ( ! for_a_test ) {
            if ( best_surdefs.length == 0 )
                throw new Error( `Impossible to find surdef for '${ this.name }' with types '${ type_0.name }', '${ type_1.name }' and '${ type_2.name }'` );
            if ( best_surdefs.length >= 2 )
                throw new Error( `Ambiguous overload for '${ this.name }' with types '${ type_0.name }', '${ type_1.name }' and '${ type_2.name }' (pertinence=${ best_surdefs[ 0 ].pertinence }), possibilities are ${ best_surdefs.map( disp_pos ).join( "" ) }` );
        }
        return best_surdefs.length ? best_surdefs[ 0 ] : null;
    }

    function_list : Array< ( ...any ) => any >;
    function_map  : Map<string,any>;
    nb_tabu_types : number;
    nb_rp_args    : number;
    constness     : string;
    surdefs       : Array<Surdef>;
    name          : string;
}

// declaration of methods
Method.decl( "to_String"           , "c"   ); // returns a native String
Method.decl( "to_Number"           , "c"   ); // returns a native Number
Method.decl( "rp_structure"        , "c"   ); // returns a native Number
Method.decl( "to_Array"            , "c"   ); // returns a native Array
Method.decl( "to_Inode"            , "c"   ); // returns an Inode object
Method.decl( "prox_ptr"            , "r"   ); // returns a Variable, which can be modified
Method.decl( "prox_ref"            , "c"   ); // returns a Variable, which should not be modified
Method.decl( "prox_val"            , "c"   ); // returns a computed Variable
Method.decl( "copy"                , "c"   ); // returns a Variable

// OT
Method.decl( "make_ot_wrapper"     , "c"   ); // returns an OtWrapper
Method.decl( "new_ot_merger"       , "c"   ); // returns an OtMerger
Method.decl( "pull_ot_changes"     , "r"   ); // takes a BinaryWriter as arg
Method.dslf( "ot_merge"            , 2     ); // merge two objects into one
Method.decl( "get_patch_type"      , "c"   ); // return a number representing the patch type (e.g. OtWrapperAtomic[ObjectId_Inode]...)
Method.decl( "cmp_patch_type"      , "c"   ); // return a number representing the patch type (e.g. OtWrapperAtomic[ObjectId_Inode]...)

Method.decl( "write_to"            , "c"   ); // takes a BinaryWriter as arg
Method.decl( "self_loc_to_glo"     , "r"   ); // + DevID + UsrId
Method.decl( "self_glo_to_loc"     , "r"   ); // + DevID + UsrId
Method.decl( "new_cursor"          , "r"   ); // return a loc_id (PI64)
Method.decl( "get_cursor"          , "c"   ); // + DevID + loc_id (PI64). Return [{beg:number,end:number,foc:number}]
Method.decl( "set_cursor"          , "c"   ); // + DevID + loc_id (PI64) + [{beg:number,end:number,foc:number}]
Method.decl( "rem_cursor"          , "r"   ); // + DevID + loc_id (PI64)
Method.decl( "get_cursor_ids"      , "c"   );
Method.decl( "mod_select"          , "rc"  );
Method.decl( "rem_item"            , "rc"  ); // remove item (e.g. using a key)
Method.decl( "obj_type"            , "c"   ); // variable type (ex: GenString -> String)
Method.decl( "key_type"            , "c"   );
Method.decl( "val_type"            , "c"   );

Method.decl( "inf_immediate"       , "cc"  );
Method.decl( "sup_immediate"       , "cc"  );
Method.decl( "equ_immediate"       , "cc"  );

Method.decl( "get_size"            , "c"   ); // return a Variable (probably a Number)

Method.dstd( "select"              , 2     );

Method.dslf( "not"                 , 1     );
Method.dslf( "neg"                 , 1     );

Method.dslf( "set"                 , 2     );
Method.dslf( "add"                 , 2     );
Method.dslf( "sub"                 , 2     );
Method.dslf( "mul"                 , 2     );
Method.dslf( "div"                 , 2     );

Method.dstd( "is_pos"              , 1     ); // true if strictly positive
Method.dstd( "is_neg"              , 1     ); // true if strictly negative
Method.dstd( "is_eqz"              , 1     ); // true if null
Method.dstd( "is_equ"              , 2     ); // true if equal
Method.dstd( "is_inf"              , 2     );
Method.dstd( "is_infeq"            , 2     );

Method.dslf( "remove"              , 3     );
Method.dslf( "insert"              , 3     );

// changes
Method.dslf( "on_change"           , 1     ); // return an id (type PatchId), that can be used to remove the callback
Method.decl( "sig_change"          , "c"   ); //
Method.dslf( "rm_on_change"        , 1     ); // takes the id (type PatchId) that has been used for `on_change`
Method.dslf( "on_change_par"       , 1     ); // add a parent object for ob_change (if this is changed, sig_change will be called on this parent). Optionnal argument: something that will be sent to parent sig_change

// sym
Method.decl( "common_symbolic_data", "c"   );
Method.decl( "symbolic_children"   , "c"   ); // return an array with the symbolic children
Method.decl( "graphviz_repr"       , "c"   ); // return a string
Method.decl( "get_graph_data"      , "c"   ); // graph_data, children, id
Method.decl( "is_symbolic"         , "c"   ); //
Method.decl( "ww_launch"           , "c"   ); // called from the scheduler WebWorker to launch the query
Method.decl( "then"                , "c"   ); // executed a function when value is known

// db
Method.decl( "bind_by_path"        , "c"   ); // path , db, category, load_only_once
Method.decl( "bind_by_object_id"   , "c"   ); // inode, db, category, load_only_once

// rights
Method.decl( "get_possible_rights" , "c"   );
Method.decl( "get_groups_in_acl"   , "c"   );
Method.decl( "get_users_in_acl"    , "c"   );
Method.decl( "get_grp_rights"      , "c"   );
Method.decl( "get_usr_rights"      , "c"   );
Method.decl( "add_grp_right"       , "r"   );
Method.decl( "add_usr_right"       , "r"   );
Method.decl( "rem_grp_right"       , "r"   );
Method.decl( "rem_usr_right"       , "r"   );

declare var require: ( string ) => any;
require( "./DefaultMethods" );

import Rp from "./Rp";
