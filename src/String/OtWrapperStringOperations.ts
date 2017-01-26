import OtWrapperString from "./OtWrapperString"
import BinaryWriter    from "../System/BinaryWriter"
import BinaryReader    from "../System/BinaryReader"
import UsrId           from "../System/UsrId"

var bin_repr = {
    Insert: function( bw: BinaryWriter, pos: number, str: string ): void { bw.write_PI8( 0 ); bw.write_PT( pos ); bw.write_String( str ); },
    Remove: function( bw: BinaryWriter, pos: number, len: number ): void { bw.write_PI8( 1 ); bw.write_PT( pos ); bw.write_PT( len ); },
    RemUnd: function( bw: BinaryWriter, pos: number, str: string ): void { bw.write_PI8( 2 ); bw.write_PT( pos ); bw.write_String( str ); },
}

function read( br: BinaryReader, cb: ( type: string, args: any ) => void ) {
    switch ( br.read_PI8() ) {
        case 0: cb( "Insert", { pos: br.read_PT(), str: br.read_String() } ); break;
        case 1: cb( "Remove", { pos: br.read_PT(), len: br.read_PT() } ); break;
        case 2: cb( "RemUnd", { pos: br.read_PT(), str: br.read_String() } ); break;
        default: cb( null, {} ); br.clear(); break;
    }
}

function skip( br: BinaryReader ): Array<number> {
    let res = new Array<number>();
    while ( br.size ) {
        res.push( br.cursor );
        switch ( br.read_PI8() ) {
            case 0: br.skip_PT(); br.skip_String(); break;
            case 1: br.skip_PT(); br.skip_PT(); break;
            case 2: br.skip_PT(); br.skip_String(); break;
        }
    }
    return res;
}

function undo_patch( val: string, br: BinaryReader, as_usr: UsrId ): string {
    const res = skip( br );
    for( let n = res.length; n--; ) {
        br.cursor = res[ n ];
        switch ( br.read_PI8() ) {
        case 0: {
            let pos = br.read_PT(), str = br.read_String();
            val=val.substr(0,pos)+val.substr((pos+str.length));
            break;
        }
        case 2: {
            let pos = br.read_PT(), str = br.read_String();
            val=val.substr(0,pos)+str+val.substr(pos);
            break;
        }
        }
    }
    return val;
}

function new_patch( val: string, bw_new: BinaryWriter, br_new: BinaryReader, as_usr: UsrId, cq_unk: BinaryWriter ): string {
    while ( br_new.size ) {
        switch ( br_new.read_PI8() ) {
        case 0: {
            let pos_new = br_new.read_PT(), str_new = br_new.read_String();
            let br_unk = new BinaryReader( cq_unk.to_Uint8Array() );
            let bw_unk = new BinaryWriter;
            while ( br_unk.size ) {
                const num_unk = br_unk.read_PI8();
                switch ( num_unk ) {
                    case 0: {
                        let pos_unk = br_unk.read_PT(), str_unk = br_unk.read_String();
                        if(!(pos_unk<=pos_new)){pos_unk+=str_new.length;}else{pos_new+=str_unk.length;}
                        bw_unk.write_PI8( 0 ); bw_unk.write_PT( pos_unk ); bw_unk.write_String( str_unk );
                        break;
                    }
                    case 1: {
                        let pos_unk = br_unk.read_PT(), len_unk = br_unk.read_PT();
                        bw_unk.write_PI8( 1 ); bw_unk.write_PT( pos_unk ); bw_unk.write_PT( len_unk );
                        break;
                    }
                    case 2: {
                        let pos_unk = br_unk.read_PT(), str_unk = br_unk.read_String();
                        bw_unk.write_PI8( 2 ); bw_unk.write_PT( pos_unk ); bw_unk.write_String( str_unk );
                        break;
                    }
                }
            }
            bw_new.write_PI8( 0 ); bw_new.write_PT( pos_new ); bw_new.write_String( str_new );
            bw_unk.transfer_to( cq_unk );
            val=val.substr(0,pos_new)+str_new+val.substr(pos_new);
            break;
        }
        case 1: {
            let pos_new = br_new.read_PT(), len_new = br_new.read_PT();
            let br_unk = new BinaryReader( cq_unk.to_Uint8Array() );
            let bw_unk = new BinaryWriter;
            while ( br_unk.size ) {
                const num_unk = br_unk.read_PI8();
                switch ( num_unk ) {
                    case 0: {
                        let pos_unk = br_unk.read_PT(), str_unk = br_unk.read_String();
                        bw_unk.write_PI8( 0 ); bw_unk.write_PT( pos_unk ); bw_unk.write_String( str_unk );
                        break;
                    }
                    case 1: {
                        let pos_unk = br_unk.read_PT(), len_unk = br_unk.read_PT();
                        bw_unk.write_PI8( 1 ); bw_unk.write_PT( pos_unk ); bw_unk.write_PT( len_unk );
                        break;
                    }
                    case 2: {
                        let pos_unk = br_unk.read_PT(), str_unk = br_unk.read_String();
                        bw_unk.write_PI8( 2 ); bw_unk.write_PT( pos_unk ); bw_unk.write_String( str_unk );
                        break;
                    }
                }
            }
            {
                let pos_tmp, str_tmp;
                pos_tmp=pos_new;str_tmp=val.substring(pos_new,pos_new+len_new);
                bw_new.write_PI8( 2 ); bw_new.write_PT( pos_tmp ); bw_new.write_String( str_tmp );
            }
            bw_unk.transfer_to( cq_unk );
            val=val.substr(0,pos_new)+val.substr((pos_new+len_new));
            break;
        }
        case 2: {
            let pos_new = br_new.read_PT(), str_new = br_new.read_String();
            let br_unk = new BinaryReader( cq_unk.to_Uint8Array() );
            let bw_unk = new BinaryWriter;
            while ( br_unk.size ) {
                const num_unk = br_unk.read_PI8();
                switch ( num_unk ) {
                    case 0: {
                        let pos_unk = br_unk.read_PT(), str_unk = br_unk.read_String();
                        bw_unk.write_PI8( 0 ); bw_unk.write_PT( pos_unk ); bw_unk.write_String( str_unk );
                        break;
                    }
                    case 1: {
                        let pos_unk = br_unk.read_PT(), len_unk = br_unk.read_PT();
                        bw_unk.write_PI8( 1 ); bw_unk.write_PT( pos_unk ); bw_unk.write_PT( len_unk );
                        break;
                    }
                    case 2: {
                        let pos_unk = br_unk.read_PT(), str_unk = br_unk.read_String();
                        bw_unk.write_PI8( 2 ); bw_unk.write_PT( pos_unk ); bw_unk.write_String( str_unk );
                        break;
                    }
                }
            }
            bw_new.write_PI8( 2 ); bw_new.write_PT( pos_new ); bw_new.write_String( str_new );
            bw_unk.transfer_to( cq_unk );
            val=val.substr(0,pos_new)+val.substr((pos_new+str_new.length));
            break;
        }
        }
    }
    return val;
}

export default { read, bin_repr, new_patch, undo_patch };
