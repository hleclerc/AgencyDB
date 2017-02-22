import BinaryReader from "./BinaryReader";
import BinaryWriter from "./BinaryWriter";
import DevId        from "./DevId";

//
export default
class GrpId {
    constructor( dev : DevId = new DevId, num : number = 0 ) {
        this.dev = dev;
        this.num = num;
    }

    toString(): string {
        return `${this.num}@${this.dev}`
    }

    static fromString( str : string ): GrpId {
        let i = str.indexOf( "@" );
        return new GrpId( new DevId( str.substr( i + 1 ) ), parseInt( str.substr( 0, i ) ) );
    }

    write_to( bw: BinaryWriter ) : void {
        bw.write_PT( this.num );
        if ( this.num )
            bw.write_obj( this.dev );
    }

    static read_from( br: BinaryReader, cur_dev: DevId ): GrpId {
        let num = br.read_PT()
        let dev = num ? DevId.read_from( br, cur_dev ) : new DevId;
        return new GrpId( dev, num );
    }

    static skip_from( br: BinaryReader ): void {
        GrpId.read_from( br, null );
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
