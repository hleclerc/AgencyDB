import methods from "./methods"
import Surdef  from "./Surdef"
import Rp      from "./Rp"

///
export default
class Method {
    static AVOID_BO_TRANS = 1;
    static AVOID_PROX = 2;

    constructor( name: string, ok_sym: boolean ) {
        const ind = name.indexOf( "__" );
        this.nb_rp_args = name.length - ind - 2;
        
        this.base_name  = name.slice( 0, ind );
        this.pattern    = name.slice( ind + 2 );
        this.name       = name;
        this.ok_sym     = ok_sym;

        this.resize( 8 );
    }

    resize( new_nb_tabu_types : number ) : void {
        if ( this.nb_tabu_types < new_nb_tabu_types ) {
            switch ( this.nb_rp_args ) {
                case 1: {
                    let unk = ( a: Rp, ...args ) => {
                        let type_0 = Rp.type_num_map[ a.type_num ];
                        a.type_off = type_0.potentially_updated_type_off()
                        this.function_list[ a.type_off ] = this.find_best_func_1( type_0, 0 ).func;
                        return this.function_list[ a.type_off ].call( null, a, ...args );
                    }

                    while ( this.function_list.length < new_nb_tabu_types )
                        this.function_list.push( unk );
                    break;
                } case 2: {
                    let unk = ( a: Rp, b: Rp, ...args ) => {
                        let type_0 = Rp.type_num_map[ a.type_num ];
                        let type_1 = Rp.type_num_map[ b.type_num ];
                        a.type_off = type_0.potentially_updated_type_off();
                        b.type_off = type_1.potentially_updated_type_off();
                        this.function_list[ a.type_off * this.nb_tabu_types + b.type_off ] = this.find_best_func_2( type_0, type_1, 0 ).func;
                        return this.function_list[ a.type_off * this.nb_tabu_types + b.type_off ].call( null, a, b, ...args );
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
                    let unk = ( a: Rp, b: Rp, c: Rp, ...args ) => {
                        let type_0 = Rp.type_num_map[ a.type_num ];
                        let type_1 = Rp.type_num_map[ b.type_num ];
                        let type_2 = Rp.type_num_map[ c.type_num ];
                        a.type_off = type_0.potentially_updated_type_off()
                        b.type_off = type_1.potentially_updated_type_off()
                        c.type_off = type_2.potentially_updated_type_off()
                        let s = `${ a.type_off } ${ b.type_off } ${ c.type_off}`;
                        this.function_list[ s ] = this.find_best_func_3( type_0, type_1, type_2, 0 ).func;
                        return this.function_list[ s ].call( null, a, b, c, ...args );
                    }

                    if ( ! this.nb_tabu_types )
                        this.function_list[ '0 0 0' ] = unk;
                    break;
                } case 4: {
                    let unk = ( a: Rp, b: Rp, c: Rp, d: Rp, ...args ) => {
                        let type_0 = Rp.type_num_map[ a.type_num ];
                        let type_1 = Rp.type_num_map[ b.type_num ];
                        let type_2 = Rp.type_num_map[ c.type_num ];
                        let type_3 = Rp.type_num_map[ d.type_num ];
                        a.type_off = type_0.potentially_updated_type_off()
                        b.type_off = type_1.potentially_updated_type_off()
                        c.type_off = type_2.potentially_updated_type_off()
                        d.type_off = type_3.potentially_updated_type_off()
                        let s = `${ a.type_off } ${ b.type_off } ${ c.type_off} ${ d.type_off}`;
                        this.function_list[ s ] = this.find_best_func_4( type_0, type_1, type_2, type_3, 0 ).func;
                        return this.function_list[ s ].call( null, a, b, c, d, ...args );
                    }

                    if ( ! this.nb_tabu_types )
                        this.function_list[ '0 0 0 0' ] = unk;
                    break;
                } default:
                    throw new Error( "TODO" );
            }
            this.nb_tabu_types = new_nb_tabu_types
        }
    }

    /** test can be a raw function ( (type0,type1,...) => boolean ) or a list to be applied for each argument
     * items in the list can be a raw string (test if procedure is present)
     * or a function (taking the type as parameter, returning bool), 
     * or a string representing a method name (like e.g. add__bb), to test if the method is defined for this type    
     */
    add_surdef( pertinence: number, test: ( (...any) => boolean ) | Array<any>, func: (...any) => any ) : void {
        this.surdefs.push( new Surdef( pertinence, test, func ) );
    }

    call_1( a: Rp, ...args ) {
        return this.function_list[ a.type_off ].call( null, a, ...args );
    }

    call_2( a: Rp, b: Rp, ...args ) {
        return this.function_list[ a.type_off * this.nb_tabu_types + b.type_off ].call( null, a, b, ...args );
    }

    call_3( a: Rp, b: Rp, c: Rp, ...args ) {
        let s = `${ a.type_off } ${ b.type_off } ${ c.type_off}`;
        if ( this.function_list[ s ] == null )
            this.function_list[ s ] = this.function_list[ '0 0 0' ];
        return this.function_list[ s ].call( null, a, b, c, ...args );
    }

    call_4( a: Rp, b: Rp, c: Rp, d: Rp, ...args ) {
        let s = `${ a.type_off } ${ b.type_off } ${ c.type_off} ${ d.type_off}`;
        if ( this.function_list[ s ] == null )
            this.function_list[ s ] = this.function_list[ `0 0 0 0` ];
        return this.function_list[ s ].call( null, a, b, c, d, ...args );
    }

    call_1s( a: VarAnc, ...args ) {
        if ( Method.int_call_s ) Method.int_call_s( a );
        return this.call_1( a.rp, ...args );
    }

    call_2s( a: VarAnc, b: Rp, ...args ) {
        if ( Method.int_call_s ) Method.int_call_s( a );
        return this.call_2( a.rp, b, ...args );
    }

    call_3s( a: VarAnc, b: Rp, c: Rp, ...args ) {
        if ( Method.int_call_s ) Method.int_call_s( a );
        return this.call_3( a.rp, b, c, ...args );
    }

    call_4s( a: VarAnc, b: Rp, c: Rp, d: Rp, ...args ) {
        if ( Method.int_call_s ) Method.int_call_s( a );
        return this.call_4( a.rp, b, c, d, ...args );
    }

    /** version for any number of rp args */
    call_gen( ...args ) {
        switch ( this.nb_rp_args ) {
            case 1: return ( this.call_1 as any )( ...args );
            case 2: return ( this.call_2 as any )( ...args );
            case 3: return ( this.call_3 as any )( ...args );
            case 4: return ( this.call_4 as any )( ...args );
            default: throw new Error( "TODO" );
        }
    }

    surdef_for( ...args ) : Surdef {
        switch ( this.nb_rp_args ) {
            case 1: return this.find_best_func_1( args[ 0 ], 1 );
            case 2: return this.find_best_func_2( args[ 0 ], args[ 1 ], 1 );
            case 3: return this.find_best_func_3( args[ 0 ], args[ 1 ], args[ 2 ], 1 );
            case 4: return this.find_best_func_4( args[ 0 ], args[ 1 ], args[ 2 ], args[ 3 ], 1 );
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
            return methods[ test ].has_surdef_for( type );
        return test.__Rp_type__ ? test == type : test( type );
    }

    /** for_a_test: 1 => look if it's possible (which forbid proxies). 2 => look if we have the exact function */
    find_best_func_1( type_0, for_a_test: number ) : Surdef {
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
                    throw new Error( `Wrong test array size (in ${ this.name })` );
                if ( Method.test_arg_surdef( s.test[ 0 ], type_0 ) )
                    test_pf( s );
            } else if ( Method.test_arg_surdef( s.test, type_0 ) )
                test_pf( s );
        }

        // method in type_0 ?
        if ( type_0.prototype[ this.name ] != null )
            test_pf( new Surdef( 2, null, ( val_0: Rp, ...args ) => type_0.prototype[ this.name ].apply( val_0, args ) ) );
 
        // proxy ?
        if ( for_a_test == 0 && [ "rp_structure__b", "prox_ptr__b", "prox_ref__b", "prox_val__b", "prox_ptr__o", "prox_ref__o", "prox_val__o" ].indexOf( this.name ) < 0 ) {
            const pref_0 = methods[ 'prox_ref__' + this.pattern[ 0 ] ], sref_0 = pref_0 ? pref_0.find_best_func_1( type_0, 1 ) : null;
            const pval_0 = methods[ 'prox_val__' + this.pattern[ 0 ] ], sval_0 = pval_0 ? pval_0.find_best_func_1( type_0, 1 ) : null;
            const pptr_0 = methods[ 'prox_ptr__' + this.pattern[ 0 ] ], sptr_0 = pptr_0 ? pptr_0.find_best_func_1( type_0, 1 ) : null;
            const thb = methods[ this.base_name + '__b' ];
            const tho = methods[ this.base_name + '__o' ];

            if ( sref_0 ) 
                test_pf( new Surdef( 1, null, ( val_0: Rp, ...args ) => thb.call_1( sref_0.func( val_0 ), ...args ) ) );
            else if ( sval_0 )
                test_pf( new Surdef( 1, null, ( val_0: Rp, ...args ) => tho.call_1( sval_0.func( val_0 ), ...args ) ) );
            else if ( sptr_0 )
                test_pf( new Surdef( 1, null, ( val_0: Rp, ...args ) => thb.call_1( sptr_0.func( val_0 ), ...args ) ) );

            // if ( this.pattern[ 0 ] == 's' ) {
            //     if ( sptr_0 ) { // if we are going to modify the value, (currently) only prox_ptr is allowed
            //         test_pf( new Surdef( 1, null, ( val_0, val_1, ...args ) => {
            //             let ptr = sptr_0.func( val_0 );
            //             methods[ 'set__so' ].call_2s( ptr, this.call_2( ptr, val_1, ...args ) );
            //             return val_0;
            //         } ) );
            //     }
            // }
        }

        // __o, __s, ... ?
        if ( for_a_test <= 1 ) {
            let pert = 0;
            for( let variant of [ "s", "o", "b" ] ) {
                if ( variant == this.pattern )
                    continue;
                if ( type_0.watch_self_op && ( this.pattern[ 0 ] == "s" || variant[ 0 ] == "s" ) )
                    continue;
                const m = methods[ this.base_name + '__' + variant + this.extr ];
                if ( m ) {
                    const s = m.find_best_func_1( type_0, 2 );
                    if ( s ) {
                        const bo_0 = this.pattern[ 0 ] == 'b' && ( variant[ 0 ] == 's' || variant[ 0 ] == 'o' ); 
                        switch ( Number( bo_0 ) ) {
                        case 0: test_pf( new Surdef( pert += 0.0001, null, s.func                                                                 ) ); break;
                        case 1: test_pf( new Surdef( pert += 0.0001, null, ( a: Rp, ...args ) => s.func( methods[ 'copy' ].call_1( a ), ...args ) ) ); break;
                        }
                    }
                }
            }
        }

        // plugins
        for( let plugin of Method.plugins )
            plugin( test_pf, for_a_test, this, type_0 );

        // summary
        if ( for_a_test == 0 ) {
            if ( best_surdefs.length == 0 )
                throw new Error( `impossible to find surdef for '${ this.name }' (with type '${ type_0.name }')` );
            if ( best_surdefs.length >= 2 )
                throw new Error( `Ambiguous overload for '${ this.name }' with type '${ type_0.name }' and pertinence ${ best_surdefs[ 0 ].pertinence }, possibilities are ${ best_surdefs.map( x => x.disp_pos() ).join( "" ) }` );
        }
        return best_surdefs.length ? best_surdefs[ 0 ] : null;
    }


