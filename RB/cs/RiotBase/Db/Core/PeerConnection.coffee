BinaryWriter   = require './../../System/BinaryWriter.js'
BinaryReader   = require './../../System/BinaryReader.js'

###
Pb: on peut initier plusieurs connections en parallèle... Prop: on accepte l'offre si dev est supérieur. Sinon, on refait l'objet
###
module.exports =
class PeerConnection
    ###~g++ -E -I`pwd`
    #define DECL_CMD( NAME, NUM, ... ) @CMD_##NAME = NUM
    #include <cpp/RiotBase/Db/Core/DbCommandsDecl.h>
    ~###

    @cfg = 'iceServers': [ 'url': 'stun:23.21.150.121' ]

    constructor: ( tracker, dev, desc_offer ) ->
        @tracker       = tracker
        @peer_dev      = dev
        @_bw           = new BinaryWriter
        @_pc           = @_make_pc PeerConnection.cfg, not desc_offer?
        @_ch           = undefined # data channel
        @_connected    = false
        @_initiator    = false
        @_pending_cmds = []

        if desc_offer
            @accept_offer desc_offer
        else
            @initiate()


    # initiate connection
    initiate: ->
        @_initiator = true

        @_pc.onnegotiationneeded = =>
            mediaConstraints = mandatory:
                OfferToReceiveAudio: false
                OfferToReceiveVideo: false
            @_pc.createOffer ( ( offer ) =>
                @_pc.setLocalDescription offer, ( =>
                    @tracker.send_webrtc_offer @peer_dev, JSON.stringify @_pc.localDescription
                ), @_log_error
            ), @_log_error, mediaConstraints

        @_set_channel @_pc.createDataChannel "chat", reliable: false

    # there is an offer. Beware: it @ is an initiator, @accept_offer will reset the RTCPeerConnection to accept the offer
    accept_offer: ( desc ) ->
        if @_initiator
            @_pc        = @_make_pc PeerConnection.cfg
            @_initiator = false

        @_pc.setRemoteDescription new RTCSessionDescription( JSON.parse( desc ) ), ( =>
            @_pc.createAnswer ( ( offer ) =>
                @_pc.setLocalDescription offer, ( =>
                    @tracker.send_webrtc_answer @peer_dev, JSON.stringify @_pc.localDescription
                ), @_log_error
            ), @_log_error
        ), @_log_error

        #
        @_pc.ondatachannel = ( evt ) =>
            @_set_channel evt.channel

    accept_answer: ( desc ) ->
        @_pc.setRemoteDescription new RTCSessionDescription( JSON.parse( desc ) ), ( => ), @_log_error

    add_ice_candidate: ( initiator, desc ) ->
        if initiator ^ @_initiator
            @_pc.addIceCandidate new RTCIceCandidate( JSON.parse( desc ) ), ( => ), @_log_error

    # messages to the peer ----------------------------------
    send_changes: ( inode, patch_data ) ->
        @_send_cmd PeerConnection.CMD_changes, inode, patch_data

    # messages from the peer --------------------------------
    recv_changes: ( inode, patch_data ) ->
        item = @tracker.db.table.find inode
        if not item?
            console.log "TODO"
        an = new BinaryWriter
        bp = new BinaryReader patch_data
        item.patch_manager.app_changes an, bp, @peer_dev, [ @tracker.dev ], @tracker.dev, @tracker.usr

    # helpers -----------------------------------------------
    _read_cmds : ( br ) ->
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

                #define BinaryData      br.read_Uint8Array()
                #define String          br.read_String()
                #define Inode           br.read_Inode()
                #define VectorClock     br.read_VectorClock()
                #define DevId           br.read_DevId()
                #define UsrId           br.read_UsrId()
                #define PI32            br.read_PT()
                #define PI8             br.read_PI8()

                #define DECL_CMD( NAME, NUM, ... ) when NUM then call_cmd( beg_cmd, @recv_##NAME, #NAME, ##__VA_ARGS__ )
                #include <cpp/RiotBase/Db/Core/DbCommandsDecl.h>

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

            #define BinaryData      @_bw.write_Uint8Array( others[ num_in_others += 1 ] )
            #define String          @_bw.write_String( others[ num_in_others += 1 ] )
            #define Inode           @_bw.write_Inode( others[ num_in_others += 1 ] )
            #define VectorClock     @_bw.write_VectorClock( others[ num_in_others += 1 ] )
            #define DevId           @_bw.write_DevId( others[ num_in_others += 1 ] )
            #define UsrId           @_bw.write_UsrId( others[ num_in_others += 1 ] )
            #define PI32            @_bw.write_PT( others[ num_in_others += 1 ] )
            #define PI8             @_bw.write_PI8( others[ num_in_others += 1 ] )

            #define DECL_CMD( NAME, NUM, ... ) when NUM then [ __VA_ARGS__ ]
            #include <cpp/RiotBase/Db/Core/DbCommandsDecl.h>

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
                    @_ch.send @_bw.to_buffer()
                else
                    @_pending_cmds.push @_bw.to_buffer()
                @_bw.clear 32
            ), 1

    _log_error: ( err ) ->
        console.error err

    _make_pc: ( cfg, initiator ) ->
        res = new ( RTCPeerConnection ? webkitRTCPeerConnection ) cfg

        res.onicecandidate = ( evt ) =>
            if evt.candidate
                @tracker.send_ice_candidate @peer_dev, initiator, JSON.stringify evt.candidate

        res

    _set_channel: ( ch ) ->
        @_ch = ch

        @_ch.onopen = ( event ) =>
            @_connected = true
            for m in @_pending_cmds
                @_ch.send m
            @_pending_cmds = []

        @_ch.onmessage = ( event ) =>
            @_read_cmds new BinaryReader event.data
