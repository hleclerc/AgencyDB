import { VarAnc } from "../Core/Variable"
import DevId      from "../System/DevId"
import Db         from "../Db"
import { assert } from "chai";

function sequ( a, b, msg?: string ) {
    assert.equal( a.toString(), b.toString(), msg );
}

export default
function test_ot<type extends VarAnc>( type: typeof VarAnc, nb_dbs: number, operations: ( vars: Array<type>, dbs: Array<Db> ) => void, expected: string, explanation = operations.toString(), disp_rights = false ) {
    const dev_ids = [ "0MWDnGjZfxZ~", "15HeHcHU1Io~", "28u858_RnDF~", "3jtameVI3fN~", "4mWRNBUMN7o~" ];
    let dbs = new Array<Db>();
    for( let i = 0; i < nb_dbs; ++i ) {
        let db = new Db;
        db.dev_id = new DevId( dev_ids[ i ] );
        dbs.push( db );
    }
    for( let i = 0; i < nb_dbs; ++i )
        for( let j = 0; j < nb_dbs; ++j )
            if ( i != j )
                dbs[ i ].connect( dbs[ j ] );
    
    let id = dbs[ 0 ].new_ItemId();
    let vars = new Array<type>();
    for( let i = 0; i < nb_dbs; ++i ) {
        let v = dbs[ i ].bind<type>( type, id );
        vars.push( v );
    }

    operations( vars, dbs );

    for( let i = 0; i < nb_dbs; ++i )
        dbs[ i ].send_changes();

    // for( let i = 0; i < nb_dbs; ++i )
    //     console.log( vars[ i ].toString() );
    for( let i = 0; i < nb_dbs; ++i ) {
        let str = vars[ i ].toString();
        if ( disp_rights )
            str += " " + vars[ i ].getUsrRights().join( " " );
        sequ( str, expected, `var n°${ i }: ${ explanation }` );
    }
}