    find_best_func_2( type_0, type_1, for_a_test: number ) : Surdef {
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
                    throw new Error( `Wrong test array size (in ${ this.name })` );
                if ( Method.test_arg_surdef( s.test[ 0 ], type_0 ) && Method.test_arg_surdef( s.test[ 1 ], type_1 ) )
                    test_pf( s );
            } else if ( s.test( type_0, type_1 ) )
                test_pf( s );
        }

        // proxy ?
        if ( for_a_test == 0 ) {
            let dla_0 = 1 + 0.1 / ( 1 + type_0.data_loss_if_prox );
            let dla_1 = 1 + 0.1 / ( 1 + type_1.data_loss_if_prox );
            if ( dla_1 == dla_0 )
                dla_1 += 0.00001;

            // test arg 0
            const pref_0 = methods[ 'prox_ref__' + this.pattern[ 0 ] ];
            const pval_0 = methods[ 'prox_val__' + this.pattern[ 0 ] ];
            const pptr_0 = methods[ 'prox_ptr__' + this.pattern[ 0 ] ];
            const sref_0 = pref_0 ? pref_0.find_best_func_1( type_0, 1 ) : null;
            const sval_0 = pval_0 ? pval_0.find_best_func_1( type_0, 1 ) : null;
            const sptr_0 = pptr_0 ? pptr_0.find_best_func_1( type_0, 1 ) : null;
            const thbx = methods[ this.base_name + '__b' + this.pattern[ 1 ] ];
            const thox = methods[ this.base_name + '__o' + this.pattern[ 1 ] ];

            if ( sref_0 ) 
                test_pf( new Surdef( dla_0, null, ( val_0: Rp, val_1: Rp, ...args ) => thbx.call_2( sref_0.func( val_0 ), val_1, ...args ) ) );
            else if ( sval_0 )
                test_pf( new Surdef( dla_0, null, ( val_0: Rp, val_1: Rp, ...args ) => thox.call_2( sval_0.func( val_0 ), val_1, ...args ) ) );
            else if ( sptr_0 )
                test_pf( new Surdef( dla_0, null, ( val_0: Rp, val_1: Rp, ...args ) => thbx.call_2( sptr_0.func( val_0 ), val_1, ...args ) ) );

            // if ( this.pattern[ 0 ] == 's' ) {
            //     if ( sptr_0 ) { // if we are going to modify the value, (currently) only prox_ptr is allowed
            //         test_pf( new Surdef( 1, null, ( val_0, val_1, ...args ) => {
            //             let ptr = sptr_0.func( val_0 );
            //             methods[ 'set__so' ].call_2s( ptr, this.call_2( ptr, val_1, ...args ) );
            //             return val_0;
            //         } ) );
            //     }
            // }

            // test arg 1
            const pref_1 = methods[ 'prox_ref__' + this.pattern[ 1 ] ];
            const pval_1 = methods[ 'prox_val__' + this.pattern[ 1 ] ];
            const pptr_1 = methods[ 'prox_ptr__' + this.pattern[ 1 ] ];
            const sref_1 = pref_1 ? pref_1.find_best_func_1( type_1, 1 ) : null;
            const sval_1 = pval_1 ? pval_1.find_best_func_1( type_1, 1 ) : null;
            const sptr_1 = pptr_1 ? pptr_1.find_best_func_1( type_1, 1 ) : null;
            const thxb = methods[ this.base_name + '__' + this.pattern[ 0 ] + 'b' ];
            const thxo = methods[ this.base_name + '__' + this.pattern[ 0 ] + 'o' ];

            if ( sref_1 ) 
                test_pf( new Surdef( dla_1, null, ( val_0: Rp, val_1: Rp, ...args ) => thxb.call_2( val_0, sref_0.func( val_1 ), ...args ) ) );
            else if ( sval_1 )
                test_pf( new Surdef( dla_1, null, ( val_0: Rp, val_1: Rp, ...args ) => thxo.call_2( val_0, sval_0.func( val_1 ), ...args ) ) );
            else if ( sptr_1 )
                test_pf( new Surdef( dla_1, null, ( val_0: Rp, val_1: Rp, ...args ) => thxb.call_2( val_0, sptr_0.func( val_1 ), ...args ) ) );
        }

        // __xo, __sx, ... ?
        if ( for_a_test <= 1 ) {
            let pert = 0;
            for( let variant of [ "so", "sb", "oo", "ob", "bo", "bb" ] ) {
                if ( variant == this.pattern )
                    continue;
                if ( type_0.watch_self_op && ( this.pattern[ 0 ] == "s" || variant[ 0 ] == "s" ) )
                    continue;
                const m = methods[ this.base_name + '__' + variant + this.extr ];
                if ( m ) {
                    const s = m.find_best_func_2( type_0, type_1, 2 );
                    if ( s ) {
                        const bo_0 = this.pattern[ 0 ] == 'b' && ( variant[ 0 ] == 's' || variant[ 0 ] == 'o' ); 
                        const bo_1 = this.pattern[ 1 ] == 'b' && ( variant[ 1 ] == 's' || variant[ 1 ] == 'o' ); 
                        switch ( Number( bo_0 ) + 2 * Number( bo_1 ) ) {
                        case 0: test_pf( new Surdef( pert += 0.0001, null, s.func                                                                                                       ) ); break;
                        case 1: test_pf( new Surdef( pert += 0.0001, null, ( a: Rp, b: Rp, ...args ) => s.func( methods[ 'copy' ].call_1( a ), b                            , ...args ) ) ); break;
                        case 2: test_pf( new Surdef( pert += 0.0001, null, ( a: Rp, b: Rp, ...args ) => s.func( a                            , methods[ 'copy' ].call_1( b ), ...args ) ) ); break;
                        case 3: test_pf( new Surdef( pert += 0.0001, null, ( a: Rp, b: Rp, ...args ) => s.func( methods[ 'copy' ].call_1( a ), methods[ 'copy' ].call_1( b ), ...args ) ) ); break;
                        }
                    }
                }
            }
        }

        // plugins
        for( let plugin of Method.plugins )
            plugin( test_pf, for_a_test, this, type_0, type_1 );

        // summary
        if ( for_a_test == 0 ) {
            if ( best_surdefs.length == 0 )
                throw new Error( `Impossible to find surdef for '${ this.name }' with types '${ type_0.name }' and '${ type_1.name }'` );
            if ( best_surdefs.length >= 2 )
                throw new Error( `Ambiguous overload for '${ this.name }' with types '${ type_0.name }' and '${ type_1.name }' (pertinence=${ best_surdefs[ 0 ].pertinence }), possibilities are ${ best_surdefs.map( x => x.disp_pos() ).join( "" ) }` );
        }
        return best_surdefs.length ? best_surdefs[ 0 ] : null;
    }

    find_best_func_3( type_0, type_1, type_2, for_a_test: number ) : Surdef {
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
                    throw new Error( `Wrong test array size (in ${ this.name })` );
                if ( Method.test_arg_surdef( s.test[ 0 ], type_0 ) && Method.test_arg_surdef( s.test[ 1 ], type_1 ) && Method.test_arg_surdef( s.test[ 2 ], type_2 ) )
                    test_pf( s );
            } else if ( s.test( type_0, type_1, type_2 ) )
                test_pf( s );
        }

        // proxy ?
        if ( for_a_test == 0 ) {
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

            // test arg 0
            const pref_0 = methods[ 'prox_ref__' + this.pattern[ 0 ] ];
            const pval_0 = methods[ 'prox_val__' + this.pattern[ 0 ] ];
            const pptr_0 = methods[ 'prox_ptr__' + this.pattern[ 0 ] ];
            const sref_0 = pref_0 ? pref_0.find_best_func_1( type_0, 1 ) : null;
            const sval_0 = pval_0 ? pval_0.find_best_func_1( type_0, 1 ) : null;
            const sptr_0 = pptr_0 ? pptr_0.find_best_func_1( type_0, 1 ) : null;
            const thbxx = methods[ this.base_name + '__b' + this.pattern[ 1 ] + this.pattern[ 2 ] ];
            const thoxx = methods[ this.base_name + '__o' + this.pattern[ 1 ] + this.pattern[ 2 ] ];

            if ( sref_0 ) 
                test_pf( new Surdef( dla_0, null, ( val_0: Rp, val_1: Rp, val_2: Rp, ...args ) => thbxx.call_3( sref_0.func( val_0 ), val_1, val_2, ...args ) ) );
            else if ( sval_0 )
                test_pf( new Surdef( dla_0, null, ( val_0: Rp, val_1: Rp, val_2: Rp, ...args ) => thoxx.call_3( sval_0.func( val_0 ), val_1, val_2, ...args ) ) );
            else if ( sptr_0 )
                test_pf( new Surdef( dla_0, null, ( val_0: Rp, val_1: Rp, val_2: Rp, ...args ) => thbxx.call_3( sptr_0.func( val_0 ), val_1, val_2, ...args ) ) );

            // if ( this.pattern[ 0 ] == 's' ) {
            //     if ( sptr_0 ) { // if we are going to modify the value, (currently) only prox_ptr is allowed
            //         test_pf( new Surdef( 1, null, ( val_0, val_1, ...args ) => {
            //             let ptr = sptr_0.func( val_0 );
            //             methods[ 'set__so' ].call_2s( ptr, this.call_2( ptr, val_1, ...args ) );
            //             return val_0;
            //         } ) );
            //     }
            // }

            // test arg 1
            const pref_1 = methods[ 'prox_ref__' + this.pattern[ 1 ] ];
            const pval_1 = methods[ 'prox_val__' + this.pattern[ 1 ] ];
            const pptr_1 = methods[ 'prox_ptr__' + this.pattern[ 1 ] ];
            const sref_1 = pref_1 ? pref_1.find_best_func_1( type_1, 1 ) : null;
            const sval_1 = pval_1 ? pval_1.find_best_func_1( type_1, 1 ) : null;
            const sptr_1 = pptr_1 ? pptr_1.find_best_func_1( type_1, 1 ) : null;
            const thxbx = methods[ this.base_name + '__' + this.pattern[ 0 ] + 'b' + this.pattern[ 2 ] ];
            const thxox = methods[ this.base_name + '__' + this.pattern[ 0 ] + 'o' + this.pattern[ 2 ] ];

            if ( sref_1 ) 
                test_pf( new Surdef( dla_1, null, ( val_0: Rp, val_1: Rp, val_2: Rp, ...args ) => thxbx.call_3( val_0, sref_0.func( val_1 ), val_2, ...args ) ) );
            else if ( sval_1 )
                test_pf( new Surdef( dla_1, null, ( val_0: Rp, val_1: Rp, val_2: Rp, ...args ) => thxox.call_3( val_0, sval_0.func( val_1 ), val_2, ...args ) ) );
            else if ( sptr_1 )
                test_pf( new Surdef( dla_1, null, ( val_0: Rp, val_1: Rp, val_2: Rp, ...args ) => thxbx.call_3( val_0, sptr_0.func( val_1 ), val_2, ...args ) ) );

            // test arg 2
            const pref_2 = methods[ 'prox_ref__' + this.pattern[ 2 ] ];
            const pval_2 = methods[ 'prox_val__' + this.pattern[ 2 ] ];
            const pptr_2 = methods[ 'prox_ptr__' + this.pattern[ 2 ] ];
            const sref_2 = pref_2 ? pref_2.find_best_func_1( type_2, 1 ) : null;
            const sval_2 = pval_2 ? pval_2.find_best_func_1( type_2, 1 ) : null;
            const sptr_2 = pptr_2 ? pptr_2.find_best_func_1( type_2, 1 ) : null;
            const thxxb = methods[ this.base_name + '__' + this.pattern[ 0 ] + this.pattern[ 1 ] + 'b' ];
            const thxxo = methods[ this.base_name + '__' + this.pattern[ 0 ] + this.pattern[ 1 ] + 'o' ];

            if ( sref_2 ) 
                test_pf( new Surdef( dla_2, null, ( val_0: Rp, val_1: Rp, val_2: Rp, ...args ) => thxbx.call_3( val_0, val_1, sref_0.func( val_2 ), ...args ) ) );
            else if ( sval_2 )
                test_pf( new Surdef( dla_2, null, ( val_0: Rp, val_1: Rp, val_2: Rp, ...args ) => thxox.call_3( val_0, val_1, sval_0.func( val_2 ), ...args ) ) );
            else if ( sptr_2 )
                test_pf( new Surdef( dla_2, null, ( val_0: Rp, val_1: Rp, val_2: Rp, ...args ) => thxbx.call_3( val_0, val_1, sptr_0.func( val_2 ), ...args ) ) );
        }

        // __xo, __sx, ... ?
        if ( for_a_test <= 1 ) {
            let pert = 0;
            for( let variant of [ "soo", "sbo", "sob", "sbb", "ooo", "oob", "obo", "obb", "boo", "bob", "bbo", "bbb" ] ) {
                if ( variant == this.pattern )
                    continue;
                if ( type_0.watch_self_op && ( this.pattern[ 0 ] == "s" || variant[ 0 ] == "s" ) )
                    continue;
                const m = methods[ this.base_name + '__' + variant + this.extr ];
                if ( m ) {
                    const s = m.find_best_func_3( type_0, type_1, type_2, 2 );
                    if ( s ) {
                        pert += 0.0001;
                        const bo_0 = this.pattern[ 0 ] == 'b' && ( variant[ 0 ] == 's' || variant[ 0 ] == 'o' ); 
                        const bo_1 = this.pattern[ 1 ] == 'b' && ( variant[ 1 ] == 's' || variant[ 1 ] == 'o' ); 
                        const bo_2 = this.pattern[ 2 ] == 'b' && ( variant[ 2 ] == 's' || variant[ 2 ] == 'o' ); 
                        switch ( Number( bo_0 ) + 2 * Number( bo_1 ) + 4 * Number( bo_2 ) ) {
                        case 0: test_pf( new Surdef( pert, null, s.func                                                                                                                                             ) ); break;
                        case 1: test_pf( new Surdef( pert, null, ( a: Rp, b: Rp, c: Rp, ...args ) => s.func( methods[ 'copy' ].call_1( a ), b                            , c                            , ...args ) ) ); break;
                        case 2: test_pf( new Surdef( pert, null, ( a: Rp, b: Rp, c: Rp, ...args ) => s.func( a                            , methods[ 'copy' ].call_1( b ), c                            , ...args ) ) ); break;
                        case 3: test_pf( new Surdef( pert, null, ( a: Rp, b: Rp, c: Rp, ...args ) => s.func( methods[ 'copy' ].call_1( a ), methods[ 'copy' ].call_1( b ), c                            , ...args ) ) ); break;
                        case 4: test_pf( new Surdef( pert, null, ( a: Rp, b: Rp, c: Rp, ...args ) => s.func( methods[ 'copy' ].call_1( a ), b                            , methods[ 'copy' ].call_1( c ), ...args ) ) ); break;
                        case 5: test_pf( new Surdef( pert, null, ( a: Rp, b: Rp, c: Rp, ...args ) => s.func( methods[ 'copy' ].call_1( a ), b                            , methods[ 'copy' ].call_1( c ), ...args ) ) ); break;
                        case 6: test_pf( new Surdef( pert, null, ( a: Rp, b: Rp, c: Rp, ...args ) => s.func( a                            , methods[ 'copy' ].call_1( b ), methods[ 'copy' ].call_1( c ), ...args ) ) ); break;
                        case 7: test_pf( new Surdef( pert, null, ( a: Rp, b: Rp, c: Rp, ...args ) => s.func( methods[ 'copy' ].call_1( a ), methods[ 'copy' ].call_1( b ), methods[ 'copy' ].call_1( c ), ...args ) ) ); break;
                        }
                    }
                }
            }
        }

        // plugins
        for( let plugin of Method.plugins )
            plugin( test_pf, for_a_test, this, type_0, type_1, type_2 );

        // summary
        if ( for_a_test == 0 ) {
            if ( best_surdefs.length == 0 )
                throw new Error( `Impossible to find surdef for '${ this.name }' with types '${ type_0.name }', '${ type_1.name }' and '${ type_2.name }'` );
            if ( best_surdefs.length >= 2 )
                throw new Error( `Ambiguous overload for '${ this.name }' with types '${ type_0.name }', '${ type_1.name }' and '${ type_2.name }' (pertinence=${ best_surdefs[ 0 ].pertinence }), possibilities are ${ best_surdefs.map( x => x.disp_pos() ).join( "" ) }` );
        }
        return best_surdefs.length ? best_surdefs[ 0 ] : null;
    }

    find_best_func_4( type_0, type_1, type_2, type_3, for_a_test: number ) : Surdef {
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
                if ( s.test.length != 4 )
                    throw new Error( `Wrong test array size (in ${ this.name })` );
                if ( Method.test_arg_surdef( s.test[ 0 ], type_0 ) && Method.test_arg_surdef( s.test[ 1 ], type_1 ) && Method.test_arg_surdef( s.test[ 2 ], type_2 ) && Method.test_arg_surdef( s.test[ 3 ], type_3 ) )
                    test_pf( s );
            } else if ( s.test( type_0, type_1, type_2, type_3 ) )
                test_pf( s );
        }

        // proxy ?
        // if ( ! for_a_test ) {
        //     let dla_0 = 1 + 0.1 / ( 1 + type_0.data_loss_if_prox );
        //     let dla_1 = 1 + 0.1 / ( 1 + type_1.data_loss_if_prox );
        //     let dla_2 = 1 + 0.1 / ( 1 + type_2.data_loss_if_prox );
        //     let dla_3 = 1 + 0.1 / ( 1 + type_3.data_loss_if_prox );
        //     if ( dla_2 == dla_0 && dla_1 == dla_0 && dla_1 == dla_0 ) {
        //         dla_3 += 0.00002;
        //         dla_2 += 0.00002;
        //         dla_1 += 0.00001;
        //     } else if ( dla_1 == dla_0 ) {
        //         dla_1 += 0.00001;
        //     } else if ( dla_2 == dla_0 ) {
        //         dla_2 += 0.00001;
        //     } else if ( dla_2 == dla_1 ) {
        //         dla_2 += 0.00001;
        //     }

        // }

        // __xo, __sx, ... ?
        if ( for_a_test <= 1 ) {
            let pert = 0;
            for( let variant of [ "seoo", "sebo", "seob", "sebb" ] ) {
                if ( variant == this.pattern )
                    continue;
                if ( type_0.watch_self_op && ( this.pattern[ 0 ] == "s" || variant[ 0 ] == "s" ) )
                    continue;
                const m = methods[ this.base_name + '__' + variant + this.extr ];
                if ( m ) {
                    const s = m.find_best_func_4( type_0, type_1, type_2, type_3, 2 );
                    if ( s ) {
                        pert += 0.0001;
                        const bo_0 = this.pattern[ 0 ] == 'b' && ( variant[ 0 ] == 's' || variant[ 0 ] == 'o' ); 
                        const bo_1 = this.pattern[ 1 ] == 'b' && ( variant[ 1 ] == 's' || variant[ 1 ] == 'o' ); 
                        const bo_2 = this.pattern[ 2 ] == 'b' && ( variant[ 2 ] == 's' || variant[ 2 ] == 'o' ); 
                        const bo_3 = this.pattern[ 3 ] == 'b' && ( variant[ 3 ] == 's' || variant[ 3 ] == 'o' ); 
                        switch ( Number( bo_0 ) + 2 * Number( bo_1 ) + 4 * Number( bo_2 ) + 8 * Number( bo_3 ) ) {
                        case  0: test_pf( new Surdef( pert, null, s.func                                                                                                                                                                                   ) ); break;
                        case  1: test_pf( new Surdef( pert, null, ( a: Rp, b: Rp, c: Rp, d: Rp, ...args ) => s.func( methods[ 'copy' ].call_1( a ), b                            , c                            , d                            , ...args ) ) ); break;
                        case  2: test_pf( new Surdef( pert, null, ( a: Rp, b: Rp, c: Rp, d: Rp, ...args ) => s.func( a                            , methods[ 'copy' ].call_1( b ), c                            , d                            , ...args ) ) ); break;
                        case  3: test_pf( new Surdef( pert, null, ( a: Rp, b: Rp, c: Rp, d: Rp, ...args ) => s.func( methods[ 'copy' ].call_1( a ), methods[ 'copy' ].call_1( b ), c                            , d                            , ...args ) ) ); break;
                        case  4: test_pf( new Surdef( pert, null, ( a: Rp, b: Rp, c: Rp, d: Rp, ...args ) => s.func( methods[ 'copy' ].call_1( a ), b                            , methods[ 'copy' ].call_1( c ), d                            , ...args ) ) ); break;
                        case  5: test_pf( new Surdef( pert, null, ( a: Rp, b: Rp, c: Rp, d: Rp, ...args ) => s.func( methods[ 'copy' ].call_1( a ), b                            , methods[ 'copy' ].call_1( c ), d                            , ...args ) ) ); break;
                        case  6: test_pf( new Surdef( pert, null, ( a: Rp, b: Rp, c: Rp, d: Rp, ...args ) => s.func( a                            , methods[ 'copy' ].call_1( b ), methods[ 'copy' ].call_1( c ), d                            , ...args ) ) ); break;
                        case  7: test_pf( new Surdef( pert, null, ( a: Rp, b: Rp, c: Rp, d: Rp, ...args ) => s.func( methods[ 'copy' ].call_1( a ), methods[ 'copy' ].call_1( b ), methods[ 'copy' ].call_1( c ), d                            , ...args ) ) ); break;
                        case  8: test_pf( new Surdef( pert, null, ( a: Rp, b: Rp, c: Rp, d: Rp, ...args ) => s.func( a                            , b                            , c                            , methods[ 'copy' ].call_1( d ), ...args ) ) ); break;
                        case  9: test_pf( new Surdef( pert, null, ( a: Rp, b: Rp, c: Rp, d: Rp, ...args ) => s.func( methods[ 'copy' ].call_1( a ), b                            , c                            , methods[ 'copy' ].call_1( d ), ...args ) ) ); break;
                        case 10: test_pf( new Surdef( pert, null, ( a: Rp, b: Rp, c: Rp, d: Rp, ...args ) => s.func( a                            , methods[ 'copy' ].call_1( b ), c                            , methods[ 'copy' ].call_1( d ), ...args ) ) ); break;
                        case 11: test_pf( new Surdef( pert, null, ( a: Rp, b: Rp, c: Rp, d: Rp, ...args ) => s.func( methods[ 'copy' ].call_1( a ), methods[ 'copy' ].call_1( b ), c                            , methods[ 'copy' ].call_1( d ), ...args ) ) ); break;
                        case 12: test_pf( new Surdef( pert, null, ( a: Rp, b: Rp, c: Rp, d: Rp, ...args ) => s.func( methods[ 'copy' ].call_1( a ), b                            , methods[ 'copy' ].call_1( c ), methods[ 'copy' ].call_1( d ), ...args ) ) ); break;
                        case 13: test_pf( new Surdef( pert, null, ( a: Rp, b: Rp, c: Rp, d: Rp, ...args ) => s.func( methods[ 'copy' ].call_1( a ), b                            , methods[ 'copy' ].call_1( c ), methods[ 'copy' ].call_1( d ), ...args ) ) ); break;
                        case 14: test_pf( new Surdef( pert, null, ( a: Rp, b: Rp, c: Rp, d: Rp, ...args ) => s.func( a                            , methods[ 'copy' ].call_1( b ), methods[ 'copy' ].call_1( c ), methods[ 'copy' ].call_1( d ), ...args ) ) ); break;
                        case 15: test_pf( new Surdef( pert, null, ( a: Rp, b: Rp, c: Rp, d: Rp, ...args ) => s.func( methods[ 'copy' ].call_1( a ), methods[ 'copy' ].call_1( b ), methods[ 'copy' ].call_1( c ), methods[ 'copy' ].call_1( d ), ...args ) ) ); break;
                        }
                    }
                }
            }
        }

        // plugins
        for( let plugin of Method.plugins )
            plugin( test_pf, for_a_test, this, type_0, type_1, type_2, type_3 );

        // summary
        if ( for_a_test == 0 ) {
            if ( best_surdefs.length == 0 )
                throw new Error( `Impossible to find surdef for '${ this.name }' with types '${ type_0.name }', '${ type_1.name }', '${ type_2.name }' and '${ type_3.name }'` );
            if ( best_surdefs.length >= 2 )
                throw new Error( `Ambiguous overload for '${ this.name }' with types '${ type_0.name }', '${ type_1.name }', '${ type_2.name }' and '${ type_3.name }' (pertinence=${ best_surdefs[ 0 ].pertinence }), possibilities are ${ best_surdefs.map( x => x.disp_pos() ).join( "" ) }` );
        }
        return best_surdefs.length ? best_surdefs[ 0 ] : null;
    }

    static plugins    = new Array<( test_pf: ( s: Surdef ) => void, for_a_test: number, method: Method, type_0, type_1?, type_2?, type_3? ) => void>();
    static int_call_s = null as ( a: VarAnc ) => void; /** interception */

    function_list     = new Array<( ...any ) => any>();
    function_map      = new Map<string,any>();
    nb_tabu_types     = 0;
    nb_rp_args        : number;
    surdefs           = new Array<Surdef>();
    base_name         : string;
    pattern           : string; /** such as sob, bo, ... */
    ok_sym            : boolean; // true if can be transformed to operation(...) if symbolic operand(s)
    name              : string;
    extr              = ""; /** _extr if arg for extr */
}

import { VarAnc } from "./Variable";
