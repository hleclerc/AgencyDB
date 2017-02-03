import OtWrapperString from "./OtWrapperString"
import BinaryWriter from "../System/BinaryWriter"
import BinaryReader from "../System/BinaryReader"
import UsrId        from "../System/UsrId"
import DevId        from "../System/DevId"

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
        case 2: { // Insert
            let pos = br.read_PT(), str = br.read_String();
            val.val.data=val.val.data.substr(0,pos)+val.val.data.substr(pos+str.length);
            break;
        }
        case 4: { // RemUnd
            let pos = br.read_PT(), str = br.read_String();
            val.val.data=val.val.data.substr(0,pos)+str+val.val.data.substr(pos);
            break;
        }
        }
    }
    return val;
}

function new_patch( val: OtWrapperString, bw_new: BinaryWriter, br_new: BinaryReader, as_usr: UsrId, cq_unk: BinaryWriter, off_unk: Array<number>, src_dev?: DevId, src_usr?: UsrId, cur_dev?: DevId, cur_usr?: UsrId ) {
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
                        console.log( 'AddUsrRight', 'AddUsrRight' );
                        let usr_unk = UsrId.read_from( br_unk ), flags_unk = br_unk.read_PT();
                        if(usr_unk==usr_new){flags_new&=~flags_unk;flags_unk&=~flags_new;}
                        bw_unk.write_PI8( 0 ); usr_unk.write_to( bw_unk ); bw_unk.write_PT( flags_unk );
                        break;
                    }
                    case 1: { // RemUsrRight
                        console.log( 'RemUsrRight', 'AddUsrRight' );
                        let usr_unk = UsrId.read_from( br_unk ), flags_unk = br_unk.read_PT();
                        bw_unk.write_PI8( 1 ); usr_unk.write_to( bw_unk ); bw_unk.write_PT( flags_unk );
                        break;
                    }
                    case 2: { // Insert
                        console.log( 'Insert', 'AddUsrRight' );
                        let pos_unk = br_unk.read_PT(), str_unk = br_unk.read_String();
                        bw_unk.write_PI8( 2 ); bw_unk.write_PT( pos_unk ); bw_unk.write_String( str_unk );
                        break;
                    }
                    case 3: { // Remove
                        console.log( 'Remove', 'AddUsrRight' );
                        let pos_unk = br_unk.read_PT(), len_unk = br_unk.read_PT();
                        bw_unk.write_PI8( 3 ); bw_unk.write_PT( pos_unk ); bw_unk.write_PT( len_unk );
                        break;
                    }
                    case 4: { // RemUnd
                        console.log( 'RemUnd', 'AddUsrRight' );
                        let pos_unk = br_unk.read_PT(), str_unk = br_unk.read_String();
                        bw_unk.write_PI8( 4 ); bw_unk.write_PT( pos_unk ); bw_unk.write_String( str_unk );
                        break;
                    }
                }
            }
            console.log( usr_new, flags_new );
            if ( right_to.AddUsrRight( val, as_usr, usr_new, flags_new ) ) {
                {
                    let usr_tmp, flags_tmp;
                    usr_tmp=usr_new;flags_tmp=flags_new&~val.right_flags.get(usr_new);
                    bw_new.write_PI8( 0 );usr_tmp.write_to( bw_new ); bw_new.write_PT( flags_tmp );
                }
                bw_unk.transfer_to( cq_unk );
                val.right_flags.set(usr_new,val.right_flags.get(usr_new)|flags_new);
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
                        console.log( 'AddUsrRight', 'RemUsrRight' );
                        let usr_unk = UsrId.read_from( br_unk ), flags_unk = br_unk.read_PT();
                        bw_unk.write_PI8( 0 ); usr_unk.write_to( bw_unk ); bw_unk.write_PT( flags_unk );
                        break;
                    }
                    case 1: { // RemUsrRight
                        console.log( 'RemUsrRight', 'RemUsrRight' );
                        let usr_unk = UsrId.read_from( br_unk ), flags_unk = br_unk.read_PT();
                        if(usr_unk==usr_new){flags_new&=~flags_unk;flags_unk&=~flags_new;}
                        bw_unk.write_PI8( 1 ); usr_unk.write_to( bw_unk ); bw_unk.write_PT( flags_unk );
                        break;
                    }
                    case 2: { // Insert
                        console.log( 'Insert', 'RemUsrRight' );
                        let pos_unk = br_unk.read_PT(), str_unk = br_unk.read_String();
                        bw_unk.write_PI8( 2 ); bw_unk.write_PT( pos_unk ); bw_unk.write_String( str_unk );
                        break;
                    }
                    case 3: { // Remove
                        console.log( 'Remove', 'RemUsrRight' );
                        let pos_unk = br_unk.read_PT(), len_unk = br_unk.read_PT();
                        bw_unk.write_PI8( 3 ); bw_unk.write_PT( pos_unk ); bw_unk.write_PT( len_unk );
                        break;
                    }
                    case 4: { // RemUnd
                        console.log( 'RemUnd', 'RemUsrRight' );
                        let pos_unk = br_unk.read_PT(), str_unk = br_unk.read_String();
                        bw_unk.write_PI8( 4 ); bw_unk.write_PT( pos_unk ); bw_unk.write_String( str_unk );
                        break;
                    }
                }
            }
            console.log( usr_new, flags_new );
            if ( right_to.RemUsrRight( val, as_usr, usr_new, flags_new ) ) {
                {
                    let usr_tmp, flags_tmp;
                    usr_tmp=usr_new;flags_tmp=flags_new&val.right_flags.get(usr_new);
                    bw_new.write_PI8( 1 );usr_tmp.write_to( bw_new ); bw_new.write_PT( flags_tmp );
                }
                bw_unk.transfer_to( cq_unk );
                val.right_flags.set(usr_new,val.right_flags.get(usr_new)&~flags_new);
                if ( new_bw_new.size )
                    new_patch( val, bw_new, new BinaryReader( new_bw_new.to_Uint8Array() ), as_usr, cq_unk, new_off_unk, src_dev, src_usr, cur_dev, cur_usr );
            }
            break;
        }
        case 2: { // Insert
            let pos_new = br_new.read_PT(), str_new = br_new.read_String();
            while ( br_unk.size ) {
                const num_unk = br_unk.read_PI8();
                switch ( num_unk ) {
                    case 0: { // AddUsrRight
                        console.log( 'AddUsrRight', 'Insert' );
                        let usr_unk = UsrId.read_from( br_unk ), flags_unk = br_unk.read_PT();
                        bw_unk.write_PI8( 0 ); usr_unk.write_to( bw_unk ); bw_unk.write_PT( flags_unk );
                        break;
                    }
                    case 1: { // RemUsrRight
                        console.log( 'RemUsrRight', 'Insert' );
                        let usr_unk = UsrId.read_from( br_unk ), flags_unk = br_unk.read_PT();
                        bw_unk.write_PI8( 1 ); usr_unk.write_to( bw_unk ); bw_unk.write_PT( flags_unk );
                        break;
                    }
                    case 2: { // Insert
                        console.log( 'Insert', 'Insert' );
                        let pos_unk = br_unk.read_PT(), str_unk = br_unk.read_String();
                        if(pos_unk>pos_new){pos_unk+=str_new.length;}else{pos_new+=str_unk.length;}
                        bw_unk.write_PI8( 2 ); bw_unk.write_PT( pos_unk ); bw_unk.write_String( str_unk );
                        break;
                    }
                    case 3: { // Remove
                        console.log( 'Remove', 'Insert' );
                        let pos_unk = br_unk.read_PT(), len_unk = br_unk.read_PT();
                        if(pos_new>=pos_unk+len_unk){pos_new-=len_unk;}else{if(pos_new<=pos_unk){pos_unk+=str_new.length;}else{let T0=len_unk-pos_new-pos_unk,T1=pos_new-pos_unk,T2;pos_new=pos_unk;bw_unk.write_PI8(3);T2=T0;len_unk=T1;bw_unk.write_PT(pos_unk+str_new.length);bw_unk.write_PT(T2);}}
                        bw_unk.write_PI8( 3 ); bw_unk.write_PT( pos_unk ); bw_unk.write_PT( len_unk );
                        break;
                    }
                    case 4: { // RemUnd
                        console.log( 'RemUnd', 'Insert' );
                        let pos_unk = br_unk.read_PT(), str_unk = br_unk.read_String();
                        if(pos_new>=pos_unk+str_unk.length){pos_new-=str_unk.length;}else{if(pos_new<=pos_unk){pos_unk+=str_new.length;}else{let T0=str_unk.substr(pos_new-pos_unk),T1=str_unk.substr(0,pos_new-pos_unk),T2;pos_new=pos_unk;bw_unk.write_PI8(4);T2=T0;str_unk=T1;bw_unk.write_PT(pos_unk+str_new.length);bw_unk.write_String(T2);}}
                        bw_unk.write_PI8( 4 ); bw_unk.write_PT( pos_unk ); bw_unk.write_String( str_unk );
                        break;
                    }
                }
            }
            console.log( pos_new, str_new );
            if ( right_to.Insert( val, as_usr, pos_new, str_new ) ) {
                bw_new.write_PI8( 2 ); bw_new.write_PT( pos_new ); bw_new.write_String( str_new );
                bw_unk.transfer_to( cq_unk );
                val.val.data=val.val.data.substr(0,pos_new)+str_new+val.val.data.substr(pos_new);
                if ( new_bw_new.size )
                    new_patch( val, bw_new, new BinaryReader( new_bw_new.to_Uint8Array() ), as_usr, cq_unk, new_off_unk, src_dev, src_usr, cur_dev, cur_usr );
            }
            break;
        }
        case 3: { // Remove
            let pos_new = br_new.read_PT(), len_new = br_new.read_PT();
            while ( br_unk.size ) {
                const num_unk = br_unk.read_PI8();
                switch ( num_unk ) {
                    case 0: { // AddUsrRight
                        console.log( 'AddUsrRight', 'Remove' );
                        let usr_unk = UsrId.read_from( br_unk ), flags_unk = br_unk.read_PT();
                        bw_unk.write_PI8( 0 ); usr_unk.write_to( bw_unk ); bw_unk.write_PT( flags_unk );
                        break;
                    }
                    case 1: { // RemUsrRight
                        console.log( 'RemUsrRight', 'Remove' );
                        let usr_unk = UsrId.read_from( br_unk ), flags_unk = br_unk.read_PT();
                        bw_unk.write_PI8( 1 ); usr_unk.write_to( bw_unk ); bw_unk.write_PT( flags_unk );
                        break;
                    }
                    case 2: { // Insert
                        console.log( 'Insert', 'Remove' );
                        let pos_unk = br_unk.read_PT(), str_unk = br_unk.read_String();
                        if(pos_unk>=pos_new+len_new){pos_unk-=len_new;}else{if(pos_unk<=pos_new){pos_new+=str_unk.length;}else{let T0=len_new-pos_unk-pos_new,T1=pos_unk-pos_new,T2;pos_unk=pos_new;new_off_unk.push(br_unk.offset);new_bw_new.write_PI8(3);T2=T0;len_new=T1;new_bw_new.write_PT(pos_new+str_unk.length);new_bw_new.write_PT(T2);}}
                        bw_unk.write_PI8( 2 ); bw_unk.write_PT( pos_unk ); bw_unk.write_String( str_unk );
                        break;
                    }
                    case 3: { // Remove
                        console.log( 'Remove', 'Remove' );
                        let pos_unk = br_unk.read_PT(), len_unk = br_unk.read_PT();
                        if(pos_unk<=pos_new){if(pos_unk+len_unk<=pos_new){pos_new-=len_unk;}else{if(pos_unk+len_unk<=pos_new+len_new){let T0=pos_unk+len_unk-pos_new;pos_new=pos_unk;len_new-=T0;len_unk-=T0;}else{len_unk-=len_new;len_new=0;}}}else{if(pos_new+len_new<=pos_unk){pos_unk-=len_new;}else{if(pos_new+len_new<=pos_unk+len_unk){let T1=pos_new+len_new-pos_unk;pos_unk=pos_new;len_unk-=T1;len_new-=T1;}else{len_new-=len_unk;len_unk=0;}}}
                        bw_unk.write_PI8( 3 ); bw_unk.write_PT( pos_unk ); bw_unk.write_PT( len_unk );
                        break;
                    }
                    case 4: { // RemUnd
                        console.log( 'RemUnd', 'Remove' );
                        let pos_unk = br_unk.read_PT(), str_unk = br_unk.read_String();
                        let T0=pos_unk+str_unk.length-pos_new,T1=len_new,T2=Boolean(pos_unk<=pos_new),T3=pos_new,T4=str_unk.substr(0,str_unk.length-T0);if(pos_unk+str_unk.length<=pos_new+len_new){len_new-=T0;pos_new=pos_unk;}else{let T5=pos_new-pos_unk,T6=T4.substr(0,T5)+T4.substr(T5+pos_new-pos_unk+len_new-pos_new);len_new=0;T4=T6;}if(T2){if(pos_unk+str_unk.length<=T3){T3-=str_unk.length;}}else{if(T3+T1<=pos_unk){pos_unk-=T1;}else{if(T3+T1<=pos_unk+T4.length){let T7=T3+T1-pos_unk;pos_unk=T3;T1-=T7;T4=T4.substr(T7);}else{let T8=T4.length;T4="";T1-=T8;}}}
                        bw_unk.write_PI8( 4 ); bw_unk.write_PT( pos_unk ); bw_unk.write_String( str_unk );
                        break;
                    }
                }
            }
            console.log( pos_new, len_new );
            if ( right_to.Remove( val, as_usr, pos_new, len_new ) ) {
                {
                    let pos_tmp, str_tmp;
                    pos_tmp=pos_new;str_tmp=val.val.data.substring(pos_new,pos_new+len_new);
                    bw_new.write_PI8( 4 );bw_new.write_PT( pos_tmp ); bw_new.write_String( str_tmp );
                }
                bw_unk.transfer_to( cq_unk );
                val.val.data=val.val.data.substr(0,pos_new)+val.val.data.substr(pos_new+len_new);
                if ( new_bw_new.size )
                    new_patch( val, bw_new, new BinaryReader( new_bw_new.to_Uint8Array() ), as_usr, cq_unk, new_off_unk, src_dev, src_usr, cur_dev, cur_usr );
            }
            break;
        }
        case 4: { // RemUnd
            let pos_new = br_new.read_PT(), str_new = br_new.read_String();
            while ( br_unk.size ) {
                const num_unk = br_unk.read_PI8();
                switch ( num_unk ) {
                    case 0: { // AddUsrRight
                        console.log( 'AddUsrRight', 'RemUnd' );
                        let usr_unk = UsrId.read_from( br_unk ), flags_unk = br_unk.read_PT();
                        bw_unk.write_PI8( 0 ); usr_unk.write_to( bw_unk ); bw_unk.write_PT( flags_unk );
                        break;
                    }
                    case 1: { // RemUsrRight
                        console.log( 'RemUsrRight', 'RemUnd' );
                        let usr_unk = UsrId.read_from( br_unk ), flags_unk = br_unk.read_PT();
                        bw_unk.write_PI8( 1 ); usr_unk.write_to( bw_unk ); bw_unk.write_PT( flags_unk );
                        break;
                    }
                    case 2: { // Insert
                        console.log( 'Insert', 'RemUnd' );
                        let pos_unk = br_unk.read_PT(), str_unk = br_unk.read_String();
                        if(pos_unk>=pos_new+str_new.length){pos_unk-=str_new.length;}else{if(pos_unk<=pos_new){pos_new+=str_unk.length;}else{let T0=str_new.substr(pos_unk-pos_new),T1=str_new.substr(0,pos_unk-pos_new),T2;pos_unk=pos_new;new_off_unk.push(br_unk.offset);new_bw_new.write_PI8(4);T2=T0;str_new=T1;new_bw_new.write_PT(pos_new+str_unk.length);new_bw_new.write_String(T2);}}
                        bw_unk.write_PI8( 2 ); bw_unk.write_PT( pos_unk ); bw_unk.write_String( str_unk );
                        break;
                    }
                    case 3: { // Remove
                        console.log( 'Remove', 'RemUnd' );
                        let pos_unk = br_unk.read_PT(), len_unk = br_unk.read_PT();
                        let T0=pos_new+str_new.length-pos_unk,T1=len_unk,T2=Boolean(pos_new<=pos_unk),T3=pos_unk,T4=str_new.substr(0,str_new.length-T0);if(pos_new+str_new.length<=pos_unk+len_unk){len_unk-=T0;pos_unk=pos_new;}else{let T5=pos_unk-pos_new,T6=T4.substr(0,T5)+T4.substr(T5+pos_unk-pos_new+len_unk-pos_unk);len_unk=0;T4=T6;}if(T2){if(pos_new+str_new.length<=T3){T3-=str_new.length;}}else{if(T3+T1<=pos_new){pos_new-=T1;}else{if(T3+T1<=pos_new+T4.length){let T7=T3+T1-pos_new;pos_new=T3;T1-=T7;T4=T4.substr(T7);}else{let T8=T4.length;T4="";T1-=T8;}}}
                        bw_unk.write_PI8( 3 ); bw_unk.write_PT( pos_unk ); bw_unk.write_PT( len_unk );
                        break;
                    }
                    case 4: { // RemUnd
                        console.log( 'RemUnd', 'RemUnd' );
                        let pos_unk = br_unk.read_PT(), str_unk = br_unk.read_String();
                        let T0=pos_unk+str_unk.length-pos_new,T1=str_new,T2=Boolean(pos_unk<=pos_new),T3=pos_new,T4=str_unk.substr(0,str_unk.length-T0);if(pos_unk+str_unk.length<=pos_new+str_new.length){pos_new=pos_unk;str_new=str_new.substr(str_new.length-T0);}else{let T5=pos_new-pos_unk,T6=T4.substr(0,T5)+T4.substr(T5+pos_new-pos_unk+str_new.length-pos_new);str_new="";T4=T6;}if(T2){if(pos_unk+str_unk.length<=T3){T3-=str_unk.length;}}else{if(T3+T1.length<=pos_unk){console.log("crout");pos_unk-=T1.length;}else{if(T3+T1.length<=pos_unk+T4.length){let T7=T4.substr(T3+T1.length-pos_unk),T8=T1.substr(0,pos_unk-T3),T9;pos_unk=T3;T9=T7;T1=T8;T4=T9;}else{let T10=pos_unk-T3,T11=T1.substr(0,T10)+T1.substr(T10+T4.length);T4="";T1=T11;}}}
                        bw_unk.write_PI8( 4 ); bw_unk.write_PT( pos_unk ); bw_unk.write_String( str_unk );
                        break;
                    }
                }
            }
            console.log( pos_new, str_new );
            if ( right_to.RemUnd( val, as_usr, pos_new, str_new ) ) {
                bw_new.write_PI8( 4 ); bw_new.write_PT( pos_new ); bw_new.write_String( str_new );
                bw_unk.transfer_to( cq_unk );
                val.val.data=val.val.data.substr(0,pos_new)+val.val.data.substr(pos_new+str_new.length);
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
    return ["add_usr_right","rem_usr_right","read","insert","remove","append"];
}

export default { read, right_to, bin_repr, new_patch, undo_patch, get_possible_rights__b };
