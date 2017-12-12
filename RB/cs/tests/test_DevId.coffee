BinaryWriter = require "../../js/RiotBase/System/BinaryWriter.js"
BinaryReader = require "../../js/RiotBase/System/BinaryReader.js"
DevId        = require "../../js/RiotBase/System/DevId.js"
check        = require "../../js/RiotBase/System/Check.js"

test_RW = ( lst ) ->
    br = new BinaryReader new Uint8Array lst
    di = br.read_DevId()
    console.log di.val

    bw = new BinaryWriter
    bw.write_DevId di
    check bw.to_Uint8Array(), lst

for i in [ 0 .. 4 ]
    test_RW ( 255 * Math.random() | 0 for i in [ 0 ... 8 ] )
