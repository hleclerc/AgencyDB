import BinaryWriter from "../RiotBase/System/BinaryWriter";
import BinaryReader from "../RiotBase/System/BinaryReader";
import ObjectId     from "../RiotBase/Variable/ObjectId";
import Inode        from "../RiotBase/System/Inode";
import DevId        from "../RiotBase/System/DevId";
import UsrId        from "../RiotBase/System/UsrId";
import check        from "../RiotBase/System/Check";
import tpc          from "./tpc";

function test_base_op() {
    let id = new ObjectId( new Inode( new DevId( "O5HeHcHU1Io~" ), 17 ) );
    check( id.toInode(), "17@O5HeHcHU1Io~" );
    check( id, "17@O5HeHcHU1Io~" );
}

function test_rw() {
    let id = new ObjectId( new Inode( new DevId( "O5HeHcHU1Io~" ), 18 ) );
    let bw = new BinaryWriter;
    bw.write_Variable( id );
    bw.write_Variable( id );
    bw.write_Variable( id );

    let br = new BinaryReader( bw.to_Uint8Array() );
    check( br.read_Variable(), id );
    br.skip_Variable();
    check( br.read_Variable(), id );
}

function test_parallel_changes() {
    let id_0 = new ObjectId( new Inode( new DevId( "O5HeHcHU1Io~" ), 17 ) );
    let id_1 = new ObjectId( new Inode( new DevId( "O5HeHcHU1Io~" ), 18 ) );

    // init
    tpc( ObjectId, function( s ) {
    }, id_0, '17@O5HeHcHU1Io~' );

    tpc( ObjectId, function( s ) {
        s.m[ 1 ].d.set( id_1 );
    }, id_0, '18@O5HeHcHU1Io~' );
}

// test_base_op()
test_rw();
// test_parallel_changes()
