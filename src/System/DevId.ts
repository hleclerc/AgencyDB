import BinaryReader from "./BinaryReader";
import BinaryWriter from "./BinaryWriter";
import Base64o      from "./Base64o";

//
// Ex of valid DevId (e.g. for testing purpose)
//   gMWDnGjZfxZ~
//   gmWRNBUMN7o~
//   O5HeHcHU1Io~
//   h8u858_RnDF~
//   ujtameVI3fN~
//
export default
class DevId {
    /** local device */
    static null_DevId_str = "-----------~";

    constructor( val : string = DevId.null_DevId_str ) {
        this.val = val;
    }

    copy(): DevId {
        return new DevId( this.val );
    }

    to_glo( cur_dev: DevId ): DevId {
        return this.loc ? cur_dev : this;
    }

    get ext(): boolean { // true if exterior device
        return this.val != DevId.null_DevId_str;
    }

    get loc(): boolean { // true if local device
        return this.val == DevId.null_DevId_str;
    }

    self_glo_to_loc( dev: DevId ) : void {
        if ( dev && this.val == dev.val )
            this.val = DevId.null_DevId_str;
    }

    self_loc_to_glo( dev: DevId ) : void {
        if ( dev && this.val == DevId.null_DevId_str )
            this.val = dev.val;
    }

    toString( cur_dev? : DevId ) : string {
        return this.loc && cur_dev ? cur_dev.val : this.val;
    }

    static fromString( str : string ) : DevId {
        return new DevId( str );
    }

    static read_from( br: BinaryReader, src_dev: DevId, cur_dev: DevId ): DevId {
        let arr = br.read_some( 8 );
        let str = Base64o.encode( arr.slice( 0 ).reverse() );
        if ( src_dev && str == DevId.null_DevId_str ) str = src_dev.val;
        return new DevId( cur_dev && str == cur_dev.val ? DevId.null_DevId_str : str );
    }

    static skip_from( br: BinaryReader ) : void {
        DevId.read_from( br, null, null );
    }

    write_to( bw: BinaryWriter, cur_dev?: DevId ) {
        if ( this.loc && cur_dev != undefined )
            return cur_dev.write_to( bw );
        bw.write_some( Base64o.decode( this.val ).reverse() );
    }

    inf( p : DevId ): boolean {
        return this.val < p.val;
    }

    sup( p : DevId ) : boolean {
        return this.val > p.val
    }

    equ( p : DevId ) : boolean {
        return this.val == p.val
    }

    val: string;
}
