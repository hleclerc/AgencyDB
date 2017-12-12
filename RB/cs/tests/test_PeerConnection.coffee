PeerConnection = require "../../js/RiotBase/Db/Core/PeerConnection.js"
TrackerClient  = require "../../js/RiotBase/Db/Core/TrackerClient.js"
check          = require "../../js/RiotBase/System/Check.js"

VectorClock    = require "../../js/RiotBase/System/VectorClock.js"
Inode          = require "../../js/RiotBase/System/Inode.js"

#
test_conn = ->
    tracker_list = []
    for i in [ 0 ... 2 ]
        do ( tracker = new TrackerClient "localhost:8676" ) ->
            tracker.send_auth "root", "my_pass", ( dev, usr ) ->
                tracker_list.push tracker
                if tracker_list.length == 2
                    conn()

    conn = ->
        pc_01 = tracker_list[ 0 ].peer_connection_to tracker_list[ 1 ].dev
        pc_01.send_changes new Inode, new Uint8Array [1,2,3]


test_conn()
