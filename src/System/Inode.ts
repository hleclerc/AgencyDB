import BinaryWriter from "./BinaryWriter";
import BinaryReader from "./BinaryReader";
import DevId        from "./DevId";

/** Keys of objects in the wild */
export default
class Inode {
    /** for known Inodes (val = 1) */
    static base_dev = new DevId( "----------3~" );

    constructor( dev: DevId = new DevId, num: number = 0 ) {
        this.dev = dev;
        this.num = num;
    }

    toString( cur_dev?: DevId ) : string {
        return `${this.num}@${this.dev.toString( cur_dev )}`;
    }

    js_repr( prec: number ): string {
        return `new Inode("${ this.dev.toString() },${ this.num })")`;
    }

    to_Boolean(): boolean {
        return Boolean( this.num );
    }

    copy(): Inode {
        return new Inode( this.dev.copy(), this.num );
    }

    static fromString( str: string ) : Inode {
        let i = str.indexOf( "@" );
        return new Inode( new DevId( str.substr( i + 1 ) ), parseInt( str.substr( 0, i ) ) );
    }

    write_to( bw: BinaryWriter, cur_dev?: DevId ) : void {
        bw.write_PT( this.num );
        if ( this.num )
            this.dev.write_to( bw, cur_dev );
    }

    static read_from( br: BinaryReader, src_dev: DevId, cur_dev: DevId ): Inode {
        let num = br.read_PT();
        let dev = num ? DevId.read_from( br, src_dev, cur_dev ) : new DevId;
        return new Inode( dev, num );
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

    inf( p : Inode ) : boolean {
        return this.num == p.num ? this.dev.inf( p.dev ) : this.num < p.num;
    }

    sup( p : Inode ) : boolean {
        return this.num == p.num ? this.dev.sup( p.dev ) : this.num > p.num;
    }

    equ( p : Inode ) : boolean {
        return this.num == p.num && this.dev.equ( p.dev );
    }

    dev: DevId;
    num: number;
}
