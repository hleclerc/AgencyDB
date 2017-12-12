Base64o = require "../../js/RiotBase/System/Base64o.js"
check   = require "../../js/RiotBase/System/Check.js"

#
test_Base64 = ->
    for i in [ 0 .. 254 ] by 7
        check Base64o.encode( [i,i+1,0,0,0,0,0,0] ) < Base64o.encode( [i+1,i,0,0,0,0,0,0] ), true

    for nt in [ 0 ... 12 ]
        a = ( Math.floor( Math.random() * 256 ) for i in [ 0 ... Math.floor( Math.random() * 10 ) ] )
        check Base64o.decode( Base64o.encode( a ) ),  a

test_Base64()
console.log Base64o.encode [0,0,0,0,0,0,0,1]
