var chars = '-0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ_abcdefghijklmnopqrstuvwxyz'

var invch = []
for( let i = 0; i < 256; ++i )
    invch.push( 0 )
for( let i = 0; i < chars.length; ++i )
    invch[ chars.charCodeAt( i ) ] = i

export default class Base64o {
    static encode( array ) {
        var num;
        var res = "";
        var index = 0;
        while( index < array.length - 2 ) {
            num = ( array[ index + 0 ] << 16 ) +
                  ( array[ index + 1 ] <<  8 ) +
                  ( array[ index + 2 ] <<  0 );
            res += chars[ num >> 18 & 0x3F ] +
                   chars[ num >> 12 & 0x3F ] +
                   chars[ num >>  6 & 0x3F ] +
                   chars[ num >>  0 & 0x3F ];
            index += 3;
        }

        switch( array.length - index ) {
            case 1:
                num = array[ index + 0 ];
                res += chars[ num >> 2        ] +
                       chars[ num << 4 & 0x3F ] +
                       "~~";
                break;
            case 2:
                num = ( array[ index + 0 ] << 8 ) +
                      ( array[ index + 1 ] << 0 );
                res += chars[ num >> 10        ] +
                       chars[ num >>  4 & 0x3F ] +
                       chars[ num <<  2 & 0x3F ] +
                       '~';
                break;
        }

        return res;
    }

    static decode( str: string ): Uint8Array {
        var nbe = str[ str.length - 2 ] == '~' ? 2 : Number( str[ str.length - 1 ] == '~' );
        var res = new Uint8Array( str.length * 3 / 4 - nbe );
        var i_s = 0, i_r = 0;
        while( i_s < str.length ) {
            let num = ( invch[ str.charCodeAt( i_s + 0 ) ] << 18 ) +
                      ( invch[ str.charCodeAt( i_s + 1 ) ] << 12 ) +
                      ( invch[ str.charCodeAt( i_s + 2 ) ] <<  6 ) +
                      ( invch[ str.charCodeAt( i_s + 3 ) ] <<  0 );
            res[ i_r++ ] = num >> 16;
            res[ i_r++ ] = num >>  8;
            res[ i_r++ ] = num >>  0;
            i_s += 4;
        }
        return res
    }
}
