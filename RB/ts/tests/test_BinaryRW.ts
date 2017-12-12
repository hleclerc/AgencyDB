import BinaryWriter from "../RiotBase/System/BinaryWriter";
import BinaryReader from "../RiotBase/System/BinaryReader";
import Inode        from "../RiotBase/System/Inode";
import DevId        from "../RiotBase/System/DevId";
import UsrId        from "../RiotBase/System/UsrId";
import check        from "../RiotBase/System/Check";

let dev = new DevId( "gmWRNBUMN7o~" );
let usr = new UsrId( dev, 14 );
let ino = new Inode( dev, 14 );

var bw = new BinaryWriter
bw.write_PI8    ( 17     );
bw.write_PT     ( 170    );
bw.write_PI32_le( 170435 );
bw.write_String ( "abc€" );
bw.write_Inode  ( ino );
bw.write_UsrId  ( usr );
// console.log( bw.to_Uint8Array() );

var br = new BinaryReader( bw.to_Uint8Array() );
check( br.read_PI8    (), 17     );
check( br.read_PT     (), 170    );
check( br.read_PI32_le(), 170435 );
check( br.read_String (), "abc€" );
check( br.read_Inode  (), ino );
check( br.read_UsrId  (), usr );

//# bw = new BinaryWriter
//# bw.write_String "ab"
//# bw.write_PI32 257
//# check bw.to_Uint8Array(), [2,97,98,129,2]
//#
//# br = new BinaryReader bw.to_Uint8Array()
//# check br.read_string(), "ab"
//# check br.read_uint(), 257
//
