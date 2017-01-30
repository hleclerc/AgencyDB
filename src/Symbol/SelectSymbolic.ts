import { VarAnc, od } from '../Core/Variable'
import methods        from '../Core/methods'
import Surdef         from '../Core/Surdef'
import Method         from '../Core/Method'
import Select         from '../Core/Select'
import Rp             from '../Core/Rp'
import { skv_link_o, 
         skv_link_b } from './SymbolicKnownValue'
import RpListSymbolic from './RpListSymbolic'
import Sym            from './Sym'

/**
 */
export default
class SelectSymbolic extends Rp {
    constructor( obj: VarAnc, ind: RpListSymbolic ) {
        super( SelectSymbolic );
        this.obj = obj;
        this.ind = ind;
    }

    /** read value */
    prox_val__b(): Rp {
        return methods[ "select__bb" ].call_2( this.obj.rp, this.ind );
    }

    obj: VarAnc;
    ind: RpListSymbolic; /** RpList means that we have multiple selection */
}
Rp.make_templ( SelectSymbolic );

/**
 */
Method.plugins.push( function( test_pf, for_a_test, method, type_0, type_1, type_2, type_3 ) {
    // ex: add__sx( SelectSymbolic( map, key ), val ) => add__sbx( map, [ key ], val )
    if ( method.pattern[ 0 ] == 's' && type_0 == SelectSymbolic ) {
        const method_extr = methods[ method.base_name + '__sb' + method.pattern.substr( 1 ) ];
        if ( method_extr ) {
            switch ( method.nb_rp_args ) {
                case 1: test_pf( new Surdef( 2, () => true, ( a: SelectSymbolic, ...args ) => { a.obj.rp = method_extr.call_2s( a.obj, a.ind, ...args ); } ) ); break;
                case 2: test_pf( new Surdef( 2, () => true, ( a: SelectSymbolic, b: Rp, ...args ) => { a.obj.rp = method_extr.call_3s( a.obj, a.ind, b, ...args ); } ) ); break;
                case 3: test_pf( new Surdef( 2, () => true, ( a: SelectSymbolic, b: Rp, c: Rp, ...args ) => { a.obj.rp = method_extr.call_4s( a.obj, a.ind, b, c, ...args ); } ) ); break;
            }
        }
    }

    // ex: add__sx( Select( map, key ), sym ) => add__sbx( map, [ key ], sym )
    if ( method.pattern[ 0 ] == 's' && type_0 == Select ) {
        const method_extr = methods[ method.base_name + '__sb' + method.pattern.substr( 1 ) ];
        if ( method_extr ) {
            switch ( method.nb_rp_args ) {
                case 2:
                    if ( type_1.prototype instanceof Sym ) 
                        test_pf( new Surdef( 3, () => true, ( a: Select, b: Rp, ...args ) => {
                            const ind = new RpListSymbolic( a.lst().map( x => skv_link_b( x ) ) );
                            a.obj.rp = method_extr.call_3s( a.obj, ind, b, ...args );
                        } ) );
                    break;
                case 3:
                    if ( type_1.prototype instanceof Sym || type_2.prototype instanceof Sym )
                        test_pf( new Surdef( 3, () => true, ( a: Select, b: Rp, c: Rp, ...args ) => {
                            const ind = new RpListSymbolic( a.lst().map( x => skv_link_b( x ) ) );
                            a.obj.rp = method_extr.call_4s( a.obj, ind, b, c, ...args );
                        } ) );
                    break;

            }
        }
    }
} )


od( () => {
    // start a new SelectSymbolic
    methods["select_ref__oo"].add_surdef( 4, ( type_0, type_1 ) => type_0.symbolic_value || type_1.symbolic_value, function( val: Sym, ind: Rp, obj: VarAnc ) {
        return new SelectSymbolic( obj, new RpListSymbolic( [ skv_link_o( ind ) ] ) );
    } );
    methods["select_ref__ob"].add_surdef( 4, ( type_0, type_1 ) => type_0.symbolic_value || type_1.symbolic_value, function( val: Sym, ind: Rp, obj: VarAnc ) {
        return new SelectSymbolic( obj, new RpListSymbolic( [ skv_link_b( ind ) ] ) );
    } );

    // add select to an existing SelectSymbolic
    methods["select_ref__oo"].add_surdef( 5, [ SelectSymbolic, () => true ], function( val: SelectSymbolic, ind: Rp, obj: VarAnc ) {
        return new SelectSymbolic( val.obj, new RpListSymbolic( [ ...val.ind.children, skv_link_o( ind ) ] ) );
    } );
    methods["select_ref__ob"].add_surdef( 5, [ SelectSymbolic, () => true ], function( val: SelectSymbolic, ind: Rp, obj: VarAnc ) {
        return new SelectSymbolic( val.obj, new RpListSymbolic( [ ...val.ind.children, skv_link_b( ind ) ] ) );
    } );
} );
