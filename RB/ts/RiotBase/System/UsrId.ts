import DevId from "./DevId";

//
export default
class UsrId {
    constructor( dev : DevId = new DevId, num : number = 0 ) {
        this.dev = dev;
        this.num = num;
    }

    toString() : string {
        return `${this.num}@${this.dev}`
    }

    static fromString( str : string ) : UsrId {
        let i = str.indexOf( "@" );
        return new UsrId( new DevId( str.substr( i + 1 ) ), parseInt( str.substr( 0, i ) ) );
    }

    loc() : boolean {
        return this.num == 0;
    }

    ext() : boolean {
        return this.num != 0;
    }

    self_glo_to_loc( dev : DevId, usr : UsrId ) : void {
        this.dev.self_glo_to_loc( dev );
        if ( this.equ( usr ) )
            this.num = 0; // -> local user
    }

    self_loc_to_glo( dev : DevId, usr : UsrId ) : void {
        if ( this.equ( usr ) )
            this.num = 0; // -> local user
        else
            this.dev.self_loc_to_glo( dev );
    }

    inf( p : UsrId ) : boolean {
        return this.num == p.num ? this.dev.inf( p.dev ) : this.num < p.num;
    }

    sup( p : UsrId ) : boolean {
        return this.num == p.num ? this.dev.sup( p.dev ) : this.num > p.num;
    }

    equ( p : UsrId ) : boolean {
        return this.num == p.num && this.dev.equ( p.dev );
    }

    dev: DevId;
    num: number;
}
