Base64o = require "./Base64o.js"
crc32c  = require "./Crc32c.js"
DevId   = require "./DevId.js"
require "./Property.js"

#
module.exports =
class BinaryWriter
    #
    constructor: ( reservation = 32 ) ->
        @data = new ArrayBuffer reservation
        @size = 0
        @mark = [] # mark stack (cf @beg_msg_length)
        @i2rd = [] # int to reduce

    # to get the output
    to_buffer: () ->
        @int_reduction()
        @data.slice 0, @size

    # for the display
    toString: ->
        String( @to_Uint8Array() )

    # to get the output as a Uint8Array
    to_Uint8Array: () ->
        @int_reduction()
        new Uint8Array @data, 0, @size

    # free data (with optionnal reservation)
    clear: ( reservation = 0 ) ->
        @data = new ArrayBuffer reservation
        @size = 0
        @mark = []
        @i2rd = []

    #
    int_reduction: () ->
        if @i2rd.length
            @i2rd.sort()

            # get needed size for each int to reduce
            davi = new DataView @data
            needed_size = [ 8 for i in [ 0 ... @i2rd.length ] ]
            for i in [ @i2rd.length - 1 .. 0 ]
                pos = @i2rd[ i ] # position that contains the offset
                val = davi.getUint32 pos # offset value
                lim = pos + 8 + val # pointed position
                j = i + 1
                while j < @i2rd.length and @i2rd[ j ] < lim
                    val -= 8 - needed_size[ j++ ] # to update offset value according to needed sizes after pos and before lim
                needed_size[ i ] = BinaryWriter.size_needed_for_unsigned val

            #
            new_size = @size
            for s in needed_size
                new_size -= 8 - s

            # copy to a new buffer, with updated offsets
            re = new ArrayBuffer new_size
            rw = new DataView re
            rs = 0

            partial_copy = ( beg, end ) =>
                len = end - beg
                dst = new Uint8Array re   , rs , len
                src = new Uint8Array @data, beg, len
                dst.set src
                rs += len

            write_uint = ( val ) ->
                while val >= 128
                    rw.setInt8 rs++, 128 + val % 128
                    val /= 128
                rw.setInt8 rs++, val


            partial_copy 0, @i2rd[ 0 ]
            for i in [ 0 ... @i2rd.length ]
                pos = @i2rd[ i ] # position that contains the offset
                val = davi.getUint32 pos # offset value
                lim = pos + 8 + val # pointed position
                j = i + 1
                while j < @i2rd.length and @i2rd[ j ] < lim
                    val -= 8 - needed_size[ j++ ] # to update offset value according to needed sizes after pos and before lim
                write_uint val
                partial_copy pos + 8,
                    if i + 1 < @i2rd.length then @i2rd[ i + 1 ] else @size

            @data = re
            @size = rs
            i2rd = []

    write_String: ( str ) ->
        # encode
        enc = if TextEncoder?
            ( new TextEncoder "utf8" ).encode str
        else
            new Buffer str, "utf8"
        @write_PT enc.byteLength
        @write_some enc

    write_PI8: ( data ) ->
        @reserve_room_for 1
        dst = new Uint8Array @data
        dst[ @size ] = data
        @size++

    write_PT: ( data ) ->
        while data >= 128
            @write_PI8 128 + data % 128
            data /= 128
        @write_PI8 data

    write_PI32: ( data ) ->
        @write_PT data

    write_PI64: ( data ) ->
        @write_PT data

    write_PI32_le: ( data ) ->
        @write_PI8 ( data >>>  0 ) & 0xFF
        @write_PI8 ( data >>>  8 ) & 0xFF
        @write_PI8 ( data >>> 16 ) & 0xFF
        @write_PI8 ( data >>> 24 ) & 0xFF

    write_DevId: ( dev_id, cur_dev ) ->
        if dev_id.loc() and cur_dev?
            dev_id = cur_dev
        @write_some Base64o.decode( dev_id.val ).reverse()

    write_uint_as_str: ( str ) ->
        br = new BitFieldInStr_Reader str
        while br.size() >= 14
            @write_PI8 128 + br.read( 7 )
        @write_PI8 br.read( 7 )

    write_UsrId: ( data, cur_dev ) ->
        @write_PT data.num
        if data.num
            @write_DevId data.dev, cur_dev

    write_GrpId: ( data, cur_dev ) ->
        @write_PT data.num
        if data.num
            @write_DevId data.dev, cur_dev

    write_Inode: ( data, cur_dev ) ->
        @write_PT data.num
        if data.num
            @write_DevId data.dev, cur_dev

    write_PatchId: ( data, cur_dev ) ->
        @write_PT data.num
        if data.num
            @write_DevId data.dev, cur_dev

    write_VectorClock: ( data, cur_dev ) ->
        @write_PT data.map.size
        data.map.forEach ( num, dev ) =>
            @write_DevId new DevId( dev ), cur_dev
            @write_PT num

    write_Variable: ( data, cur_dev ) ->
        data.write_to this, cur_dev

    write_Number: ( data, cur_dev ) ->
        data.write_to this, cur_dev

    write_crc32c: ( bcmd ) ->
        @write_PI32_le crc32c new Uint8Array @data, bcmd, @size - bcmd

    write_Uint8Array: ( arr ) ->
        @write_PT arr.byteLength
        @write_some arr

    write_some: ( arr ) ->
        if arr instanceof BinaryWriter
            return @write_some arr.to_Uint8Array()
        if arr not instanceof Uint8Array
            console.log "TODO: use type != Uint8Array", arr
        @reserve_room_for arr.byteLength
        ( new Uint8Array @data, @size, arr.byteLength ).set arr
        @size += arr.byteLength

    beg_msg_length: () ->
        @mark.push @size
        @reserve_room_for 8
        @size += 8

    end_msg_length: () ->
        mark = @mark.pop()
        davi = new DataView @data
        davi.setUint32 mark, @size - mark - 8
        @i2rd.push mark

    reserve_room_for: ( size ) ->
        if size > @data.byteLength - @size
            old   = @data
            @data = new ArrayBuffer Math.max 2 * old.byteLength, old.byteLength + size
            dst   = new Uint8Array @data, 0, @size
            src   = new Uint8Array old, 0, @size
            dst.set src

    @size_needed_for_unsigned: ( val ) ->
        res = 1
        while val >= 128
            val /= 128
            res++
        res
