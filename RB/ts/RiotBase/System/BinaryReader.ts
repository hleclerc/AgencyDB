import VectorClock from "./VectorClock";
import PatchId     from "./PatchId";
import Base64o     from "./Base64o";
import crc32c      from "./Crc32c";
import Inode       from "./Inode";
import DevId       from "./DevId";
import UsrId       from "./UsrId";
import GrpId       from "./GrpId";

declare var TextDecoder: any;
declare var Buffer     : any;
declare function require( s: string );

export default
class BinaryReader {
    constructor( data : Uint8Array, cursor : number = 0, len : number = data.byteLength - cursor ) {
        this.cursor = cursor;
        this.data   = data.slice( cursor, cursor + len );
    }

    get size() : number {
       return this.data.byteLength - this.cursor;
    }

    get offset() : number {
        return this.cursor;
    }

    read_PI8() : number {
        let res = this.data[ this.cursor ];
        ++this.cursor;
        return res;
    }

    read_PT() : number {
        if ( this.cursor >= this.data.length )
            return 0;
        let res = this.data[ this.cursor ];
        this.cursor += 1;
        if ( res < 128 || this.cursor >= this.data.length )
            return res
        let shift = 7;
        res -= 128;
        while ( true ) {
            if ( this.cursor >= this.data.length )
                return res;
            let val = this.data[ this.cursor ]
            this.cursor += 1;
            if ( val < 128 || this.cursor >= this.data.length )
                return res + ( val << shift );
            res += ( val - 128 ) << shift;
            shift += 7;
        }
    }

    skip_PT() : void {
        this.read_PT();
    }

    read_FP32() : number {
        let res = new DataView( this.data.buffer, this.data.byteOffset + this.cursor ).getFloat32( 0, true );
        this.cursor += 4;
        return res;
    }

    skip_FP32() {
        this.skip_some( 4 );
    }

    read_FP64() : number {
        let res = new DataView( this.data.buffer, this.data.byteOffset + this.cursor ).getFloat64( 0, true );
        this.cursor += 8;
        return res;
    }

    skip_FP64() {
        this.skip_some( 8 );
    }

    read_Date() : number {
        if ( this.cursor >= this.data.length )
            return 0;
        let res = this.data[ this.cursor ];
        this.cursor += 1;
        if ( res < 128 || this.cursor >= this.data.length )
            return res
        let shift = 7;
        res -= 128;
        while ( true ) {
            if ( this.cursor >= this.data.length )
                return res;
            let val = this.data[ this.cursor ]
            this.cursor += 1;
            if ( val < 128 || this.cursor >= this.data.length )
                return res + val * Math.pow( 2.0, shift );
            res += ( val - 128 ) * Math.pow( 2.0, shift );
            shift += 7;
        }
    }

    read_PI32_le() : number {
        return this.read_PI8() + ( this.read_PI8() << 8 ) + ( this.read_PI8() << 16 ) + ( this.read_PI8() * 2.0 ** 24 )
    }

    read_String() : string {
        let len = this.read_PT();
        let arr = this.data.slice( this.cursor, this.cursor + len );
        let res = typeof TextDecoder != "undefined" ? new TextDecoder( "utf8" ).decode( arr ) : new Buffer( arr, "utf8" ).toString();
        this.cursor += len;
        return res;
    }

    skip_String() : void {
        this.skip_some( this.read_PT() );
    }

    read_Uint8Array() : Uint8Array {
        return this.read_some( this.read_PT() );
    }

    skip_Uint8Array() : void {
        this.skip_some( this.read_PT() );
    }

    read_some( len : number ) : Uint8Array {
        let res = this.data.slice( this.cursor, this.cursor + len );
        this.cursor += len;
        return res;
    }

    skip_some( len : number ) : void {
        this.cursor += len;
    }

    //     read_uint_as_str:{}
    //         bf = new BitFieldInStr_Writer
    //         if this.cursor >= this.data.length
    //             return bf.str()
    //         val = this.data[ this.cursor ]
    //         this.cursor += 1
    //         if val < 128
    //             bf.append_uint val, 7
    //             return bf.str()
    //         bf.append_uint val - 128, 7
    //         while true
    //             if this.cursor >= this.data.length
    //                 return bf.str()
    //             val = this.data[ this.cursor ]
    //             this.cursor += 1
    //             if val < 128
    //                 bf.append_uint val, 7
    //                 return bf.str()
    //             bf.append_uint val - 128, 7

