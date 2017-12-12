DevId = require "./DevId.js"

#
module.exports =
class PatchId
    @loc_dev = new DevId

    constructor: ( dev = new DevId, num = 0 ) ->
        @dev = dev
        @num = num

    inf: ( p ) ->
        if @num == p.num then @dev.inf( p.dev ) else @num < p.num

    sup: ( p ) ->
        if @num == p.num then @dev.sup( p.dev ) else @num > p.num

    equ: ( p ) ->
        @num == p.num and @dev.equ( p.dev )
