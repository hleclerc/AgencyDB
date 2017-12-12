Number = require "../../js/RiotBase/Variable/Number.js"
Array  = require "../../js/RiotBase/Variable/Array.js"
check  = require "../../js/RiotBase/System/Check.js"

test_base_op = ->
    # insert, remove, display
    a = new Array [ 1, 2 ]
    a.insert 2, new Number 3
    check a.length, 3
    check a, "[1,2,3]"
    a.splice 1, 1
    check a, "[1,3]"

    # array of array
    a.insert 1, [ "a", "b" ]
    check a, "[1,[a,b],3]"

    # selection
    a.at( 0 ).val = 10
    a.at( 2 ).val = 30
    check a, "[10,[a,b],30]"

    # proxy
    if Proxy?
        check a[ 0 ], "10"

test_change_num = ->
    cpt = 0
    a = new Array [ 1, 2 ]
    a.onchange -> cpt += a.at( 0 ).toNumber()
    a.val = [ 1, 2 ]
    setTimeout ( ->
        a.val = [ 10, 2 ]
        setTimeout ( ->
            check cpt, 10
        ), 1
    ), 1

test_change_arr = ->
    cpt = ""
    a = new Array [ [ 1, "a" ], 2 ]
    a.onchange -> cpt += a.at( 0 ).at( 1 ).toString()

    a.val = [ [ 1, "a" ], 2 ]
    setTimeout ( ->
        a.val = [ [ 1, "b" ], 2 ]
        setTimeout ( ->
            check a, "[[1,b],2]"
            check cpt, "b"
        ), 1
    ), 1

test_base_op()
test_change_num()
test_change_arr()
