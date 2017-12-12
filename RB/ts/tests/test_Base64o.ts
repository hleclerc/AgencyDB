import Base64o from "../RiotBase/System/Base64o";
import check   from "../RiotBase/System/Check";


function test_Base64o() {
    for( let i = 0; i <= 254; i += 7 )
        check( Base64o.encode( [i,i+1,0,0,0,0,0,0] ) < Base64o.encode( [i+1,i,0,0,0,0,0,0] ), true );

    for( let nt = 0; nt < 12; nt += 1 ) {
        let a = [];
        for( let i = 0, j = Math.floor( Math.random() * 10 ); i < j; i += 1 )
            a.push( Math.floor( Math.random() * 256 ) );
        check( Base64o.decode( Base64o.encode( a ) ),  a );
    }

    check( Base64o.encode( [0,0,0,0,0,0,0,1] ), "----------3~" );
    check( Base64o.encode( [0,0,0,0,0,0,0,2] ), "----------7~" ); 
}

test_Base64o()
