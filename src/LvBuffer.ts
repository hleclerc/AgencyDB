import Variable       from "./Core/Variable"
import methods        from "./Core/methods"
import Rp             from "./Core/Rp"
import GenSymbol      from "./Symbol/GenSymbol"
import GenBuffer      from "./Buffer/GenBuffer"
import { BN_FP64 }    from "./Number/Bn"
import LvNumber       from "./LvNumber"

//
export default
class LvBuffer extends Variable<LvBuffer> {
    static NativeType = Buffer || ArrayBuffer;

    constructor( val ?: Rp | LvBuffer | Buffer | string | Array<number> ) {
        if      ( val instanceof Rp       ) super( val );
        else if ( val instanceof LvBuffer ) super( methods[ "copy__b" ].call_1( this.rp ) );
        else if ( val instanceof Buffer   ) super( new GenBuffer( val ) );
        else if ( val == undefined        ) super( new GenBuffer( Buffer.from( [] ) ) );
        else                                super( new GenBuffer( Buffer.from( val as any ) ) );
    }

    static symbol( name: string ): LvBuffer {
        return new LvBuffer( new GenSymbol( LvBuffer, name ) );
    }

    copy(): LvBuffer {
        return new LvBuffer( methods[ "copy__b" ].call_1( this.rp ) );
    }

    toBuffer(): Buffer {
        return methods[ "to_Buffer" ].call_1( this.rp );
    }

    get(): Buffer {
        return this.toBuffer();
    }

    set( nv: Buffer | LvBuffer ) {
        this.rp = nv instanceof Buffer ? methods[ "set__so" ].call_2s( this, new GenBuffer( nv ) ) : methods[ "set__sb" ].call_2s( this, ( nv as LvBuffer ).rp );
        return this;
    }

    get val(): Buffer {
        return this.get();
    }

    set val( nv ) {
        this.set( nv );
    }

    get length(): LvNumber {
        return this.byteLength;
    }

    get byteLength(): LvNumber {
        return new LvNumber( methods[ "get_size__b" ].call_1( this.rp ) );
    }

    /** return value at position `pos` (not to be modified) */
    at( pos: number | LvNumber ): LvNumber {
        return new LvNumber( typeof pos == 'number' ? methods[ "select__bo" ].call_2( this.rp, new BN_FP64( pos ) ) : methods[ "select__bb" ].call_2( this.rp, ( pos as LvNumber ).rp ) );
    }

    /** return a proxy that gives charAt if read, but it also enable writes */
    select( index: number | LvNumber ): LvNumber {
        return new LvNumber( 
            index instanceof LvNumber ? 
                methods["select_ref__bb"].call_2( this.rp, index.rp, this ) :
                methods["select_ref__bo"].call_2( this.rp, new BN_FP64( index ), this )
        );
    }

    // append( val: LvBuffer | LvBuffer ): LvBuffer {
    //     this.rp = typeof val == 'Buffer' ? methods[ "add__so" ].call_2s( this, new GenBuffer( val ) ) : methods[ "add__sb" ].call_2s( this, val.rp );
    //     return this;
    // }

    // /** Joins two or more Buffers, and returns a new joined Buffers */
    // concat( ...val: Array<LvBuffer | LvBuffer> ): LvBuffer {
    //     if ( val.length >  1 ) return this.concat( val[ 0 ] ).concat( ...val.slice( 1 ) );
    //     if ( val.length == 0 ) return this;
    //     const v = val[ 0 ];
    //     return new LvBuffer( typeof v == 'Buffer' ? methods[ "add__bo" ].call_2( this.rp, new GenBuffer( v ) ) : methods[ "add__bb" ].call_2( this.rp, v.rp ) );
    // }

    // insert( pos, sup ) {
    //     methods["self_insert_bb"].call_3( this, Number.rp_conv( pos ), Buffer.rp_conv( sup ) );
    //     return this;
    // }

    // remove( pos, len ) {
    //     methods["self_remove_bb"].call_3( this, Number.rp_conv( pos ), Number.rp_conv( len ) );
    //     return this;
    // }

    // clear() {
    //     this.remove( 0, this.length.toNumber() );
    //     return this;
    // }

    // new_cursor( carets = new Array<Caret>() ) : number {
    //     return methods["new_cursor"].call_1( this, carets );
    // }

    // get_cursor( loc_id : number, dev_id = new DevId ) : Array<Caret> {
    //     return methods["get_cursor"].call_1( this, dev_id, loc_id );
    // }

    // set_cursor( loc_id : number, val : Array<Caret>, dev_id = new DevId ) {
    //     return methods["set_cursor"].call_1( this, dev_id, loc_id, val );
    // }

    // rem_cursor( loc_id : number ) {
    //     return methods["rem_cursor"].call_1( this, new DevId, loc_id );
    // }

    // get_cursor_ids() {
    //     return methods["get_cursor_ids"].call_1( this.rp );
    // }


    // //     charCodeAt: -> // Returns the Unicode of the character at the specified index
    // //     endsWith: -> // Checks whether a Buffer ends with specified Buffer/characters
    // //     fromCharCode: -> // Converts Unicode values to characters
    // //     includes: -> // Checks whether a Buffer contains the specified Buffer/characters
    // //     indexOf: -> // Returns the position of the first found occurrence of a specified value in a Buffer
    // //     lastIndexOf: -> // Returns the position of the last found occurrence of a specified value in a Buffer
    // //     localeCompare: -> // Compares two Buffers in the current locale
    // //     match: -> // Searches a Buffer for a match against a regular expression, and returns the matches
    // //     replace: -> // Searches a Buffer for a specified value, or a regular expression, and returns a new Buffer where the specified values are replaced
    // //     search: -> // Searches a Buffer for a specified value, or regular expression, and returns the position of the match
    // //     slice: -> // Extracts a part of a Buffer and returns a new Buffer
    // //     split: -> // Splits a Buffer into an array of subBuffers
    // //     startsWith: -> // Checks whether a Buffer begins with specified characters
    // //     substr: -> // Extracts the characters from a Buffer, beginning at a specified start position, and through the specified number of character
    // //     subBuffer: -> // Extracts the characters from a Buffer, between two specified indices
    // //     toLocaleLowerCase: -> // Converts a Buffer to lowercase letters, according to the host's locale
    // //     toLocaleUpperCase: -> // Converts a Buffer to uppercase letters, according to the host's locale
    // //     toLowerCase: -> // Converts a Buffer to lowercase letters
    // //     toBuffer: -> // Returns the value of a Buffer object
    // //     toUpperCase: -> // Converts a Buffer to uppercase letters
    // //     trim: -> // Removes whitespace from both ends of a Buffer
    // //     valueOf: -> // Returns the primitive value of a Buffer object
}
