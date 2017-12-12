import PatchManager from "../../Variable/Ot/PatchManager";
import PatchTypes   from "../../Variable/Ot/PatchTypes";
import BinaryWriter from '../../System/BinaryWriter';
import BinaryReader from '../../System/BinaryReader';
import Inode        from '../../System/Inode';
import DevId        from '../../System/DevId';

/// Pb: on peut initier plusieurs connections en parallèle... Prop: on accepte l'offre si dev est supérieur. Sinon, on refait l'objet
export default
class PeerConnection {
    static cfg = { 'iceServers': [ ] }; //  { 'urls': 'stun:23.21.150.121' }

    constructor( tracker : TrackerClient, dev : DevId, desc_offer? : string ) {
        this.tracker       = tracker;
        this.peer_dev      = dev;
        this._bw           = new BinaryWriter;
        this._pc           = this._make_pc( PeerConnection.cfg, ! desc_offer );
        this._ch           = undefined; // data channel
        this._connected    = false;
        this._initiator    = false;
        this._pending_cmds = [];

        if ( desc_offer )
            this._accept_offer( desc_offer );
        else
            this._initiate();
    }

    // messages to the peer ----------------------------------
    send_changes( inode : Inode, patch_data : Uint8Array ) {
        if ( patch_data.byteLength ) {
            // console.error( `send changes for ${ inode }: ${ patch_data.slice( 0, 10 ) } at ${ Date.now() - 1458834611723 }` );
            this._send_cmd( DbCmds.changes, { inode: inode, data: patch_data } );
        }
    }

    send_obj_data( inode : Inode, variable_data : Uint8Array ) {
        this._send_cmd( DbCmds.obj_data, { inode: inode, data: variable_data } );
    }

    // messages from the peer --------------------------------
    recv_changes( inode : Inode, patch_data : Uint8Array ) {
        // console.log( `recv changes for ${ inode }: ${ patch_data.slice( 0, 5 ) } at ${ Date.now() - 1458834611723 } dev: ${ this.tracker.db.dev }` ); //  in ${ this.tracker.db.dev }
        let item = this.tracker.db.table.find( inode );
        if ( ! item  )
            throw new Error( "TODO" );
        let bp = new BinaryReader( patch_data );
        if ( item.link )
            throw new Error( "TODO: add stuff in a swalloed (merged) item" );
        item.patch_manager.app_changes( bp, this.peer_dev, [ this.tracker.db.dev ], this.tracker.db.dev, this.tracker.db.usr, ( pd ) => this.send_changes( inode, pd ) );
    }

    recv_obj_data( inode : Inode, patch_data : Uint8Array ) {
        // console.log( `recv data for ${ inode }` ); //  in ${ this.tracker.db.dev }
        let item = this.tracker.db.table.find( inode );
        if ( item ) {
            if ( ! item.patch_manager.variable.is_symbolic() )
                throw new Error( "Weird" );

            // we try to read the va
            let br = new BinaryReader( patch_data );
            let tv = br.read_PT();
            if ( ! PatchTypes.readers[ tv ] )
                throw new Error( "TODO: make a reader for patch_type " + PatchTypes.list[ tv ] );
            item.patch_manager = PatchManager.read_from( br, tv, this.peer_dev, this.tracker.db.dev, this.tracker.db.usr );
            for( let wl of item.when_loaded )
                wl( this.peer_dev );
            item.when_loaded = [];
        }
    }

    // helpers -----------------------------------------------------------------
    // initiate connection
    _initiate() {
        this._initiator = true;

        this._pc.onnegotiationneeded = () => {
            let mediaConstraints = { mandatory: {
                offerToReceiveAudio: false,
                offerToReceiveVideo: false
            } };
            this._pc.createOffer( ( offer ) => {
                this._pc.setLocalDescription( offer, () => {
                    this.tracker.send_webrtc_offer( this.peer_dev, JSON.stringify( this._pc.localDescription ) );
                }, this._log_error );
            }, this._log_error, mediaConstraints );
        };

        this._set_channel( this._pc.createDataChannel( "chat", { ordered: false } ) );
    }

    _log_error( err ) {
        console.error( err );
    }

    // there is an offer. Beware: it this. is an initiator, this.accept_offer will reset the RTCPeerConnection to accept the offer
    _accept_offer( desc : string ) {
        if ( this._initiator ) {
            this._pc        = this._make_pc( PeerConnection.cfg, true );
            this._initiator = false;
        }

        this._pc.setRemoteDescription( new RTCSessionDescription( JSON.parse( desc ) ), () => {
            this._pc.createAnswer ( ( offer ) => {
                this._pc.setLocalDescription( offer, () => {
                    this.tracker.send_webrtc_answer( this.peer_dev, JSON.stringify( this._pc.localDescription ) );
                }, this._log_error );
            }, this._log_error )
        }, this._log_error );

        //
        this._pc.ondatachannel = ( evt ) => {
            this._set_channel( (<any>evt).channel );
        };
    }

    accept_answer( desc : string  ) {
        this._pc.setRemoteDescription( new RTCSessionDescription( JSON.parse( desc ) ), () => null, this._log_error );
    }

    add_ice_candidate( desc : string ) {
        this._pc.addIceCandidate( new RTCIceCandidate( JSON.parse( desc ) ), () => null, this._log_error );
    }


    // helpers -----------------------------------------------
    _read_cmds( br : BinaryReader ) {
        while ( br.size ) {
            DbCmds._read_cmd( br, this, this.tracker.db.dev, this.tracker.db.usr );
        }
    }

    _send_cmd( cmd, args ) {
        let bcmd = this._bw.size;
        this._bw.write_PI8( cmd.num );
        cmd.write( this._bw, args, this.tracker.db.dev, this.tracker.db.usr );
        this._bw.write_crc32c( bcmd );

        // say that we will have things to send
        if ( this._connected ) {
            if ( ! this._timeout_something_to_send ) {
                this._timeout_something_to_send = setTimeout( () => {
                    this._timeout_something_to_send = null;
                    this._ch.send( this._bw.to_Uint8Array() );
                    this._bw.clear( 32 );
                }, 0 );
            }
        } else {
            this._pending_cmds.push( this._bw.to_Uint8Array() );
            this._bw.clear( 32 );
        }
    }

    _make_pc( cfg : RTCConfiguration, initiator : boolean ) {
        let rtt = typeof RTCPeerConnection !== 'undefined' ? RTCPeerConnection : webkitRTCPeerConnection;
        let res = new rtt( cfg );

        res.onicecandidate = ( evt ) => {
            if ( evt.candidate )
                this.tracker.send_ice_candidate( this.peer_dev, initiator, JSON.stringify( evt.candidate ) );
        };

        return res;
    }

    _set_channel( ch : RTCDataChannel ) {
        this._ch = ch;

        this._ch.onopen = ( event ) => {
            this._connected = true;
            for( let m of this._pending_cmds )
                this._ch.send( m );
            this._pending_cmds = [];
        };

        this._ch.onmessage = ( event ) => {
            this._read_cmds( new BinaryReader( new Uint8Array( event.data ) ) );
        };
    }

    tracker                   : TrackerClient;
    peer_dev                  : DevId;
    _bw                       : BinaryWriter;
    _pc                       : RTCPeerConnection;
    _ch                       : RTCDataChannel;
    _connected                : boolean;
    _initiator                : boolean;
    _pending_cmds             : Array<Uint8Array>;
    _timeout_something_to_send: number;
}


import RenderGraphviz from "../../System/RenderGraphviz";
import TrackerClient  from "./TrackerClient";
import DbCmds         from "./DbCmds";
