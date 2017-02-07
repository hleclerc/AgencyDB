import RpDirectory  from "./RpDirectory"
import Inode        from "../System/Inode"
import BinaryWriter from "../System/BinaryWriter"
import BinaryReader from "../System/BinaryReader"
import UsrId        from "../System/UsrId"
import DevId        from "../System/DevId"

var right_to = {
    AddUsrRight: function( val: RpDirectory, _as_usr: UsrId, usr: UsrId, flags: number ): boolean { var _r,_f=val.right_flags.get(_as_usr);_r=_f&1;return _r; },
    RemUsrRight: function( val: RpDirectory, _as_usr: UsrId, usr: UsrId, flags: number ): boolean { var _r,_f=val.right_flags.get(_as_usr);_r=_f&2;return _r; },
    SetVal: function( val: RpDirectory, _as_usr: UsrId, name: string, inode: Inode ): boolean { var _r,_f=val.right_flags.get(_as_usr);_r=_f&4;return _r; },
    Delete: function( val: RpDirectory, _as_usr: UsrId, name: string ): boolean { var _r,_f=val.right_flags.get(_as_usr);_r=_f&16;return _r; },
    DelUnd: function( val: RpDirectory, _as_usr: UsrId, name: string, inode: Inode ): boolean { var _r,_f=val.right_flags.get(_as_usr);_r=_f&16;return _r; },
}

var apply = {
    AddUsrRight: function( val: RpDirectory, usr: UsrId, flags: number ) { val.right_flags.set(usr,val.right_flags.get(usr)|flags); },
    RemUsrRight: function( val: RpDirectory, usr: UsrId, flags: number ) { val.right_flags.set(usr,val.right_flags.get(usr)&~flags); },
    SetVal: function( val: RpDirectory, name: string, inode: Inode ) { val.set(name,inode); },
    Delete: function( val: RpDirectory, name: string ) { val.delete(name); },
    DelUnd: function( val: RpDirectory, name: string, inode: Inode ) { val.delete(name); },
}

var reg = {
    AddUsrRight: function( bw: BinaryWriter, val: RpDirectory, usr: UsrId, flags: number ) { let T0=flags&~val.right_flags.get(usr);if(T0){bw.write_PI8(0);bw.write_obj(usr);bw.write_PT(T0);} },
    RemUsrRight: function( bw: BinaryWriter, val: RpDirectory, usr: UsrId, flags: number ) { let T0=flags&val.right_flags.get(usr);if(T0){bw.write_PI8(1);bw.write_obj(usr);bw.write_PT(T0);} },
    SetVal: function( bw: BinaryWriter, val: RpDirectory, name: string, inode: Inode ) { bw.write_PI8(2);bw.write_String(name);bw.write_obj(inode); },
    Delete: function( bw: BinaryWriter, val: RpDirectory, name: string ) { bw.write_PI8(3);bw.write_String(name); },
    DelUnd: function( bw: BinaryWriter, val: RpDirectory, name: string, inode: Inode ) { bw.write_PI8(4);bw.write_String(name);bw.write_obj(inode); },
}

/**  test rights, apply and register */
var tar = {
    AddUsrRight: function( val: RpDirectory, _as_usr: UsrId, usr: UsrId, flags: number ): Uint8Array { if ( ! right_to.AddUsrRight(val,_as_usr,usr, flags) )return null;let bw=new BinaryWriter;reg.AddUsrRight(bw,val,usr, flags);apply.AddUsrRight(val,usr, flags);return bw.to_Uint8Array(); },
    RemUsrRight: function( val: RpDirectory, _as_usr: UsrId, usr: UsrId, flags: number ): Uint8Array { if ( ! right_to.RemUsrRight(val,_as_usr,usr, flags) )return null;let bw=new BinaryWriter;reg.RemUsrRight(bw,val,usr, flags);apply.RemUsrRight(val,usr, flags);return bw.to_Uint8Array(); },
    SetVal: function( val: RpDirectory, _as_usr: UsrId, name: string, inode: Inode ): Uint8Array { if ( ! right_to.SetVal(val,_as_usr,name, inode) )return null;let bw=new BinaryWriter;reg.SetVal(bw,val,name, inode);apply.SetVal(val,name, inode);return bw.to_Uint8Array(); },
    Delete: function( val: RpDirectory, _as_usr: UsrId, name: string ): Uint8Array { if ( ! right_to.Delete(val,_as_usr,name) )return null;let bw=new BinaryWriter;reg.Delete(bw,val,name);apply.Delete(val,name);return bw.to_Uint8Array(); },
    DelUnd: function( val: RpDirectory, _as_usr: UsrId, name: string, inode: Inode ): Uint8Array { if ( ! right_to.DelUnd(val,_as_usr,name, inode) )return null;let bw=new BinaryWriter;reg.DelUnd(bw,val,name, inode);apply.DelUnd(val,name, inode);return bw.to_Uint8Array(); },
}

