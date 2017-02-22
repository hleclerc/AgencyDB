import UsrId    from "../System/UsrId"
import Graphviz from "./Graphviz"
import methods  from "./methods"
import Rp       from "./Rp"

// Methods that will work with an OtWrapper, "self_on_change_par", "pull_ot_changes", "new_ot_merger"
for( let m of [ "on_change__s", "add_patch_manager__s", "add_usr_right__s", "rem_usr_right__s" ].map( n => methods[ n ] ) )
    m.add_surdef( 0.5, [ "make_ot_wrapper__s" ], ( a: Rp, ...args ) => m.call_1( methods[ "make_ot_wrapper__s" ].call_1( a ), ...args ) );

// for( let n of [ "self_ot_merge_b" ] ) {
//     methods[ n ].add_surdef( 1 + 0.1 / ( 1 + 5 ), [ "make_ot_wrapper", () => true ], ( ...args ) => {
//         args[ 0 ].rp = methods["make_ot_wrapper"].call_1( args[ 0 ] );
//         return methods[ n ].call_2.apply( methods[ n ], args );
//     } );
// }

methods[ "js_repr__b"          ].add_surdef( 0.0, () => true, ( a: Rp, prec, cg ) => methods[ "to_String__b" ].call_1( a ) );
methods[ "copy__o"             ].add_surdef( 9.0, () => true, ( a: Rp ) => a );
methods[ "write_graphviz__b"   ].add_surdef( 0.0, () => true, function( a: Rp, gr: Graphviz ) { gr.node( a, -1, 0, methods[ "js_repr__b" ].call_1( a, 0 ) ); } );

methods[ "get_users_in_acl__b" ].add_surdef( 0.0, () => true, function( a: Rp ) { return [ new UsrId ]; } );
methods[ "get_usr_rights__b"   ].add_surdef( 0.0, () => true, function( a: Rp, usr: UsrId ) { return usr.loc ? methods[ "get_possible_rights__b" ].call_1( a ) : []; } );

