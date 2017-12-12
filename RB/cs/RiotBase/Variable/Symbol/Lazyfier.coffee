Variable = require "../Core/Variable.js"
Method   = require "../Core/Method.js"
Rp       = require "../Core/Rp.js"
GenSym   = require "./GenSym.js"

#
module.exports =
class Lazyfier extends Rp
    Rp.make_templ this,
        unknown_value: true
        term_value   : true

    init: ( sym ) ->
        @sym = sym

    copy: ->
        ot = Method.methods.obj_type.call_1 @sym
        new ot Lazyfier.New Method.methods.copy.call_1 @sym

    prox_ref: ->
        @sym

    # helpers
    @copy_rp_without_lazifier = ( val ) ->
         Method.methods.copy.call_1 if val.rp.type_num != Lazyfier.type_num then val else val.rp.sym

    @symbol = ( cl, name ) ->
        new cl Lazyfier.New new cl GenSym.New name


require "./SymOp.js"
