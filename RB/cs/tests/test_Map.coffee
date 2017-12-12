BinaryWriter = require "../../js/RiotBase/System/BinaryWriter.js"
String       = require "../../js/RiotBase/Variable/String.js"
Map          = require "../../js/RiotBase/Variable/Map.js"
check        = require "../../js/RiotBase/System/Check.js"

test_base_op = ->
    obj = new Map String, String
    obj.set "a", "A"
    check obj, "{a:A}"
    obj.get( "a" ).set "Z"
    check obj.get( "a" ), "Z"

test_onchange = ->
    cpt = ""
    obj = new Map String, String
    obj.set "a", "A"
    obj.onchange -> cpt += obj.get( "a" ).toString()
    obj.set "a", "A"
    setTimeout ( ->
        obj.set "a", "Z"
        setTimeout ( ->
            check cpt, "Z"
        ), 1
    ), 1

test_ot_pull = ->
    obj = new Map String, String
    obj.set "a", "A"

    c0 = new BinaryWriter
    obj.pull_ot_changes c0
    # console.log c0.to_Uint8Array()

    # new item
    cbj = new Map String, String
    om = cbj.new_ot_merger()
    pd = new BinaryWriter
    om.new_patch pd, c0.to_Uint8Array()
    check cbj, "{a:A}"

    # mod val
    obj.set "a", "B"
    c1 = new BinaryWriter
    obj.pull_ot_changes c1
    om.new_patch pd, c1.to_Uint8Array()
    check cbj, "{a:B}"

    # rem item
    obj.del "a"
    check obj, "{}"
    c2 = new BinaryWriter
    obj.pull_ot_changes c2
    om.new_patch pd, c2.to_Uint8Array()
    check cbj, "{}"


# test_base_op()
# test_onchange()
test_ot_pull()
