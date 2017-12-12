import Variable from "./Variable";
import Method   from "./Method";
import Rp       from "./Rp";

Method.methods[ "self_set_o" ].add_surdef( 0, ( type_0, type_1 ) => type_0.known_term_value && type_1.known_term_value, function( a, b ) {
    return a.rp = b.rp;
} );

Method.methods[ "rp_structure" ].add_surdef( 0, ( type_0 ) => ! Method.methods["prox_ref"].has_surdef_for( type_0 ) && ! Method.methods["prox_ptr"].has_surdef_for( type_0 ), function( a ) {
    return Method.methods[ "to_String" ].call_1( a );
} );
Method.methods[ "rp_structure" ].add_surdef( 0.1, ( type_0 ) => Method.methods["prox_ref"].has_surdef_for( type_0 ), function( a ) {
    return Rp.type_num_map[ a.rp.type_num ].name + "(" + Method.methods[ "rp_structure" ].call_1( Method.methods[ "prox_ref" ].call_1( a ) ) + ")";
} );
Method.methods[ "rp_structure" ].add_surdef( 0, ( type_0 ) => Method.methods["prox_ptr"].has_surdef_for( type_0 ), function( a ) {
    return Rp.type_num_map[ a.rp.type_num ].name + "(" + Method.methods[ "rp_structure" ].call_1( Method.methods[ "prox_ptr" ].call_1( a ) ) + ")";
} );

Method.methods[ "equ_immediate" ].add_surdef( 0, ( type_0, type_1 ) => Method.methods['to_Number'].pert_surdef_for( type_0 ) >= 0 && Method.methods['to_Number'].pert_surdef_for( type_1 ) >= 0, function( a : Variable, b : Variable ) {
    return a.toNumber() == b.toNumber();
} );

Method.methods[ "inf_immediate" ].add_surdef( 0, ( type_0, type_1 ) => Method.methods['to_Number'].pert_surdef_for( type_0 ) >= 0 && Method.methods['to_Number'].pert_surdef_for( type_1 ) >= 0, function( a : Variable, b : Variable ) {
    return a.toNumber() < b.toNumber();
} );

Method.methods[ "sup_immediate" ].add_surdef( 0, ( type_0, type_1 ) => Method.methods['to_Number'].pert_surdef_for( type_0 ) >= 0 && Method.methods['to_Number'].pert_surdef_for( type_1 ) >= 0, function( a : Variable, b : Variable ) {
    return a.toNumber() > b.toNumber();
} );

Method.methods["self_glo_to_loc"].add_surdef( 0, ( type ) => true, function( val, cur_dev, cur_usr ) {
} );

Method.methods["self_loc_to_glo"].add_surdef( 0, ( type ) => true, function( val, cur_dev, cur_usr ) {
} );

// symbolic
Method.methods["common_symbolic_data"].add_surdef( 0, ( type ) => true, function( val ) {
    return null;
} );

Method.methods["symbolic_children"].add_surdef( 0, ( type ) => true, function( val ) {
    return [];
} );

Method.methods["graphviz_repr"].add_surdef( 0, ( type ) => true, function( val ) {
    return val.rp.constructor.name;
} );

Method.methods["get_graph_data"].add_surdef( 0, ( type ) => true, function( val, graph_data, children, id ) {
    let csd = val.common_symbolic_data(); // to get the then list
    let bw = new BinaryWriter;
    val.write_to( bw );

    graph_data.push( {
        id  : id,
        name: "variable",
        data: bw.to_Uint8Array()
    } );
} );

Method.methods["is_symbolic"].add_surdef( 0, ( type ) => type.unknown_value, function( val ) {
    return true;
} );

Method.methods["is_symbolic"].add_surdef( 0, ( type ) => type.known_value, function( val ) {
    return false;
} );

Method.methods["then"].add_surdef( 0, ( type ) => type.known_value, function( val, callback ) {
    callback( val );
} );

