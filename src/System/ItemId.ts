import BinaryWriter from "./BinaryWriter";
import BinaryReader from "./BinaryReader";
import DevId        from "./DevId";

//
export default
class ItemId {
    /** for known ItemIds (val = 1) */
    static base_dev = new DevId( "----------3~" );

    constructor( dev: DevId = new DevId, num: number = 0 ) {
        this.dev = dev;
        this.num = num;
    }

    toString( cur_dev?: DevId ) : string {
        return `${this.num}@${this.dev.toString( cur_dev )}`;
    }

    static fromString( str: string ) : ItemId {
        let i = str.indexOf( "@" );
        return new ItemId( new DevId( str.substr( i + 1 ) ), parseInt( str.substr( 0, i ) ) );
    }

    write_to( bw: BinaryWriter, cur_dev?: DevId ) : void {
        bw.write_PT( this.num );
        if ( this.num )
            this.dev.write_to( bw, cur_dev );
    }

    static read_from( br: BinaryReader, src_dev: DevId, cur_dev: DevId ): ItemId {
        let num = br.read_PT();
        let dev = num ? DevId.read_from( br, src_dev, cur_dev ) : new DevId;
        return new ItemId( dev, num );
    }

    static skip_from( br: BinaryReader ) {
        if ( br.read_PT() )
            DevId.skip_from( br );
    }

    self_glo_to_loc( dev : DevId ) : void {
        this.dev.self_glo_to_loc( dev );
    }

    self_loc_to_glo( dev : DevId ) : void {
        this.dev.self_loc_to_glo( dev );
    }

    inf( p : ItemId ) : boolean {
        return this.num == p.num ? this.dev.inf( p.dev ) : this.num < p.num;
    }

    sup( p : ItemId ) : boolean {
        return this.num == p.num ? this.dev.sup( p.dev ) : this.num > p.num;
    }

    equ( p : ItemId ) : boolean {
        return this.num == p.num && this.dev.equ( p.dev );
    }

    dev: DevId;
    num: number;
}
