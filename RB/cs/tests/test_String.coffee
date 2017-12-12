PatchManager = require "../../js/RiotBase/Variable/Ot/PatchManager.js"
Lazyfier     = require "../../js/RiotBase/Variable/Symbol/Lazyfier.js"
BinaryWriter = require "../../js/RiotBase/System/BinaryWriter.js"
BinaryReader = require "../../js/RiotBase/System/BinaryReader.js"
String       = require "../../js/RiotBase/Variable/String.js"
DevId        = require "../../js/RiotBase/System/DevId.js"
UsrId        = require "../../js/RiotBase/System/UsrId.js"
check        = require "../../js/RiotBase/System/Check.js"

test_base_op = ->
    str = new String "abcd"
    check str, "abcd"
    check str.length, 4
    check str.charAt( 2 ), "c"
    check str.insert( 1, "Z" ), "aZbcd"
    check str.append( "efg" ), "aZbcdefg"
    check str.remove( 2, 1 ), "aZcdefg"

test_BinaryWriter = ->
    bw = new BinaryWriter
    str = new String "abcd"
    bw.write_String str.toString()
    check bw, "4,97,98,99,100"

test_sym = ->
    sym = Lazyfier.symbol Str, "s"
    check sym, "s"
    check sym.charAt( 2 ), "select(s,2)"

test_on_change = ->
    a = new String "a"
    cpt = 0
    cid = a.onchange -> cpt += 1

    a.append "b"

    setTimeout ( ->
        check cpt, 1
        a.rm_onchange cid
        a.append "c"
        setTimeout ( ->
            check a, "abc"
            check cpt, 1
        ), 100
    ), 100

test_patch = ->
    a = new String "ab"
    changes = new BinaryWriter
    a.pull_ot_changes changes
    check changes, "4,0,2,97,98"
    # basic rights
    check new Boolean( a.rp.right_for_op_insert new UsrId, 0, "..." ), true
    # basic insertion
    check a.insert( 1, "0" ), "a0b"
    # cursors
    c0 = a.new_cursor 1
    c1 = a.new_cursor 2
    c1.set 2
    check c0, 0
    check c1, 2
    check c1.get(), 2

    check a.insert( 1, "1" ), "a10b"
    check c0, 0
    check c1, 3

    changes.clear()
    a.pull_ot_changes changes
    check changes, "4,1,1,48,7,0,1,0,7,0,2,0,9,0,2,2,0,4,1,1,49"

test_patch_manager = ->
    d0 = new DevId "gMWDnGjZfxZ~"
    d1 = new DevId "O5HeHcHU1Io~"
    us = new UsrId d0, 17

    s0 = new String "ab"
    p0 = new PatchManager s0
    c0 = p0.get_changes d1, d0, us
    # console.log c0

    s1 = new String
    p1 = new PatchManager s1
    aw = new BinaryWriter
    p1.app_changes aw, new BinaryReader( c0 ), d0, [ d1 ], d1, us
    console.log s1.toString()

test_parallel_changes = ->
    tpc = ( cl, res, init, fun ) ->
        class Machine
            constructor: ( dev_id ) ->
                @usr_id = new UsrId new DevId( "gMWDnGjZfxZ~" ), 17
                @dev_id = dev_id
                @d = new cl
                @pm = new PatchManager @d

        class System
            constructor: ->
                @m = [
                    new Machine new DevId "gMWDnGjZfxZ~"
                    new Machine new DevId "O5HeHcHU1Io~"
                    # new Machine new DevId "ujtameVI3fN~"
                ]

            sync: ( lm = @m ) ->
                # get_changes
                pd = for m0 in lm
                    for m1 in lm
                        if m0 != m1
                            m1.pm.get_changes m0.dev_id, m1.dev_id, m1.usr_id # m1 -> m0
                # app_changes
                for m0, n0 in lm
                    for m1, n1 in lm
                        if m0 != m1
                            ans = new BinaryWriter
                            console.log n0, n1, pd[ n0 ][ n1 ]
                            m0.pm.app_changes ans, new BinaryReader( pd[ n0 ][ n1 ] ), m1.dev_id, [ m0.dev_id ], m0.dev_id, m0.usr_id


        # initialisation of system
        s = new System
        s.m[ 0 ].d.set init
        s.sync()

        # operations
        fun s
        s.sync()

        # check that everything is correct
        lst_ans = for m in s.m
            m.d.toString()

        ans = lst_ans[ 0 ]
        for v, i in lst_ans when i
            if v != ans
                ans += " [#{v}]"
        check ans, res

    # tpc String, "ab", "", ( s ) ->
    #     s.m[ 0 ].d.append "ab"

    tpc String, "ac", "abc", ( s ) ->
        s.m[ 0 ].d.remove 1, 1



test_selection = ->
    s = new String "abc"
    c = s.new_cursor 1
    c.val = [ [ 2, 2 ] ]
    check c, "[[2,2]]"
    check c.length, 1
    s.remove 1, 1
    check c, "[[1,1]]"
    s.insert 0, "A"
    check c, "[[2,2]]"


# test_base_op()
# test_BinaryWriter()
# test_sym()
# test_on_change()
# test_patch()
# test_patch_manager()
test_parallel_changes()
# test_selection()
