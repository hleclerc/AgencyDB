import RpMap from "./RpMap"
import BinaryWriter from "../System/BinaryWriter"
import BinaryReader from "../System/BinaryReader"
import UsrId        from "../System/UsrId"
import DevId        from "../System/DevId"

var right_to = {
    AddUsrRight: function( val: RpMap, _as_usr: UsrId, usr: UsrId, flags: number ): boolean { var _r,_f=val.right_flags.get(_as_usr);_r=_f&1;return _r; },
    RemUsrRight: function( val: RpMap, _as_usr: UsrId, usr: UsrId, flags: number ): boolean { var _r,_f=val.right_flags.get(_as_usr);_r=_f&2;return _r; },
}

var apply = {
    AddUsrRight: function( val: RpMap, usr: UsrId, flags: number ) { val.right_flags.set(usr,val.right_flags.get(usr)|flags); },
    RemUsrRight: function( val: RpMap, usr: UsrId, flags: number ) { val.right_flags.set(usr,val.right_flags.get(usr)&~flags); },
}

var reg = {
    AddUsrRight: function( bw: BinaryWriter, val: RpMap, usr: UsrId, flags: number ) { let T0=flags&~val.right_flags.get(usr);if(T0){bw.write_PI8(0);bw.write_obj(usr);bw.write_PT(T0);} },
    RemUsrRight: function( bw: BinaryWriter, val: RpMap, usr: UsrId, flags: number ) { let T0=flags&val.right_flags.get(usr);if(T0){bw.write_PI8(1);bw.write_obj(usr);bw.write_PT(T0);} },
}

/**  test rights, apply and register */
var tar = {
    AddUsrRight: function( val: RpMap, _as_usr: UsrId, usr: UsrId, flags: number ): Uint8Array { if ( ! right_to.AddUsrRight(val,_as_usr,usr, flags) )return null;let bw=new BinaryWriter;reg.AddUsrRight(bw,val,usr, flags);apply.AddUsrRight(val,usr, flags);return bw.to_Uint8Array(); },
    RemUsrRight: function( val: RpMap, _as_usr: UsrId, usr: UsrId, flags: number ): Uint8Array { if ( ! right_to.RemUsrRight(val,_as_usr,usr, flags) )return null;let bw=new BinaryWriter;reg.RemUsrRight(bw,val,usr, flags);apply.RemUsrRight(val,usr, flags);return bw.to_Uint8Array(); },
}

function read( br: BinaryReader, cb: ( type: string, args: any ) => void, src_dev?: DevId, src_usr?: UsrId, cur_dev?: DevId, cur_usr?: UsrId ) {
    switch ( br.read_PI8() ) {
        case 0: cb( "AddUsrRight", { usr: UsrId.read_from( br, src_dev, src_usr, cur_dev, cur_usr ), flags: br.read_PT() } ); break;
        case 1: cb( "RemUsrRight", { usr: UsrId.read_from( br, src_dev, src_usr, cur_dev, cur_usr ), flags: br.read_PT() } ); break;
        default: cb( null, {} ); br.clear(); break;
    }
}

function skip( br: BinaryReader ): Array<number> {
    let res = new Array<number>();
    while ( br.size ) {
        res.push( br.cursor );
        switch ( br.read_PI8() ) {
            case 0: UsrId.skip_from( br ); br.skip_PT(); break;
            case 1: UsrId.skip_from( br ); br.skip_PT(); break;
        }
    }
    return res;
}

function undo_patch( val: RpMap, br: BinaryReader, as_usr: UsrId ) {
    const res = skip( br );
    for( let n = res.length; n--; ) {
        br.cursor = res[ n ];
        switch ( br.read_PI8() ) {
        case 0: { // AddUsrRight
            let usr = UsrId.read_from( br ), flags = br.read_PT();
            val.right_flags.set(usr,val.right_flags.get(usr)^flags);
            break;
        }
        case 1: { // RemUsrRight
            let usr = UsrId.read_from( br ), flags = br.read_PT();
            val.right_flags.set(usr,val.right_flags.get(usr)^flags);
            break;
        }
        }
    }
    return val;
}

