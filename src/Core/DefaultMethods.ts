import Graphviz from "./Graphviz"
import methods  from "./methods"
import Rp       from "./Rp"

// onchange
//methods[ "add__bb" ].add_surdef( 10, [ GenString, GenString ], ( a: GenString, b: GenString ) => new GenString( a.data + b.data ) );
// Methods that will work with an OtWrapper
for( let m of [ "on_change__s", "add_patch_manager__s" ].map( n => methods[ n ] ) )
    m.add_surdef( 0.5, [ "make_ot_wrapper__s" ], ( a: Rp, ...args ) => m.call_1( methods[ "make_ot_wrapper__s" ].call_1( a ), ...args ) );
// , "self_on_change_par", "pull_ot_changes", "new_ot_merger", "get_possible_rights", "get_groups_in_acl", "get_users_in_acl",
// "get_grp_rights", "get_usr_rights", "add_grp_right", "add_usr_right", "rem_grp_right", "rem_usr_right"

// for( let n of [ "self_ot_merge_b" ] ) {
//     methods[ n ].add_surdef( 1 + 0.1 / ( 1 + 5 ), [ "make_ot_wrapper", () => true ], ( ...args ) => {
//         args[ 0 ].rp = methods["make_ot_wrapper"].call_1( args[ 0 ] );
//         return methods[ n ].call_2.apply( methods[ n ], args );
//     } );
// }

methods[ "js_repr__b"        ].add_surdef( 0.0, () => true, ( a: Rp, prec, cg ) => methods[ "to_String__b" ].call_1( a ) );
methods[ "copy__o"           ].add_surdef( 9.0, () => true, ( a: Rp ) => a );
methods[ "write_graphviz__b" ].add_surdef( 0.0, () => true, function( a: Rp, gr: Graphviz ) { gr.node( a, -1, 0, methods[ "js_repr__b" ].call_1( a, 0 ) ); } );
