class Arg {
    cpp_decl( string_prefix : string ) {
        switch ( this.type ) {
            case "String"     : return `${ string_prefix }String `;
            default           : return this.type + ' ';
        }
    }

    cpp_decl_cb() {
        return this.cpp_decl( 'Cb' );
    }

    cpp_decl_cm() {
        return this.cpp_decl( 'Cm' );
    }

    cpp_type( string_prefix : string ) {
        switch ( this.type ) {
            case "VectorClock": return `const ${ this.type } &`;
            case "String"     : return `const ${ string_prefix }String &`;
            default           : return this.type + ' ';
        }
    }

    cpp_type_cb() {
        return this.cpp_type( 'Cb' );
    }

    cpp_type_cm() {
        return this.cpp_type( 'Cm' );
    }

    type: string;
    name: string;
}

class Func {
    name: string;
    type: string;
    args: Arg[];
}

export default function( s : string ) : Func {
    let res = new Func;

    let i = s.indexOf( '(' );
    let j = s.indexOf( ')' );
    if ( i >= 0 && j >= 0 ) {
        res.name = s.slice( 0, i );

        res.args = [];
        for( let t of s.slice( i + 1, j ).split( ',' ) ) {
            let u = t.match( "([^ ]+) *([^ ]+)" );
            if ( u ){
                let arg = new Arg;
                arg.type = u[ 1 ];
                arg.name = u[ 2 ];

                res.args.push( arg );
            }
        }
    } else {
        res.name = s;
        res.args = [];
        res.type = "";
    }

    return res;
}
