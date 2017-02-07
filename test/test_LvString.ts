/// <reference path="../node_modules/@types/mocha/index.d.ts"/>
import DevId      from "../src/System/DevId"
import UsrId      from "../src/System/UsrId"
import { VarAnc } from "../src/Core/Variable"
import Method     from "../src/Core/Method"
import test_ot    from "../src/Test/test_ot"
import LvString   from "../src/LvString"
import Db         from "../src/Db"
import { assert } from "chai"

function sequ( a, b, msg?: string ) {
    assert.equal( a.toString(), b.toString(), msg );
}

/** helper */
function test_ot_str( init: string, operations: ( vars: Array<LvString>, dbs: Array<Db> ) => void, expected: string, explanation = operations.toString(), nb_dbs = 2 ) {
    test_ot<LvString>( LvString, nb_dbs, ( vls, dbs ) => {
        if ( init ) {
            vls[ 0 ].selfConcat( init );
            dbs[ 0 ].send_changes();
        }
        operations( vls, dbs );
    }, expected, explanation );
}

/**
 * OnChange; 
 *   changement permanent => remonte à chaque opération _sxx
 *   lorsque ce n'est pas symbolique => prop: on fait un proxy, mais celui-ci transfère les fonctions lorsque ça devient symbolique
 * 
 * Autre approche: 
 */
