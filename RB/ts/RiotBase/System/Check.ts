function _make_req( msg: string ) : void {
    var req = new XMLHttpRequest;
    req.open( "GET", msg, true );
    req.send( null );
}

declare class Error {
    stack: string;
}

export default function check( expr, value, add_stack = 0 ) {
    var se = expr.toString();
    var sv = value.toString();
    var st = ( new Error ).stack.split( "\n" )[ 2 + add_stack ];
    if ( typeof self != "undefined" ) {
        var fl = st.match( /https?:\/\/[^\/]*\/([^)]*)/ ); // .replace( /^\s*at/g, '' ).replace( /^\s+|\s+$/g, '' )
        _make_req( `/_test?st=${Number( se == sv )}&fl=${encodeURIComponent( fl ? fl[ 1 ] : st )}&se=${encodeURIComponent( se )}&sv=${encodeURIComponent( sv )}` );
    } else {
        var fl = st.match( /\(([^)]*)\)/ ); // .replace( /^\s*at/g, '' ).replace( /^\s+|\s+$/g, '' )
        console.log( fl[ 1 ] + ": (" + ( se == sv ? "OK" : "BAD" ) + ") " + se );
    }
}

export function quit_server() {
    _make_req( "/_quit_server" );
}
//      # console feedback
//      fl = ( new Error ).stack.split("\n")[ 2 ].replace( /^\s*at/g, '' ).replace( /^\s+|\s+$/g, '' )
//      console.log fl, "(#{if se == sv then "OK" else "BAD"})", se
//
//      # send info to the server
//      req = new XMLHttpRequest
//      req.open "GET", , true
//      req.send null
