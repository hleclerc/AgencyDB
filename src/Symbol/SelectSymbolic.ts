import { VarAnc, od } from '../Core/Variable'
import methods        from '../Core/methods'
import Surdef         from '../Core/Surdef'
import Method         from '../Core/Method'
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
    if ( method.pattern[ 0 ] == 's' && type_0 == SelectSymbolic ) {
        const method_extr = methods[ method.base_name + '__sb' + method.pattern.substr( 1 ) ];
        if ( method_extr ) {
            switch ( method.nb_rp_args ) {
                case 1: test_pf( new Surdef( 1, () => true, ( a: SelectSymbolic, ...args ) => { a.obj.rp = method_extr.call_2s( a.obj, a.ind, ...args ); } ) ); break;
                case 2: test_pf( new Surdef( 1, () => true, ( a: SelectSymbolic, b: Rp, ...args ) => { a.obj.rp = method_extr.call_3s( a.obj, a.ind, b, ...args ); } ) ); break;
                case 3: test_pf( new Surdef( 1, () => true, ( a: SelectSymbolic, b: Rp, c: Rp, ...args ) => { a.obj.rp = method_extr.call_4s( a.obj, a.ind, b, c, ...args ); } ) ); break;
            }
        }
    }
} )


od( () => {
    methods["select_ref__bo"].add_surdef( 4, ( type_0, type_1 ) => type_0.symbolic_value, function( val: Sym, ind: Rp, obj: VarAnc ) {
        return new SelectSymbolic( obj, new RpListSymbolic( [ skv_link_o( ind ) ] ) );
    } );

    methods["select_ref__bb"].add_surdef( 4, ( type_0, type_1 ) => type_0.symbolic_value, function( val: Sym, ind: Rp, obj: VarAnc ) {
        return new SelectSymbolic( obj, new RpListSymbolic( [ skv_link_b( ind ) ] ) );
    } );

    // methods["select_ref__bo"].add_surdef( 4, ( type_0, type_1 ) => type_1 instanceof Sym, function( val: Rp, ind: Sym, obj: VarAnc ) {
    //     return new SelectSymbolic( obj, ind );
    // } );

    // methods["select_ref__bo"].add_surdef( 5, ( type_0, type_1 ) => type_0 == SelectSymbolic || type_1 instanceof Sym, function( val: Select, ind: Rp, obj: VarAnc ) {
    //     return new Select( val.obj, RpList.append( val.ind, ind ) );
    // } );
} );
