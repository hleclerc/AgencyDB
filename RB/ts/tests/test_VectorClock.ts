import BinaryWriter from "../RiotBase/System/BinaryWriter"
import BinaryReader from "../RiotBase/System/BinaryReader"
import VectorClock  from "../RiotBase/System/VectorClock"
import DevId        from "../RiotBase/System/DevId"
import check        from "../RiotBase/System/Check"

function test_VectorClock_RW() {
    let v = new VectorClock;
    v.inc( "oiyvdsACDSZ~" );
    v.inc( "dshSCQcxohZ~" );
    v.inc( "oiyvdsACDSZ~" );
    check( v.toString(), "{oiyvdsACDSZ~:2,dshSCQcxohZ~:1}" );

    let bw = new BinaryWriter;
    bw.write_VectorClock( v );
    bw.write_VectorClock( v );

    let br = new BinaryReader( bw.to_Uint8Array() );
    check( br.read_VectorClock(), "{oiyvdsACDSZ~:2,dshSCQcxohZ~:1}" );
    check( br.read_VectorClock(), "{oiyvdsACDSZ~:2,dshSCQcxohZ~:1}" );
}

function test_VectorClock_update() {
    let v = new VectorClock;
    check( v.update( "oiyvdsACDSZ~", 1 ), true );
    check( v.update( "oiyvdsACDSZ~", 2 ), true );
    console.log( v );
}

test_VectorClock_RW    ()
test_VectorClock_update()
