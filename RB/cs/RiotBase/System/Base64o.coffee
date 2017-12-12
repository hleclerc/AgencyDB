chars = '-0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ_abcdefghijklmnopqrstuvwxyz'

invch = ( 0 for i in [ 0 ... 256 ] )
for i in [ 0 ... chars.length ]
    invch[ chars.charCodeAt i ] = i

#
module.exports =
    encode: ( array ) ->
        res = ""
        index = 0
        while index < array.length - 2
            num = ( array[ index + 0 ] << 16 ) +
                  ( array[ index + 1 ] <<  8 ) +
                  ( array[ index + 2 ] <<  0 )
            res += chars[ num >> 18 & 0x3F ] +
                   chars[ num >> 12 & 0x3F ] +
                   chars[ num >>  6 & 0x3F ] +
                   chars[ num >>  0 & 0x3F ]
            index += 3
        switch array.length - index
            when 1
                num = array[ index + 0 ]
                res += chars[ num >> 2        ] +
                       chars[ num << 4 & 0x3F ] +
                       "~~"
            when 2
                num = ( array[ index + 0 ] << 8 ) +
                      ( array[ index + 1 ] << 0 )
                res += chars[ num >> 10        ] +
                       chars[ num >>  4 & 0x3F ] +
                       chars[ num <<  2 & 0x3F ] +
                       '~'
        return res

    decode: ( str ) ->
        nbe = if str[ str.length - 2 ] == '~' then 2 else str[ str.length - 1 ] == '~'
        res = new Uint8Array str.length * 3 / 4 - nbe
        i_s = i_r = 0
        while i_s < str.length
            num = ( invch[ str.charCodeAt i_s + 0 ] << 18 ) +
                  ( invch[ str.charCodeAt i_s + 1 ] << 12 ) +
                  ( invch[ str.charCodeAt i_s + 2 ] <<  6 ) +
                  ( invch[ str.charCodeAt i_s + 3 ] <<  0 )
            res[ i_r++ ] = num >> 16
            res[ i_r++ ] = num >>  8
            res[ i_r++ ] = num >>  0
            i_s += 4
        return res
