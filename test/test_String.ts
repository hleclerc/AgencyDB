/// <reference path="../node_modules/@types/mocha/index.d.ts"/>
import DevId      from "../src/System/DevId";
import UsrId      from "../src/System/UsrId";
import { VarAnc } from "../src/Core/Variable";
import Method     from "../src/Core/Method";
import test_ot    from "../src/Test/test_ot";
import LvString   from "../src/LvString";
import Db         from "../src/Db";
import { assert } from "chai";

function sequ( a, b, msg?: string ) {
    assert.equal( a.toString(), b.toString(), msg );
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
        sequ( s.append( "ouille" ), "testouille"  );
        sequ( s.insert( 9, "s" )  , "testouillse" );
        sequ( s.substr( 2, 4 )    , "stou"        );
        sequ( s.substring( 2, 4 ) , "st"          );
        sequ( s.remove( 2, 8 )    , "tee"         );
    });

    it( 'on change', ( done ) => {
        let s = new LvString( "test" );

        let cpt = 0;
        s.onChange( () => ++cpt );
        s.append( "_" );
        s.append( "_" );
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
        a.append( '+' );
        b.append( '+' );
        sequ( a, "a+" );
        sequ( b, "b+" );

        assert.equal( intercept.length, 1, "modification of b should be counted: it is created after" ); 
        assert.equal( intercept[ 0 ], a, "interception of the right variable" );
    });

    it( 'operationnal tranform, sending', () => {
        test_ot<LvString>( LvString, 2, ( vls, dbs ) => {
            vls[ 0 ].append( "a" );
        }, "a", "send from 0" );

        test_ot<LvString>( LvString, 2, ( vls, dbs ) => {
            vls[ 1 ].append( "a" );
        }, "a", "send from 1" );
    });

    it( 'operationnal tranform, succession remove / insertions', () => {
        test_ot<LvString>( LvString, 2, ( vls, dbs ) => {
            vls[ 0 ].append( "a" );
            dbs[ 0 ].send_changes();
            vls[ 1 ].append( "b" );
        }, "ab", "succession of insertions" );

        test_ot<LvString>( LvString, 2, ( vls, dbs ) => {
            vls[ 0 ].append( "abcd" );
            dbs[ 0 ].send_changes();
            vls[ 1 ].remove( 1, 2 );
        }, "ad", "succession of insertion + removal" );
    });

    it( 'operationnal tranform, parallel remove / insertions', () => {
        test_ot<LvString>( LvString, 2, ( vls, dbs ) => {
            vls[ 0 ].insert( 0, "abcd" );
            dbs.forEach( x => x.send_changes() );
            vls[ 0 ].insert( 1, "X" );
            vls[ 1 ].insert( 3, "Y" );
        }, "aXbcYd", "concurrent insertions" );

        test_ot<LvString>( LvString, 2, ( vls, dbs ) => {
            vls[ 0 ].insert( 0, "abcd" );
            dbs.forEach( x => x.send_changes() );
            vls[ 1 ].insert( 3, "Y" );
            vls[ 0 ].insert( 1, "X" );
        }, "aXbcYd", "concurrent insertions" );

        // test_ot<LvString>( LvString, 2, ( vls, dbs ) => {
        //     vls[ 0 ].insert( 0, "012345" );
        //     dbs.forEach( x => x.send_changes() );
        //     vls[ 0 ].insert( 3, "ab" );
        //     vls[ 1 ].remove( 1, 1 );
        // }, "02ab345", "concurrent insertion and removal" );
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

        s.remUsrRight( new UsrId, "insert" );
        sequ( s.getUsrRights( new UsrId ), pr.filter( x => x != "insert" ) );
        s.insert( 0, "m" );
        sequ( s, "s" );
        s.insert( 1, "murf" ); // we still have to right to append
        sequ( s, "smurf" );

        s.addUsrRight( new UsrId, "insert" );
        sequ( s.getUsrRights( new UsrId ), pr );
        s.remUsrRight( new UsrId, "insert" );

        s.remUsrRight( new UsrId, "add_usr_right" );
        sequ( s.getUsrRights( new UsrId ), pr.filter( x => x != "insert" && x != "add_usr_right" ) );
        s.addUsrRight( new UsrId, "insert" ); // should fail
        sequ( s.getUsrRights( new UsrId ), pr.filter( x => x != "insert" && x != "add_usr_right" ) );
    });

    // it( 'operationnal tranform, rights', () => {
    //     const pr = [ 'add_usr_right', 'rem_usr_right', 'read', 'insert', 'remove', 'append' ];
    //     // basic string
    //     let s = new LvString( "s" );
    //     sequ( s.getPossibleRights(), pr );
    //     sequ( s.getUsersInAccessConstrolList(), [ new UsrId ] );
    //     sequ( s.getUsrRights( new UsrId ), pr );
    //     sequ( s.getUsrRights( new UsrId( new DevId, 2 ) ), [] );

    //     // OtWrapper
    //     s.onChange( val => console.log( "changed:", val.toString() ) );
    //     sequ( s.getPossibleRights(), pr );
    //     sequ( s.getUsersInAccessConstrolList(), [ new UsrId ] );
    //     sequ( s.getUsrRights( new UsrId ), pr );
    //     sequ( s.getUsrRights( new UsrId( new DevId, 2 ) ), [] );

    //     s.remUsrRight( new UsrId, [ "insert" ] );
    //     sequ( s.getUsrRights( new UsrId ), pr.filter( x => x != "insert" ) );
    // });
});
 

