import Undefined from "./Core/Undefined";
import Variable  from "./Core/Variable";
import Method    from "./Core/Method";
import GenMap    from "./Map/GenMap";
import Rp        from "./Core/Rp";

declare var global: any;

function default_new_val() : Variable {
    return new Variable( new Undefined );
}

//
export default
class Map<K,V> extends Variable {
    static NativeType = ( typeof self != "undefined" ? self : global ).Map // shortcut for people willing to import this as 'Array'
    static default_new_val = default_new_val;

    constructor( val?, new_val = default_new_val ) {
        if      ( val instanceof Rp         ) super( val );
        else if ( val instanceof Map        ) super( val.copy().rp );
        else if ( val instanceof Object     ) super( new GenMap( val, new_val ) );
        else if ( typeof val == "undefined" ) super( new GenMap( {}, new_val ) );
        else throw new Error( "TODO" );
    }

    copy() : Map<K,V> {
        return Method.methods["copy"].call_1( this );
    }

    set( key, val? ) : boolean {
        if ( val == null )
            return Variable.prototype.set.call( this, key );
        let rp = Variable.rp_conv_on( val );
        return rp.o ? Method.methods["self_set_o"].call_2( this.get( key ), rp.v ) : Method.methods["self_set_b"].call_2( this.get( key ), rp.v );
    }

    get( key ) : V { // if key is undefined, return a native object. Else, return the value for the key, or undefined
        return Method.methods["mod_select"].call_2( this, Variable.rp_conv_cp( key ) );
    }

    delete( key ) { // remove item with key `key`
        Method.methods["rem_item"].call_2( this, Variable.rp_conv_cp( key ) );
    }
}
