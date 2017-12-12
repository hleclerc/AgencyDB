/// This file is generated (by bin/Method_template.py). Do not modify it directly.
import BinaryWriter from '../../System/BinaryWriter';
import BinaryReader from '../../System/BinaryReader';
import DevId        from '../../System/DevId';
import UsrId        from '../../System/UsrId';

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
    login: { // 
        num: 4,
        args: [ { type: 'String', name: 'user' }, { type: 'String', name: 'pass' }, ],
        write( bw : BinaryWriter, args, cur_dev : DevId, cur_usr : UsrId ) {
            if ( typeof args[ 'user' ] == 'undefined' ) throw Error( 'Undefined arg: user' ); 
            if ( typeof args[ 'pass' ] == 'undefined' ) throw Error( 'Undefined arg: pass' ); 
            bw.write_String( args[ 'user' ] ); 
            bw.write_String( args[ 'pass' ] ); 
        }
    },
    logout: { // 
        num: 5,
        args: [ ],
        write( bw : BinaryWriter, args, cur_dev : DevId, cur_usr : UsrId ) {
        }
    },
    bind_obj: { // 
        num: 6,
        args: [ { type: 'Inode', name: 'id' }, { type: 'VectorClock', name: 'known' }, ],
        write( bw : BinaryWriter, args, cur_dev : DevId, cur_usr : UsrId ) {
            if ( typeof args[ 'id'    ] == 'undefined' ) throw Error( 'Undefined arg: id'    ); 
            if ( typeof args[ 'known' ] == 'undefined' ) throw Error( 'Undefined arg: known' ); 
            bw.write_Inode      ( args[ 'id'    ], cur_dev ); 
            bw.write_VectorClock( args[ 'known' ] ); 
        }
    },
    load_and_bind_obj: { // 
        num: 7,
        args: [ { type: 'Inode', name: 'id' }, ],
        write( bw : BinaryWriter, args, cur_dev : DevId, cur_usr : UsrId ) {
            if ( typeof args[ 'id' ] == 'undefined' ) throw Error( 'Undefined arg: id' ); 
            bw.write_Inode( args[ 'id' ], cur_dev ); 
        }
    },
    load_obj: { // 
        num: 8,
        args: [ { type: 'Inode', name: 'id' }, ],
        write( bw : BinaryWriter, args, cur_dev : DevId, cur_usr : UsrId ) {
            if ( typeof args[ 'id' ] == 'undefined' ) throw Error( 'Undefined arg: id' ); 
            bw.write_Inode( args[ 'id' ], cur_dev ); 
        }
    },
    get_dev_info: { // 
        num: 9,
        args: [ { type: 'DevId', name: 'dev' }, ],
        write( bw : BinaryWriter, args, cur_dev : DevId, cur_usr : UsrId ) {
            if ( typeof args[ 'dev' ] == 'undefined' ) throw Error( 'Undefined arg: dev' ); 
            bw.write_DevId( args[ 'dev' ], cur_dev ); 
        }
    },
    ice_candidate: { // 
        num: 10,
        args: [ { type: 'DevId', name: 'destination' }, { type: 'PI8', name: 'initiator' }, { type: 'String', name: 'jsep' }, ],
        write( bw : BinaryWriter, args, cur_dev : DevId, cur_usr : UsrId ) {
            if ( typeof args[ 'destination' ] == 'undefined' ) throw Error( 'Undefined arg: destination' ); 
            if ( typeof args[ 'initiator'   ] == 'undefined' ) throw Error( 'Undefined arg: initiator'   ); 
            if ( typeof args[ 'jsep'        ] == 'undefined' ) throw Error( 'Undefined arg: jsep'        ); 
            bw.write_DevId ( args[ 'destination' ], cur_dev ); 
            bw.write_PI8   ( args[ 'initiator'   ] ); 
            bw.write_String( args[ 'jsep'        ] ); 
        }
    },
    webrtc_offer: { // 
        num: 11,
        args: [ { type: 'DevId', name: 'destination' }, { type: 'String', name: 'data' }, ],
        write( bw : BinaryWriter, args, cur_dev : DevId, cur_usr : UsrId ) {
            if ( typeof args[ 'destination' ] == 'undefined' ) throw Error( 'Undefined arg: destination' ); 
            if ( typeof args[ 'data'        ] == 'undefined' ) throw Error( 'Undefined arg: data'        ); 
            bw.write_DevId ( args[ 'destination' ], cur_dev ); 
            bw.write_String( args[ 'data'        ] ); 
        }
    },
    webrtc_answer: { // 
        num: 12,
        args: [ { type: 'DevId', name: 'destination' }, { type: 'String', name: 'data' }, ],
        write( bw : BinaryWriter, args, cur_dev : DevId, cur_usr : UsrId ) {
            if ( typeof args[ 'destination' ] == 'undefined' ) throw Error( 'Undefined arg: destination' ); 
            if ( typeof args[ 'data'        ] == 'undefined' ) throw Error( 'Undefined arg: data'        ); 
            bw.write_DevId ( args[ 'destination' ], cur_dev ); 
            bw.write_String( args[ 'data'        ] ); 
        }
    },
    create_user: { // 
        num: 13,
        args: [ { type: 'String', name: 'user' }, { type: 'String', name: 'pass' }, { type: 'String', name: 'email' }, ],
        write( bw : BinaryWriter, args, cur_dev : DevId, cur_usr : UsrId ) {
            if ( typeof args[ 'user'  ] == 'undefined' ) throw Error( 'Undefined arg: user'  ); 
            if ( typeof args[ 'pass'  ] == 'undefined' ) throw Error( 'Undefined arg: pass'  ); 
            if ( typeof args[ 'email' ] == 'undefined' ) throw Error( 'Undefined arg: email' ); 
            bw.write_String( args[ 'user'  ] ); 
            bw.write_String( args[ 'pass'  ] ); 
            bw.write_String( args[ 'email' ] ); 
        }
    },
};
