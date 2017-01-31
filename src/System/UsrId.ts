import BinaryReader from "./BinaryReader";
import BinaryWriter from "./BinaryWriter";
import DevId        from "./DevId";

//
export default
class UsrId {
    static need_dev_for_read = true;
    static need_usr_for_read = true;

    constructor( dev = new DevId, num = 0 ) {
        this.dev = dev;
        this.num = num;
    }

    toString(): string {
        return `${this.num}@${this.dev}`
    }

    copy(): UsrId {
        return new UsrId( this.dev.copy(), this.num );
    }

    static fromString( str: string ): UsrId {
        let i = str.indexOf( "@" );
        return new UsrId( new DevId( str.substr( i + 1 ) ), parseInt( str.substr( 0, i ) ) );
    }

    write_to( bw: BinaryWriter ): void {
        bw.write_PT( this.num );
        if ( this.num )
            bw.write_obj( this.dev );
    }

    static read_from( br: BinaryReader, src_dev?: DevId, src_usr?: UsrId, cur_dev?: DevId, cur_usr?: UsrId ): UsrId {
        let num = br.read_PT();
        if ( ! num )
            return ! src_usr || src_usr.equ( cur_usr ) ? new UsrId : src_usr;
        let res = new UsrId( DevId.read_from( br, src_dev, cur_dev ), num );
        return res.equ( cur_usr ) ? new UsrId : res;
    }

    static skip_from( br: BinaryReader ): void {
        UsrId.read_from( br, null, null, null, null );
    }

    get loc(): boolean {
        return this.num == 0;
    }

    get ext(): boolean {
        return this.num != 0;
    }

    self_glo_to_loc( dev: DevId, usr: UsrId ): void {
        this.dev.self_glo_to_loc( dev );
        if ( this.equ( usr ) )
            this.num = 0; // -> local user
    }

    self_loc_to_glo( dev: DevId, usr: UsrId ): void {
        if ( this.equ( usr ) )
            this.num = 0; // -> local user
        else
            this.dev.self_loc_to_glo( dev );
    }

    inf( p: UsrId ): boolean {
        return this.num == p.num ? this.dev.inf( p.dev ) : this.num < p.num;
    }

    sup( p: UsrId ): boolean {
        return this.num == p.num ? this.dev.sup( p.dev ) : this.num > p.num;
    }

    equ( p: UsrId ): boolean {
        return this.num == p.num && this.dev.equ( p.dev );
    }

    dev: DevId;
    num: number;
}
