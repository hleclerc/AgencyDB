/// This file is generated (by bin/Method_template.py). Do not modify it directly.
import BinaryWriter from '../../System/BinaryWriter';
import BinaryReader from '../../System/BinaryReader';
import DevId        from '../../System/DevId';
import UsrId        from '../../System/UsrId';

function check_cmd( br : BinaryReader, beg_cmd : number ) : boolean {
    let ccrc = br.make_crc32c( beg_cmd );
    let wcrc = br.read_PI32_le();
    return ccrc == wcrc;
}

///
export default {
    error_msg: { // 
        num: 0,
        args: [ { type: 'String', name: 'msg' }, ],
        write( bw : BinaryWriter, args, cur_dev : DevId, cur_usr : UsrId ) {
            if ( typeof args[ 'msg' ] == 'undefined' ) throw Error( 'Undefined arg: msg' ); 
            bw.write_String( args[ 'msg' ] ); 
        }
    },
    corrupted_data: { // 
        num: 1,
        args: [ ],
        write( bw : BinaryWriter, args, cur_dev : DevId, cur_usr : UsrId ) {
        }
    },
    ans_login: { // 
        num: 3,
        args: [ { type: 'DevId', name: 'dev' }, { type: 'UsrId', name: 'usr' }, { type: 'String', name: 'token' }, ],
        write( bw : BinaryWriter, args, cur_dev : DevId, cur_usr : UsrId ) {
            if ( typeof args[ 'dev'   ] == 'undefined' ) throw Error( 'Undefined arg: dev'   ); 
            if ( typeof args[ 'usr'   ] == 'undefined' ) throw Error( 'Undefined arg: usr'   ); 
            if ( typeof args[ 'token' ] == 'undefined' ) throw Error( 'Undefined arg: token' ); 
            bw.write_DevId ( args[ 'dev'   ], cur_dev ); 
            bw.write_UsrId ( args[ 'usr'   ], cur_dev, cur_usr ); 
            bw.write_String( args[ 'token' ] ); 
        }
    },
    ans_logout: { // 
        num: 4,
        args: [ ],
        write( bw : BinaryWriter, args, cur_dev : DevId, cur_usr : UsrId ) {
        }
    },
    bind_obj: { // 
        num: 5,
        args: [ { type: 'Inode', name: 'id' }, { type: 'DevId', name: 'dev' }, ],
        write( bw : BinaryWriter, args, cur_dev : DevId, cur_usr : UsrId ) {
            if ( typeof args[ 'id'  ] == 'undefined' ) throw Error( 'Undefined arg: id'  ); 
            if ( typeof args[ 'dev' ] == 'undefined' ) throw Error( 'Undefined arg: dev' ); 
            bw.write_Inode( args[ 'id'  ], cur_dev ); 
            bw.write_DevId( args[ 'dev' ], cur_dev ); 
        }
    },
    req_inode_data: { // 
        num: 6,
        args: [ { type: 'Inode', name: 'id' }, { type: 'DevId', name: 'dev' }, ],
        write( bw : BinaryWriter, args, cur_dev : DevId, cur_usr : UsrId ) {
            if ( typeof args[ 'id'  ] == 'undefined' ) throw Error( 'Undefined arg: id'  ); 
            if ( typeof args[ 'dev' ] == 'undefined' ) throw Error( 'Undefined arg: dev' ); 
            bw.write_Inode( args[ 'id'  ], cur_dev ); 
            bw.write_DevId( args[ 'dev' ], cur_dev ); 
        }
    },
    req_inode_updates: { // 
        num: 7,
        args: [ { type: 'Inode', name: 'id' }, { type: 'DevId', name: 'dev' }, { type: 'VectorClock', name: 'known' }, ],
        write( bw : BinaryWriter, args, cur_dev : DevId, cur_usr : UsrId ) {
            if ( typeof args[ 'id'    ] == 'undefined' ) throw Error( 'Undefined arg: id'    ); 
            if ( typeof args[ 'dev'   ] == 'undefined' ) throw Error( 'Undefined arg: dev'   ); 
            if ( typeof args[ 'known' ] == 'undefined' ) throw Error( 'Undefined arg: known' ); 
            bw.write_Inode      ( args[ 'id'    ], cur_dev ); 
            bw.write_DevId      ( args[ 'dev'   ], cur_dev ); 
            bw.write_VectorClock( args[ 'known' ] ); 
        }
    },
    req_and_bind_inode_data: { // 
        num: 8,
        args: [ { type: 'Inode', name: 'id' }, { type: 'DevId', name: 'dev' }, ],
        write( bw : BinaryWriter, args, cur_dev : DevId, cur_usr : UsrId ) {
            if ( typeof args[ 'id'  ] == 'undefined' ) throw Error( 'Undefined arg: id'  ); 
            if ( typeof args[ 'dev' ] == 'undefined' ) throw Error( 'Undefined arg: dev' ); 
            bw.write_Inode( args[ 'id'  ], cur_dev ); 
            bw.write_DevId( args[ 'dev' ], cur_dev ); 
        }
    },
    webrtc_offer: { // 
        num: 9,
        args: [ { type: 'DevId', name: 'dev' }, { type: 'String', name: 'sdp' }, ],
        write( bw : BinaryWriter, args, cur_dev : DevId, cur_usr : UsrId ) {
            if ( typeof args[ 'dev' ] == 'undefined' ) throw Error( 'Undefined arg: dev' ); 
            if ( typeof args[ 'sdp' ] == 'undefined' ) throw Error( 'Undefined arg: sdp' ); 
            bw.write_DevId ( args[ 'dev' ], cur_dev ); 
            bw.write_String( args[ 'sdp' ] ); 
        }
    },
    webrtc_answer: { // 
        num: 10,
        args: [ { type: 'DevId', name: 'dev' }, { type: 'String', name: 'sdp' }, ],
        write( bw : BinaryWriter, args, cur_dev : DevId, cur_usr : UsrId ) {
            if ( typeof args[ 'dev' ] == 'undefined' ) throw Error( 'Undefined arg: dev' ); 
            if ( typeof args[ 'sdp' ] == 'undefined' ) throw Error( 'Undefined arg: sdp' ); 
            bw.write_DevId ( args[ 'dev' ], cur_dev ); 
            bw.write_String( args[ 'sdp' ] ); 
        }
    },
    ice_candidate: { // 
        num: 11,
        args: [ { type: 'DevId', name: 'dev' }, { type: 'PI8', name: 'initiator' }, { type: 'String', name: 'sdp' }, ],
        write( bw : BinaryWriter, args, cur_dev : DevId, cur_usr : UsrId ) {
            if ( typeof args[ 'dev'       ] == 'undefined' ) throw Error( 'Undefined arg: dev'       ); 
            if ( typeof args[ 'initiator' ] == 'undefined' ) throw Error( 'Undefined arg: initiator' ); 
            if ( typeof args[ 'sdp'       ] == 'undefined' ) throw Error( 'Undefined arg: sdp'       ); 
            bw.write_DevId ( args[ 'dev'       ], cur_dev ); 
            bw.write_PI8   ( args[ 'initiator' ] ); 
            bw.write_String( args[ 'sdp'       ] ); 
        }
    },
    ans_create_user: { // 
        num: 12,
        args: [ { type: 'String', name: 'res' }, ],
        write( bw : BinaryWriter, args, cur_dev : DevId, cur_usr : UsrId ) {
            if ( typeof args[ 'res' ] == 'undefined' ) throw Error( 'Undefined arg: res' ); 
            bw.write_String( args[ 'res' ] ); 
        }
    },

    _read_cmd( br: BinaryReader, tr : TrackerClient, cur_dev : DevId, cur_usr : UsrId ) {
        let beg_cmd = br.cursor;
        let cmd = br.read_PI8();
        switch ( cmd ) {
            case 0: { // error_msg
                let msg = br.read_String();
                if ( ! check_cmd( br, beg_cmd ) ) throw( `Invalid checksum for func=error_msg` );
                tr.recv_error_msg( msg );
                break;
            }
            case 1: { // corrupted_data
                if ( ! check_cmd( br, beg_cmd ) ) throw( `Invalid checksum for func=corrupted_data` );
                tr.recv_corrupted_data(  );
                break;
            }
            case 3: { // ans_login
                let dev = br.read_DevId( cur_dev );
                let usr = br.read_UsrId( cur_dev, cur_usr );
                let token = br.read_String();
                if ( ! check_cmd( br, beg_cmd ) ) throw( `Invalid checksum for func=ans_login` );
                tr.recv_ans_login( dev, usr, token );
                break;
            }
            case 4: { // ans_logout
                if ( ! check_cmd( br, beg_cmd ) ) throw( `Invalid checksum for func=ans_logout` );
                tr.recv_ans_logout(  );
                break;
            }
            case 5: { // bind_obj
                let id = br.read_Inode( cur_dev );
                let dev = br.read_DevId( cur_dev );
                if ( ! check_cmd( br, beg_cmd ) ) throw( `Invalid checksum for func=bind_obj` );
                tr.recv_bind_obj( id, dev );
                break;
            }
            case 6: { // req_inode_data
                let id = br.read_Inode( cur_dev );
                let dev = br.read_DevId( cur_dev );
                if ( ! check_cmd( br, beg_cmd ) ) throw( `Invalid checksum for func=req_inode_data` );
                tr.recv_req_inode_data( id, dev );
                break;
            }
            case 7: { // req_inode_updates
                let id = br.read_Inode( cur_dev );
                let dev = br.read_DevId( cur_dev );
                let known = br.read_VectorClock();
                if ( ! check_cmd( br, beg_cmd ) ) throw( `Invalid checksum for func=req_inode_updates` );
                tr.recv_req_inode_updates( id, dev, known );
                break;
            }
            case 8: { // req_and_bind_inode_data
                let id = br.read_Inode( cur_dev );
                let dev = br.read_DevId( cur_dev );
                if ( ! check_cmd( br, beg_cmd ) ) throw( `Invalid checksum for func=req_and_bind_inode_data` );
                tr.recv_req_and_bind_inode_data( id, dev );
                break;
            }
            case 9: { // webrtc_offer
                let dev = br.read_DevId( cur_dev );
                let sdp = br.read_String();
                if ( ! check_cmd( br, beg_cmd ) ) throw( `Invalid checksum for func=webrtc_offer` );
                tr.recv_webrtc_offer( dev, sdp );
                break;
            }
            case 10: { // webrtc_answer
                let dev = br.read_DevId( cur_dev );
                let sdp = br.read_String();
                if ( ! check_cmd( br, beg_cmd ) ) throw( `Invalid checksum for func=webrtc_answer` );
                tr.recv_webrtc_answer( dev, sdp );
                break;
            }
            case 11: { // ice_candidate
                let dev = br.read_DevId( cur_dev );
                let initiator = br.read_PI8();
                let sdp = br.read_String();
                if ( ! check_cmd( br, beg_cmd ) ) throw( `Invalid checksum for func=ice_candidate` );
                tr.recv_ice_candidate( dev, initiator, sdp );
                break;
            }
            case 12: { // ans_create_user
                let res = br.read_String();
                if ( ! check_cmd( br, beg_cmd ) ) throw( `Invalid checksum for func=ans_create_user` );
                tr.recv_ans_create_user( res );
                break;
            }
        }
    }
};

import TrackerClient from './TrackerClient';