function new_patch( val: RpMap, bw_new: BinaryWriter, br_new: BinaryReader, as_usr: UsrId, cq_unk: BinaryWriter, off_unk: Array<number>, src_dev?: DevId, src_usr?: UsrId, cur_dev?: DevId, cur_usr?: UsrId ) {
    for( let nbr_unk = 0; br_new.size; ++nbr_unk ) {
        let br_unk = new BinaryReader( cq_unk.to_Uint8Array(), off_unk ? off_unk[ nbr_unk ] : 0 );
        let bw_unk = new BinaryWriter, new_bw_new = new BinaryWriter, new_off_unk = new Array<number>();
        switch ( br_new.read_PI8() ) {
        case 0: { // AddUsrRight
            let usr_new = UsrId.read_from( br_new, src_dev, src_usr, cur_dev, cur_usr ), flags_new = br_new.read_PT();
            while ( br_unk.size ) {
                const num_unk = br_unk.read_PI8();
                switch ( num_unk ) {
                    case 0: { // AddUsrRight
                        let usr_unk = UsrId.read_from( br_unk ), flags_unk = br_unk.read_PT();
                        if(usr_unk==usr_new){flags_new&=~flags_unk;flags_unk&=~flags_new;}
                        bw_unk.write_PI8( 0 ); usr_unk.write_to( bw_unk ); bw_unk.write_PT( flags_unk );
                        break;
                    }
                    case 1: { // RemUsrRight
                        let usr_unk = UsrId.read_from( br_unk ), flags_unk = br_unk.read_PT();
                        bw_unk.write_PI8( 1 ); usr_unk.write_to( bw_unk ); bw_unk.write_PT( flags_unk );
                        break;
                    }
                }
            }
            if ( right_to.AddUsrRight( val, as_usr, usr_new, flags_new ) ) {
                reg.AddUsrRight( bw_new, val, usr_new, flags_new );
                apply.AddUsrRight( val, usr_new, flags_new );
                bw_unk.transfer_to( cq_unk );
                if ( new_bw_new.size )
                    new_patch( val, bw_new, new BinaryReader( new_bw_new.to_Uint8Array() ), as_usr, cq_unk, new_off_unk, src_dev, src_usr, cur_dev, cur_usr );
            }
            break;
        }
        case 1: { // RemUsrRight
            let usr_new = UsrId.read_from( br_new, src_dev, src_usr, cur_dev, cur_usr ), flags_new = br_new.read_PT();
            while ( br_unk.size ) {
                const num_unk = br_unk.read_PI8();
                switch ( num_unk ) {
                    case 0: { // AddUsrRight
                        let usr_unk = UsrId.read_from( br_unk ), flags_unk = br_unk.read_PT();
                        bw_unk.write_PI8( 0 ); usr_unk.write_to( bw_unk ); bw_unk.write_PT( flags_unk );
                        break;
                    }
                    case 1: { // RemUsrRight
                        let usr_unk = UsrId.read_from( br_unk ), flags_unk = br_unk.read_PT();
                        if(usr_unk==usr_new){flags_new&=~flags_unk;flags_unk&=~flags_new;}
                        bw_unk.write_PI8( 1 ); usr_unk.write_to( bw_unk ); bw_unk.write_PT( flags_unk );
                        break;
                    }
                }
            }
            if ( right_to.RemUsrRight( val, as_usr, usr_new, flags_new ) ) {
                reg.RemUsrRight( bw_new, val, usr_new, flags_new );
                apply.RemUsrRight( val, usr_new, flags_new );
                bw_unk.transfer_to( cq_unk );
                if ( new_bw_new.size )
                    new_patch( val, bw_new, new BinaryReader( new_bw_new.to_Uint8Array() ), as_usr, cq_unk, new_off_unk, src_dev, src_usr, cur_dev, cur_usr );
            }
            break;
        }
        }
    }
    return val;
}

function get_possible_rights__b(): Array<string> {
    return ["add_usr_right","rem_usr_right","set","add","delete"];
}

export default { read, right_to, apply, reg, tar, new_patch, undo_patch, get_possible_rights__b };
