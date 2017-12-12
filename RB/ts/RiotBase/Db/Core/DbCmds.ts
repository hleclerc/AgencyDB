/// This file is generated (by bin/Method_template.py). Do not modify it directly.
import BinaryWriter from '../../System/BinaryWriter';
import BinaryReader from '../../System/BinaryReader';

function check_cmd( br : BinaryReader, beg_cmd : number ) : boolean {
    let ccrc = br.make_crc32c( beg_cmd );
    let wcrc = br.read_PI32_le();
    return ccrc == wcrc;
}

///
export default {
    changes        : { // changes: 
        num: 0,
        args: [ { type: 'Inode', name: 'inode' }, { type: 'Uint8Array', name: 'data' }, ],
        write( bw : BinaryWriter, args, cur_dev, cur_usr ) {
            if ( typeof args[ 'inode' ] == 'undefined' ) throw Error( 'Undefined arg: inode' ); 
            if ( typeof args[ 'data'  ] == 'undefined' ) throw Error( 'Undefined arg: data'  ); 
            bw.write_Inode     ( args[ 'inode' ], cur_dev ); 
            bw.write_Uint8Array( args[ 'data'  ] ); 
        }
    },
    obj_data       : { // obj_data: 
        num: 1,
        args: [ { type: 'Inode', name: 'inode' }, { type: 'Uint8Array', name: 'data' }, ],
        write( bw : BinaryWriter, args, cur_dev, cur_usr ) {
            if ( typeof args[ 'inode' ] == 'undefined' ) throw Error( 'Undefined arg: inode' ); 
            if ( typeof args[ 'data'  ] == 'undefined' ) throw Error( 'Undefined arg: data'  ); 
            bw.write_Inode     ( args[ 'inode' ], cur_dev ); 
            bw.write_Uint8Array( args[ 'data'  ] ); 
        }
    },

    _read_cmd( br: BinaryReader, tr : PeerConnection, cur_dev, cur_usr ) {
        let beg_cmd = br.cursor;
        let cmd = br.read_PI8();
        switch ( cmd ) {
            case 0: { // changes
                let inode = br.read_Inode( cur_dev );
                let data = br.read_Uint8Array();
                if ( ! check_cmd( br, beg_cmd ) ) throw( `Invalid checksum for func=changes` );
                // console.log( "recv changes", inode, data );
                tr.recv_changes( inode, data );
                break;
            }
            case 1: { // obj_data
                let inode = br.read_Inode( cur_dev );
                let data = br.read_Uint8Array();
                if ( ! check_cmd( br, beg_cmd ) ) throw( `Invalid checksum for func=obj_data` );
                // console.log( "recv obj_data", inode, data );
                tr.recv_obj_data( inode, data );
                break;
            }
        }
    }
};

import PeerConnection from './PeerConnection';
