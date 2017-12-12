import Variable from "./Core/Variable"
import Caret    from "./String/Caret";

declare var global: any;

//
export default
class String extends Variable {
    static NativeType = ( typeof self != "undefined" ? self : global ).String // shortcut for people willing to import this as 'String';

    constructor( val? ) {
        if      ( val instanceof Rp         ) super( val );
        else if ( val instanceof String     ) super( this.copy().rp );
        else if ( typeof( val ) == "string" ) super( new GenString( val ) );
        else if ( val == undefined          ) super( new GenString( "" ) );
        else throw new Error( "String from a " + val.constructor.name );
    }

    get val() : string {
        return this.toString();
    }

    set val( nv: string ) {
        // try to find what are the changes
        throw new Error( "TODO" );
        // old = this.toString()
        // if val != old
        //     d = val.length - old.length
        //     if d > 0
        //         li = 0
        //         while val[ li ] == old[ li ]
        //             li += 1
        //         ri = val.length - 1
        //         while val[ ri ] == old[ ri - d ] and ri > li
        //             ri -= 1
        //         // TODO: ...
        //         this.insert li, val.substr li, d
        //     else
        //         console.error "TODO"
    }

    toString() : string {
        return Method.methods["to_String"].call_1( this );
    }

    get length() : Number {
        return Method.methods["get_size"].call_1( this );
    }

    copy() : String {
        return Method.methods["copy"].call_1( this );
    }

    charAt( index ) {
        return Method.methods["select_bb"].call_2( this, Number.rp_conv( index ) );// Returns the character at the specified index (position)
    }

    append( sup ) {
        Method.methods["self_add_b"].call_2( this, String.rp_conv( sup ) );
        return this;
    }

    insert( pos, sup ) {
        Method.methods["self_insert_bb"].call_3( this, Number.rp_conv( pos ), String.rp_conv( sup ) );
        return this;
    }

    remove( pos, len ) {
        Method.methods["self_remove_bb"].call_3( this, Number.rp_conv( pos ), Number.rp_conv( len ) );
        return this;
    }

    clear() {
        this.remove( 0, this.length.toNumber() );
        return this;
    }

    new_cursor( carets = new Array<Caret>() ) : number {
        return Method.methods["new_cursor"].call_1( this, carets );
    }

    get_cursor( loc_id : number, dev_id = new DevId ) : Array<Caret> {
        return Method.methods["get_cursor"].call_1( this, dev_id, loc_id );
    }

    set_cursor( loc_id : number, val : Array<Caret>, dev_id = new DevId ) {
        return Method.methods["set_cursor"].call_1( this, dev_id, loc_id, val );
    }

    rem_cursor( loc_id : number ) {
        return Method.methods["rem_cursor"].call_1( this, new DevId, loc_id );
    }

    get_cursor_ids() {
        return Method.methods["get_cursor_ids"].call_1( this );
    }


    //     charCodeAt: -> // Returns the Unicode of the character at the specified index
    //     concat: -> // Joins two or more strings, and returns a new joined strings
    //     endsWith: -> // Checks whether a string ends with specified string/characters
    //     fromCharCode: -> // Converts Unicode values to characters
    //     includes: -> // Checks whether a string contains the specified string/characters
    //     indexOf: -> // Returns the position of the first found occurrence of a specified value in a string
    //     lastIndexOf: -> // Returns the position of the last found occurrence of a specified value in a string
    //     localeCompare: -> // Compares two strings in the current locale
    //     match: -> // Searches a string for a match against a regular expression, and returns the matches
    //     replace: -> // Searches a string for a specified value, or a regular expression, and returns a new string where the specified values are replaced
    //     search: -> // Searches a string for a specified value, or regular expression, and returns the position of the match
    //     slice: -> // Extracts a part of a string and returns a new string
    //     split: -> // Splits a string into an array of substrings
    //     startsWith: -> // Checks whether a string begins with specified characters
    //     substr: -> // Extracts the characters from a string, beginning at a specified start position, and through the specified number of character
    //     substring: -> // Extracts the characters from a string, between two specified indices
    //     toLocaleLowerCase: -> // Converts a string to lowercase letters, according to the host's locale
    //     toLocaleUpperCase: -> // Converts a string to uppercase letters, according to the host's locale
    //     toLowerCase: -> // Converts a string to lowercase letters
    //     toString: -> // Returns the value of a String object
    //     toUpperCase: -> // Converts a string to uppercase letters
    //     trim: -> // Removes whitespace from both ends of a string
    //     valueOf: -> // Returns the primitive value of a String object
}

import GenString from "./String/GenString"
import DevId     from "../System/DevId"
import Method    from "./Core/Method"
import Rp        from "./Core/Rp"
import Number    from "./Number"


//
// import GenString from "./String/GenString"
// import Method    from "./Core/Method"
// import Rp        from "./Core/Rp"
// import Number    from "./Number"
// import Array     from "./Array"
