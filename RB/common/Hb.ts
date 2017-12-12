import Handlebars = require( 'handlebars' );

Handlebars.registerHelper( 'format', function( l : number, options ) {
    let sp = options.fn( this );
    while( sp.length < l )
        sp += ' ';
    return sp;
} );

Handlebars.registerHelper( 'trim', function( options ) {
    return options.fn( this ).trim();
} );

Handlebars.registerHelper( 'join', function( l : Array<any>, options ) {
    let res = '';
    for ( let i = 0; i < l.length; i++ ) {
        if ( i )
            res += ', ';
        res += options.fn( l[ i ] );
    }
    return res;

} );
Handlebars.registerHelper( 'join_spba', function( l : Array<any>, options ) {
    if ( l.length == 0 )
        return '';

    let res = ' ';
    for ( let i = 0; i < l.length; i++ ) {
        if ( i )
            res += ', ';
        res += options.fn( l[ i ] );
    }
    return res + ' ';
} );

export default Handlebars;
