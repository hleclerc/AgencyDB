import read_cmds from './read_cmds'

console.log( `// This file has been generated using ${ process.argv.slice( 1 ).join( " and " ) }` );
console.log( `` );

function cpp_corr( type: string ): string {
    switch( type ) {
        case "String": return "const Hpipe::CbStringPtr &";
    }
    return type + " ";
}

for( const cmd of read_cmds( process.argv[ 2 ] ) )
    console.log( `${ cmd.doc.map( l => "/// " + l + "\n" ).join( '' ) }void ${ cmd.name }( ${ cmd.args.map( arg => `${ cpp_corr( arg.type ) }${ arg.name}` ).join( ', ' ) } );` );
