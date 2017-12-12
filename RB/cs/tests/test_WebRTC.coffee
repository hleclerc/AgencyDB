`import {TrackerClient} from "/js/TrackerClient"`
# `import { crc32c } from "/js/Crc32c"`


tc = new TrackerClient()

tc.connect "root", "my_pass"
tc.initiate_rtc_peer_connection "127.0.0.1"




# # RTCPeerConnection = null
# # getUserMedia = null
# # attachMediaStream = null
# # reattachMediaStream = null
# # webrtcDetectedBrowser = null
# # 
# # if navigator.mozGetUserMedia
# #     webrtcDetectedBrowser = 'firefox'
# #     RTCPeerConnection = mozRTCPeerConnection
# #     RTCSessionDescription = mozRTCSessionDescription
# #     RTCIceCandidate = mozRTCIceCandidate
# #     getUserMedia = navigator.mozGetUserMedia.bind(navigator)
# #     attachMediaStream = (element, stream) ->
# #         element.mozSrcObject = stream
# #         element.play()
# #     reattachMediaStream = (to, from) ->
# #         console.log('Reattaching media stream')
# #         to.mozSrcObject = from.mozSrcObject
# #         to.play()
# #     MediaStream.prototype.getVideoTracks = ->
# #         []
# #     MediaStream.prototype.getAudioTracks = ->
# #         []
# # else if navigator.webkitGetUserMedia
# #     webrtcDetectedBrowser = 'chrome'
# #     RTCPeerConnection = webkitRTCPeerConnection
# #     getUserMedia = navigator.webkitGetUserMedia.bind(navigator)
# #     attachMediaStream = (element, stream) ->
# #         element.src = webkitURL.createObjectURL(stream)
# #     reattachMediaStream = ( to, from ) ->
# #         to.src = from.src
# #     if not webkitMediaStream.prototype.getVideoTracks
# #         webkitMediaStream.prototype.getVideoTracks = ->
# #             this.videoTracks
# #         webkitMediaStream.prototype.getAudioTracks = ->
# #             this.audioTracks
# #     if not webkitRTCPeerConnection.prototype.getLocalStreams
# #         webkitRTCPeerConnection.prototype.getLocalStreams = ->
# #             this.localStreams
# #         webkitRTCPeerConnection.prototype.getRemoteStreams = ->
# #             this.remoteStreams
# # else
# #     console.err 'Browser does not appear to be WebRTC-capable'
# if navigator.webkitGetUserMedia
#     RTCPeerConnection = webkitRTCPeerConnection
# 
# 
# # RTCPeerConnection
# cfg = 'iceServers': [ 'url': 'stun:23.21.150.121' ]
# con = 'optional': [ 'DtlsSrtpKeyAgreement': true ]
# peerConnection = new RTCPeerConnection cfg, con
# 
# peerConnection.onicecandidate = (e) ->
#     console.log 'ICE candidate', e
#     #   if e.candidate == null
#     #     $('#localOffer').html(JSON.stringify(pc1.localDescription))
# 
# peerConnection.onnegotiationneeded = ->
#     console.log 'onnegotiationneeded'
#     # pc.createOffer(localDescCreated, logError);
# 
# # DataChannel
# dataChannelOptions =
#     ordered: false # do not guarantee order
#     reliable: false
#     maxRetransmits: false
#         
# dataChannel = peerConnection.createDataChannel "myLabel", dataChannelOptions
# dataChannel.onerror   = (error) -> console.log "Data Channel Error:", error
# dataChannel.onmessage = (event) -> console.log "Got Data Channel Message:", event.data
# dataChannel.onopen    = -> dataChannel.send "Hello World!"
# dataChannel.onclose   = -> console.log "The Data Channel is Closed"
# 
# # create offer
# onCreateSessionDescriptionError = ( error ) ->
#     console.err 'Failed to create session description: ' + error.toString()
# 
# gotDescription2 = ( desc ) ->
#     peerConnection.setRemoteDescription desc
#     console.log desc
#     #remoteConnection.setLocalDescription desc
#     #localConnection.setRemoteDescription desc
#   
# gotDescription1 = ( desc ) ->
#     # console.log "sending desc: ", desc.sdp
#     
#     req = new XMLHttpRequest
#     req.open "POST", "https://localhost:8676/new_connection", true
#     req.onreadystatechange = ( aEvt ) ->
#         if req.readyState == 4
#             if req.status == 200
#                 gotDescription2 JSON.parse req.responseText
#             else
#                 console.log "Erreur pendant le chargement de la page.", req.responseText
#     req.send "#{encodeURIComponent "root"}\n#{encodeURIComponent "my_pass"}\n#{JSON.stringify desc}"
# 
# 
# peerConnection.createOffer gotDescription1, onCreateSessionDescriptionError
# 
# 
# # test entre navigateurs. Envoi de l'offre de A à B (setLocalDescription).
# # Envoi de la réponse de B à A (setRemoteDescription) en passant par S... Comment A peut recevoir un message du serveur sans connection active ?
# # pour la connection purement entre navigateurs, il semble qu'on ne peut pas se passer de connections TCP
# # 
# # Autre solution: on met en place une session WebRTC entre A et S, puis entre B et S et A et S.
# # Ça implique que S réponde par une offre WebRTC.
# 
# # offre:
# #   v=0
# #   o=- 556282638653384614 2 IN IP4 127.0.0.1
# #   s=-
# #   t=0 0
# #   a=msid-semantic: WMS
# #   m=application 9 DTLS/SCTP 5000
# #   c=IN IP4 0.0.0.0
# #   a=ice-ufrag:QS1AZGRFJdXAwN4b
# #   a=ice-pwd:uDpO0/lHtqzq888zh1Sw9Ow1
# #   a=fingerprint:sha-256 2E:DB:02:74:E1:9B:16:F1:E3:1E:16:18:B3:72:31:1A:17:1C:06:29:33:0D:0C:4C:AE:EB:49:0D:DC:73:26:9F
# #   a=setup:actpass
# #   a=mid:data
# #   a=sctpmap:5000 webrtc-datachannel 1024
# # 
# # réponse:
# # {"type":"answer","sdp":"
# #   v=0
# #   o=- 4325449460774275391 2 IN IP4 127.0.0.1
# #   s=-t=0 0
# #   a=group:BUNDLE audio video data
# #   a=msid-semantic: WMS xAvw7XVOUPhhopgBpCZFWqeYI4h4OUf5twxf
# #   m=audio 36058 UDP/TLS/RTP/SAVPF 111 103 104 9 0 8 106 105 13 126
# #   c=IN IP4 192.168.0.31
# #   a=rtcp:9 IN IP4 0.0.0.0
# #   a=candidate:2075100108 1 udp 2122262783 2a01:e35:2427:d3b0:c2aa:771d:5537:47dd 58082 typ host generation 0
# #   a=candidate:2533458265 1 udp 2122194687 192.168.0.31 36058 typ host generation 0
# #   a=ice-ufrag:Fv9lKnp+RnVj9U7t
# #   a=ice-pwd:RwfbCQZpOmvWD49PfSxyuALu
# #   a=fingerprint:sha-256 07:43:00:43:78:CF:D9:A5:14:45:0D:E3:03:77:C2:15:2C:31:CC:3E:02:15:87:9C:09:59:F2:FB:AC:EF:A2:C2
# #   a=setup:active
# #   a=mid:audio
# #   a=extmap:1 urn:ietf:params:rtp-hdrext:ssrc-audio-level
# #   a=extmap:3 http://www.webrtc.org/experiments/rtp-hdrext/abs-send-time
# #   a=sendrecv
# #   a=rtcp-mux
# #   a=rtpmap:111 opus/48000/2
# #   a=fmtp:111 minptime=10; useinbandfec=1
# #   a=rtpmap:103 ISAC/16000
# #   a=rtpmap:104 ISAC/32000
# #   a=rtpmap:9 G722/8000
# #   a=rtpmap:0 PCMU/8000
# #   a=rtpmap:8 PCMA/8000
# #   a=rtpmap:106 CN/32000
# #   a=rtpmap:105 CN/16000
# #   a=rtpmap:13 CN/8000
# #   a=rtpmap:126 telephone-event/8000
# #   a=maxptime:60
# #   a=ssrc:3897524309 cname:5Rpt6DbIG24ZiMCC
# #   a=ssrc:3897524309 msid:xAvw7XVOUPhhopgBpCZFWqeYI4h4OUf5twxf 8323bd95-87f4-4f74-b78a-8d86bdea37c3
# #   a=ssrc:3897524309 mslabel:xAvw7XVOUPhhopgBpCZFWqeYI4h4OUf5twxf
# #   a=ssrc:3897524309 label:8323bd95-87f4-4f74-b78a-8d86bdea37c3
# #   m=video 9 UDP/TLS/RTP/SAVPF 100 101 116 117 96
# #   c=IN IP4 0.0.0.0
# #   a=rtcp:9 IN IP4 0.0.0.0
# #   a=ice-ufrag:Fv9lKnp+RnVj9U7t
# #   a=ice-pwd:RwfbCQZpOmvWD49PfSxyuALu
# #   a=fingerprint:sha-256 07:43:00:43:78:CF:D9:A5:14:45:0D:E3:03:77:C2:15:2C:31:CC:3E:02:15:87:9C:09:59:F2:FB:AC:EF:A2:C2a=setup:active
# #   a=mid:video
# #   a=extmap:2 urn:ietf:params:rtp-hdrext:toffset
# #   a=extmap:3 http://www.webrtc.org/experiments/rtp-hdrext/abs-send-time
# #   a=extmap:4 urn:3gpp:video-orientation
# #   a=inactive
# #   a=rtcp-mux
# #   a=rtpmap:100 VP8/90000
# #   a=rtcp-fb:100 ccm fir
# #   a=rtcp-fb:100 nack
# #   a=rtcp-fb:100 nack pli
# #   a=rtcp-fb:100 goog-remb
# #   a=rtpmap:101 VP9/90000
# #   a=rtcp-fb:101 ccm fir
# #   a=rtcp-fb:101 nack
# #   a=rtcp-fb:101 nack pli
# #   a=rtcp-fb:101 goog-remb
# #   a=rtpmap:116 red/90000
# #   a=rtpmap:117 ulpfec/90000
# #   a=rtpmap:96 rtx/90000
# #   a=fmtp:96 apt=100
# #   m=application 9 DTLS/SCTP 5000
# #   c=IN IP4 0.0.0.0
# #   b=AS:30
# #   a=ice-ufrag:Fv9lKnp+RnVj9U7t
# #   a=ice-pwd:RwfbCQZpOmvWD49PfSxyuALu
# #   a=fingerprint:sha-256 07:43:00:43:78:CF:D9:A5:14:45:0D:E3:03:77:C2:15:2C:31:CC:3E:02:15:87:9C:09:59:F2:FB:AC:EF:A2:C2
# #   a=setup:active
# #   a=mid:data
# #   a=sctpmap:5000 webrtc-datachannel 1024
# # "}
# 
# #   localConnection.setLocalDescription desc
# #   remoteConnection.setRemoteDescription desc
# #   remoteConnection.createAnswer gotDescription2, onCreateSessionDescriptionError
# 
# 
# 
# 
# 
# 
# 
# 
# 
# 
# 
# 
# 
# 
# 
# 
# # --------------------------------------------------
# 
# 
# # proposition: on envoie la proposition au tracker en https avec l'authentification.
# #  -> on peut répondre en faisant une websocket
# #  -> ou communiquer en WebRTC...
# #     => le tracker peut répondre 
# #     => un canal vers chaque tracker, et potentiellement un canal par inode. On peut aussi mutualiser les 
# 
# 
# # 
# # pc1 = new RTCPeerConnection 
# # dc1 = pc1.createDataChannel 'test', { reliable: true }
# 
# 
# 
# # navigator.getUserMedia = navigator.getUserMedia or
# #                          navigator.webkitGetUserMedia or
# #                          navigator.mozGetUserMedia or
# #                          navigator.msGetUserMedia
# # navigator.getUserMedia { video: false, audio: false },
# #     (stream) ->
# #         setupDC1()
# #         pc1.createOffer  (desc) ->
# #             pc1.setLocalDescription(desc, function () {}, function () {})
# #                 console.log('created local offer', desc)
# #         },
# #         function () { console.warn("Couldn't create offer") },
# #         sdpConstraints)
# #     , (error) ->
# #         console.log 'Error adding stream to pc1: ' + error
# 
# # sdpConstraints = 
# #   optional: []
# #   mandatory:
# #     OfferToReceiveAudio: false,
# #     OfferToReceiveVideo: false
# #   
# # dc1 = pc1.createDataChannel 'test', reliable: true
# #     
# # createLocalOffer = ->
# #   console.log('video1')
# #   navigator.getUserMedia = navigator.getUserMedia ||
# #                            navigator.webkitGetUserMedia ||
# #                            navigator.mozGetUserMedia ||
# #                            navigator.msGetUserMedia
# #   navigator.getUserMedia({video: true, audio: true}, function (stream) {
# #     var video = document.getElementById('localVideo')
# #     video.src = window.URL.createObjectURL(stream)
# #     video.play()
# #     pc1.addStream(stream)
# #     console.log(stream)
# #     console.log('adding stream to pc1')
# #     setupDC1()
# #     pc1.createOffer(function (desc) {
# #       pc1.setLocalDescription(desc, function () {}, function () {})
# #       console.log('created local offer', desc)
# #     },
# #     function () { console.warn("Couldn't create offer") },
# #     sdpConstraints)
# #   }, function (error) {
# #     console.log('Error adding stream to pc1: ' + error)
# #   })
# # 
