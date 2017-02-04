import MapWithStringifiedKeys from "../Core/MapWithStringifiedKeys";
import UsrId                  from "../System/UsrId";
import OtWrapper              from "./OtWrapper"

class MapRights extends MapWithStringifiedKeys<UsrId,number> {
    get( key: UsrId ): number {
        return this.map.get( key.toString() ) || 0;
    }
}

abstract class OtWrapperWithRightFlags extends OtWrapper {
    constructor( cl, usr: UsrId ) {
        super( cl );
        this.right_flags.set( usr, Math.pow( 2, this.get_possible_rights__b().length ) - 1 );

        // first patch
        // let str = this.val.toString();
        // if ( str.length )
        //     OtWrapperString.reg_op( this.last_ops, OtWrapperString.op_insert, 0, str );
    }

    static make_templ( cl ) {
        OtWrapper.make_templ( cl );
    }

    abstract get_possible_rights__b(): Array<string>;

    get_users_in_acl__b(): Array<UsrId> {
        let res = new Array<UsrId>();
        this.right_flags.map.forEach( ( val, key ) => res.push( UsrId.fromString( key ) ) );
        return res;
    }

    get_usr_rights__b( usr: UsrId ): Array<UsrId> {
        let flags = this.right_flags.get( usr ), res = [], cmp = 1;
        if ( flags ) {
            this.get_possible_rights__b().forEach( val => {
                if ( flags & cmp )
                    res.push( val );
                cmp <<= 1;
            } );
        }
        return res;
    }

    abstract add_usr_right__s( usr: UsrId, right_types: Array<string>, as_usr?: UsrId );
    abstract rem_usr_right__s( usr: UsrId, right_types: Array<string>, as_usr?: UsrId );

    _right_flags_from_right_type( right_types: Array<string> ): number {
        let flags = 0, cmp = 1, cp_rt = [ ...right_types ];
        this.get_possible_rights__b().forEach( val => {
            for( let ind; ( ind = cp_rt.indexOf( val ) ) >= 0; ) {
                cp_rt.splice( ind, 1 );
                flags |= cmp;
            }
            cmp <<= 1;
        } );
        for( const right_type of cp_rt )
            console.error( `Error: unknown right type '${ right_type }'` );
        return flags;
    }

    right_flags = new MapRights; /** usr_id => flags */
}
export default OtWrapperWithRightFlags;
