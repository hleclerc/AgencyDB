import MapWithStringifiedKeys from "../Core/MapWithStringifiedKeys";
import UsrId                  from "../System/UsrId";
import OtWrapper              from "./OtWrapper"

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

    add_usr_right__s( usr: UsrId, right_types: Array<string>, as_usr = new UsrId ) {
        let flags = this._right_flags_from_right_type( right_types );
        if ( flags ) {
            const old_val = this.right_flags.get( usr ) || 0, new_val = old_val | flags;
            if ( new_val != old_val && ( this.right_flags.get( as_usr ) || 0 ) & Math.pow( 2, this.get_possible_rights__b().indexOf( "add_usr_right" ) ) ) {
                this.right_flags.set( usr, new_val );
            }
        }
        return this;
    }

    rem_usr_right__s( usr: UsrId, right_types: Array<string>, as_usr = new UsrId ) {
        let flags = this._right_flags_from_right_type( right_types );
        if ( flags ) {
            const old_val = this.right_flags.get( usr ) || 0, new_val = old_val & ~flags;
            if ( new_val != old_val && ( this.right_flags.get( as_usr ) || 0 ) & Math.pow( 2, this.get_possible_rights__b().indexOf( "rem_usr_right" ) ) ) {
                if ( new_val )
                    this.right_flags.set( usr, new_val );
                else
                    this.right_flags.delete( usr );
            }
        }
        return this;
    }

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

    right_flags = new MapWithStringifiedKeys<UsrId,number>(); /** usr_id => flags */
}
export default OtWrapperWithRightFlags;
