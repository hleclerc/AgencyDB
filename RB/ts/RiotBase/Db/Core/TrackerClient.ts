import BinaryWriter      from '../../System/BinaryWriter';
import BinaryReader      from '../../System/BinaryReader';
import VectorClock       from '../../System/VectorClock';
import Inode             from '../../System/Inode';
import DevId             from '../../System/DevId';
import UsrId             from '../../System/UsrId';
import Map               from '../../System/Map';
import PeerConnection    from "./PeerConnection";

declare function require_node( s: string );
let _WebSocket = WebSocket || require_node( 'ws' );

//
export default
class TrackerClient {
    constructor( ip, db ) {
        this.ip = ip;
        this.db = db;

        // default configuration
        // this.cfg_rtc_peer_connection      = 'iceServers': [ 'url': 'stun:23.21.150.121' ]

        // start the stuff
        this._initiate_web_socket( ip );
    }


    // desc is a potential offer
    peer_connection_to( dev : DevId, initiator? : boolean, desc? : string ) : PeerConnection {
        let pco = this._peer_connections.get( dev.val );
        if ( ! pco ) {
            pco = { initiator: null, answerer : null };
            this._peer_connections.set( dev.val, pco );
        }

        // if we're looking for a given type (initiator or not)
        if ( typeof initiator != "undefined" ) {
            if ( initiator ) {
                if ( ! pco.initiator )
                    pco.initiator = new PeerConnection( this, dev );
                return pco.initiator;
            }
            // else, we're looking for an answerer
            if ( ! pco.answerer ) {
                if ( ! desc )
                    throw new Error( "Weird: a new answerer peer_connection must have a desc" );
                pco.answerer = new PeerConnection( this, dev, desc );
            }
            return pco.answerer;
        }

        // if we don't mind to have an answerer or not (e.g. because we simply want to have something that sends messages). First look for something connected
        if ( pco.answerer && pco.answerer._connected )
            return pco.answerer;
        if ( pco.initiator && pco.initiator._connected )
            return pco.initiator;

        // else, look for an instanciation
        if ( pco.answerer )
            return pco.answerer;
        if ( pco.initiator )
            return pco.initiator;

        // else, make a new one
        return desc ? pco.answerer  = new PeerConnection( this, dev, desc ) :
                      pco.initiator = new PeerConnection( this, dev );
    }

    // send_..................................................................
    send_auth( user : string, pass : string, login_callback? : ( dev: DevId, usr: UsrId, token: string ) => void, logout_callback? : () => void ) {
        this._send_cmd( TrackerServerCmds.login, { user: user, pass: pass } );
        if ( login_callback )
            this._login_callbacks.push( login_callback );
        if ( logout_callback )
            this._logout_callbacks.push( logout_callback );
    }

    send_create_user( user: string, pass: string, email: string, cb?: ( string ) => void ) {
        this._send_cmd( TrackerServerCmds.create_user, { user: user, pass: pass, email: email } );
        if ( cb )
            this._create_user_callbacks.push( cb );
    }

    send_logout() {
        this._send_cmd( TrackerServerCmds.logout, {} );
    }

    send_bind_obj( id: Inode, known: VectorClock ) {
        this._send_cmd( TrackerServerCmds.bind_obj, { id: id, known: known } );
    }

    send_load_and_bind_obj( id: Inode ) {
        this._send_cmd( TrackerServerCmds.load_and_bind_obj, { id: id } );
    }

    send_load_obj( id: Inode ) {
        this._send_cmd( TrackerServerCmds.load_obj, { id: id } );
    }

    send_ice_candidate( destination : DevId, initiator : boolean, jsep : string ) {
        this._send_cmd( TrackerServerCmds.ice_candidate, { destination: destination, initiator: initiator, jsep : jsep } );
    }

    send_webrtc_offer( destination : DevId, desc : string ) {
        this._send_cmd( TrackerServerCmds.webrtc_offer, { destination: destination, data: desc } );
    }

    send_webrtc_answer( destination : DevId, desc : string ) {
        this._send_cmd( TrackerServerCmds.webrtc_answer, { destination: destination, data: desc } );
    }

    // recv_..................................................................
    recv_error_msg( msg : string ) {
        throw new Error( "TODO" );
    }

    recv_corrupted_data() {
        throw new Error( "TODO" );
    }

    recv_ans_login( dev : DevId, usr : UsrId, token : string ) {
        for( let c of this._login_callbacks )
            c( dev, usr, token );
    }

    recv_ans_create_user( res : string ) {
        for( let c of this._create_user_callbacks )
            c( res );
    }

    recv_ans_logout() {
        for( let c of this._logout_callbacks )
            c();
    }

    recv_bind_obj( inode : Inode, dev : DevId ) {
        // throw new Error( "TODO" );
        // console.log "TODO: bind", inode, dev
        // console.log this.db.dev, inode, dev
    }

