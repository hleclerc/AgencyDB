import read_cmds from './read_cmds'

const add_args_by_type = {
    String: [ "PI32", "PI32" ],
    PI32  : [ "PI32" ],
} as { [key:string]: Array<string> };

let cmds = read_cmds( process.argv[ 2 ] );

let cmd_data = ""
for( const cmd of cmds ) {
    cmd_data += `${ cmd.doc.map( l => "\n# " + l ).join( '' ) }\n${ cmd.name } = ${ cmd.num }`;
    let nbt = new Map<string,number>(); // type -> nb used
    function nft( t, nbt ) {
        if ( ! nbt.has( t ) ) nbt.set( t, 0 );
        const res = nbt.get( t );
        nbt.set( t, res + 1 );
        return res;
    }
    let nargs = new Array<string>();
    for( const arg of cmd.args ) {
        let nbt_loc = new Map<string,number>(); // type -> nb used
        const lst_names = [ arg.type, ...add_args_by_type[ arg.type ] ].map( ( t, i ) => `__${ t }_${ nft( t, i ? nbt_loc : nbt ) }` );
        cmd_data += ` read_${ arg.type }[ ${ lst_names.map( name => `"${ name }"` ).join( ', ' ) } ]`;
        nargs.push( lst_names[ 0 ] );
    }
    cmd_data += ` { ${ cmd.name }( ${ nargs.join( ', ' ) } ); }`;
}

process.stdout.write(
`# This file has been generated using ${ process.argv.slice( 1 ).join( " and " ) }
beg_flags
    --never-ending
end_flags

read_PI32[ value, shift ] = 
    add_variable[ "unsigned", shift ]
    add_variable[ "unsigned", value ]
    { value = 0; shift = 0; }
    ( 128 .. 255 { value += PI32( *data - 128 ) << shift; shift += 7; } )**
    (   0 .. 127 { value += PI32( *data       ) << shift; } )

read_String[ value, len, shift ] =
    read_PI32[ len, shift ]
    beg_str_next[ value ]
    skip[ len ]
    end_str_next[ value ]
${ cmd_data }

cmds = ${ cmds.map( cmd => cmd.name ).join( ' | ' ) }

main = cmds**
` )
