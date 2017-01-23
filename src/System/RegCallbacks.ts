declare type CB = () => void;

var callbacks = new Array<CB>();
var timeout   = undefined;

function call_all() {
    timeout = undefined;
    for( let c of callbacks )
        c();
    callbacks = [];
}

export default {
    /** register a callback or an array of callback to be called during the next tick */
    add: ( l: Array<CB> | CB ) => {
        if ( l instanceof Array ) {
            for( let f of l )
                this.add( f );
        } else {
            if ( callbacks.indexOf( l ) < 0 ) {
                callbacks.push( l );
                if ( timeout == undefined )
                    timeout = setTimeout( call_all, 1 );
            }
        }
    }
}
