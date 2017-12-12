
# bit field (or stream) stored in reverse order, in a string of ['?'...'~']
module.exports =
class BitFieldInStr_Reader

    constructor: ( str ) ->
        @str = str # a string
        @off = 0   # in bits

    size: ->
        @str.length * 6 - @off

    read: ( nbits ) ->
        if ( @off / 6 | 0 ) == ( ( @off + nbits - 1 ) / 6 | 0 )
            old_off = @off
            @off += nbits
            ( @str.charCodeAt( old_off / 6 | 0 ) - 63 >> old_off % 6 ) & ( ( 1 << nbits ) - 1 )
        else
            beg = 6 - @off % 6
            @read( beg ) + ( @read( nbits - beg ) << beg )

    #
    # append_uint: ( val, nbits ) -> # start with little
    #     # @psize = 1 -> on peut envoyer 32. Après ça, @psize = 0 et
    #     if nbits >= 6 - @psize
    #         @_str += String.fromCharCode 63 + @pdata + ( ( val & ( ( 1 << 6 - @psize ) - 1 ) ) << @psize )
    #         nbits -= 6 - @psize
    #         val >>>= 6 - @psize
    #         @psize = 0
    #         @pdata = 0
    #         while nbits >= 6
    #             @_str += String.fromCharCode 63 + ( val & 63 )
    #             val >>>= 6
    #             nbits -= 6
    #     @pdata += val << @psize
    #     @psize += nbits
    #
    # binary_repr: ->
    #     res = ""
    #     for v in @_str
    #         n = v.charCodeAt( 0 ) - 63
    #         for i in [ 0 ... 6 ]
    #             res += n & 1
    #             n >>= 1
    #     n = @pdata
    #     for i in [ 0 ... @psize ]
    #         res += n & 1
    #         n >>= 1
    #     res
    #
    # str: ->
    #     @_str + String.fromCharCode( 63 + ( @pdata << ( 6 - @psize ) ) )
