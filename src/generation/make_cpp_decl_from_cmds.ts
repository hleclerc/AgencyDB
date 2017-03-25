import read_cmds from './read_cmds'

console.log( `// This file has been generated using ${ process.argv.slice( 1 ).join( " and " ) }` );
console.log( `` );

function cpp_corr( type: string ): string {
    switch( type ) {
        case "String": return "const Hpipe::CbStringPtr &";
    }
    return type + " ";
}
function need_rw( type: string ): boolean {
    switch( type ) {
        case "UsrId": return true;
    }
    return false;
}
function bw_corr( type: string ): string {
    switch( type ) {
        case "PI8":
        case "PI16":
        case "PI32":
        case "PI64":
            return "unsigned";
    }
    return type;
}

// declaration of parse functions
for( const cmd of read_cmds( process.argv[ 2 ] ) )
    console.log( `${ cmd.doc.map( l => "/// " + l + "\n" ).join( '' ) }void ${ cmd.name }( ${ cmd.args.map( arg => `${ cpp_corr( arg.type ) }${ arg.name}` ).join( ', ' ) } );` );

// write functions
console.log( `` );
for( const cmd of read_cmds( process.argv[ 2 ] ) ) {
    let cnt = `bw.write_byte( ${ cmd.num } );`;
    for( const arg of cmd.args ) {
        if ( need_rw( arg.type ) )
            cnt += ` ${ arg.name }.write_to( bw );`;
        else
            cnt += ` bw << ${ arg.name };`;
    }
    console.log( `${ cmd.doc.map( l => "/// " + l + "\n" ).join( '' ) }void write_${ cmd.name }( Hpipe::BinStream<Hpipe::CbQueue> &bw, ${ cmd.args.map( arg => `${ cpp_corr( arg.type ) }${ arg.name}` ).join( ', ' ) } ) { ${ cnt } }` );
}
