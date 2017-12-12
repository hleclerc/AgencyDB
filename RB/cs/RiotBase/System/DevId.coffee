
#
# Ex of valid DevId (e.g. for testing purpose)
#   gMWDnGjZfxZ~
#   gmWRNBUMN7o~
#   O5HeHcHU1Io~
#   h8u858_RnDF~
#   ujtameVI3fN~
#
module.exports =
class DevId
    @null_DevId_str = "-----------~"

    constructor: ( val = DevId.null_DevId_str ) ->
        @val = val

    to_glo: ( cur_dev ) ->
        if @loc() then cur_dev else this

    ext: -> # true if exterior device
        @val != DevId.null_DevId_str

    loc: -> # true if local device
        @val == DevId.null_DevId_str

    toString: ->
        @val

    inf: ( p ) ->
        @val < p.val

    sup: ( p ) ->
        @val > p.val

    equ: ( p ) ->
        @val == p.val
