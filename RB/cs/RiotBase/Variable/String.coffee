Variable = require "./Core/Variable.js"
require "../System/Property.js"


#
module.exports =
class String extends Variable
    Variable.make_templ this
    @NativeType = ( window ? global ).String # shortcut for people willing to import this as 'Array'

    constructor: ( val = "" ) ->
        switch
            when val instanceof Rp         then @rp = val
            when typeof( val ) == "string" then @rp = GenString.New val
            else console.error "TODO", val

    @property 'length',
        get: -> Method.methods.get_size.call_1 this

    @property 'val',
        get: -> @toString()
        set: ( val ) ->
            # try to find what are the changes
            old = @toString()
            if val != old
                d = val.length - old.length
                if d > 0
                    li = 0
                    while val[ li ] == old[ li ]
                        li += 1
                    ri = val.length - 1
                    while val[ ri ] == old[ ri - d ] and ri > li
                        ri -= 1
                    # TODO: ...
                    @insert li, val.substr li, d
                else
                    console.error "TODO"


    charAt: ( index ) ->
        Method.methods.select_bb.call_2 this, Number.rp_conv index # Returns the character at the specified index (position)

    append: ( sup ) ->
        Method.methods.self_add_b.call_2 this, String.rp_conv sup
        this

    insert: ( pos, sup ) ->
        Method.methods.self_insert_bb.call_3 this, Number.rp_conv( pos ), String.rp_conv( sup )
        this

    remove: ( pos, len ) ->
        Method.methods.self_remove_bb.call_3 this, Number.rp_conv( pos ), Number.rp_conv( len )
        this

    clear: ->
        @remove 0, @length.toNumber()
        this

    new_cursor: ( pos = [] ) ->
        Method.methods.new_cursor.call_1 this, pos

    get_cursor: ( loc_id, dev_id = new DevId ) ->
        Method.methods.get_cursor.call_1 this, dev_id, loc_id

    rem_cursor: ( loc_id ) ->
        Method.methods.rem_cursor.call_1 this, new DevId, loc_id

    get_cursor_ids: ->
        Method.methods.get_cursor_ids.call_1 this


    #     charCodeAt: -> # Returns the Unicode of the character at the specified index
    #     concat: -> # Joins two or more strings, and returns a new joined strings
    #     endsWith: -> # Checks whether a string ends with specified string/characters
    #     fromCharCode: -> # Converts Unicode values to characters
    #     includes: -> # Checks whether a string contains the specified string/characters
    #     indexOf: -> # Returns the position of the first found occurrence of a specified value in a string
    #     lastIndexOf: -> # Returns the position of the last found occurrence of a specified value in a string
    #     localeCompare: -> # Compares two strings in the current locale
    #     match: -> # Searches a string for a match against a regular expression, and returns the matches
    #     replace: -> # Searches a string for a specified value, or a regular expression, and returns a new string where the specified values are replaced
    #     search: -> # Searches a string for a specified value, or regular expression, and returns the position of the match
    #     slice: -> # Extracts a part of a string and returns a new string
    #     split: -> # Splits a string into an array of substrings
    #     startsWith: -> # Checks whether a string begins with specified characters
    #     substr: -> # Extracts the characters from a string, beginning at a specified start position, and through the specified number of character
    #     substring: -> # Extracts the characters from a string, between two specified indices
    #     toLocaleLowerCase: -> # Converts a string to lowercase letters, according to the host's locale
    #     toLocaleUpperCase: -> # Converts a string to uppercase letters, according to the host's locale
    #     toLowerCase: -> # Converts a string to lowercase letters
    #     toString: -> # Returns the value of a String object
    #     toUpperCase: -> # Converts a string to uppercase letters
    #     trim: -> # Removes whitespace from both ends of a string
    #     valueOf: -> # Returns the primitive value of a String object

GenString = require "./String/GenString.js"
DevId     = require "../System/DevId.js"
Method    = require "./Core/Method.js"
Rp        = require "./Core/Rp.js"
Number    = require "./Number.js"
Array     = require "./Array.js"
