Method = require "../Core/Method.js"
Rp     = require "../Core/Rp.js"

#
module.exports =
class GenArraySelect extends Rp
    Rp.make_templ this

    init: ( arr, item ) ->
        @arr  = arr
        @item = item

    prox_ptr: ->
        @arr.rp.sig_change_val @item
        @item

    prox_ref: ->
        @item
