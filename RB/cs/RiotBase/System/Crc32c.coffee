makeCRCTable = ->
    for c in [ 0...256 ]
        for k in [ 0...8 ]
            if c & 1
                # 0xEDB88320 for CRC
                c = 0x82F63B78 ^ ( c >>> 1 )
            else
                c = c >>> 1
        c

# arr is assumed to be an Uint8Array
module.exports =
crc32c = ( str ) ->
    crcTable = window.crcTable or ( window.crcTable = makeCRCTable() )
    crc = 0 ^ ( -1 )

    str.forEach ( val ) ->
        crc = ( crc >>> 8 ) ^ crcTable[ ( crc ^ val ) & 0xFF ]

    ( crc ^ ( -1 ) ) >>> 0
