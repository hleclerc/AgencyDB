import OtWrapperString from "./OtWrapperString"
import BinaryWriter    from "../System/BinaryWriter"
import BinaryReader    from "../System/BinaryReader"
import UsrId           from "../System/UsrId"
import DevId           from "../System/DevId"

var bin_repr = {
    AddUsrRight: function( bw: BinaryWriter, usr: UsrId, flags: number ): void { bw.write_PI8( 0 ); usr.write_to( bw ); bw.write_PT( flags ); },
    RemUsrRight: function( bw: BinaryWriter, usr: UsrId, flags: number ): void { bw.write_PI8( 1 ); usr.write_to( bw ); bw.write_PT( flags ); },
    Insert: function( bw: BinaryWriter, pos: number, str: string ): void { bw.write_PI8( 2 ); bw.write_PT( pos ); bw.write_String( str ); },
    Remove: function( bw: BinaryWriter, pos: number, len: number ): void { bw.write_PI8( 3 ); bw.write_PT( pos ); bw.write_PT( len ); },
    RemUnd: function( bw: BinaryWriter, pos: number, str: string ): void { bw.write_PI8( 4 ); bw.write_PT( pos ); bw.write_String( str ); },
}

var right_to = {
    AddUsrRight: function( val: OtWrapperString, _as_usr: UsrId, usr: UsrId, flags: number ): boolean { var _r,_f=val.right_flags.get(_as_usr);_r=_f&1;return _r; },
    RemUsrRight: function( val: OtWrapperString, _as_usr: UsrId, usr: UsrId, flags: number ): boolean { var _r,_f=val.right_flags.get(_as_usr);_r=_f&2;return _r; },
    Insert: function( val: OtWrapperString, _as_usr: UsrId, pos: number, str: string ): boolean { var _r,_f=val.right_flags.get(_as_usr);_r=_f&8||_f&32&&pos==val.val.data.length;return _r; },
    Remove: function( val: OtWrapperString, _as_usr: UsrId, pos: number, len: number ): boolean { var _r,_f=val.right_flags.get(_as_usr);_r=_f&16;return _r; },
    RemUnd: function( val: OtWrapperString, _as_usr: UsrId, pos: number, str: string ): boolean { var _r,_f=val.right_flags.get(_as_usr);_r=_f&16;return _r; },
}

function read( br: BinaryReader, cb: ( type: string, args: any ) => void, src_dev?: DevId, src_usr?: UsrId, cur_dev?: DevId, cur_usr?: UsrId ) {
    switch ( br.read_PI8() ) {
        case 0: cb( "AddUsrRight", { usr: UsrId.read_from( br, src_dev, src_usr, cur_dev, cur_usr ), flags: br.read_PT() } ); break;
        case 1: cb( "RemUsrRight", { usr: UsrId.read_from( br, src_dev, src_usr, cur_dev, cur_usr ), flags: br.read_PT() } ); break;
        case 2: cb( "Insert", { pos: br.read_PT(), str: br.read_String() } ); break;
        case 3: cb( "Remove", { pos: br.read_PT(), len: br.read_PT() } ); break;
        case 4: cb( "RemUnd", { pos: br.read_PT(), str: br.read_String() } ); break;
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
            case 2: br.skip_PT(); br.skip_String(); break;
            case 3: br.skip_PT(); br.skip_PT(); break;
            case 4: br.skip_PT(); br.skip_String(); break;
        }
    }
    return res;
}

function undo_patch( val: OtWrapperString, br: BinaryReader, as_usr: UsrId ) {
    const res = skip( br );
    for( let n = res.length; n--; ) {
        br.cursor = res[ n ];
        switch ( br.read_PI8() ) {
        case 0: {
            let usr = UsrId.read_from( br ), flags = br.read_PT();
            val.right_flags.set(usr,xor_bin(val.right_flags.get(usr),flags));
            break;
        }
        case 1: {
            let usr = UsrId.read_from( br ), flags = br.read_PT();
            val.right_flags.set(usr,xor_bin(val.right_flags.get(usr),flags));
            break;
        }
        case 2: {
            let pos = br.read_PT(), str = br.read_String();
            val.val.data=val.val.data.substr(0,pos)+val.val.data.substr((pos+str.length));
            break;
        }
        case 4: {
            let pos = br.read_PT(), str = br.read_String();
            val.val.data=val.val.data.substr(0,pos)+str+val.val.data.substr(pos);
            break;
        }
        }
    }
    return val;
}

