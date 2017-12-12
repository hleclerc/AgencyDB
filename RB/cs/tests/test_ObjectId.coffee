BinaryWriter = require "../../js/RiotBase/System/BinaryWriter.js"
ObjectId     = require "../../js/RiotBase/Variable/ObjectId.js"
Inode        = require "../../js/RiotBase/System/Inode.js"
DevId        = require "../../js/RiotBase/System/DevId.js"
check        = require "../../js/RiotBase/System/Check.js"

test_base_op = ->
    obj = new ObjectId new Inode new DevId( "abc" ), 17
    check obj.toInode(), "17@abc"
    check obj, "17@abc"

test_ot = ->
    inode = new Inode new DevId, 17
    obj = new ObjectId inode

    c0 = new BinaryWriter
    obj.pull_ot_changes c0
    # console.log c0.to_Uint8Array()

    cbj = new ObjectId
    om = cbj.new_ot_merger()
    pd = new BinaryWriter
    om.new_patch pd, c0.to_Uint8Array()
    check cbj, inode


# test_base_op()
test_ot()