    read_DevId( cur_dev? : DevId ) : DevId {
        let arr = this.read_some( 8 );
        let str = Base64o.encode( arr.slice( 0 ).reverse() );
        return new DevId( cur_dev && str == cur_dev.val ? DevId.null_DevId_str : str );
    }

    skip_DevId() : void {
        this.read_DevId();
    }

    read_Inode( cur_dev? : DevId ) : Inode {
        let num = this.read_PT();
        let dev = num ? this.read_DevId( cur_dev ) : new DevId;
        return new Inode( dev, num );
    }

    skip_Inode() {
        if ( this.read_PT() )
            this.skip_DevId();
    }

    read_PatchId( cur_dev? : DevId ) : PatchId {
        let num = this.read_PT();
        let dev = num ? this.read_DevId( cur_dev ) : new DevId;
        return new PatchId( dev, num );
    }

    read_UsrId( cur_dev? : DevId, cur_usr? : UsrId ) : UsrId {
        let num = this.read_PT();
        let dev = num ? this.read_DevId( cur_dev ) : new DevId;
        if ( cur_usr && num == cur_usr.num && dev.val == cur_usr.dev.val )
            return new UsrId;
        return new UsrId( dev, num );
    }

    skip_UsrId() : void {
        this.read_UsrId();
    }

    read_GrpId( cur_dev? : DevId ) : GrpId {
        let num = this.read_PT()
        let dev = num ? this.read_DevId( cur_dev ) : new DevId;
        return new GrpId( dev, num );
    }

    skip_GrpId() : void {
        this.read_GrpId();
    }

    read_VectorClock( cur_dev? : DevId ) : VectorClock {
        let res = new VectorClock;
        // patches
        for( let len = this.read_PT(); len--; ) {
            let dev = this.read_DevId( cur_dev );
            let num = this.read_PT();
            res.map.set( dev.val, num );
        }
        // merges
        for( let len = this.read_PT(); len--; )
            res.merges.push( this.read_Inode( cur_dev ) );
        return res;
    }

    read_Variable( dev_id = new DevId, usr_id = new UsrId ) : Variable {
        let num_type = this.read_PT();
        let reader = DeclTypesForRW.readers[ num_type ];
        if ( reader )
            return reader( this, dev_id, usr_id );

        // if not possible to read it now, copy data for this variable
        let beg = this.cursor;
        DeclTypesForRW.skip_functions[ num_type ]( this );
        let dat = this.data.slice( beg, this.cursor );

        // and make a DelayedVariableCreation
        let DelayedVariableCreation = require( "../Variable/Symbol/DelayedVariableCreation" ).default;
        let Lazyfier                = require( "../Variable/Symbol/Lazyfier" ).default;
        let Variable                = require( "../Variable/Core/Variable" ).default;
        let dv                      = new DelayedVariableCreation( num_type, dev_id, usr_id, dat );
        return new Variable( new Lazyfier( new Variable( dv ) ) );
    }

    skip_Variable() : void {
        DeclTypesForRW.skip_functions[ this.read_PT() ]( this );
    }

    read_list<T>( Type, cur_dev? : DevId, cur_usr? : UsrId ) : Array<T> {
        let res = [];
        for( let n = this.read_PT(); n--; )
            res.push( Type.read_from( this, cur_dev, cur_usr ) );
        return res;
    }

    skip_list( Type ) {
        for( let n = this.read_PT(); n--; )
            Type.skip_from( this );
    }

    make_crc32c( bcmd : number ) {
        return crc32c( this.data.slice( bcmd, this.cursor ) );
    }

    slice( len : number) : BinaryReader {
        return new BinaryReader( this.data.slice( this.cursor, this.cursor + len ) );
    }

    cursor: number;
    data  : Uint8Array;
}

import DeclTypesForRW from "../Variable/Core/DeclTypesForRW";
import Variable       from "../Variable/Core/Variable";
import Rp             from "../Variable/Core/Rp";
