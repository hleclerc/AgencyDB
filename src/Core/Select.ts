import { VarAnc } from './Variable'
import methods    from './methods'
import Surdef     from './Surdef'
import Method     from './Method'
import RpList     from './RpList'
import Rp         from './Rp'

/**
 */
export default
class Select extends Rp {
    constructor( obj: VarAnc, ind: Rp ) {
        super( Select );
        this.obj = obj;
        this.ind = ind;
    }

    /** read value */
    prox_val__b(): Rp {
        return methods[ "select__bb" ].call_2( this.obj.rp, this.ind );
    }

    /** list of Rp (decomposition of this.ind) */
    lst(): Array<Rp> {
        if ( this.ind instanceof RpList ) {
            let res = new Array<Rp>();
            this.ind.get_lst( res );
            return res;
        }
        return [ this.ind ];
    }

    obj: VarAnc;
    ind: Rp; /** RpList means that we have multiple selection */
}
Rp.make_templ( Select );

/**
 */
Method.plugins.push( function( test_pf, for_a_test, method, type_0, type_1, type_2, type_3 ) {
    if ( method.pattern[ 0 ] == 's' && type_0 == Select ) {
        const method_extr = methods[ method.base_name + '__sb' + method.pattern.substr( 1 ) ];
        if ( method_extr ) {
            switch ( method.nb_rp_args ) {
                case 1: test_pf( new Surdef( 2, () => true, ( a: Select, ...args ) => { a.obj.rp = method_extr.call_2s( a.obj, a.ind, ...args ); } ) ); break;
                case 2: test_pf( new Surdef( 2, () => true, ( a: Select, b: Rp, ...args ) => { a.obj.rp = method_extr.call_3s( a.obj, a.ind, b, ...args ); } ) ); break;
                case 3: test_pf( new Surdef( 2, () => true, ( a: Select, b: Rp, c: Rp, ...args ) => { a.obj.rp = method_extr.call_4s( a.obj, a.ind, b, c, ...args ); } ) ); break;
            }
        }
    }
} )

methods["select_ref__oo"].add_surdef( 2, [ () => true, () => true ], function( val: Rp, ind: Rp, obj: VarAnc ) {
    return new Select( obj, ind );
} );

methods["select_ref__oo"].add_surdef( 3, [ Select, () => true ], function( val: Select, ind: Rp, obj: VarAnc ) {
    return new Select( val.obj, RpList.append( val.ind, ind ) );
} );
