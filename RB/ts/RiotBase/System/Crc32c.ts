function makeCRCTable() : Array<number> {
    var res = [];
    for( let i = 0; i < 256; ++i ) {
        let c = i;
        for( let k = 0; k < 8; ++k )
            if ( c & 1 ) // 0xEDB88320 for CRC
                c = 0x82F63B78 ^ ( c >>> 1 );
            else
                c = c >>> 1;
        res.push( c );
    }
    return res;
}

declare var global: any;
declare var window: any;

//
export default
function crc32c( str: Uint8Array ) : number {
    let w = window || global;
    let crcTable = w.crcTable || ( w.crcTable = makeCRCTable() );
    let crc = 0 ^ ( -1 );

    str.forEach( val =>
        crc = ( crc >>> 8 ) ^ crcTable[ ( crc ^ val ) & 0xFF ]
    );

    return ( crc ^ ( -1 ) ) >>> 0;
}
