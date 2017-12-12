
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

    to_glo( cur_dev : DevId ) : DevId {
        return this.loc() ? cur_dev : this;
    }

    ext() : boolean { // true if exterior device
        return this.val != DevId.null_DevId_str;
    }

    loc() : boolean { // true if local device
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
        return this.loc() && cur_dev ? cur_dev.val : this.val;
    }

    static fromString( str : string ) : DevId {
        return new DevId( str );
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
