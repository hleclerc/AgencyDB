import DevId from "./DevId";

//
export default
class GrpId {
    constructor( dev : DevId = new DevId, num : number = 0 ) {
        this.dev = dev;
        this.num = num;
    }

    toString() : string {
        return `${this.num}@${this.dev}`
    }

    static fromString( str : string ) : GrpId {
        let i = str.indexOf( "@" );
        return new GrpId( new DevId( str.substr( i + 1 ) ), parseInt( str.substr( 0, i ) ) );
    }

    inf( p : GrpId ) : boolean {
        return this.num == p.num ? this.dev.inf( p.dev ) : this.num < p.num;
    }

    sup( p : GrpId ) : boolean {
        return this.num == p.num ? this.dev.sup( p.dev ) : this.num > p.num;
    }

    self_glo_to_loc( dev : DevId ) : void {
        this.dev.self_glo_to_loc( dev );
    }

    self_loc_to_glo( dev : DevId ) : void {
        this.dev.self_loc_to_glo( dev );
    }

    equ( p : GrpId ) : boolean {
        return this.num == p.num && this.dev.equ( p.dev );
    }

    dev: DevId;
    num: number;
}
