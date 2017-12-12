BinaryWriter = require "../../js/RiotBase/System/BinaryWriter.js"
BinaryReader = require "../../js/RiotBase/System/BinaryReader.js"
VectorClock  = require "../../js/RiotBase/System/VectorClock.js"
DevId        = require "../../js/RiotBase/System/DevId.js"
check        = require "../../js/RiotBase/System/Check.js"

test_VectorClock_RW = ->
    v = new VectorClock
    v.inc new DevId "oiyvdsACDSZ~"
    v.inc new DevId "dshSCQcxohZ~"
    v.inc new DevId "oiyvdsACDSZ~"
    check v.toString(), "{oiyvdsACDSZ~:2,dshSCQcxohZ~:1}"

    bw = new BinaryWriter
    bw.write_VectorClock v
    bw.write_VectorClock v

    br = new BinaryReader bw.to_Uint8Array()
    check br.read_VectorClock(), "{oiyvdsACDSZ~:2,dshSCQcxohZ~:1}"
    check br.read_VectorClock(), "{oiyvdsACDSZ~:2,dshSCQcxohZ~:1}"

test_VectorClock_update = ->
    v = new VectorClock
    check v.update( new DevId( "oiyvdsACDSZ~" ), 1 ), true
    check v.update( new DevId( "oiyvdsACDSZ~" ), 2 ), true
    console.log v

test_VectorClock_RW()
