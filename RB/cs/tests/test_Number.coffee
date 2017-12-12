PatchManager = require "../../js/RiotBase/Variable/Ot/PatchManager.js"
Lazyfier     = require "../../js/RiotBase/Variable/Symbol/Lazyfier.js"
Number       = require "../../js/RiotBase/Variable/Number.js"
check        = require "../../js/RiotBase/System/Check.js"

class Methods
    constructor: ( @a ) ->
    is_infeq: ( b ) -> @a <= b
    is_supeq: ( b ) -> @a >= b
    is_inf:   ( b ) -> @a <  b
    is_sup:   ( b ) -> @a >  b
    is_equ:   ( b ) -> @a == b
    is_neq:   ( b ) -> @a != b
    add:      ( b ) -> @a +  b
    sub:      ( b ) -> @a -  b
    mul:      ( b ) -> @a *  b
    div:      ( b ) -> @a /  b
    not:            -> not @a
    neg:            ->  -  @a

test_base_op = ->
    for i in [ "is_inf", "is_infeq", "is_sup", "is_supeq", "is_equ", "is_neq", "add", "sub", "mul", "div" ]
        for [ c0, c1 ] in [ [ 1, 2 ], [ 2, 1 ], [ 1, 1 ] ]
            check new Number( c0 )[ i ]( c1 ), new Methods( c0 )[ i ]( c1 )
    for i in [ "not", "neg" ]
        for c in [ 0, 1 ]
            check new Number( c )[ i ](), new Methods( c )[ i ]()

test_onchange = ( v ) ->
    val = new Number 1
    cpt = 0
    val.onchange -> cpt += 1
    val.self_add v
    check val, 1 + v
    setTimeout ( -> check cpt, v ), 100

test_base_op()
test_onchange 1
test_onchange 0
