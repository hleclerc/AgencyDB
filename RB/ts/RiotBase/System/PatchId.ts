import DevId from "./DevId";

//
export default
class PatchId {
    constructor( dev : DevId = new DevId, num : number = 0 ) {
        this.dev = dev;
        this.num = num;
    }

    toString( cur_dev? : DevId ) : string {
        return `${ this.num }@${ this.dev.toString( cur_dev ) }`
    }

    static fromString( str : string ) : PatchId {
        let i = str.indexOf( "@" );
        return new PatchId( new DevId( str.substr( i + 1 ) ), parseInt( str.substr( 0, i ) ) );
    }

    self_glo_to_loc( dev : DevId ) : void {
        this.dev.self_glo_to_loc( dev );
    }

    self_loc_to_glo( dev : DevId ) : void {
        this.dev.self_loc_to_glo( dev );
    }

    inf( p : PatchId ) : boolean {
        return this.num == p.num ? this.dev.inf( p.dev ) : this.num < p.num;
    }

    sup( p : PatchId ) : boolean {
        return this.num == p.num ? this.dev.sup( p.dev ) : this.num > p.num;
    }

    equ( p : PatchId ) : boolean {
        return this.num == p.num && this.dev.equ( p.dev );
    }

    dev: DevId;
    num: number;
}
