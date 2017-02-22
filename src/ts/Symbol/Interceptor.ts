import { VarAnc } from "../Core/Variable"
import methods    from "../Core/methods"
import Method     from "../Core/Method"
import Rp         from "../Core/Rp"

export default
class Interceptor {
    static run( res: Map<VarAnc,{ o: Rp, b: Rp, n: Rp }>, func, crea_cb: ( v: VarAnc ) => Rp ) {
        if ( ! func )
            return;

        // we don't want to intercept modification of variable created during the call of func
        const date = ++VarAnc.date;

        // init with values in res, use again the old value
        for( let [ variable, oan ] of res )
            variable.rp = oan.b;

        // install interceptors such as if not already done, we install a new value and register the variable each time there's a _s modification
        const old_int_call_s = Method.int_call_s;

        Method.int_call_s = function( a: VarAnc ): void {
            if ( a.date < date && ! res.get( a ) ) {
                const b = crea_cb( a );
                res.set( a, { o: a.rp, b, n: null } );
                a.rp = b;
            }
        }; 

        //
        func();

        // save the new values (in oan.n) and restore (variable.rp)
        for( let [ variable, oan ] of res ) {
            oan.n = variable.rp;
            variable.rp = oan.o;
        }

        //
        Method.int_call_s = old_int_call_s;
    }
}