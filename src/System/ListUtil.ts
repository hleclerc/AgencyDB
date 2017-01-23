export default {
    remove<T>( l : Array<T>, v : T ) {
        let i = l.indexOf( v );
        if ( i >= 0 )
            l.splice( i, 1 );
    },
    /** for native items */
    equ<T>( a : Array<T>, b : Array<T> ) {
        if ( a.length != b.length )
            return false;
        for( let i = 0; i < a.length; ++i )
            if ( a[ i ] != b[ i ] )
                return false;
        return true;
    },
    /** for items with an equ method*/
    equ_obj<T>( a : Array<{equ:(b:T)=>boolean}>, b : Array<T> ) {
        if ( a.length != b.length )
            return false;
        for( let i = 0; i < a.length; ++i )
            if ( ! a[ i ].equ( b[ i ] ) )
                return false;
        return true;
    },
    /**  */
    push_unique<T>( a : Array<T>, b : T ) {
        if ( a.indexOf( b ) < 0 )
            a.push( b );
    },
    /**  */
    copy_obj<T>( a : Array<{copy:()=>T}> ) : Array<T> {
        let r = new Array<T>();
        for( let i of a )
            r.push( i.copy() );
        return r;
    }
};