// xx_b -> xx_o
for( let m of Method.std_mts ) {
    let n = m.name;
    switch ( m.nb_rp_args ) {
        case 1:
            Method.methods[ n + "_b" ].add_surdef( -1.0, ( type_0 ) => Method.methods[ n + "_o" ].pert_surdef_for( type_0 ) >= 0, function( a, ...args ) {
                return Method.methods[ n + "_o" ].call_1( a.copy(), ...args );
            } );
            break;
        case 2:
            Method.methods[ n + "_bb" ].add_surdef( -1.0, ( type_0, type_1 ) => Method.methods[ n + "_bo" ].pert_surdef_for( type_0, type_1 ) >= 0, function( a, b, ...args ) {
                return Method.methods[ n + "_bo" ].call_1( a, b.copy(), ...args );
            } );
            Method.methods[ n + "_bb" ].add_surdef( -1.1, ( type_0, type_1 ) => Method.methods[ n + "_ob" ].pert_surdef_for( type_0, type_1 ) >= 0, function( a, b, ...args ) {
                return Method.methods[ n + "_ob" ].call_1( a.copy(), b, ...args );
            } );
            Method.methods[ n + "_bb" ].add_surdef( -1.2, ( type_0, type_1 ) => Method.methods[ n + "_oo" ].pert_surdef_for( type_0, type_1 ) >= 0, function( a, b, ...args ) {
                return Method.methods[ n + "_oo" ].call_1( a.copy(), b.copy(), ...args );
            } );

            Method.methods[ n + "_ob" ].add_surdef( -1.2, ( type_0, type_1 ) => Method.methods[ n + "_oo" ].pert_surdef_for( type_0, type_1 ) >= 0, function( a, b, ...args ) {
                return Method.methods[ n + "_oo" ].call_1( a, b.copy(), ...args );
            } );
            Method.methods[ n + "_bo" ].add_surdef( -1.2, ( type_0, type_1 ) => Method.methods[ n + "_oo" ].pert_surdef_for( type_0, type_1 ) >= 0, function( a, b, ...args ) {
                return Method.methods[ n + "_oo" ].call_1( a.copy(), b, ...args );
            } );
            break;
        case 3:
            Method.methods[ n + "_bbb" ].add_surdef( -1.0, ( type_0, type_1, type_2 ) => Method.methods[ n + "_bbo" ].pert_surdef_for( type_0, type_1, type_2 ) >= 0, function( a, b, c, ...args ) {
                return Method.methods[ n + "_bbo" ].call_1( a, b, c.copy(), ...args );
            } );
            Method.methods[ n + "_bbb" ].add_surdef( -1.1, ( type_0, type_1, type_2 ) => Method.methods[ n + "_bob" ].pert_surdef_for( type_0, type_1, type_2 ) >= 0, function( a, b, c, ...args ) {
                return Method.methods[ n + "_bob" ].call_1( a, b.copy(), c, ...args );
            } );
            Method.methods[ n + "_bbb" ].add_surdef( -1.2, ( type_0, type_1, type_2 ) => Method.methods[ n + "_boo" ].pert_surdef_for( type_0, type_1, type_2 ) >= 0, function( a, b, c, ...args ) {
                return Method.methods[ n + "_boo" ].call_1( a, b.copy(), c.copy(), ...args );
            } );
            Method.methods[ n + "_bbb" ].add_surdef( -1.3, ( type_0, type_1, type_2 ) => Method.methods[ n + "_obo" ].pert_surdef_for( type_0, type_1, type_2 ) >= 0, function( a, b, c, ...args ) {
                return Method.methods[ n + "_obo" ].call_1( a.copy(), b, c.copy(), ...args );
            } );
            Method.methods[ n + "_bbb" ].add_surdef( -1.4, ( type_0, type_1, type_2 ) => Method.methods[ n + "_oob" ].pert_surdef_for( type_0, type_1, type_2 ) >= 0, function( a, b, c, ...args ) {
                return Method.methods[ n + "_oob" ].call_1( a.copy(), b.copy(), c, ...args );
            } );
            Method.methods[ n + "_bbb" ].add_surdef( -1.5, ( type_0, type_1, type_2 ) => Method.methods[ n + "_ooo" ].pert_surdef_for( type_0, type_1, type_2 ) >= 0, function( a, b, c, ...args ) {
                return Method.methods[ n + "_ooo" ].call_1( a.copy(), b.copy(), c.copy(), ...args );
            } );

            Method.methods[ n + "_bbo" ].add_surdef( -1.2, ( type_0, type_1, type_2 ) => Method.methods[ n + "_boo" ].pert_surdef_for( type_0, type_1, type_2 ) >= 0, function( a, b, c, ...args ) {
                return Method.methods[ n + "_boo" ].call_1( a, b.copy(), c, ...args );
            } );
            Method.methods[ n + "_bbo" ].add_surdef( -1.3, ( type_0, type_1, type_2 ) => Method.methods[ n + "_obo" ].pert_surdef_for( type_0, type_1, type_2 ) >= 0, function( a, b, c, ...args ) {
                return Method.methods[ n + "_obo" ].call_1( a.copy(), b, c, ...args );
            } );
            Method.methods[ n + "_bbo" ].add_surdef( -1.5, ( type_0, type_1, type_2 ) => Method.methods[ n + "_ooo" ].pert_surdef_for( type_0, type_1, type_2 ) >= 0, function( a, b, c, ...args ) {
                return Method.methods[ n + "_ooo" ].call_1( a.copy(), b.copy(), c, ...args );
            } );

            Method.methods[ n + "_bob" ].add_surdef( -1.2, ( type_0, type_1, type_2 ) => Method.methods[ n + "_boo" ].pert_surdef_for( type_0, type_1, type_2 ) >= 0, function( a, b, c, ...args ) {
                return Method.methods[ n + "_boo" ].call_1( a, b, c.copy(), ...args );
            } );
            Method.methods[ n + "_bob" ].add_surdef( -1.4, ( type_0, type_1, type_2 ) => Method.methods[ n + "_oob" ].pert_surdef_for( type_0, type_1, type_2 ) >= 0, function( a, b, c, ...args ) {
                return Method.methods[ n + "_oob" ].call_1( a.copy(), b, c, ...args );
            } );
            Method.methods[ n + "_bob" ].add_surdef( -1.5, ( type_0, type_1, type_2 ) => Method.methods[ n + "_ooo" ].pert_surdef_for( type_0, type_1, type_2 ) >= 0, function( a, b, c, ...args ) {
                return Method.methods[ n + "_ooo" ].call_1( a.copy(), b, c.copy(), ...args );
            } );

            Method.methods[ n + "_boo" ].add_surdef( -1.5, ( type_0, type_1, type_2 ) => Method.methods[ n + "_ooo" ].pert_surdef_for( type_0, type_1, type_2 ) >= 0, function( a, b, c, ...args ) {
                return Method.methods[ n + "_ooo" ].call_1( a.copy(), b, c, ...args );
            } );

            Method.methods[ n + "_obb" ].add_surdef( -1.3, ( type_0, type_1, type_2 ) => Method.methods[ n + "_obo" ].pert_surdef_for( type_0, type_1, type_2 ) >= 0, function( a, b, c, ...args ) {
                return Method.methods[ n + "_obo" ].call_1( a, b, c.copy(), ...args );
            } );
            Method.methods[ n + "_obb" ].add_surdef( -1.4, ( type_0, type_1, type_2 ) => Method.methods[ n + "_oob" ].pert_surdef_for( type_0, type_1, type_2 ) >= 0, function( a, b, c, ...args ) {
                return Method.methods[ n + "_oob" ].call_1( a, b.copy(), c, ...args );
            } );
            Method.methods[ n + "_obb" ].add_surdef( -1.5, ( type_0, type_1, type_2 ) => Method.methods[ n + "_ooo" ].pert_surdef_for( type_0, type_1, type_2 ) >= 0, function( a, b, c, ...args ) {
                return Method.methods[ n + "_ooo" ].call_1( a, b.copy(), c.copy(), ...args );
            } );

            Method.methods[ n + "_obo" ].add_surdef( -1.5, ( type_0, type_1, type_2 ) => Method.methods[ n + "_ooo" ].pert_surdef_for( type_0, type_1, type_2 ) >= 0, function( a, b, c, ...args ) {
                return Method.methods[ n + "_ooo" ].call_1( a, b.copy(), c, ...args );
            } );

            Method.methods[ n + "_oob" ].add_surdef( -1.5, ( type_0, type_1, type_2 ) => Method.methods[ n + "_ooo" ].pert_surdef_for( type_0, type_1, type_2 ) >= 0, function( a, b, c, ...args ) {
                return Method.methods[ n + "_ooo" ].call_1( a, b, c.copy(), ...args );
            } );

            break;
        default:
            throw new Error( "TODO" );
    }
}

