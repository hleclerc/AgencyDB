SortedArray = require "../../js/RiotBase/System/SortedArray.js"

#
test_SortedArray = ->
    class Obj
        constructor: ( @val ) ->
        inf: ( val ) -> @val < val
        sup: ( val ) -> @val > val
        toString: -> "" + @val

    arr = new SortedArray [ new Obj 1 ]
    for i in [ 0 .. 20 ]
        val = Math.floor( Math.random() * 10 )
        arr.get val, -> new Obj val
    console.log arr.toString()

test_SortedArray()
