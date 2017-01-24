import OtWrapperString from "./OtWrapperString"
import BinaryWriter    from "../System/BinaryWriter"
import BinaryReader    from "../System/BinaryReader"
import UsrId           from "../System/UsrId"

var bin_repr = {
    insert: function( pos: number, sup: string ): Uint8Array { let bw = new BinaryWriter; bw.write_PI8( 0 ); bw.write_PT( pos ); bw.write_String( sup ); return bw.to_Uint8Array(); },
    remove: function( pos: number, len: number ): Uint8Array { let bw = new BinaryWriter; bw.write_PI8( 1 ); bw.write_PT( pos ); bw.write_PT( len ); return bw.to_Uint8Array(); },
}

function read( br: BinaryReader, cb: ( type: string, args: any ) => void ) {
    const num = br.read_PI8();
    switch ( num ) {
        case 0: cb( "insert", { pos: br.read_PT(), sup: br.read_String() } ); break;
        case 1: cb( "remove", { pos: br.read_PT(), len: br.read_PT() } ); break;
        default: cb( null, {} ); br.clear(); break;
    }
}

function new_patch( val: OtWrapperString, bw_new: BinaryWriter, br_new: BinaryReader, as_usr: UsrId, cq_unk: BinaryWriter ) {
    while ( br_new.size ) {
        const num = br_new.read_PI8();
        switch ( num ) {
        case 0: {
            let pos_new = br_new.read_PT(); let sup_new = br_new.read_String();
            let br_unk = new BinaryReader( cq_unk.to_Uint8Array() );
            while ( br_unk.size ) {
                const num_unk = br_unk.read_PI8();
                switch ( num_unk ) {
                    case 0: {
                        let pos_unk = br_unk.read_PT(); let sup_unk = br_unk.read_String();
                        if(!(pos_unk<=pos_new)){pos_unk+=sup_new.length;}else{pos_new+=sup_unk.length;}
                        break;
                    }
                    case 1: {
                        let pos_unk = br_unk.read_PT(); let len_unk = br_unk.read_PT();
                        break;
                    }
                    default: break;
                }
            }
            val.val.data = val.val.data.slice( 0, pos_new ) + sup_new + val.val.data.slice( pos_new );
            bw_new.write_PI8( 0 ); bw_new.write_PT( pos_new ); bw_new.write_String( sup_new );
            break;
        }
        case 1: {
            let pos_new = br_new.read_PT(); let len_new = br_new.read_PT();
            let br_unk = new BinaryReader( cq_unk.to_Uint8Array() );
            while ( br_unk.size ) {
                const num_unk = br_unk.read_PI8();
                switch ( num_unk ) {
                    case 0: {
                        let pos_unk = br_unk.read_PT(); let sup_unk = br_unk.read_String();
                        break;
                    }
                    case 1: {
                        let pos_unk = br_unk.read_PT(); let len_unk = br_unk.read_PT();
                        break;
                    }
                    default: break;
                }
            }
            val.val.data = val.val.data.slice( 0, pos_new ) + val.val.data.slice( pos_new + len_new );
            bw_new.write_PI8( 1 ); bw_new.write_PT( pos_new ); bw_new.write_PT( len_new );
            break;
        }
        }
    }
}

function undo_patch( val: OtWrapperString, br: BinaryReader, as_usr: UsrId ) {
    while ( br.size ) {
        const num = br.read_PI8();
        switch ( num ) {
        case 0: {
            let pos = br.read_PT(); let sup = br.read_String();
            val.val.data = val.val.data.slice( 0, pos ) + val.val.data.slice( pos + sup.length );
            break;
        }
        case 1: {
            let pos = br.read_PT(); let len = br.read_PT();
            val.val.data = "proute";
            break;
        }
        }
    }
}

export default { read, bin_repr, new_patch, undo_patch };