// self_xx_b -> self_xx_o
for( let m of Method.slf_mts ) {
    let n = m.name;
    switch ( m.nb_rp_args ) {
        case 1:
            break;
        case 2:
            Method.methods[ "self_" + n + "_b" ].add_surdef( -1.0, ( type_0, type_1 ) => Method.methods[ "self_" + n + "_o" ].pert_surdef_for( type_0, type_1 ) >= 0, function( a, b, ...args ) {
                return Method.methods[ "self_" + n + "_o" ].call_2( a, b.copy(), ...args );
            } );
            break;
        case 3:
            Method.methods[ "self_" + n + "_bb" ].add_surdef( -1.0, ( type_0, type_1, type_2 ) => Method.methods[ "self_" + n + "_bo" ].pert_surdef_for( type_0, type_1, type_2 ) >= 0, function( a, b, c, ...args ) {
                return Method.methods[ "self_" + n + "_bo" ].call_3( a, b, c.copy(), ...args );
            } );
            Method.methods[ "self_" + n + "_bb" ].add_surdef( -1.1, ( type_0, type_1, type_2 ) => Method.methods[ "self_" + n + "_ob" ].pert_surdef_for( type_0, type_1, type_2 ) >= 0, function( a, b, c, ...args ) {
                return Method.methods[ "self_" + n + "_ob" ].call_3( a, b.copy(), c, ...args );
            } );
            Method.methods[ "self_" + n + "_bb" ].add_surdef( -1.2, ( type_0, type_1, type_2 ) => Method.methods[ "self_" + n + "_oo" ].pert_surdef_for( type_0, type_1, type_2 ) >= 0, function( a, b, c, ...args ) {
                return Method.methods[ "self_" + n + "_oo" ].call_3( a, b.copy(), c.copy(), ...args );
            } );

            Method.methods[ "self_" + n + "_ob" ].add_surdef( -1.2, ( type_0, type_1, type_2 ) => Method.methods[ "self_" + n + "_oo" ].pert_surdef_for( type_0, type_1, type_2 ) >= 0, function( a, b, c, ...args ) {
                return Method.methods[ "self_" + n + "_oo" ].call_3( a, b, c.copy(), ...args );
            } );
            Method.methods[ "self_" + n + "_bo" ].add_surdef( -1.2, ( type_0, type_1, type_2 ) => Method.methods[ "self_" + n + "_oo" ].pert_surdef_for( type_0, type_1, type_2 ) >= 0, function( a, b, c, ...args ) {
                return Method.methods[ "self_" + n + "_oo" ].call_3( a, b.copy(), c, ...args );
            } );
            break;
        default:
            throw new Error( "TODO" );
    }
}