describe( 'String', () => {
    it( 'basic operations', () => {
        let s = new LvString( "test" );
        sequ( s                   , "test"        );
        sequ( s.length            , 4             );
        sequ( s.selfConcat( "ouille" ), "testouille"  );
        sequ( s.insert( 9, "s" )  , "testouillse" );
        sequ( s.substr( 2, 4 )    , "stou"        );
        sequ( s.substring( 2, 4 ) , "st"          );
        sequ( s.remove( 2, 8 )    , "tee"         );
    });

    it( 'on change', ( done ) => {
        let s = new LvString( "test" );

        let cpt = 0;
        s.onChange( () => ++cpt );
        s.selfConcat( "_" );
        s.selfConcat( "_" );
        setTimeout( () => {
            sequ( s, "test__" );
            sequ( cpt, 1 );
            done();
        }, 20 );
    });

    it( 'select', () => {
        let a = new LvString( "test" );
        sequ( a.charAt( 2 ), "s" );

        let intercept = [];
        Method.int_call_s = ( a ) => intercept.push( a );   

        let s = a.select( 1 );
        sequ( a, "test", "basic test of RefProxy" );
        sequ( s, "e", "read sub value" );
        s.set( '3' );
        sequ( a, "t3st", "string with modified sub value" );

        assert.equal( intercept.length, 2, "nb interceptions ('s' is considered as modified even if it's not really the case)" ); 
        assert.equal( intercept[ 0 ], s, "interception of the right variable" );
        assert.equal( intercept[ 1 ], a, "interception of the right variable" );
    });

    it( 'intercept + creation date', () => {
        let a = new LvString( "a" );

        let intercept = [];
        const date = ++VarAnc.date;
        Method.int_call_s = ( a ) => { if ( a.date < date ) intercept.push( a ); };   

        let b = new LvString( "b" );
        a.selfConcat( '+' );
        b.selfConcat( '+' );
        sequ( a, "a+" );
        sequ( b, "b+" );

        assert.equal( intercept.length, 1, "modification of b should be counted: it is created after" ); 
        assert.equal( intercept[ 0 ], a, "interception of the right variable" );
    });

    it( 'operationnal tranform, basic sending', () => {
        test_ot<LvString>( LvString, 2, ( vls, dbs ) => {
            vls[ 0 ].selfConcat( "a" );
        }, "a" );

        test_ot<LvString>( LvString, 2, ( vls, dbs ) => {
            vls[ 1 ].selfConcat( "a" );
        }, "a" );
    });

    // it( 'transformation of a basic string to an OtWrapperString', () => {
    //     let s = new LvString( "s" );
    //     // console.log( s. );
    //     //let db = new Db;
        
    // });

    it( 'operationnal tranform, successive remove or insertion', () => {
        test_ot_str( "a", ( vls, dbs ) => {
            vls[ 1 ].selfConcat( "b" );
        }, "ab" );

        test_ot_str( "abcd", ( vls, dbs ) => {
            vls[ 1 ].remove( 1, 2 );
        }, "ad" );
    });

    it( 'operationnal tranform, parallel remove / insertions', () => {
        test_ot_str( "abcd", ( vls, dbs ) => {
            vls[ 0 ].insert( 1, "X" );
            vls[ 1 ].insert( 3, "Y" );
        }, "aXbcYd" );

        test_ot_str( "abcd", ( vls, dbs ) => {
            vls[ 1 ].insert( 3, "Y" );
            vls[ 0 ].insert( 1, "X" );
        }, "aXbcYd" );

        test_ot_str( "012345", ( vls, dbs ) => {
            vls[ 0 ].insert( 3, "ab" );
            vls[ 1 ].remove( 1, 1 );
        }, "02ab345" );

        test_ot_str( "012345", ( vls, dbs ) => {
            vls[ 1 ].remove( 1, 4 );
            vls[ 0 ].insert( 3, "ab" );
        }, "0ab5" );

        test_ot_str( "012345", ( vls, dbs ) => {
            vls[ 1 ].remove( 4, 1 );
            vls[ 0 ].insert( 3, "ab" );
        }, "012ab35" );


        test_ot_str( "0123456789", ( vls, dbs ) => {
            vls[ 1 ].remove( 1, 2 );
            vls[ 0 ].remove( 5, 2 );
        }, "034789" );

        test_ot_str( "0123456789", ( vls, dbs ) => {
            vls[ 0 ].remove( 1, 4 );
            vls[ 1 ].remove( 3, 4 );
        }, "0789" );

        test_ot_str( "0123456789", ( vls, dbs ) => {
            vls[ 0 ].remove( 1, 8 );
            vls[ 1 ].remove( 3, 4 );
        }, "09" );

        test_ot_str( "0123456789", ( vls, dbs ) => {
            vls[ 0 ].remove( 7, 2 );
            vls[ 1 ].remove( 1, 2 );
        }, "034569" );

        test_ot_str( "0123456789", ( vls, dbs ) => {
            vls[ 1 ].remove( 3, 4 );
            vls[ 0 ].remove( 1, 4 );
        }, "0789" );

        test_ot_str( "0123456789", ( vls, dbs ) => {
            vls[ 1 ].remove( 3, 4 );
            vls[ 0 ].remove( 1, 8 );
        }, "09" );

    });

    it( 'basic rights', () => {
        const pr = [ 'add_usr_right', 'rem_usr_right', 'read', 'insert', 'remove', 'append' ];
        // basic string
        let s = new LvString( "s" );
        sequ( s.getPossibleRights(), pr );
        sequ( s.getUsersInAccessConstrolList(), [ new UsrId ] );
        sequ( s.getUsrRights( new UsrId ), pr );
        sequ( s.getUsrRights( new UsrId( new DevId, 2 ) ), [] );

        // OtWrapper
        s.onChange( val => console.log( "changed:", val.toString() ) );
        sequ( s.getPossibleRights(), pr );
        sequ( s.getUsersInAccessConstrolList(), [ new UsrId ] );
        sequ( s.getUsrRights( new UsrId ), pr );
        sequ( s.getUsrRights( new UsrId( new DevId, 2 ) ), [] );

        s.remUsrRight( "insert" );
        sequ( s.getUsrRights( new UsrId ), pr.filter( x => x != "insert" ) );
        s.insert( 0, "m" );
        sequ( s, "s" );
        s.insert( 1, "murf" ); // we still have to right to append
        sequ( s, "smurf" );

        s.addUsrRight( "insert" );
        sequ( s.getUsrRights( new UsrId ), pr );
        s.remUsrRight( "insert" );

        s.remUsrRight( "add_usr_right" );
        sequ( s.getUsrRights( new UsrId ), pr.filter( x => x != "insert" && x != "add_usr_right" ) );
        s.addUsrRight( "insert" ); // should fail
        sequ( s.getUsrRights( new UsrId ), pr.filter( x => x != "insert" && x != "add_usr_right" ) );
    });

    it( 'operationnal tranform, rights', () => {
        test_ot<LvString>( LvString, 2, ( vls, dbs ) => {
        }, " add_usr_right rem_usr_right read insert remove append", undefined, true );

        test_ot<LvString>( LvString, 2, ( vls, dbs ) => {
            vls[ 0 ].remUsrRight( "read" );
        }, " add_usr_right rem_usr_right insert remove append", undefined, true );

        test_ot<LvString>( LvString, 2, ( vls, dbs ) => {
            vls[ 0 ].remUsrRight( "rem_usr_right" );
            vls[ 1 ].remUsrRight( "read" ); // will be canceled
        }, " add_usr_right read insert remove append", undefined, true );
    });

    // it( 'operationnal tranform, cursor', () => {
    //     // cursor
    //     tpc( String, function( s ) {
    //         s.m[ 1 ].d.new_cursor( [ Caret.New( 2 ) ] );
    //     }, "0123", "0123 C({beg:2,end:2,foc:1})" );
    
    //     tpc( String, function( s ) {
    //         s.m[ 0 ].d.insert( 1, "a" );
    //         s.m[ 1 ].d.new_cursor( [ Caret.New( 2 ) ] );
    //     }, "0123", "0a123 C({beg:3,end:3,foc:1})" );
    
    //     tpc( String, function( s ) {
    //         s.m[ 0 ].d.new_cursor( [ Caret.New( 2 ) ] );
    //         s.m[ 1 ].d.insert( 1, "a" );
    //     }, "0123", "0a123 C({beg:3,end:3,foc:1})" );
    
    //     tpc( String, function( s ) {
    //         s.m[ 0 ].d.remove( 1, 2 );
    //         s.m[ 1 ].d.new_cursor( [ Caret.New( 2 ) ] );
    //     }, "0123", "03 C({beg:1,end:1,foc:1})" );
    
    //     tpc( String, function( s ) {
    //         s.m[ 0 ].d.new_cursor( [ Caret.New( 2 ) ] );
    //         s.m[ 1 ].d.remove( 1, 2 );
    //     }, "0123", "03 C({beg:1,end:1,foc:1})" );
    // });
});
 

