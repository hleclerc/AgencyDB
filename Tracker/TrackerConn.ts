//// nsmake alias TrackerCom.ts (..%2Fsrc%2Fgeneration%2Fmake_ts_decl_from_cmds.ts TrackerCmdsDecl.cmds).ts
import BinaryWriter from '../src/ts/System/BinaryWriter'
import TrackerCom   from './TrackerCom'

/**
 * 
 */
export default
class TrackerConn {
    constructor( ip: string ) {
        this.connection = new WebSocket( `wss://${ ip }/ws` );

        this.connection.onopen = () => {
            const data = this.bw.to_Uint8Array();
            if ( data.length ) {
                this.connection.send( data );
                this.bw.clear();
            }
        };

        this.connection.onerror = error => {
            console.log( 'WebSocket Error ' + error );
        };

        this.connection.onmessage = e => {
            console.log( 'Server: ' + e.data );
        };

    }

    onSubmit( email: string, password: string, remember_me: boolean, cb: ( err: string ) => void ) {
        TrackerCom.write.pwd_login( this.bw, email, password );
        this._update_send_timeout();
    }

    _update_send_timeout() {
        if ( this.timeout )
            return;
        this.timeout = setTimeout( () => {
            const data = this.bw.to_Uint8Array();
            if ( data.length ) {
                this.connection.send( data );
                this.bw.clear();
            }
        }, 0 );
    }

    bw         = new BinaryWriter;
    timeout    = null as any;
    connection : WebSocket;
}