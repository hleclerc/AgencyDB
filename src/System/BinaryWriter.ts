// import VectorClock from "./VectorClock";
// import Base64o     from "./Base64o";
// import PatchId     from "./PatchId";
// import crc32c      from "./Crc32c";
// import UsrId       from "./UsrId";
// import GrpId       from "./GrpId";
// import DevId       from "./DevId";
// import Inode       from "./Inode";

declare var TextEncoder: any;
// declare var Buffer     : any;

// interface HasAWriteTo {
//     write_to( bw : BinaryWriter, only_for_a_copy : boolean, cur_dev : DevId, cur_usr : UsrId ) : void;
// }

//
export default class BinaryWriter {
    //
    constructor( reservation = 32 ) {
        this.data = new ArrayBuffer( reservation );
    }

    /** for the display */
    toString() : string {
        return String( this.to_Uint8Array() );
    }

    /** to get the output as a Uint8Array */
    to_Uint8Array() : Uint8Array {
        this.int_reduction();
        return new Uint8Array( this.data, 0, this.size );
    }

    /** free data (with optionnal re-reservation) */
    clear( reservation = 0 ) : void {
        this.data = new ArrayBuffer( reservation );
        this.size = 0;
        this.mark = [];
        this.i2rd = [];
    }

    /** */
    int_reduction() : void {
        if ( this.i2rd.length ) {
            this.i2rd.sort();

            // get needed size for each int to reduce
            let davi = new DataView( this.data );
            let needed_size = [];
            for( let i = 0; i < this.i2rd.length; ++i )
                needed_size.push( 8 );
            for( let i = this.i2rd.length; i--; ) {
                let pos = this.i2rd[ i ]         // position that contains the offset
                let val = davi.getUint32( pos ); // offset value
                let lim = pos + 8 + val;         // pointed position
                let j = i + 1
                while ( j < this.i2rd.length && this.i2rd[ j ] < lim )
                    val -= 8 - needed_size[ j++ ]; // to update offset value according to needed sizes after pos and before lim
                needed_size[ i ] = BinaryWriter.size_needed_for_unsigned( val );
            }

            //
            let new_size = this.size;
            for( let s of needed_size )
                new_size -= 8 - s;

            // copy to a new buffer, with updated offsets
            let re = new ArrayBuffer( new_size );
            let rw = new DataView( re );
            let rs = 0;

            let partial_copy = ( beg, end ) => {
                let len = end - beg;
                let dst = new Uint8Array( re, rs, len );
                let src = new Uint8Array( this.data, beg, len );
                dst.set( src );
                rs += len;
            }

            let write_uint = ( val ) => {
                while ( val >= 128 ) {
                    rw.setInt8( rs++, 128 + val % 128 );
                    val /= 128;
                }
                rw.setInt8( rs++, val );
            }

            partial_copy( 0, this.i2rd[ 0 ] );
            for( let i = 0; i < this.i2rd.length; ++i ) {
                let pos = this.i2rd[ i ];        // position that contains the offset
                let val = davi.getUint32( pos ); // offset value
                let lim = pos + 8 + val;         // pointed position
                let j = i + 1;
                while ( j < this.i2rd.length && this.i2rd[ j ] < lim )
                    val -= 8 - needed_size[ j++ ] // to update offset value according to needed sizes after pos and before lim
                write_uint( val );
                partial_copy( pos + 8, i + 1 < this.i2rd.length ? this.i2rd[ i + 1 ] : this.size );
            }

            this.data = re;
            this.size = rs;
            this.i2rd = [];
        }
    }

    write_some( arr : Uint8Array ) {
        this.reserve_room_for( arr.byteLength );
        new Uint8Array( this.data, this.size, arr.byteLength ).set( arr );
        this.size += arr.byteLength
    }

    /** generic write, that call obj.write_to( this ) */
    write_obj( obj: { write_to: ( bw: BinaryWriter ) => void } ): void {
        obj.write_to( this );
    }