// self_xx( a, ... ) -> set( a, xx( a ), ... )
for( let m of Method.slf_mts ) {
    let n = m.name;
    switch ( m.nb_rp_args ) {
        case 1:
            Method.methods[ `self_${n}` ].add_surdef( 0, ( type_0 ) => Method.methods[ `${n}_b` ].has_surdef_for( type_0 ), function( ...args ) {
                return Method.methods["self_set_o"].call_2( args[ 0 ], Method.methods[ `${n}_b` ].call_1( ...args ) );
            } );
            break;
        case 2:
            Method.methods[ `self_${n}_b` ].add_surdef( 0, ( type_0, type_1 ) => Method.methods[ `${n}_bb` ].has_surdef_for( type_0, type_1 ), function( ...args ) {
                return Method.methods["self_set_o"].call_2( args[ 0 ], Method.methods[ `${n}_bb` ].call_2( ...args ) );
            } );
            break;
        case 3:
            Method.methods[ `self_${n}_bb` ].add_surdef( 0, ( type_0, type_1, type_2 ) => Method.methods[ `${n}_bbb` ].has_surdef_for( type_0, type_1, type_1 ), function( ...args ) {
                return Method.methods["self_set_o"].call_2( args[ 0 ], Method.methods[ `${n}_bbb` ].call_3( ...args ) );
            } );
            break;
        default:
            throw new Error( "TODO" );
    }
}


import BinaryWriter from "../../System/BinaryWriter";
