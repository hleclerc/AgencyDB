
# bit stream to construct a string of ['?'...'~'] from bytes
# useful
module.exports =
class BitFieldInStr_Writer
    constructor: () ->
        @psize = 0 # pending (not already in str) data size, in bits. Always < 6.
        @pdata = 0 # pending (not already in str) 6 bits data content
        @_str = "" # registered (not pending) data

    append_uint: ( val, nbits ) -> # start with little
        # @psize = 1 -> on peut envoyer 32. Après ça, @psize = 0 et
        # if nbits >= 6 - @psize
        #     @_str += String.fromCharCode 63 + @pdata + ( ( val & ( ( 1 << 6 - @psize ) - 1 ) ) << @psize )
        #     nbits -= 6 - @psize
        #     val >>>= 6 - @psize
        #     @psize = 0
        #     @pdata = 0
        #     while nbits >= 6
        #         @_str += String.fromCharCode 63 + ( val & 63 )
        #         val >>>= 6
        #         nbits -= 6
        @pdata += ( val << 6 - @psize ) & 63
        @psize += nbits

    binary_repr: ->
        res = ""
        for v in @_str
            n = v.charCodeAt( 0 ) - 63
            for i in [ 0 ... 6 ]
                res += n & 1
                n >>= 1
        n = @pdata
        for i in [ 0 ... @psize ]
            res += n & 1
            n >>= 1
        res.split("").reverse().join ""

    str: ->
        @_str + String.fromCharCode( 63 + @pdata )
