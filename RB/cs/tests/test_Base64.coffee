Base64 = require "../../js/RiotBase/System/Base64.js"
check  = require "../../js/RiotBase/System/Check.js"

#
test_Base64 = ->
    check Base64.encode( [1,2,3,4,5,6,7,8] ), "AQIDBAUGBwg="
    check Base64.decode( "AQIDBAUGBwg=" ), [1,2,3,4,5,6,7,8]

    for nt in [ 0 ... 12 ]
        a = ( Math.floor( Math.random() * 256 ) for i in [ 0 ... Math.floor( Math.random() * 10 ) ] )
        check Base64.decode( Base64.encode( a ) ),  a


test_Base64()
