import read_cmds from './read_cmds'

console.log( `// This file has been generated using ${ process.argv.slice( 1 ).join( " and " ) }` );
console.log( `import BinaryWriter from '../src/ts/System/BinaryWriter'` );
console.log( `import UsrId        from '../src/ts/System/UsrId'` );
console.log( `` );

function ts_corr( type: string ): string {
    switch( type ) {
        case "String": return "string";
    }
    return type;
}

function need_rw( type: string ): boolean {
    switch( type ) {
        case "UsrId": return true;
    }
    return false;
}

let cmds = read_cmds( process.argv[ 2 ] );
console.log( `export default {` );
console.log( `    write: {` );
for( const cmd of cmds ) {
    let cnt = `bw.write_PI8( ${ cmd.num } );`;
    for( const arg of cmd.args ) {
        if ( need_rw( arg.type ) )
            cnt += ` ${ arg.name }.write_to( bw );`;
        else
            cnt += ` bw.write_${ arg.type }( ${ arg.name } );`;
    }
    console.log( `${ cmd.doc.map( l => "        /// " + l + "\n" ).join( '' ) }        ${ cmd.name }( bw: BinaryWriter, ${ cmd.args.map( arg => `${ arg.name}: ${ ts_corr( arg.type ) }` ).join( ', ' ) } ) { ${ cnt } },` );
}
//    console.log( `${ cmd.doc.map( l => "/// " + l + "\n" ).join( '' ) }void ${ cmd.name }( ${ cmd.args.map( arg => `${ cpp_corr( arg.type ) }${ arg.name}` ).join( ', ' ) } );` );
console.log( `    },` );
console.log( `}` );