function read( br: BinaryReader, cb: ( type: string, args: any ) => void, src_dev?: DevId, src_usr?: UsrId, cur_dev?: DevId, cur_usr?: UsrId ) {
    switch ( br.read_PI8() ) {
        case 0: cb( "AddUsrRight", { usr: UsrId.read_from( br, src_dev, src_usr, cur_dev, cur_usr ), flags: br.read_PT() } ); break;
        case 1: cb( "RemUsrRight", { usr: UsrId.read_from( br, src_dev, src_usr, cur_dev, cur_usr ), flags: br.read_PT() } ); break;
        case 2: cb( "SetVal", { name: br.read_String(), inode: Inode.read_from( br, src_dev, cur_dev ) } ); break;
        case 3: cb( "Delete", { name: br.read_String() } ); break;
        case 4: cb( "DelUnd", { name: br.read_String(), inode: Inode.read_from( br, src_dev, cur_dev ) } ); break;
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
            case 2: br.skip_String(); Inode.skip_from( br ); break;
            case 3: br.skip_String(); break;
            case 4: br.skip_String(); Inode.skip_from( br ); break;
        }
    }
    return res;
}

function undo_patch( val: RpDirectory, br: BinaryReader, as_usr: UsrId ) {
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

function new_patch( val: RpDirectory, bw_new: BinaryWriter, br_new: BinaryReader, as_usr: UsrId, cq_unk: BinaryWriter, off_unk: Array<number>, src_dev?: DevId, src_usr?: UsrId, cur_dev?: DevId, cur_usr?: UsrId ) {
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
                    case 2: { // SetVal
                        let name_unk = br_unk.read_String(), inode_unk = Inode.read_from( br_unk );
                        bw_unk.write_PI8( 2 ); bw_unk.write_String( name_unk ); inode_unk.write_to( bw_unk );
                        break;
                    }
                    case 3: { // Delete
                        let name_unk = br_unk.read_String();
                        bw_unk.write_PI8( 3 ); bw_unk.write_String( name_unk );
                        break;
                    }
                    case 4: { // DelUnd
                        let name_unk = br_unk.read_String(), inode_unk = Inode.read_from( br_unk );
                        bw_unk.write_PI8( 4 ); bw_unk.write_String( name_unk ); inode_unk.write_to( bw_unk );
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
                    case 2: { // SetVal
                        let name_unk = br_unk.read_String(), inode_unk = Inode.read_from( br_unk );
                        bw_unk.write_PI8( 2 ); bw_unk.write_String( name_unk ); inode_unk.write_to( bw_unk );
                        break;
                    }
                    case 3: { // Delete
                        let name_unk = br_unk.read_String();
                        bw_unk.write_PI8( 3 ); bw_unk.write_String( name_unk );
                        break;
                    }
                    case 4: { // DelUnd
                        let name_unk = br_unk.read_String(), inode_unk = Inode.read_from( br_unk );
                        bw_unk.write_PI8( 4 ); bw_unk.write_String( name_unk ); inode_unk.write_to( bw_unk );
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
        case 2: { // SetVal
            let name_new = br_new.read_String(), inode_new = Inode.read_from( br_new, src_dev, cur_dev );
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
                        bw_unk.write_PI8( 1 ); usr_unk.write_to( bw_unk ); bw_unk.write_PT( flags_unk );
                        break;
                    }
                    case 2: { // SetVal
                        let name_unk = br_unk.read_String(), inode_unk = Inode.read_from( br_unk );
                        bw_unk.write_PI8( 2 ); bw_unk.write_String( name_unk ); inode_unk.write_to( bw_unk );
                        break;
                    }
                    case 3: { // Delete
                        let name_unk = br_unk.read_String();
                        bw_unk.write_PI8( 3 ); bw_unk.write_String( name_unk );
                        break;
                    }
                    case 4: { // DelUnd
                        let name_unk = br_unk.read_String(), inode_unk = Inode.read_from( br_unk );
                        bw_unk.write_PI8( 4 ); bw_unk.write_String( name_unk ); inode_unk.write_to( bw_unk );
                        break;
                    }
                }
            }
            if ( right_to.SetVal( val, as_usr, name_new, inode_new ) ) {
                reg.SetVal( bw_new, val, name_new, inode_new );
                apply.SetVal( val, name_new, inode_new );
                bw_unk.transfer_to( cq_unk );
                if ( new_bw_new.size )
                    new_patch( val, bw_new, new BinaryReader( new_bw_new.to_Uint8Array() ), as_usr, cq_unk, new_off_unk, src_dev, src_usr, cur_dev, cur_usr );
            }
            break;
        }
        case 3: { // Delete
            let name_new = br_new.read_String();
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
                        bw_unk.write_PI8( 1 ); usr_unk.write_to( bw_unk ); bw_unk.write_PT( flags_unk );
                        break;
                    }
                    case 2: { // SetVal
                        let name_unk = br_unk.read_String(), inode_unk = Inode.read_from( br_unk );
                        bw_unk.write_PI8( 2 ); bw_unk.write_String( name_unk ); inode_unk.write_to( bw_unk );
                        break;
                    }
                    case 3: { // Delete
                        let name_unk = br_unk.read_String();
                        bw_unk.write_PI8( 3 ); bw_unk.write_String( name_unk );
                        break;
                    }
                    case 4: { // DelUnd
                        let name_unk = br_unk.read_String(), inode_unk = Inode.read_from( br_unk );
                        bw_unk.write_PI8( 4 ); bw_unk.write_String( name_unk ); inode_unk.write_to( bw_unk );
                        break;
                    }
                }
            }
            if ( right_to.Delete( val, as_usr, name_new ) ) {
                reg.Delete( bw_new, val, name_new );
                apply.Delete( val, name_new );
                bw_unk.transfer_to( cq_unk );
                if ( new_bw_new.size )
                    new_patch( val, bw_new, new BinaryReader( new_bw_new.to_Uint8Array() ), as_usr, cq_unk, new_off_unk, src_dev, src_usr, cur_dev, cur_usr );
            }
            break;
        }
        case 4: { // DelUnd
            let name_new = br_new.read_String(), inode_new = Inode.read_from( br_new, src_dev, cur_dev );
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
                        bw_unk.write_PI8( 1 ); usr_unk.write_to( bw_unk ); bw_unk.write_PT( flags_unk );
                        break;
                    }
                    case 2: { // SetVal
                        let name_unk = br_unk.read_String(), inode_unk = Inode.read_from( br_unk );
                        bw_unk.write_PI8( 2 ); bw_unk.write_String( name_unk ); inode_unk.write_to( bw_unk );
                        break;
                    }
                    case 3: { // Delete
                        let name_unk = br_unk.read_String();
                        bw_unk.write_PI8( 3 ); bw_unk.write_String( name_unk );
                        break;
                    }
                    case 4: { // DelUnd
                        let name_unk = br_unk.read_String(), inode_unk = Inode.read_from( br_unk );
                        bw_unk.write_PI8( 4 ); bw_unk.write_String( name_unk ); inode_unk.write_to( bw_unk );
                        break;
                    }
                }
            }
            if ( right_to.DelUnd( val, as_usr, name_new, inode_new ) ) {
                reg.DelUnd( bw_new, val, name_new, inode_new );
                apply.DelUnd( val, name_new, inode_new );
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
