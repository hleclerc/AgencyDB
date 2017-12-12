Variable = require "./Core/Variable.js"
require "../System/Property.js"
"use strict"

#
module.exports =
class Array extends Variable
    Variable.make_templ this
    @NativeType = ( window ? global ).Array # shortcut for people willing to import this as 'Array'

    constructor: ( val = [] ) ->
        switch
            when val instanceof Rp               then @rp = val
            when val instanceof Array.NativeType then @rp = GenArray.New val
            when val instanceof Array            then @rp = Method.methods.copy.call_1( val ).rp
            else console.error "TODO", val

        if Proxy?
            proxy_handler =
                get: ( o, n ) ->
                    if typeof( n ) == "string"
                        if not isNaN( index = parseInt n, 10 )
                            return o.at index
                    o[ n ]
                set: ( o, n, v ) ->
                    o[ n ] = v
            return new Proxy this, proxy_handler

    @property 'length',
        get: -> Method.methods.get_size.call_1 this

    @property 'val',
        get: -> @toArray()
        set: ( val ) ->
            ol = @length.val
            nl = val.length
            if ol > nl
                @splice nl, ol - nl
            for i in [ 0 ... Math.min( ol, nl ) ] by 1
                @set i, val[ i ]
            for i in [ ol ... nl ] by 1
                @push val[ i ]

    set: ( key, val ) ->
        if ( rp = Variable.rp_conv_on val ).o
            Method.methods.self_set_o.call_2 @get( key ), rp.v
        else
            Method.methods.self_set_b.call_2 @get( key ), rp.v
        this

    get: ( key ) ->
        Method.methods.mod_select.call_2 this, Number.rp_conv key

    item: ( key ) ->
        2

    at: ( key ) ->
        @get key

    push: ( item ) ->
        insert @length, item

    insert: ( pos, item ) ->
        if ( rp = Variable.rp_conv_on item ).o
            Method.methods.self_insert_bo.call_3 this, Number.rp_conv( pos ), rp.v
        else
            Method.methods.self_insert_bb.call_3 this, Number.rp_conv( pos ), rp.v
        this

    splice: ( pos, len = 1, items... ) ->
        vlen = Number.rp_conv len
        if vlen.val
            Method.methods.self_remove_bb.call_3 this, Number.rp_conv( pos ), vlen
        for item in items
            if ( rp = Variable.rp_conv_on item ).o
                Method.methods.self_insert_bo.call_3 this, Number.rp_conv( pos ), rp.v
            else
                Method.methods.self_insert_bb.call_3 this, Number.rp_conv( pos ), rp.v
        this

    # concat()	Joins two or more arrays, and returns a copy of the joined arrays
    # copyWithin()	Copies array elements within the array, to and from specified positions
    # every()	Checks if every element in an array pass a test
    # fill()	Fill the elements in an array with a static value
    # filter()	Creates a new array with every element in an array that pass a test
    # find()	Returns the value of the first element in an array that pass a test
    # findIndex()	Returns the index of the first element in an array that pass a test
    # forEach()	Calls a function for each array element
    # indexOf()	Search the array for an element and returns its position
    # isArray()	Checks whether an object is an array
    # join()	Joins all elements of an array into a string
    # lastIndexOf()	Search the array for an element, starting at the end, and returns its position
    # map()	Creates a new array with the result of calling a function for each array element
    # pop()	Removes the last element of an array, and returns that element
    # push()	Adds new elements to the end of an array, and returns the new length
    # reduce()	Reduce the values of an array to a single value (going left-to-right)
    # reduceRight()	Reduce the values of an array to a single value (going right-to-left)
    # reverse()	Reverses the order of the elements in an array
    # shift()	Removes the first element of an array, and returns that element
    # slice()	Selects a part of an array, and returns the new array
    # some()	Checks if any of the elements in an array pass a test
    # sort()	Sorts the elements of an array
    # splice()	Adds/Removes elements from an array
    # toString()	Converts an array to a string, and returns the result
    # unshift()	Adds new elements to the beginning of an array, and returns the new length
    # valueOf()	Returns the primitive value of an array

GenArray = require "./Array/GenArray.js"
DevId          = require "../System/DevId.js"
Method         = require "./Core/Method.js"
Rp             = require "./Core/Rp.js"
Number         = require "./Number.js"
