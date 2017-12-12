VectorClock = require "./VectorClock.js"
PatchId     = require "./PatchId.js"
Base64o     = require "./Base64o.js"
crc32c      = require "./Crc32c.js"
Inode       = require "./Inode.js"
DevId       = require "./DevId.js"
UsrId       = require "./UsrId.js"
GrpId       = require "./GrpId.js"
require "./Property.js"

module.exports =
class BinaryReader
    # data expected to be a buffer (@data will be a Uint8Array)
    constructor: ( data, cursor = 0, len ) ->
        @cursor = cursor
        if not len?
            len = data.byteLength - cursor
        @data = new Uint8Array data, cursor, len
        @marks = []

    @property 'size',
        get: -> @data.length - @cursor

    read_PI8: ->
        res = @data[ @cursor ]
        @cursor += 1
        res

    read_PT: ->
        if @cursor >= @data.length
            return 0
        res = @data[ @cursor ]
        @cursor += 1
        if res < 128 or @cursor >= @data.length
            return res
        res -= 128
        shift = 7
        while true
            if @cursor >= @data.length
                return res
            val = @data[ @cursor ]
            @cursor += 1
            if val < 128 or @cursor >= @data.length
                return res + ( val << shift )
            res += ( val - 128 ) << shift
            shift += 7

    read_PI32_le: ->
        @read_PI8() + ( @read_PI8() << 8 ) + ( @read_PI8() << 16 ) + ( @read_PI8() * Math.pow( 2.0, 24 ) )

    read_String: ->
        len = @read_PT()
        arr = @data.buffer.slice @cursor, @cursor + len # new Uint8Array
        res = if TextEncoder?
            ( new TextDecoder "utf8" ).decode arr
        else
            ( new Buffer arr, "utf8" ).toString()
        @cursor += len
        res

    read_Uint8Array: ->
        @read_some @read_PT()

    read_some: ( len ) ->
        res = new Uint8Array @data.buffer, @cursor, len
        @cursor += len
        res

    read_uint_as_str: ->
        bf = new BitFieldInStr_Writer
        if @cursor >= @data.length
            return bf.str()
        val = @data[ @cursor ]
        @cursor += 1
        if val < 128
            bf.append_uint val, 7
            return bf.str()
        bf.append_uint val - 128, 7
        while true
            if @cursor >= @data.length
                return bf.str()
            val = @data[ @cursor ]
            @cursor += 1
            if val < 128
                bf.append_uint val, 7
                return bf.str()
            bf.append_uint val - 128, 7

    read_DevId: ( cur_dev ) ->
        arr = @read_some( 8 )
        str = Base64o.encode arr.slice( 0 ).reverse()
        new DevId if cur_dev? and str == cur_dev.val then DevId.null_DevId_str else str

    read_Inode: ( cur_dev ) ->
        num = @read_PT()
        dev = if num then @read_DevId cur_dev else new DevId
        new Inode dev, num

    read_PatchId: ( cur_dev ) ->
        num = @read_PT()
        dev = if num then @read_DevId cur_dev else new DevId
        new PatchId dev, num

    read_UsrId: ( cur_dev ) ->
        num = @read_PT()
        dev = if num then @read_DevId cur_dev else new DevId
        new UsrId dev, num

    read_GrpId: ->
        num = @read_PT()
        dev = if num then @read_DevId cur_dev else new DevId
        new GrpId dev, num

    read_VectorClock: ( cur_dev ) ->
        len = @read_PT()
        res = new VectorClock
        for i in [ 0 ... len ]
            dev = @read_DevId cur_dev
            num = @read_PT()
            res.map.set dev, num
        res

    read_Variable: ( cur_dev ) ->
        num_type = @read_PT()
        cl = Rp.num_type_map[ num_type ]
        if not cl.read_from? then console.error cl
        cl.read_from this, cur_dev

    make_crc32c: ( bcmd ) ->
        crc32c @data.slice bcmd, @cursor

    skip_some: ( len ) ->
        @cursor += len

    skip_Uint8Array: ->
        @skip_some @read_PT()

    slice: ( len ) ->
        new BinaryReader new Uint8Array @data.buffer, @cursor, @len

    @property 'buffer',
        get: -> @data.buffer

    @property 'offset',
        get: -> @cursor

    new_mark: () ->
        @marks.push @cursor

    pop_mark: () ->
        @cursor = @marks.pop()

Rp = require "./../Variable/Core/Rp.js"