function new_patch( val: OtWrapperString, bw_new: BinaryWriter, br_new: BinaryReader, as_usr: UsrId, cq_unk: BinaryWriter, src_dev?: DevId, src_usr?: UsrId, cur_dev?: DevId, cur_usr?: UsrId ) {
    while ( br_new.size ) {
        switch ( br_new.read_PI8() ) {
        case 0: {
            let usr_new = UsrId.read_from( br_new, src_dev, src_usr, cur_dev, cur_usr ), flags_new = br_new.read_PT();
            if ( right_to.AddUsrRight( val, as_usr, usr_new, flags_new ) ) {
                let br_unk = new BinaryReader( cq_unk.to_Uint8Array() );
                let bw_unk = new BinaryWriter;
                while ( br_unk.size ) {
                    const num_unk = br_unk.read_PI8();
                    switch ( num_unk ) {
                        case 0: {
                            let usr_unk = UsrId.read_from( br_unk ), flags_unk = br_unk.read_PT();
                            bw_unk.write_PI8( 0 ); usr_unk.write_to( bw_unk ); bw_unk.write_PT( flags_unk );
                            break;
                        }
                        case 1: {
                            let usr_unk = UsrId.read_from( br_unk ), flags_unk = br_unk.read_PT();
                            bw_unk.write_PI8( 1 ); usr_unk.write_to( bw_unk ); bw_unk.write_PT( flags_unk );
                            break;
                        }
                        case 2: {
                            let pos_unk = br_unk.read_PT(), str_unk = br_unk.read_String();
                            bw_unk.write_PI8( 2 ); bw_unk.write_PT( pos_unk ); bw_unk.write_String( str_unk );
                            break;
                        }
                        case 3: {
                            let pos_unk = br_unk.read_PT(), len_unk = br_unk.read_PT();
                            bw_unk.write_PI8( 3 ); bw_unk.write_PT( pos_unk ); bw_unk.write_PT( len_unk );
                            break;
                        }
                        case 4: {
                            let pos_unk = br_unk.read_PT(), str_unk = br_unk.read_String();
                            bw_unk.write_PI8( 4 ); bw_unk.write_PT( pos_unk ); bw_unk.write_String( str_unk );
                            break;
                        }
                    }
                }
                {
                    let usr_tmp, flags_tmp;
                    usr_tmp=usr_new;flags_tmp=flags_new&~val.right_flags.get(usr_new);
                    bw_new.write_PI8( 0 ); bw_new.write_null( usr_tmp ); bw_new.write_PT( flags_tmp );
                }
                bw_unk.transfer_to( cq_unk );
                val.right_flags.set(usr_new,val.right_flags.get(usr_new)|flags_new);
            }
            break;
        }
        case 1: {
            let usr_new = UsrId.read_from( br_new, src_dev, src_usr, cur_dev, cur_usr ), flags_new = br_new.read_PT();
            if ( right_to.RemUsrRight( val, as_usr, usr_new, flags_new ) ) {
                let br_unk = new BinaryReader( cq_unk.to_Uint8Array() );
                let bw_unk = new BinaryWriter;
                while ( br_unk.size ) {
                    const num_unk = br_unk.read_PI8();
                    switch ( num_unk ) {
                        case 0: {
                            let usr_unk = UsrId.read_from( br_unk ), flags_unk = br_unk.read_PT();
                            bw_unk.write_PI8( 0 ); usr_unk.write_to( bw_unk ); bw_unk.write_PT( flags_unk );
                            break;
                        }
                        case 1: {
                            let usr_unk = UsrId.read_from( br_unk ), flags_unk = br_unk.read_PT();
                            bw_unk.write_PI8( 1 ); usr_unk.write_to( bw_unk ); bw_unk.write_PT( flags_unk );
                            break;
                        }
                        case 2: {
                            let pos_unk = br_unk.read_PT(), str_unk = br_unk.read_String();
                            bw_unk.write_PI8( 2 ); bw_unk.write_PT( pos_unk ); bw_unk.write_String( str_unk );
                            break;
                        }
                        case 3: {
                            let pos_unk = br_unk.read_PT(), len_unk = br_unk.read_PT();
                            bw_unk.write_PI8( 3 ); bw_unk.write_PT( pos_unk ); bw_unk.write_PT( len_unk );
                            break;
                        }
                        case 4: {
                            let pos_unk = br_unk.read_PT(), str_unk = br_unk.read_String();
                            bw_unk.write_PI8( 4 ); bw_unk.write_PT( pos_unk ); bw_unk.write_String( str_unk );
                            break;
                        }
                    }
                }
