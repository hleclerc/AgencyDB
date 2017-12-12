
# good for search. Bad for insertion
module.exports =
class SortedArray
    constructor: ( items = [] ) ->
        @items = items

    toString: ->
        "" + @items

    # if val_if_not_found is defined, it is called to make a new item
    # else, if not item found for key `key`, return undefined
    get: ( key, val_if_not_found ) ->
        beg = 0
        end = @items.length
        while end > beg
            mid = Math.floor beg + ( end - beg ) / 2
            if @items[ mid ].inf key
                beg = mid + 1
            else
                end = mid
        if beg == @items.length or @items[ beg ].sup( key )
            res = val_if_not_found?()
            if res?
                @items.splice beg, 0, res
            return res
        @items[ beg ]

    # set or replace
    set: ( key, val ) ->
        beg = 0
        end = @items.length
        while end > beg
            mid = Math.floor beg + ( end - beg ) / 2
            if @items[ mid ].inf key
                beg = mid + 1
            else
                end = mid
        if beg == @items.length or @items[ beg ].sup( key )
            @items.splice beg, 0, val
        else
            @items.splice beg, 1, val

    del: ( key ) ->
        beg = 0
        end = @items.length
        while end > beg
            mid = Math.floor beg + ( end - beg ) / 2
            if @items[ mid ].inf key
                beg = mid + 1
            else
                end = mid
        if beg != @items.length and not @items[ beg ].sup( key )
            @items.splice beg, 1
