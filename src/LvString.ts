import Variable, { toLv_array } from "./Core/Variable"
import methods                  from "./Core/methods"
import Rp                       from "./Core/Rp"
               
import RpString                from "./LvString/RpString"
import RpSymbol                from "./Symbol/RpSymbol"
import { BN_PT }                from "./LvNumber/Bn"
import LvNumber                 from "./LvNumber"
             
// import Caret                    from "./String/Caret";
// import DevId                    from "../System/DevId"

//
export default
class LvString extends Variable<LvString> {
    static NativeType = String;

    constructor( val = "" as Rp | LvString | string ) {
        if      ( val instanceof Rp       ) super( val );
        else if ( val instanceof LvString ) super( methods[ "copy__b" ].call_1( this.rp ) );
        else                                super( LvString.makeRp( val ) );
    }

    static makeRp( val: string ): Rp {
        return new RpString( val );
    }

    static symbol( name: string ): LvString {
        return new LvString( new RpSymbol( LvString, name ) );
    }

    copy(): LvString {
        return new LvString( methods[ "copy__b" ].call_1( this.rp ) );
    }

    get(): string {
        return this.toString();
    }

    set( nv: string | LvString ) {
        this.rp = typeof nv == 'string' ? methods[ "set__so" ].call_2s( this, new RpString( nv ) ) : methods[ "set__sb" ].call_2s( this, ( nv as LvString ).rp );
        return this;
    }

    get val(): string {
        return this.get();
    }

    set val( nv ) {
        this.set( nv );
    }

    get length(): LvNumber {
        return new LvNumber( methods[ "get_size__b" ].call_1( this.rp ) );
    }

    /** */
    charAt( index: number | LvNumber ): LvString {
        return new LvString( typeof index == 'number' ? methods[ "select__bo" ].call_2( this.rp, new BN_PT( index ) ) : methods[ "select__bb" ].call_2( this.rp, ( index as LvNumber ).rp ) );
    }

    /** */
    substr( from: number | LvNumber, length: number | LvNumber ): LvString {
        return new LvString( typeof from == 'number' ? (
            typeof length == 'number' ?
                methods["slice__boo"].call_3( this.rp, new BN_PT( from ), new BN_PT( from + length )                                ) :
                methods["slice__boo"].call_3( this.rp, new BN_PT( from ), methods["add__ob"].call_2( new BN_PT( from ), length.rp ) )
        ) : (
            typeof length == 'number' ?
                methods["slice__bbo"].call_3( this.rp, from.rp          , methods["add__bo"].call_2( from.rp, new BN_PT( length ) ) ) :
                methods["slice__bbo"].call_3( this.rp, from.rp          , methods["add__bb"].call_2( from.rp, length.rp )           )
        ) );
    }

    /** */
    beginning( length: number | LvNumber ): LvString {
        return new LvString( typeof length == 'number' ?
            methods["beginning__bo"].call_2( this.rp, new BN_PT( length ) ) :
            methods["beginning__bb"].call_2( this.rp, length.rp           )
        );
    }

    /** */
    selfBeginning( from: number | LvNumber ): LvString {
        this.rp = typeof from == 'number' ?
            methods["beginning__so"].call_2s( this, new BN_PT( from ) ) :
            methods["beginning__sb"].call_2s( this, from.rp           );
        return this;
    }

    /** */
    ending( from: number | LvNumber ): LvString {
        return new LvString( typeof from == 'number' ?
            methods["ending__bo"].call_2( this.rp, new BN_PT( from ) ) :
            methods["ending__bb"].call_2( this.rp, from.rp           )
        );
    }

    /** */
    selfEnding( from: number | LvNumber ): LvString {
        this.rp = typeof from == 'number' ?
            methods["ending__so"].call_2s( this, new BN_PT( from ) ) :
            methods["ending__sb"].call_2s( this, from.rp           );
        return this;
    }

    /** */
    substring( beg: number | LvNumber, end: number | LvNumber ): LvString {
        return new LvString( typeof beg == 'number' ? (
            typeof end == 'number' ?
                methods["slice__boo"].call_3( this.rp, new BN_PT( beg ), new BN_PT( end ) ) :
                methods["slice__bob"].call_3( this.rp, new BN_PT( beg ), end.rp           )
        ) : (
            typeof end == 'number' ?
                methods["slice__bbo"].call_3( this.rp, beg.rp          , new BN_PT( end ) ) :
                methods["slice__bbb"].call_3( this.rp, beg.rp          , end.rp           )
        ) );
    }

