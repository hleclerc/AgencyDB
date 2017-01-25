import OtWrapperString from "./OtWrapperString"
import BinaryWriter    from "../System/BinaryWriter"
import BinaryReader    from "../System/BinaryReader"
import UsrId           from "../System/UsrId"

var bin_repr = {
    Insert: function( bw: BinaryWriter, pos: number, str: string ): void { bw.write_PI8( 0 ); bw.write_PT( pos ); bw.write_String( str ); },
    Remove: function( bw: BinaryWriter, pos: number, len: number ): void { bw.write_PI8( 1 ); bw.write_PT( pos ); bw.write_PT( len ); },
}

function read( br: BinaryReader, cb: ( type: string, args: any ) => void ) {
    switch ( br.read_PI8() ) {
        case 0: cb( "Insert", { pos: br.read_PT(), str: br.read_String() } ); break;
        case 1: cb( "Remove", { pos: br.read_PT(), len: br.read_PT() } ); break;
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
        }
    }
    return res;
}

function undo_patch( val: string, br: BinaryReader, as_usr: UsrId ) {
}

function new_patch( val: OtWrapperString, bw_new: BinaryWriter, br_new: BinaryReader, as_usr: UsrId, cq_unk: BinaryWriter ) {
}

export default { read, bin_repr, new_patch, undo_patch };
