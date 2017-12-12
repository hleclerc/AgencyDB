BinaryWriter   = require './../../System/BinaryWriter.js'
BinaryReader   = require './../../System/BinaryReader.js'
DevId          = require './../../System/DevId.js'
UsrId          = require './../../System/UsrId.js'
_WebSocket     = WebSocket or require_node( 'ws' )
PeerConnection = require "./PeerConnection.js"

#
module.exports =
class TrackerClient
    ###~g++ -E -I`pwd`
    #define DECL_CMD( NAME, NUM, ... ) @CMD_##NAME = NUM
    #include <cpp/RiotBase/Tracker/Core/TrackerServerCommandsDecl.h>
    ~###

    constructor: ( ip, db ) ->
        #
        @ip                           = ip
        @db                           = db
        @dev                          = new DevId
        @usr                          = new UsrId

        # default configuration
        # @cfg_rtc_peer_connection      = 'iceServers': [ 'url': 'stun:23.21.150.121' ]

        # helper attributes
        @_pending_web_socket_commands = []
        @_timeout_something_to_send   = null # via the websocket
        @_auth_callbacks              = []
        @_web_socket                  = null
        @_connected                   = false
        @_bw                          = new BinaryWriter

        @_peer_connections            = new Map

        # start the stuff
        @_initiate_web_socket ip

    #
    peer_connection_to: ( dev, desc ) ->
        if not @_peer_connections.has( dev.val )
            pc = new PeerConnection this, dev, desc
            @_peer_connections.set dev.val, pc
        @_peer_connections.get dev.val

    # send_..................................................................
    send_auth: ( user, pass, callback ) ->
        @_send_cmd TrackerClient.CMD_login, user, pass
        if callback?
            @_auth_callbacks.push callback

    send_bind_obj: ( inode, vector_clock ) ->
        @_send_cmd TrackerClient.CMD_bind_obj, inode, vector_clock

    send_ice_candidate: ( dev, initiator, sdp ) ->
        @_send_cmd TrackerClient.CMD_ice_candidate, dev, initiator, sdp

    send_webrtc_offer: ( dev, desc ) ->
        @_send_cmd TrackerClient.CMD_webrtc_offer, dev, desc

    send_webrtc_answer: ( dev, desc ) ->
        @_send_cmd TrackerClient.CMD_webrtc_answer, dev, desc

    # recv_..................................................................
    recv_ans_login: ( dev, usr ) ->
        if dev.ext()
            @dev = dev
            @usr = usr
            for c in @_auth_callbacks
                c dev, usr
        else
            console.error "TODO: auth pb"

    recv_bind_obj: ( inode, dev ) ->
        # console.log "TODO: bind", inode, dev
        # console.log @db.dev, inode, dev

    recv_req_inode_data: ( inode, dev, known ) ->
        # find object
        obj = @db.table.find inode
        if not obj?
            console.log "TODO"

        # list of the missing patches
        to_be_sent = []
        obj.patch_manager.freeze_last_mod()
        obj.patch_manager.vector_clock.map.forEach ( num, dev ) =>
            knum = known.val dev
            for snum in [ knum + 1 .. num ]
                to_be_sent.push obj.patch_manager.find_patch_index dev, snum

        # message
        msg = obj.patch_manager.get_msg_for_patch_list @dev, @usr, known, to_be_sent
        pc = @peer_connection_to dev
        pc.send_changes inode, msg

    recv_webrtc_offer: ( dev, desc ) ->
        if dev.val < @dev.val
            @peer_connection_to dev, desc

    recv_webrtc_answer: ( dev, desc ) ->
        pc = @peer_connection_to dev
        pc.accept_answer desc

    recv_ice_candidate: ( dev, initiator, cand ) ->
        pc = @peer_connection_to dev
        pc.add_ice_candidate initiator, cand

    # helpers ...............................................................
    _initiate_web_socket: ( ip ) ->
        if not @_web_socket
            @_web_socket = new WebSocket "wss://#{ip}"

            @_web_socket.onopen = =>
                @_connected = true
                # send pending stuff
                for buf in @_pending_web_socket_commands
                    @_web_socket.send buf
                @_pending_web_socket_commands = []

            @_web_socket.onclose = =>
                @_connected = false
                console.error "websocket has been closed. TODO: retry (somewhere else ?)"

            @_web_socket.onmessage = ( blob ) =>
                callback = ( msg ) =>
                    @_read_cmds new BinaryReader msg

                # blob -> ArrayBuffer
                fileReader = new FileReader
                fileReader.onload = ( toto ) ->
                    callback this.result
                fileReader.readAsArrayBuffer blob.data

    _read_cmds: ( br ) ->
        call_cmd = ( beg_cmd, func, name, args... ) =>
            ccrc = br.make_crc32c beg_cmd
            wcrc = br.read_PI32_le()
            if ccrc != wcrc
                throw "Invalid checksum #{ccrc} != #{wcrc}, args=#{args} for func=#{name}"
            if not func?
                console.error name
            func.apply this, args

        while br.size
            beg_cmd = br.cursor
            cmd = br.read_PI8()
            switch cmd
                ###~g++ -E -I`pwd`

                #define String          br.read_String()
                #define Inode           br.read_Inode()
                #define VectorClock     br.read_VectorClock()
                #define DevId           br.read_DevId()
                #define UsrId           br.read_UsrId()
                #define PI32            br.read_PT()
                #define PI8             br.read_PI8()

                #define DECL_CMD( NAME, NUM, ... ) when NUM then call_cmd( beg_cmd, @recv_##NAME, #NAME, ##__VA_ARGS__ )
                #include <cpp/RiotBase/Tracker/Core/TrackerClientCommandsDecl.h>

                ~###
                else
                    console.error "Undefined command (=#{cmd})"
                    return false
    _send_cmd: ( cmd, others... ) ->
        num_in_others = -1
        bcmd = @_bw.size
        @_bw.write_PI8 cmd
        switch cmd
            ###~g++ -E -I`pwd`

            #define String          @_bw.write_String( others[ num_in_others += 1 ] )
            #define Inode           @_bw.write_Inode( others[ num_in_others += 1 ] )
            #define VectorClock     @_bw.write_VectorClock( others[ num_in_others += 1 ] )
            #define DevId           @_bw.write_DevId( others[ num_in_others += 1 ] )
            #define UsrId           @_bw.write_UsrId( others[ num_in_others += 1 ] )
            #define PI32            @_bw.write_PT( others[ num_in_others += 1 ] )
            #define PI8             @_bw.write_PI8( others[ num_in_others += 1 ] )

            #define DECL_CMD( NAME, NUM, ... ) when NUM then [ __VA_ARGS__ ]
            #include <cpp/RiotBase/Tracker/Core/TrackerServerCommandsDecl.h>

            ~###
            else
                console.error "Undefined command (=#{cmd})"
                return false
        @_bw.write_crc32c bcmd

        # say that we will have things to send
        if not @_timeout_something_to_send
            @_timeout_something_to_send = setTimeout ( =>
                @_timeout_something_to_send = null
                if @_connected
                    @_web_socket.send @_bw.to_buffer()
                else
                    @_pending_web_socket_commands.push @_bw.to_buffer()
                @_bw.clear 32
            ), 1
