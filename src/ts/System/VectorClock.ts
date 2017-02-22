import BinaryWriter from "./BinaryWriter";
import BinaryReader from "./BinaryReader";
import ListUtil     from "./ListUtil";
import MapUtil      from "./MapUtil";
import PatchId      from "./PatchId";
import Inode        from "./Inode";
import DevId        from "./DevId";

//
export default
class VectorClock {
    constructor() {
        this.map    = new Map<string,number>();
        this.merges = new Array<Inode>();
    }

    toString(): string {
        let lst = [];
        this.map.forEach( ( num, dev ) => lst.push( `${dev}:${num}` ) );
        return "{" + lst.concat( this.merges.map( ( x ) => x.toString() ) ).join( "," ) + "}";
    }

    toJSON() {
        let lst = [];
        this.map.forEach( ( num, dev ) => lst.push( `${dev}:${num}` ) );
        return {
            patches: lst,
            merges: this.merges.map( ( x ) => x.toString() )
        };
    }

    static fromJSON( d ): VectorClock {
        let res = new VectorClock;
        for( let p of d.patches ){
            let l = p.split( ':' );
            res.map.set( l[ 0 ], parseInt( l[ 1 ], 10 ) );
        }
        for( let p of d.merges )
            res.merges.push( Inode.fromString( p ) );
        return res;        
    }

    write_to( bw: BinaryWriter ) {
        bw.write_PT( this.map.size );
        this.map.forEach( ( num, dev ) => {
            bw.write_obj( new DevId( dev ) );
            bw.write_PT ( num );
        } );
        bw.write_PT( this.merges.length );
        for( let inode of this.merges )
            bw.write_obj( inode );
    }

    static read_from( br: BinaryReader, src_dev: DevId, cur_dev: DevId ): VectorClock {
        let res = new VectorClock;
        // patches
        for( let len = br.read_PT(); len--; ) {
            let dev = DevId.read_from( br, src_dev, cur_dev );
            res.map.set( dev.val, br.read_PT() );
        }
        // merges
        for( let len = br.read_PT(); len--; )
            res.merges.push( Inode.read_from( br, src_dev, cur_dev ) );
        return res;
    }

    copy(): VectorClock {
        let res = new VectorClock;
        this.map.forEach( ( num: number, dev: string ) => {
            res.map.set( dev, num );
        } )
        this.merges.forEach( ( inode: Inode ) => {
            res.merges.push( new Inode( new DevId( inode.dev.val ), inode.num ) );
        } )
        return res;
    }

    contains_patch( patch_id: PatchId ): boolean {
        return this.contains( patch_id.dev.val, patch_id.num );
    }

    contains_merge( inode: Inode ): boolean {
        for( let ti of this.merges )
            if ( ti.equ( inode ) )
                return true;
        return false;
    }

    contains( dev_id: string, num: number ): boolean {
        return num <= ( this.map.get( dev_id ) || 0 );
    }

    update_VectorClock( known: VectorClock ) {
        known.map.forEach( ( knum, dev ) => {
            if ( knum > this.val( dev ) )
                this.map.set( dev, knum );
        } );
        known.map.forEach( ( knum, dev ) => {
            if ( knum > this.val( dev ) )
                this.map.set( dev, knum );
        } );
    }

    /** return true if num = num + 1 (the only authorized update) */
    update_patch( patch_id: PatchId ): boolean {
        return this.update( patch_id.dev.val, patch_id.num );
    }

    /** return true if num = num + 1 (the only authorized update) */
    update( dev_id: string, num: number ): boolean {
        if ( num == this.val( dev_id ) + 1 ) {
            this.map.set( dev_id, num );
            return true;
        }
        return false;
    }

    /**  */
    update_merge( inode: Inode ): void {
        let i = this.merges.indexOf( inode );
        if ( i < 0 )
            this.merges.push( inode );
    }

    val( dev_id: string ): number {
        return this.map.get( dev_id ) || 0
    }

    /** add 1 to the number for dev_id */
    self_inc( dev_id: string ): number {
        let res = this.val( dev_id ) + 1
        this.map.set( dev_id, res );
        return res;
    }

    sub( p: PatchId ): void {
        let num = this.map.get( p.dev.val );
        if ( num && num >= p.num )
            this.map.set( p.dev.val, p.num - 1 );
    }

    equ( v: VectorClock ): boolean {
        return ListUtil.equ_obj( this.merges, v.merges ) && MapUtil.equ( this.map, v.map );
    }

    clear() {
        this.map.clear();
        this.merges = [];
    }

    map   : Map<string,number>; /** dev str -> number */
    merges: Array<Inode>;      /**  */
}
