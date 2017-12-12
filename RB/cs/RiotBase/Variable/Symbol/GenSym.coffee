Method = require "../Core/Method.js"
Rp     = require "../Core/Rp.js"

#
module.exports =
class GenSym extends Rp
    Rp.make_templ this

    init: ( name ) ->
        @name = name

    to_String: ->
        @name

    copy: ->
        ot = Method.methods.obj_type rp: this
        new ot this
