import read_cmds from './read_cmds'

const add_args_by_type = {
    String: { cns: [ "String"       ], tmp: [ "PI32", "PI32" ] },
    PI32  : { cns: [ "PI32"         ], tmp: [ "PI32"         ] },
    DevId : { cns: [ "PI64"         ], tmp: [ "PI32"         ] },
    UsrId : { cns: [ "PI64", "PI64" ], tmp: [ "PI32"         ] },
} as { [key:string]: { cns: Array<string>, tmp: Array<string> } };

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
        let lst_names = [];
        let nbl = new Map<string,number>(); // type -> nb used
        const add_args = add_args_by_type[ arg.type ];
        lst_names.push( ...add_args.cns.map( ( t, i ) => `__${ t }_${ nft( t, nbt ) }` ) );
        lst_names.push( ...add_args.tmp.map( ( t, i ) => `__${ t }_${ nft( t, nbl ) }` ) );
        cmd_data += ` read_${ arg.type }[ ${ lst_names.map( name => `"${ name }"` ).join( ', ' ) } ]`;
        if ( add_args.cns.length == 1 )
            nargs.push( lst_names[ 0 ] );
        else
            nargs.push( `{ ${ lst_names.slice( 0, add_args.cns.length ).join( ', ' ) } }` );
    }
    cmd_data += ` { ${ cmd.name }( ${ nargs.join( ', ' ) } ); }`;
}

process.stdout.write(
`# This file has been generated using ${ process.argv.slice( 1 ).join( " and " ) }
beg_flags
    --never-ending
end_flags

read_PI[ value, shift, type = "PI32" ] = 
    add_variable[ "unsigned", shift ]
    add_variable[ type, value ]
    { value = 0; shift = 0; }
    ( 128 .. 255 { value += type( *data - 128 ) << shift; shift += 7; } )**
    (   0 .. 127 { value += type( *data       ) << shift; } )

read_String[ value, len, shift ] =
    read_PI[ len, shift, "PI32" ]
    beg_str_next[ value ]
    skip[ len ]
    end_str_next[ value ]

read_DevId[ value, shift ] = 
    read_PI[ value, shift, "PI64" ]

read_UsrId[ num, dev, shift ] = 
    read_PI[ num, shift, "PI64" ]
    read_PI[ dev, shift, "PI64" ]

${ cmd_data }

cmds = ${ cmds.map( cmd => cmd.name ).join( ' | ' ) }

main = cmds**
` )