    /** returns a proxy that gives charAt if read, and enables writes */
    select( index: number | LvNumber ): LvString {
        return new LvString( 
            index instanceof LvNumber ? 
                methods["select_ref__ob"].call_2( this.rp, index.rp, this ) :
                methods["select_ref__oo"].call_2( this.rp, new BN_PT( index ), this )
        );
    }

    selfConcat( val: LvString | string ): LvString {
        this.rp = typeof val == 'string' ? methods[ "add__so" ].call_2s( this, new RpString( val ) ) : methods[ "add__sb" ].call_2s( this, val.rp );
        return this;
    }

    /** Joins two or more strings, and returns a new joined strings */
    concat( ...val: Array<LvString | string> ): LvString {
        if ( val.length >  1 ) return this.concat( val[ 0 ] ).concat( ...val.slice( 1 ) );
        if ( val.length == 0 ) return this;
        const v = val[ 0 ];
        return new LvString( typeof v == 'string' ? methods[ "add__bo" ].call_2( this.rp, new RpString( v ) ) : methods[ "add__bb" ].call_2( this.rp, v.rp ) );
    }

    /** insert `sup` at position `pos` */
    insert( pos: LvNumber | number, sup: LvString | string ): LvString {
        this.rp = typeof pos == 'number' ? (
            typeof sup == 'string' ?
                methods["insert__soo"].call_3s( this, new BN_PT( pos ), new RpString( sup ) ) :
                methods["insert__sob"].call_3s( this, new BN_PT( pos ), sup.rp               )
        ) : (
            typeof sup == 'string' ?
                methods["insert__soo"].call_3s( this, pos.rp            , new RpString( sup ) ) :
                methods["insert__sob"].call_3s( this, pos.rp            , sup.rp               )
        );
        return this;
    }

    /** insert `len` characters from position `pos` */
    remove( pos: LvNumber | number, len: LvNumber | number ): LvString {
        this.rp = typeof pos == 'number' ? (
            typeof len == 'number' ?
                methods["remove__soo"].call_3s( this, new BN_PT( pos ), new BN_PT( len ) ) :
                methods["remove__sob"].call_3s( this, new BN_PT( pos ), len.rp           )
        ) : (
            typeof len == 'number' ?
                methods["remove__soo"].call_3s( this, pos.rp            , new BN_PT( len ) ) :
                methods["remove__sob"].call_3s( this, pos.rp            , len.rp           )
        );
        return this;
    }

    /** set lenth to 0 */
    clear(): LvString {
        return this.remove( 0, this.length.toNumber() );
    }

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
    // //     endsWith: -> // Checks whether a string ends with specified string/characters
    // //     fromCharCode: -> // Converts Unicode values to characters
    // //     includes: -> // Checks whether a string contains the specified string/characters
    // //     indexOf: -> // Returns the position of the first found occurrence of a specified value in a string
    // //     lastIndexOf: -> // Returns the position of the last found occurrence of a specified value in a string
    // //     localeCompare: -> // Compares two strings in the current locale
    // //     match: -> // Searches a string for a match against a regular expression, and returns the matches
    // //     replace: -> // Searches a string for a specified value, or a regular expression, and returns a new string where the specified values are replaced
    // //     search: -> // Searches a string for a specified value, or regular expression, and returns the position of the match
    // //     slice: -> // Extracts a part of a string and returns a new string
    // //     split: -> // Splits a string into an array of substrings
    // //     startsWith: -> // Checks whether a string begins with specified characters
    // //     substr: -> // Extracts the characters from a string, beginning at a specified start position, and through the specified number of character
    // //     substring: -> // Extracts the characters from a string, between two specified indices
    // //     toLocaleLowerCase: -> // Converts a string to lowercase letters, according to the host's locale
    // //     toLocaleUpperCase: -> // Converts a string to uppercase letters, according to the host's locale
    // //     toLowerCase: -> // Converts a string to lowercase letters
    // //     toString: -> // Returns the value of a String object
    // //     toUpperCase: -> // Converts a string to uppercase letters
    // //     trim: -> // Removes whitespace from both ends of a string
    // //     valueOf: -> // Returns the primitive value of a String object
}

toLv_array.push( val =>
    typeof val == "string" ? new LvString( val ) : null
);
