import PatchManager    from "../RiotBase/Variable/Ot/PatchManager";
import BinaryReader    from "../RiotBase/System/BinaryReader";
import DevId           from "../RiotBase/System/DevId";
import UsrId           from "../RiotBase/System/UsrId";
import check           from "../RiotBase/System/Check";

export default
function tpc( cl, fun, init, res, nb_machines = 3, log = false ) {
    class Machine {
        constructor( dev_id : DevId ) {
            this.usr_id = new UsrId( new DevId( "h8u858_RnDF~" ), 17 );
            this.dev_id = dev_id;
            this.d      = new cl;
            this.pm     = new PatchManager( this.d );
        }

        usr_id : UsrId;
        dev_id : DevId;
        d      : any; // cl
        pm     : PatchManager;
    }

    class System {
        m : Array<Machine>;

        constructor() {
            let devs = [ "gMWDnGjZfxZ~", "O5HeHcHU1Io~", "ujtameVI3fN~" ];
            this.m = [];
            for( let i = 0; i < nb_machines; ++i )
                this.m.push( new Machine( new DevId( devs[ i ] ) ) );
        }

        sync( excluded = [] ) {
            let lm = [];
            for( let m of this.m )
                if ( excluded.indexOf( m ) < 0 )
                    lm.push( m );

            // get_changes
            let pd = [];
            for( let m0 of lm ) {
                let p = [];
                for( let m1 of lm )
                    p.push( m0 != m1 ? m1.pm.get_changes( m0.dev_id, m1.dev_id, m1.usr_id ) : null ); // m1 -> m0
                pd.push( p );
            }

            // app_changes
            for( let n0 = 0; n0 < lm.length; n0 += 1 ) {
                let m0 = lm[ n0 ];
                for( let n1 = 0; n1 < lm.length; n1 += 1 ) {
                    let m1 = lm[ n1 ];
                    if ( m0 != m1 && pd[ n0 ][ n1 ].byteLength ) {
                        if ( log )
                            console.log( `${ n1 } (${m1.dev_id}) -> ${ n0 } (${m0.dev_id})` ); // : ${ pd[ n0 ][ n1 ] }
                        m0.pm.app_changes( new BinaryReader( pd[ n0 ][ n1 ] ), m1.dev_id, [ m0.dev_id ], m0.dev_id, m0.usr_id );
                    }
                }
            }
        }
    }

    // initialisation of system
    let s = new System;
    s.m[ 0 ].d.set( init );
    s.sync();

    // console.log( "yuk" );
    // s.m.map( function( m ) {
    //     console.log( m.pm.patches.map( ( p ) => `${ p.patch_id.toString( m.dev_id ) }@${ p.date }` ) );
    // } );

    // operations
    if ( log )
        console.log( "fun ----------------------------" );
    fun( s );
    s.sync();

    // s.m.map( function( m ) {
    //     console.log( m.pm.patches.map( ( p ) => `${ p.patch_id.toString( m.dev_id ) }@${ p.data }` ) );
    // } );

    // check that everything is correct
    // console.log( "yul" );
    let lst_ans = s.m.map( function( m ) {
        let res = m.d.toString();

        if ( m.d.get_cursor_ids != null )
            for( let cid of m.d.get_cursor_ids() )
                res += ` C(${ m.d.get_cursor( cid.loc_id, cid.dev_id ) })`;
        // console.log( m.pm.patches.map( ( p ) => `${ p.patch_id.toString( m.dev_id ) }:${ p.date }` ) );

        for( let usr of m.d.get_users_in_acl() )
            if ( ! usr.equ( new UsrId ) )
                for ( let r of m.d.get_usr_rights( usr ) )
                    res += " " + r;

        return res;
    } );

    let ans = lst_ans[ 0 ];
    for( let i = 1; i < lst_ans.length; i += 1 )
        if ( lst_ans[ i ] != lst_ans[ 0 ] )
            ans += ` [${ i }:${ lst_ans[ i ] }]`;
    check( ans, res, 1 );
}
