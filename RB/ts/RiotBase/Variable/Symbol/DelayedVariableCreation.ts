import BinaryReader   from "../../System/BinaryReader";
import BinaryWriter   from "../../System/BinaryWriter";
import DevId          from "../../System/DevId";
import UsrId          from "../../System/UsrId";
import Map            from "../../System/Map";
import DeclTypesForRW from "../Core/DeclTypesForRW";
import Variable       from "../Core/Variable";
import Rp             from "../Core/Rp";
import LazySource     from "./LazySource";

declare function __req( dir: string, url: string ); // execute

var delayed_variable_creations = new Map<number,Array<DelayedVariableCreation>>(); // num_type, DelayedVariableCreation
var files                      = new Map<String,number>(); // name, stage (1=loading, 2=loaded)


function load_file_for_typenum( num_type : number, callback ) {
    let fname = DeclTypesForRW.files[ num_type ];
    let stage = files.get( fname );
    if ( ! stage ) {
        // loading
        files.set( fname, 1 );

        // request
        let xhr = new XMLHttpRequest;
        xhr.open( "GET", fname, true );
        xhr.onreadystatechange = function( aEvt ) {
            if ( xhr.readyState == 4 ) {
                if ( xhr.status == 200 ) {
                    // loaded
                    files.set( fname, 2 );

                    eval( xhr.responseText );
                    __req( "", fname ); // execute

                    //
                    callback( num_type );
                } else
                    console.error( "Pb loading " + fname + ".", xhr.responseText );
             }
        }
        xhr.send();
    }
}

//
export default
class DelayedVariableCreation extends LazySource {
    constructor( num_type : number, dev_id : DevId, usr_id : UsrId, data : Uint8Array ) {
        super( DelayedVariableCreation, false );
        console.log( "n", num_type );
        this.num_type = num_type
        this.dev_id   = dev_id;
        this.usr_id   = usr_id;
        this.data     = data;

        if ( typeof importScripts == 'undefined' ) { // if in min, we register this in LazySource only after load and local execution of the file
            load_file_for_typenum( this.num_type, ( num_type ) => {
                this._reg_in_lazy_sources();
            } );
        }
    }

    ww_launch() { // called by the scheduler WebWorker (-> will call ww_set_lazy_value when data is loaded)
        var v;
        if ( v = delayed_variable_creations.get( this.num_type ) )
            v.push( this );
        else
            delayed_variable_creations.set( this.num_type, [ this ] );

        load_file_for_typenum( this.num_type, ( num_type ) => {
            let l = delayed_variable_creations.get( num_type );
            if ( l ) {
                for( let v of l ) {
                    let br = new BinaryReader( v.data );
                    v.ww_set_lazy_value( br.read_Variable( v.dev_id, v.usr_id ) );
                }
                delayed_variable_creations.delete( num_type );
            }
        } );
    }

    to_String() : string {
        return `DelayedVariableCreation(type=${ this.num_type })`;
    }

    obj_type() {
        return Variable;
    }

    write_to( bw : BinaryWriter, cur_dev : DevId, cur_usr : UsrId ) {
        throw new Error( "TODO" );
        // bw.write_PT( DelayedVariableCreation.num_for_RW );
        // bw.beg_msg_length();
        // bw.write_PT        ( this.num_type );
        // bw.write_DevId     ( this.dev_id   );
        // bw.write_UsrId     ( this.usr_id   );
        // bw.write_Uint8Array( this.data     );
        // bw.end_msg_length();
    }

    static read_from( br : BinaryReader, dev_id : DevId, usr_id : UsrId ) : Variable {
        return new Variable( new DelayedVariableCreation( br.read_PT(), br.read_DevId( dev_id ), br.read_UsrId( dev_id, usr_id ), br.read_Uint8Array() ) );
    }

    exec() : Variable {
        let br = new BinaryReader( this.data );
        return br.read_Variable( this.dev_id, this.usr_id );
    }

    num_type : number;
    dev_id   : DevId;
    usr_id   : UsrId;
    data     : Uint8Array;
}
LazySource.make_templ( DelayedVariableCreation );
