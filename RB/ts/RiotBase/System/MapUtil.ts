import Map from "./Map";

export default {
    get_or_set<K,V>( m : Map<K,V>, k : K, f : () => V ) : V {
        if ( m.has( k ) )
            return m.get( k );
        let v = f();
        m.set( k, v );
        return v;
    },
    /** for native items */
    equ<K,V>( a : Map<K,V>, b : Map<K,V> ) {
        if ( a.size != b.size )
            return false;
        let res = true;
        a.forEach( function( v: V, k: K ) {
            if ( b.get( k ) != v )
                res = false; // HUM...
        } );
        return res;
    }
};