    recv_req_inode_updates( inode : Inode, dev : DevId, known : VectorClock ) {
        // find object
        let obj = this.db.table.find( inode );
        if ( ! obj )
            throw new Error( "TODO" );

        // update known
        obj.patch_manager.update_known_patches_for( dev, known );
        let msg = obj.patch_manager.get_changes( dev, this.db.dev, this.db.usr );
        this.peer_connection_to( dev ).send_changes( inode, msg );
    }

    recv_req_inode_data( inode : Inode, dev : DevId ) {
        // find object
        let item = this.db.table.find( inode );
        if ( ! item )
            throw new Error( "TODO" );

        // message
        item.patch_manager.variable.then( () => {
            // bw.write_Variable( obj.patch_manager.variable, false, this.db.dev, this.db.usr );
            let bw = new BinaryWriter;
            item.patch_manager.write_to( bw, dev, this.db.dev, this.db.usr );
            this.peer_connection_to( dev ).send_obj_data( inode, bw.to_Uint8Array() );
        } );
    }

    recv_req_and_bind_inode_data( inode : Inode, dev : DevId ) {
        // find object
        let obj = this.db.table.find( inode );
        if ( ! obj )
            throw new Error( "TODO" );

        // message
        // let msg = obj.patch_manager.get_msg_for_patch_list( dev, this.usr, known, to_be_sent );
        // if ( msg.byteLength ) {
        //     let pc = this.peer_connection_to( dev );
        //     pc.send_changes( inode, msg );
        // }
        throw new Error( "TODO" );
    }

    recv_webrtc_offer( dev : DevId, desc : string ) {
        this.peer_connection_to( dev, false, desc );
    }

    recv_webrtc_answer( dev : DevId, desc : string ) {
        let pc = this.peer_connection_to( dev, true ) as PeerConnection;
        pc.accept_answer( desc );
    }

    recv_ice_candidate( dev : DevId, initiator : number, cand : string ) {
        let pc = this.peer_connection_to( dev, initiator == 0 ) as PeerConnection;
        pc.add_ice_candidate( cand );
    }

    // helpers ...............................................................
    _read_cmds( br : BinaryReader ) {
        while ( br.size ) {
            TrackerClientCmds._read_cmd( br, this, this.db.dev, this.db.usr );
        }
    }

    _send_cmd( cmd, args ) {
        let bcmd = this._bw.size;
        this._bw.write_PI8( cmd.num );
        cmd.write( this._bw, args, this.db.dev, this.db.usr );
        this._bw.write_crc32c( bcmd );

        // say that we will have things to send
        if ( this._connected ) {
            if ( ! this._timeout_something_to_send ) {
                this._timeout_something_to_send = setTimeout( () => {
                    this._timeout_something_to_send = null;
                    this._web_socket.send( this._bw.to_Uint8Array() );
                    this._bw.clear( 32 );
                }, 0 );
            }
        } else {
            this._pending_web_socket_commands.push( this._bw.to_Uint8Array() );
            this._bw.clear( 32 );
        }
    }

    _initiate_web_socket( ip ) {
        if ( ! this._web_socket ) {
            this._web_socket = new WebSocket( `wss://${ip}` );

            this._web_socket.onopen = () => {
                this._connected = true;
                // send pending stuff
                for( let buf of this._pending_web_socket_commands )
                    this._web_socket.send( buf );
                this._pending_web_socket_commands = [];
            }

            this._web_socket.onclose = ( ev ) => {
                console.log( "websocket has been closed." );
                this._connected = false;
            }

            this._web_socket.onmessage = ( blob ) => {
                let callback = ( msg : ArrayBuffer ) => {
                    this._read_cmds( new BinaryReader( new Uint8Array( msg ) ) );
                };

                // blob -> ArrayBuffer
                let fileReader = new FileReader;
                fileReader.onload = function( toto ) {
                    callback( this.result );
                }
                fileReader.readAsArrayBuffer( blob.data );
            }
        }
    }

    ip                           : string;
    db                           : Db;
    dev                          : DevId;
    usr                          : UsrId;

    // helper attributes
    _pending_web_socket_commands = [] as Array<Uint8Array>;
    _timeout_something_to_send   : number; // via the websocket
    _login_callbacks             = [] as Array< ( dev : DevId, usr : UsrId, token : string ) => void >;
    _logout_callbacks            = [] as Array< () => void >;
    _create_user_callbacks       = [] as Array< ( string ) => void >;

    _web_socket                  : WebSocket;
    _connected                   = false;
    _bw                          = new BinaryWriter;

    _peer_connections            = new Map<string,{initiator:PeerConnection,answerer:PeerConnection}>(); // dev_id ->
}

import Db                from "../Db";
import TrackerServerCmds from "./TrackerServerCmds";
import TrackerClientCmds from "./TrackerClientCmds";
