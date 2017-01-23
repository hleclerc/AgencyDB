import Method from "./Method";

var methods = {} as { [name: string]: Method };

function comb_bo( nb_rp_args: number ): Array<String> {
    switch ( nb_rp_args ) {
        case 0: return [ "" ]
        case 1: return [ "b" ,"o" ]
        case 2: return [ "bb", "bo", "ob", "oo" ]
        case 3: return [ "bbb", "bob", "obb", "oob", "bbo", "boo", "obo", "ooo" ]
    }
    throw new Error("TODO");
}

export function decl( name: string, ok_sym = true ) : void {
    methods[ name ] = new Method( name, ok_sym );
}

export function dstd( name: string, nb_rp_args: number, ok_sym = true ) {
    for( let v of comb_bo( nb_rp_args ) )
        decl( name + "__" + v, ok_sym );
}

export function dslf( name: string, nb_rp_args: number, ok_sym = true ) {
    // xxx_bb, xxx_bo, ...
    dstd( name, nb_rp_args, ok_sym );

    // xxx_sb, xxx_so, ... + xxx_sb_extr, self_xxx_so_extr, ...
    for( let v of comb_bo( nb_rp_args - 1 ) ) {
        decl( name + "__s"  + v, ok_sym );
        decl( name + "__sb" + v, ok_sym ); // with a select list
    }
}

export default methods; 