    write_PI8( data : number ): void {
        this.reserve_room_for( 1 );
        new DataView( this.data ).setUint8( this.size, data );
        this.size++;
    }

    write_PT( data : number ) : void {
        while ( data >= 128 ) {
            this.write_PI8( 128 + data % 128 );
            data /= 128;
        }
        this.write_PI8( data );
    }

    write_FP64( data : number ) {
        this.reserve_room_for( 8 );
        let d = new DataView( this.data, this.size );
        d.setFloat64( 0, data, true /*little endian*/ );
        this.size += 8;
    }

    write_Date( data : number ) : void {
        this.write_PT( data );
    }

    write_PI32( data ) : void {
        this.write_PT( data );
    }

    write_PI64( data ) : void {
        this.write_PT( data );
    }

    write_PI32_le( data ) : void {
        this.write_PI8( ( data >>>  0 ) & 0xFF );
        this.write_PI8( ( data >>>  8 ) & 0xFF );
        this.write_PI8( ( data >>> 16 ) & 0xFF );
        this.write_PI8( ( data >>> 24 ) & 0xFF );
    }

    write_String( str: string ): void { // -> utf8 format
        let enc = typeof TextEncoder != "undefined" ? new TextEncoder( "utf8" ).encode( str ) : new Buffer( str, "utf8" );
        this.write_PT( enc.byteLength );
        this.write_some( enc );
    }

    write_Uint8Array( data: Uint8Array ): void {
        this.write_PT( data.byteLength );
        this.write_some( data );
    }


    // write_uint_as_str: ( str ) ->
    //     br = new BitFieldInStr_Reader str
    //     while br.size() >= 14
    //         this.write_PI8 128 + br.read( 7 )
    //     this.write_PI8 br.read( 7 )

    // /** only_for_a_copy = true if we don't want informations like history, users, rights... */
    // write_Variable( data : Variable, only_for_a_copy = false, dev_id = new DevId, usr_id = new UsrId ) {
    //     Method.methods["write_to"].call_1( data, this, only_for_a_copy, dev_id, usr_id );
    // }

    // write_crc32c( bcmd : number ) : void {
    //     this.write_PI32_le( crc32c( new Uint8Array( this.data, bcmd, this.size - bcmd ) ) )
    // }

    // write_Uint8Array( arr : Uint8Array ) : void {
    //     this.write_PT( arr.byteLength );
    //     this.write_some( arr );
    // }

    // write_list( lst : Array<HasAWriteTo>, only_for_a_copy = true, cur_dev? : DevId, cur_usr? : UsrId ) : void {
    //     this.write_PT( lst.length );
    //     for( let c of lst )
    //         c.write_to( this, only_for_a_copy, cur_dev, cur_usr );
    // }

    beg_msg_length() : void {
        this.mark.push( this.size );
        this.reserve_room_for( 8 );
        this.size += 8;
    }

    end_msg_length() : void {
        let mark = this.mark.pop()
        let davi = new DataView( this.data );
        davi.setUint32( mark, this.size - mark - 8 );
        this.i2rd.push( mark );
    }

    reserve_room_for( size : number ) : void {
        if ( size > this.data.byteLength - this.size ) {
            let old   = this.data;
            this.data = new ArrayBuffer( Math.max( 2 * old.byteLength, old.byteLength + size ) );
            let dst   = new Uint8Array( this.data, 0, this.size );
            let src   = new Uint8Array( old, 0, this.size );
            dst.set( src );
        }
    }

    static size_needed_for_unsigned( val ) : number {
        let res = 1;
        while ( val >= 128 ){
            val /= 128;
            res++;
        }
        return res;
    }

    data : ArrayBuffer;
    size = 0;
    mark = new Array<number>(); // mark stack (cf this.beg_msg_length)
    i2rd = new Array<number>(); // int to reduce
}
