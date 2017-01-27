import { VarAnc } from './Variable';
import methods    from './methods';
import Surdef     from './Surdef';
import Method     from './Method';
import Rp         from './Rp';

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

    obj: VarAnc;
    ind: Rp;
}
Rp.make_templ( Select );

/**
 * Prop: appel de set_seo( obj, ind, value )
 * 
 * ATTENTION: CALL_Xs => avec le select, pour l'instant, on ne sait pas qu'on modifie la variable
 */
Method.plugins.push( function( test_pf, for_a_test, method, type_0, type_1, type_2, type_3 ) {
    if ( method.pattern[ 0 ] == 's' && type_0 == Select ) {
        const method_extr = methods[ method.base_name + '__sb' + method.pattern.substr( 1 ) ];
        if ( method_extr ) {
            switch ( method.nb_rp_args ) {
                case 1: test_pf( new Surdef( 1, () => true, ( a: Select, ...args ) => { a.obj.rp = method_extr.call_2s( a.obj, a.ind, ...args ); } ) ); break;
                case 2: test_pf( new Surdef( 1, () => true, ( a: Select, b: Rp, ...args ) => { a.obj.rp = method_extr.call_3s( a.obj, a.ind, b, ...args ); } ) ); break;
                case 3: test_pf( new Surdef( 1, () => true, ( a: Select, b: Rp, c: Rp, ...args ) => { a.obj.rp = method_extr.call_4s( a.obj, a.ind, b, c, ...args ); } ) ); break;
            }
        }
    }
} )
