
#
module.exports =
class VectorClock
    constructor: () ->
        @map = new Map # DevId (using .val which is of native type string) -> PT num

    contains: ( dev_id, num ) ->
        if not num?
            return @contains dev_id.dev, dev_id.num
        num <= ( @map.get( dev_id.val ) or 0 )

    update: ( dev_id, num ) -> # return true if num = num + 1 (the only authorized update)
        if not num?
            return @update dev_id.dev, dev_id.num
        if num == ( @map.get( dev_id.val ) or 0 ) + 1
            @map.set dev_id.val, num
            true

    val: ( dev_id )->
        @map.get( dev_id.val ) or 0

    inc: ( dev_id )->
        res = ( @map.get( dev_id.val ) or 0 ) + 1
        @map.set dev_id.val, res
        res

    toString: ->
        lst = []
        @map.forEach ( num, dev ) ->
            lst.push "#{dev}:#{num}"
        "{#{lst}}"
